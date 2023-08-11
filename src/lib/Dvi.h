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

#ifdef DVI_API
# undef DVI_API
#endif

#ifndef DVI_STATIC
# ifdef _WIN32
#  ifdef DVI_BUILD
#   define DVI_API __declspec(dllexport)
#  else
#   define DVI_API __declspec(dllimport)
#  endif
# elif defined __SUNPRO_C || defined __SUNPRO_CC
#  define DVI_API __global
# elif (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER
#  define DVI_API __attribute__ ((visibility("default")))
# else
#  define DVI_API
# endif
#else
# define DVI_API
#endif

DVI_API int dvi_init(void);
DVI_API int dvi_shutdown(void);

#include "dvi_log.h"
#include "dvi_document.h"

#endif /* DVI_H */
