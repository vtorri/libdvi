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

#ifndef DVI_FONT_H
#define DVI_FONT_H

#include "dvi_map.h"

typedef struct _Dvi_Fonts Dvi_Fonts;
typedef struct _Dvi_Font_Tfm Dvi_Font_Tfm;

struct _Dvi_Fonts
{
    int pixel_width[DVI_MAX_WIDTHS + 1]; /* actual character widths, in pixels, signed quads [39] */
    int width[DVI_MAX_WIDTHS + 1]; /* character widths, in DVI units, signed quads [30] */
    Dvi_Font_Tfm *fonts;
    unsigned int nf; /* the number of known fonts, unsigned quad [30] */
    unsigned int width_ptr; /* the number of known character widths, unsigned quad [30] */
};

struct _Dvi_Font_Tfm
{
    char *name;
    Dvi_Map *map;
    int num; /* signed quad [15] [59] fnt_def* */
    int check_sum; /* signed quad [30] [61] */
    int scaled_size; /* signed quad [30] [61] */
    int design_size; /* signed quad [30] [61] */
    int space; /* signed quad [30]] */
    int bc; /* signed quad [30]] */
    int ec; /* signed quad [30]] */
    int width_base; /* signed quad [30]] */
};

void dvi_font_define(const Dvi_Document *doc,
                     const unsigned char **cur_loc,
                     int e);

void dvi_font_del(Dvi_Fonts *fontes);

#endif /* DVI_FONT_H */
