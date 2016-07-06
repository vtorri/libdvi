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

#include "Dvi.h"
#include "dvi_page.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/


typedef struct _Dvi_Page Dvi_Page;

struct _Dvi_Pages
{
    Dvi_Page **pages;
    int total_pages;
};

struct _Dvi_Page
{
    const unsigned char *loc; /* location in file */
};

static Dvi_Page *
dvi_page_new(const unsigned char *loc)
{
    Dvi_Page *page;

    page = (Dvi_Page *)calloc(1, sizeof(Dvi_Page));
    if (!page)
        return NULL;

    page->loc = loc;

    return page;
}

static void
dvi_page_del(Dvi_Page *page)
{
    free(page);
}


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


Dvi_Pages *
dvi_pages_new(int total_pages)
{
    Dvi_Pages *pages;

    pages = (Dvi_Pages *)calloc(1, sizeof(Dvi_Pages));
    if (!pages)
        return NULL;

    pages->total_pages = total_pages;

    pages->pages = (Dvi_Page **)calloc(total_pages, sizeof(Dvi_Page*));
    if (!pages->pages)
    {
        free(pages);
        return NULL;
    }

    return pages;
}

void
dvi_pages_del(Dvi_Pages *pages)
{
    int i;

    /* we assume pages is always set */

    for (i = 0; i < pages->total_pages; i++)
        dvi_page_del(pages->pages[i]);
    free(pages->pages);
    free(pages);
}

void
dvi_pages_page_add(Dvi_Pages *pages, int idx, const unsigned char *loc)
{
    Dvi_Page *page;

    /* we assume pages is always set */

    if ((idx < 0) || (idx >= pages->total_pages))
        return;

    if (pages->pages[idx])
        return;

    page = dvi_page_new(loc);
    if (page)
    {
        pages->pages[idx] = page;
    }
}


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
