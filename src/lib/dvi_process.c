/* libdvi - DVI file interpreter
 * Copyright (C) 2016 Vincent Torri
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>

#ifdef _WIN32
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# include <windows.h>
# include <process.h>
# undef WIN32_LEAN_AND_MEAN
#else
/* # include <sys/types.h> */
/* # include <sys/stat.h> */
/* # include <sys/mman.h> */
/* # include <unistd.h> */
/* # include <fcntl.h> */
#endif

#include "Dvi.h"
#include "dvi_process.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/


typedef struct _Dvi_Process_Data Dvi_Process_Data;

struct _Dvi_Process_Data
{
    char *path;
#ifdef _WIN32
    HANDLE pipe;
#endif
};

#ifdef _WIN32

static unsigned int __stdcall
dvi_process_read_pipe_cb(void *ptr)
{
    Dvi_Process_Data *data = (Dvi_Process_Data *)ptr;
    DWORD read_bytes;

    while (1)
    {
        BOOL res;

        res = ReadFile(data->pipe, data->path, MAX_PATH, &read_bytes, NULL);
        if (!res || (read_bytes == 0))
            break;

        while ((data->path[read_bytes - 1] == '\r') ||
               (data->path[read_bytes - 1] == '\n'))
            read_bytes--;
        data->path[read_bytes] = '\0';
        break;
    }

    _endthreadex(0);

    return 0;
}

#endif


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


#ifdef _WIN32

char *
dvi_process_run(const char *arg)
{
    char cmd[32767];
    char path[MAX_PATH];
    SECURITY_ATTRIBUTES sa;
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    Dvi_Process_Data data;
    HANDLE objs[2];
    HANDLE pipe_stdout_read = NULL;
    HANDLE pipe_stdout_write = NULL;
    HANDLE thread = NULL;
    size_t len;
    unsigned int thread_id;

    if (!arg)
      return NULL;

    len = strlen(KPSEWHICH);
    memcpy(cmd, KPSEWHICH, len);
    cmd[len] = ' ';
    memcpy(cmd + len + 1, arg, strlen(arg) + 1);

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&pipe_stdout_read, &pipe_stdout_write, &sa, 0))
    {
        DVI_LOG_ERR("Fail to create pipe.");
        return NULL;
    }

    if (!SetHandleInformation(pipe_stdout_read, HANDLE_FLAG_INHERIT, 0))
    {
        DVI_LOG_ERR("Fail to set pipe information.");
        goto error;
    }

    path[0] = '\0';
    data.pipe = pipe_stdout_read;
    data.path = path;
    thread = (HANDLE)_beginthreadex(NULL, 0, &dvi_process_read_pipe_cb, &data, 0, &thread_id);
    if (!thread)
    {
        DVI_LOG_ERR("Fail to create thread.");
        goto error;
    }

    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.hStdOutput = pipe_stdout_write;
    si.dwFlags |= STARTF_USESTDHANDLES;

    if (!CreateProcess(NULL, cmd, NULL, NULL, TRUE,
                       NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW | CREATE_SUSPENDED,
                       NULL, NULL, &si, &pi))
    {
        DVI_LOG_ERR("Fail to create child.");
        return NULL;
    }

    CloseHandle(pipe_stdout_write);
    pipe_stdout_write = NULL;

    if (ResumeThread(pi.hThread) == ((DWORD)-1))
    {
        DVI_LOG_ERR("Fail to resume thread process.");
        goto error;
    }

    objs[0] = thread;
    objs[1] = pi.hProcess;
    WaitForMultipleObjects(2, objs, TRUE, INFINITE );

    CloseHandle(thread);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    CloseHandle(pipe_stdout_read);

    return strdup(data.path);

  error:
    if (pi.hThread) CloseHandle(pi.hThread);
    if (pi.hProcess) CloseHandle(pi.hProcess);
    if (pipe_stdout_write) CloseHandle(pipe_stdout_write);
    if (pipe_stdout_read) CloseHandle(pipe_stdout_read);

    return NULL;
}

#else

char *
dvi_process_run(const char *arg)
{
    return NULL;
}

#endif


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
