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

#ifndef DVI_INTERPRETER_H
#define DVI_INTERPRETER_H

unsigned char dvi_interpreter_preamble_parse(const Dvi_Document *doc,
					     unsigned char *id,
                                             int *num,
                                             int *den,
                                             int *mag,
                                             char **comment);

unsigned char dvi_interpreter_postpostamble_parse(const Dvi_Document *doc,
                                                  unsigned char *id,
                                                  int *postamble_loc);

unsigned char dvi_interpreter_postamble_parse(Dvi_Document *doc,
					      int postamble_loc,
                                              int *final_bop,
                                              int *num,
                                              int *den,
                                              int *mag,
                                              unsigned short *max_s,
                                              unsigned short *total_pages);

unsigned char dvi_interpreter_fnt_def_parse(Dvi_Document *doc,
                                            const unsigned char *cur_loc);

#endif /* DVI_INTERPRETER_H */
