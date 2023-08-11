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

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <limits.h>

#ifdef _WIN32
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# include <windows.h>
# undef WIN32_LEAN_AND_MEAN
# include <io.h>
#else
# include <sys/types.h>
# include <unistd.h>
#endif

#include "Dvi.h"
#include "dvi_private.h"
#include "dvi_log.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/


static const char *_dvi_log_level_names[] =
{
    "ERR",
    "WRN",
    "INF",
    "DBG"
};

#ifdef NDEBUG
static Dvi_Log_Level _dvi_log_level = DVI_LOG_LEVEL_ERR;
#else
static Dvi_Log_Level _dvi_log_level = DVI_LOG_LEVEL_DBG;
#endif

static Dvi_Log_Print_Cb _dvi_log_print_cb = dvi_log_print_cb_stderr;
static void *_dvi_log_print_cb_data = NULL;

#ifdef _WIN32

static unsigned char _dvi_log_is_posix = 1;
static HANDLE _dvi_log_handle_stdout = NULL;
static HANDLE _dvi_log_handle_stderr = NULL;

static WORD
_dvi_log_win32_print_level_color_get(int level, WORD original_background)
{
    WORD foreground;

    switch (level)
    {
        case DVI_LOG_LEVEL_ERR:
            foreground = FOREGROUND_INTENSITY | FOREGROUND_RED;
            break;
        case DVI_LOG_LEVEL_WARN:
            foreground = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;
            break;
        case DVI_LOG_LEVEL_INFO:
            foreground = FOREGROUND_INTENSITY | FOREGROUND_GREEN;
            break;
        case DVI_LOG_LEVEL_DBG:
          foreground = FOREGROUND_INTENSITY | FOREGROUND_BLUE;
          break;
        default:
            foreground = FOREGROUND_BLUE;
            break;
    }

    return original_background | foreground;
}

static void
_dvi_log_win32_print_prefix_func(FILE *stream,
                                 Dvi_Log_Level level,
                                 const char *file,
                                 const char *fct,
                                 int line)
{
    CONSOLE_SCREEN_BUFFER_INFO scbi_stdout;
    CONSOLE_SCREEN_BUFFER_INFO scbi_stderr;
    CONSOLE_SCREEN_BUFFER_INFO *scbi;
    HANDLE handle;
    WORD color;
    BOOL use_color;

    if (_dvi_log_handle_stdout != INVALID_HANDLE_VALUE)
    {
        if (!GetConsoleScreenBufferInfo(_dvi_log_handle_stdout, &scbi_stdout))
            return;
    }

    if (_dvi_log_handle_stderr != INVALID_HANDLE_VALUE)
    {
        if (!GetConsoleScreenBufferInfo(_dvi_log_handle_stderr, &scbi_stderr))
            return;
    }

    handle  = (stream == stdout) ? _dvi_log_handle_stdout : _dvi_log_handle_stderr;
    scbi = (stream == stdout) ? &scbi_stdout : &scbi_stderr;
    use_color = (_isatty(_fileno(stream)) != 1) && (handle != INVALID_HANDLE_VALUE);
    color = use_color ? _dvi_log_win32_print_level_color_get(level, scbi->wAttributes & ~7) : 0;
    if (use_color)
    {
        fflush(stream);
        SetConsoleTextAttribute(handle, color);
    }

    fprintf(stream, "%s", _dvi_log_level_names[level]);
    if (use_color)
    {
        fflush(stream);
        SetConsoleTextAttribute(handle, scbi->wAttributes);
    }

    fprintf(stream, ": %s:%d %s()", file, line, fct);
}

#endif

static const char *
_dvi_log_posix_print_level_color_get(int level)
{
    switch (level)
    {
        case DVI_LOG_LEVEL_ERR:
            return "\033[31m";
        case DVI_LOG_LEVEL_WARN:
            return "\033[33;1m";
        case DVI_LOG_LEVEL_INFO:
            return "\033[32;1m";
        case DVI_LOG_LEVEL_DBG:
            return "\033[34;1m";
        default:
            return "\033[34m";
    }
}

static void
_dvi_log_posix_print_prefix_func(FILE *stream,
                                 Dvi_Log_Level level,
                                 const char *file,
                                 const char *fct,
                                 int line)
{
    fprintf(stream, "%s%s" "\033[0m" ": %s:%d "
            "\033[1m" "%s()" "\033[0m" " ",
            _dvi_log_posix_print_level_color_get(level),
            _dvi_log_level_names[level],
            file, line, fct);
}

