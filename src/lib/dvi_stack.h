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

#ifndef DVI_STACK_H
#define DVI_STACK_H

typedef struct Dvi_Stack_ Dvi_Stack;
typedef struct Dvi_Stack_Item_ Dvi_Stack_Item;

DAPI Dvi_Stack *dvi_stack_new(unsigned char max_size);

DAPI void dvi_stack_del(Dvi_Stack *stack);

DAPI void dvi_stack_reset(Dvi_Stack *stack);

DAPI void dvi_stack_push(Dvi_Stack *stack, const Dvi_Stack_Item *item);

DAPI Dvi_Stack_Item *dvi_stack_pop(Dvi_Stack *stack);

#endif /* DVI_STACK_H */
