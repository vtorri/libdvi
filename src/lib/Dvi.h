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

#ifndef DVI_H
#define DVI_H

#ifdef DAPI
# undef DAPI
#endif

#ifdef _WIN32
# ifdef DVI_BUILD
#  ifdef DLL_EXPORT
#   define DAPI __declspec(dllexport)
#  else
#   define DAPI
#  endif
# else
#  define DAPI __declspec(dllimport)
# endif
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define DAPI __attribute__ ((visibility("default")))
#  else
#   define DAPI
#  endif
# else
#  define DAPI
# endif
#endif

DAPI int dvi_init(void);
DAPI int dvi_shutdown(void);

#include "dvi_log.h"

#undef DAPI
#define DAPI

#endif /* DVI_H */