static void
_dvi_log_fprint_cb(FILE *stream,
                   Dvi_Log_Level level,
                   const char *file,
                   const char *fct,
                   int line,
                   const char *fmt,
                   void *data, /* later for XML output */
                   va_list args)
{
    va_list args_copy;
    char *str;
    int res;
    int s;

#ifdef _WIN32
    args_copy = args;
#else
    va_copy(args_copy, args);
#endif

    s = vsnprintf(NULL, 0, fmt, args_copy);
    if (s == -1)
        return;

    str = (char *)malloc((s + 2) * sizeof(char));
    if (!str)
        return;

    s = vsnprintf(str, s + 1, fmt, args);
    if (s == -1)
    {
        free(str);
        return;
    }
    str[s] = '\n';
    str[s + 1] = '\0';

#ifdef _WIN32
    if (!_dvi_log_is_posix)
        _dvi_log_win32_print_prefix_func(stream, level, file, fct, line);
    else
#endif
    _dvi_log_posix_print_prefix_func(stream, level, file, fct, line);
    res = fprintf(stream, "%s", str);
    free(str);

    if (res < 0)
        return;

    if ((int)res != (s + 1))
        fprintf(stderr, "ERROR: %s(): want to write %d bytes, %d written\n", __func__, s + 1, res);

    (void)data;
}


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


void
dvi_log_init(void)
{
    char *endptr;
    const char *level;

#ifdef _WIN32
    DWORD mode;

    if (GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &mode))
    {
        _dvi_log_handle_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
        _dvi_log_handle_stderr = GetStdHandle(STD_ERROR_HANDLE);
        _dvi_log_is_posix = 0;
    }
    else
        _dvi_log_is_posix = 1;
#endif

    level = getenv("DVI_LOG_LEVEL");
    if (level)
    {
        long l;

        l = strtol(level, &endptr, 10);
        if (!((errno == ERANGE && (l == LONG_MAX || l == LONG_MIN)) ||
              (errno != 0 && l == 0) ||
              (endptr == level)))
        {
            if ((l < DVI_LOG_LEVEL_ERR) || (l >= DVI_LOG_LEVEL_LAST))
                fprintf(stderr, "INF: DVI_LOG_LEVEL has an invalid value.\n");
            else
                _dvi_log_level = (int)l;
        }
    }
}


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/


DVI_API void
dvi_log_print_cb_set(Dvi_Log_Print_Cb cb, void *data)
{
    _dvi_log_print_cb = cb;
    _dvi_log_print_cb_data = data;
}


DVI_API void
dvi_log_print_cb_stderr(Dvi_Log_Level level,
                        const char *file,
                        const char *fct,
                        int line,
                        const char *fmt,
                        void *data,
                        va_list args)
{
    _dvi_log_fprint_cb(stderr, level, file, fct, line, fmt, data, args);
}

DVI_API void
dvi_log_print_cb_stdout(Dvi_Log_Level level,
                        const char *file,
                        const char *fct,
                        int line,
                        const char *fmt,
                        void *data,
                        va_list args)
{
    _dvi_log_fprint_cb(stdout, level, file, fct, line, fmt, data, args);
}

DVI_API void
dvi_log_print(Dvi_Log_Level level,
              const char *file,
              const char *fct,
              int line,
              const char *fmt, ...)
{
    va_list args;

    if (level > _dvi_log_level)
        return;

    if (!fmt)
    {
        fprintf(stderr, "ERROR: %s() fmt == NULL\n", __func__);
        return;
    }

    va_start(args, fmt);
    _dvi_log_print_cb(level, file, fct, line, fmt, _dvi_log_print_cb_data, args);
    va_end(args);
}

DVI_API void dvi_log_level_set(Dvi_Log_Level level)
{
    if ((level < DVI_LOG_LEVEL_ERR) || (level >= DVI_LOG_LEVEL_LAST))
    {
        DVI_LOG_WARN("level %d not corect", level);
        return;
    }

    _dvi_log_level = level;
}

DVI_API Dvi_Log_Level
dvi_log_level_get(void)
{
    return _dvi_log_level;
}
