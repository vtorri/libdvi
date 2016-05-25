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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef DVI_MAP_H
#define DVI_MAP_H

/**
 * @defgroup Map Shared mapped file functions
 *
 * The main purpose of this file is to provide a cross-platform way to
 * map a file in shared memory. It is used only for mapping DVI files in
 * memory.
 *
 * @{
 */

typedef struct _Dvi_Map Dvi_Map;

Dvi_Map *dvi_map_new(const char *filename);

void dvi_map_del(Dvi_Map *map);

const unsigned char *dvi_map_base_get(const Dvi_Map *map);

size_t dvi_map_length_get(const Dvi_Map *map);

/**
 * @}
 */

#endif /* DVI_MAP_H */
