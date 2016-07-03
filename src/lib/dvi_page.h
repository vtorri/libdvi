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

#ifndef DVI_PAGE_H
#define DVI_PAGE_H

/**
 * @defgroup Page DVI page functions
 *
 * The main purpose of this file is to provide functions to manage pages
 * of a DVI file.
 *
 * @{
 */

typedef struct _Dvi_Pages Dvi_Pages;

Dvi_Pages *dvi_pages_new(int total_pages);

void dvi_pages_del(Dvi_Pages *pages);

void dvi_pages_page_add(Dvi_Pages *pages, int idx, const unsigned char *loc);

/**
 * @}
 */

#endif /* DVI_PAGE_H */
