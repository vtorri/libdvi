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

#include "Dvi.h"
#include "dvi_log.h"
#include "dvi_kpathsea.h"
#include "dvi_private.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */

static int _dvi_init_count = 0;

/**
 * @endcond
 */

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

DAPI int
dvi_init(void)
{

    if (++_dvi_init_count != 1)
        return _dvi_init_count;

    dvi_log_init();

    if (!dvi_kpath_sea_init())
      return --_dvi_init_count;

    return _dvi_init_count;
}

DAPI int
dvi_shutdown(void)
{
    if (_dvi_init_count <= 0)
    {
        DVI_LOG_ERR("Init count not greater than 0 in shutdown.");
        return 0;
    }

    if (--_dvi_init_count != 0)
        return _dvi_init_count;

    dvi_kpathsea_shutdown();

    return _dvi_init_count;
}
