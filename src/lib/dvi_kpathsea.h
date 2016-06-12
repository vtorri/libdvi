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

#ifndef DVI_KPATHSEA_H
#define DVI_KPATHSEA_H

typedef struct _Dvi_Kpathsea Dvi_Kpathsea;

Dvi_Kpathsea *dvi_kpathsea_new(const char *prog_name);

void dvi_kpathsea_free(Dvi_Kpathsea *kps);

char *dvi_kpathsea_path_name_get(Dvi_Kpathsea *kps, const char *name);

#endif /* DVI_FONT_H */