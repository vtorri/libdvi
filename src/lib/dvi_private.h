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

#ifndef DVI_PRIVATE_H
#define DVI_PRIVATE_H

#include <math.h>

/*
 * @def DVI_MAX_PAGES
 * @brief Define the maximum number of pages [41]
 *
 * This value should be configurable.
 */
#define DVI_MAX_PAGES 1000000

/*
 * @def DVI_RESOLUTION
 * @brief Define the resolution [41]
 */
#define DVI_RESOLUTION 300.0

/*
 * @def DVI_MAX_WIDTHS
 * @brief Define the maximum number of different characters among all fonts [5]
 */
#define DVI_MAX_WIDTHS 10000

static __inline__ int
dvi_round(double d)
{
    return (int)floor(d + 0.5);
}

static __inline__ unsigned char
dvi_read_uint8(const unsigned char *ptr)
{
    return *ptr;
}

static __inline__ unsigned short
dvi_read_uint16(const unsigned char *ptr)
{
    return (unsigned short)((ptr[0] << 8) | ptr[1]);
}

static __inline__ unsigned int
dvi_read_uint24(const unsigned char *ptr)
{
    return (unsigned int)((ptr[0] << 16) | (ptr[1] << 8) | ptr[2]);
}

static __inline__ unsigned int
dvi_read_uint32(const unsigned char *ptr)
{
    return (unsigned int)((ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3]);
}

static __inline__ signed char
dvi_read_sint8(const unsigned char *ptr)
{
    return (signed char)*ptr;
}

static __inline__ signed short
dvi_read_sint16(const unsigned char *ptr)
{
    return (signed short)((ptr[0] << 8) | ptr[1]);
}

static __inline__ signed int
dvi_read_sint24(const unsigned char *ptr)
{
    return (signed int)((ptr[0] << 16) | (ptr[1] << 8) | ptr[2]);
}

static __inline__ signed int
dvi_read_sint32(const unsigned char *ptr)
{
    return (signed int)((ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3]);
}

/* log */
void dvi_log_init(void);

#endif /* DVI_PRIVATE_H */
