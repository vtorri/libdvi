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

#ifndef DVI_OPCODES_H
#define DVI_OPCODES_H

typedef enum
{
    DVI_OPCODE_SETCHAR_0 = 0,    /* typeset character numbers 0 to 127 */
    DVI_OPCODE_SET1      = 128,  /* c[1] typeset character c */
    DVI_OPCODE_SET2      = 129,  /* c[2] typeset character c */
    DVI_OPCODE_SET3      = 130,  /* c[3] typeset character c */
    DVI_OPCODE_SET4      = 131,  /* c[4] typeset character c */
    DVI_OPCODE_SETRULE   = 132,  /* a[4] b[4] typeset rectangle of size a x b */
    DVI_OPCODE_PUT1      = 133,  /* c[1] typeset character c from font */
    DVI_OPCODE_PUT2      = 134,  /* c[2] typeset character c from font */
    DVI_OPCODE_PUT3      = 135,  /* c[3] typeset character c from font */
    DVI_OPCODE_PUT4      = 136,  /* c[4] typeset character c from font */
    DVI_OPCODE_PUTRULE   = 137,  /* a[4] b[4] typeset rectangle of size a x b */
    DVI_OPCODE_NOP       = 138,  /* no operation */
    DVI_OPCODE_BOP       = 139,  /* c0[4] c1[4] ... c9[4] p[4] beginning of a page */
    DVI_OPCODE_EOP       = 140,  /* end of page */
    DVI_OPCODE_PUSH      = 141,  /* push on the stack */
    DVI_OPCODE_POP       = 142,  /* pop on the stack */
    DVI_OPCODE_RIGHT1    = 143,  /* b[1] increase h by b */
    DVI_OPCODE_RIGHT2    = 144,  /* b[2] increase h by b */
    DVI_OPCODE_RIGHT3    = 145,  /* b[3] increase h by b */
    DVI_OPCODE_RIGHT4    = 146,  /* b[4] increase h by b */
    DVI_OPCODE_W0        = 147,  /* increase h by w */
    DVI_OPCODE_W1        = 148,  /* b[1] set w to b increase h by b */
    DVI_OPCODE_W2        = 149,  /* b[2] set w to b increase h by b */
    DVI_OPCODE_W3        = 150,  /* b[3] set w to b increase h by b */
    DVI_OPCODE_W4        = 151,  /* b[4] set w to b increase h by b */
    DVI_OPCODE_X0        = 152,  /* increase h by x */
    DVI_OPCODE_X1        = 153,  /* b[1] set x to b increase h by b */
    DVI_OPCODE_X2        = 154,  /* b[2] set x to b increase h by b */
    DVI_OPCODE_X3        = 155,  /* b[3] set x to b increase h by b */
    DVI_OPCODE_X4        = 156,  /* b[4] set x to b increase h by b */
    DVI_OPCODE_DOWN1     = 157,  /* a[1] increase v by a */
    DVI_OPCODE_DOWN2     = 158,  /* a[2] increase v by a */
    DVI_OPCODE_DOWN3     = 159,  /* a[3] increase v by a */
    DVI_OPCODE_DOWN4     = 160,  /* a[4] increase v by a */
    DVI_OPCODE_Y0        = 161,  /* increase v by y */
    DVI_OPCODE_Y1        = 162,  /* a[1] set y to a increase v by a */
    DVI_OPCODE_Y2        = 163,  /* a[2] set y to a increase v by a */
    DVI_OPCODE_Y3        = 164,  /* a[3] set y to a increase v by a */
    DVI_OPCODE_Y4        = 165,  /* a[4] set y to b increase v by a */
    DVI_OPCODE_Z0        = 166,  /* increase v by z */
    DVI_OPCODE_Z1        = 167,  /* a[1] set z to a increase v by a */
    DVI_OPCODE_Z2        = 168,  /* a[2] set z to a increase v by a */
    DVI_OPCODE_Z3        = 169,  /* a[3] set z to a increase v by a */
    DVI_OPCODE_Z4        = 170,  /* a[4] set z to a increase v by a */
    DVI_OPCODE_FNT_NUM_0 = 171,  /* set font to 0 ... 64 */
    DVI_OPCODE_FNT1      = 235,  /* k[1] set font to k */
    DVI_OPCODE_FNT2      = 236,  /* k[1] set font to k */
    DVI_OPCODE_FNT3      = 237,  /* k[1] set font to k */
    DVI_OPCODE_FNT4      = 238,  /* k[1] set font to k */
    DVI_OPCODE_XXX1      = 239,  /* k[1] x[k] (k+2)-byte nop unless special is used */
    DVI_OPCODE_XXX2      = 240,  /* k[2] x[k] (k+2)-byte nop unless special is used */
    DVI_OPCODE_XXX3      = 241,  /* k[3] x[k] (k+2)-byte nop unless special is used */
    DVI_OPCODE_XXX4      = 242,  /* k[4] x[k] (k+2)-byte nop unless special is used */
    DVI_OPCODE_FNT_DEF1  = 243,  /* k[1] c[4] s[4] d[4] a[4] l[1] n[a+l] define font k */
    DVI_OPCODE_FNT_DEF2  = 244,  /* k[2] c[4] s[4] d[4] a[4] l[1] n[a+l] define font k */
    DVI_OPCODE_FNT_DEF3  = 245,  /* k[3] c[4] s[4] d[4] a[4] l[1] n[a+l] define font k */
    DVI_OPCODE_FNT_DEF4  = 246,  /* k[4] c[4] s[4] d[4] a[4] l[1] n[a+l] define font k */
    DVI_OPCODE_PRE       = 247,  /* i[1] num[4] den[4] mag[4] k[1] x[k] beginning of preamble */
    DVI_OPCODE_POST      = 248,  /* p[4] num[4] den[4] mag[4] l[4] u[4] s[2] t[2] beginning of postamble */
    DVI_OPCODE_POST_POST = 249   /* q[4] i[1] 223... (>=4) beginning of post-postamble */
} Dvi_Opcode;

#endif /* DVI_OPCODES_H */
