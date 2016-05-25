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
#include "dvi_log.h"
#include "dvi_stack.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */

#define DVI_STACK_MAX_SIZE 100

struct Dvi_Stack_
{
    Dvi_Stack_Item *data;
    unsigned char max_size;
    unsigned char top;
};

struct Dvi_Stack_Item_
{
    int h;
    int v;
    int w;
    int x;
    int y;
    int z;
    int hh;
    int vv;
};

/**
 * @endcond
 */

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


Dvi_Stack *
dvi_stack_new(unsigned char max_size)
{
    Dvi_Stack *stack;

    if (max_size <= 0)
    {
        DVI_LOG_ERR("Ivalid maximum stack size, must be greater than 0");
        return NULL;
    }

    if (max_size > DVI_STACK_MAX_SIZE)
    {
        DVI_LOG_ERR("Ivalid maximum stack size, must be less than %d", DVI_STACK_MAX_SIZE);
        return NULL;
    }

    stack = (Dvi_Stack *)calloc(1, sizeof(Dvi_Stack));
    if (!stack)
    {
        DVI_LOG_ERR("Can not allocate memory for a stack");
        return NULL;
    }

    stack->data = calloc(max_size, sizeof(Dvi_Stack_Item));
    if (!stack->data)
    {
        DVI_LOG_ERR("Can not allocate memory for stack items");
        free(stack);
        return NULL;
    }

    stack->max_size = max_size;

    return stack;
}

void
dvi_stack_del(Dvi_Stack *stack)
{
    if (!stack)
        return;

    free(stack->data);
    free(stack);
}

void
dvi_stack_reset(Dvi_Stack *stack)
{
    stack->top = 0;
}

void
dvi_stack_push(Dvi_Stack *stack, const Dvi_Stack_Item *item)
{
    if (stack->top == stack->max_size)
    {
        DVI_LOG_ERR("Can not push item on stack, stack overflow");
        return;
    }

    stack->data[stack->top] = *item;
    stack->top++;
}

Dvi_Stack_Item *
dvi_stack_pop(Dvi_Stack *stack)
{
    if (stack->top == 0)
    {
        DVI_LOG_ERR("Can not pop item from stack, stack underflow");
        return NULL;
    }

    stack->top--;

    return &stack->data[stack->top];
}


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
