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

#ifndef DVI_DOCUMENT_PRIVATE_H
#define DVI_DOCUMENT_PRIVATE_H

#include "dvi_map.h"
#include "dvi_stack.h"
#include "dvi_font.h"

struct _Dvi_Document
{
    char *filename;
    Dvi_Map *map;

    char *comment;
    double conv;
    double true_conv;
    double tfm_conv;
    Dvi_Stack *stack;
    Dvi_Fonts *fontes;
};

static __inline__ const unsigned char *
dvi_document_base_get(const Dvi_Document *doc)
{
    return dvi_map_base_get(doc->map);
}

static __inline__ size_t
dvi_document_length_get(const Dvi_Document *doc)
{
    return dvi_map_length_get(doc->map);
}

static __inline__ double
dvi_document_conv_get(const Dvi_Document *doc)
{
    return doc->conv;
}

static __inline__ double
dvi_document_true_conv_get(const Dvi_Document *doc)
{
    return doc->true_conv;
}

#endif /* DVI_DOCUMENT_PRIVATE_H */
