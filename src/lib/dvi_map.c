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

#include <stdlib.h>

#ifdef _WIN32
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# include <windows.h>
# undef WIN32_LEAN_AND_MEAN
#else
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <unistd.h>
# include <fcntl.h>
#endif

#include "Dvi.h"
#include "dvi_map.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/


struct _Dvi_Map
{
    void *base;
    size_t length;
#ifdef _WIN32
    HANDLE file;
    HANDLE map;
#else
    int fd;
#endif
};


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


#ifdef _WIN32

Dvi_Map *
dvi_map_new(const char *filename)
{
    BY_HANDLE_FILE_INFORMATION info;
    Dvi_Map *map;

    map = (Dvi_Map *)calloc(1, sizeof(Dvi_Map));
    if (!map)
        return NULL;

    map->file = CreateFile(filename,
                           GENERIC_READ,
                           FILE_SHARE_READ,
                           NULL,
                           OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL,
                           NULL);
    if (map->file == INVALID_HANDLE_VALUE)
    {
        DVI_LOG_ERR("Can not open file %s", filename);
        goto free_map;
    }

    if (!GetFileInformationByHandle(map->file, &info))
    {
        DVI_LOG_ERR("Can not retrieve informations of file %s", filename);
        goto close_file;
    }

    if (!(info.dwFileAttributes | FILE_ATTRIBUTE_NORMAL))
    {
        DVI_LOG_ERR("file %s is not a normal file", filename);
        goto close_file;
    }

#ifdef _WIN64
    map->length = (((size_t)info.nFileSizeHigh) << 32) | (size_t)info.nFileSizeLow;
#else
    map->length = (size_t)info.nFileSizeLow;
#endif

    map->map = CreateFileMapping(map->file,
                                 NULL, PAGE_READONLY,
                                 0, 0, NULL);
    if (!map->map)
    {
        DVI_LOG_ERR("Can not create file mapping for file %s", filename);
        goto close_file;
    }

    map->base = MapViewOfFile(map->map, FILE_MAP_READ, 0, 0, 0);
    if (!map->base)
    {
        DVI_LOG_ERR("Can not create view for file mapping 0x%p", map->map);
        goto close_file_map;
    }

    return map;

  close_file_map:
    CloseHandle(map->map);
  close_file:
    CloseHandle(map->file);
  free_map:
    free(map);

    return NULL;
}

void
dvi_map_del(Dvi_Map *map)
{
    UnmapViewOfFile(map->base);
    CloseHandle(map->map);
    CloseHandle(map->file);
    free(map);
}

#else

Dvi_Map *
dvi_map_new(const char *filename)
{
    struct stat st;
    Dvi_Map *map;

    map = (Dvi_Map *)calloc(1, sizeof(Dvi_Map));
    if (!map)
        return NULL;

    map->fd = open(filename, O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (map->fd == -1)
    {
        DVI_LOG_ERR("Can not open file %s", filename);
        goto free_map;
    }

    if (fstat(map->fd, &st) == -1)
    {
        DVI_LOG_ERR("Can not retrieve stat from file %s", filename);
        goto close_fd;
    }

    if (!(S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)))
    {
        DVI_LOG_ERR("file %s is not a regular file nor a symbolic link", filename);
        goto close_fd;
    }

    map->length = st.st_size;

    map->base = mmap(NULL, map->length, PROT_READ, MAP_SHARED, map->fd, 0);
    if (!map->base)
    {
        DVI_LOG_ERR("Can not map file %s into memory", filename);
        goto close_fd;
    }

    return map;

  close_fd:
    close(map->fd);
  free_map:
    free(map);

    return NULL;
}

void
dvi_map_del(Dvi_Map *map)
{
    munmap(map->base, map->length);
    close(map->fd);
    free(map);
}

#endif

const unsigned char *
dvi_map_base_get(const Dvi_Map *map)
{
    return map->base;
}

size_t
dvi_map_length_get(const Dvi_Map *map)
{
    return map->length;
}


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
