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

# include <stdlib.h>
# include <string.h>

# include <stdio.h> /* for kpathsea */

# ifdef _WIN32
#  ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN
#  endif
#  include <windows.h>
#  undef WIN32_LEAN_AND_MEAN
# endif

# include "Dvi.h"
# include "dvi_private.h"
# include "dvi_log.h"
# include "dvi_kpathsea.h"
# include "dvi_kpathsea_private.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */

typedef kpathsea (*_dvi_kpathsea_new_t)(void);
typedef void (*_dvi_kpathsea_set_program_name_t)(kpathsea kpse,
                                                 const char *argv0,
                                                 const char *progname);
typedef void (*_dvi_kpathsea_init_prog_t)(kpathsea kpse,
                                          const char *prefix,
                                          unsigned int dpi,
                                          const char *mode,
                                          const char *fallback);
typedef char *(*_dvi_kpathsea_find_file_t)(kpathsea kpse,
                                           const char *name,
                                           kpse_file_format_type format,
                                           int must_exist);
typedef char *(*_dvi_kpathsea_find_glyph_t)(kpathsea kpse,
                                            const char *font_name,
                                            unsigned int dpi,
                                            kpse_file_format_type format,
                                            kpse_glyph_file_type *glyph_file);
typedef FILE *(*_dvi_kpathsea_open_file_t)(kpathsea kpse,
                                           const char *name,
                                           kpse_file_format_type format);
typedef void (*_dvi_kpathsea_finish_t)(kpathsea kpse);

static _dvi_kpathsea_new_t _dvi_kpathsea_new;
static _dvi_kpathsea_set_program_name_t _dvi_kpathsea_set_program_name;
static _dvi_kpathsea_init_prog_t _dvi_kpathsea_init_prog;
static _dvi_kpathsea_find_file_t _dvi_kpathsea_find_file;
static _dvi_kpathsea_find_glyph_t _dvi_kpathsea_find_glyph;
static _dvi_kpathsea_open_file_t _dvi_kpathsea_open_file;
static _dvi_kpathsea_finish_t _dvi_kpathsea_finish;

static kpathsea _dvi_kpathsea_instance = NULL;

/**
 * @endcond
 */

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/

unsigned char
dvi_kpathsea_init(void)
{
#ifdef HAVE_TEXLIVE
    _dvi_kpathsea_new = kpathsea_new;
    _dvi_kpathsea_set_program_name = kpathsea_set_program_name;
    _dvi_kpathsea_init_prog = kpathsea_init_prog;
    _dvi_kpathsea_find_file = kpathsea_find_file;
    _dvi_kpathsea_find_glyph = kpathsea_find_glyph;
    _dvi_kpathsea_open_file = kpathsea_open_file;
    _dvi_kpathsea_finish = kpathsea_finish;
#else
    _dvi_kpathsea_new = miktex_kpathsea_new;
    _dvi_kpathsea_set_program_name = miktex_kpathsea_set_program_name;
    _dvi_kpathsea_init_prog = miktex_kpathsea_init_prog;
    _dvi_kpathsea_find_file = miktex_kpathsea_find_file;
    _dvi_kpathsea_find_glyph = miktex_kpathsea_find_glyph;
    _dvi_kpathsea_open_file = miktex_kpathsea_open_file;
    _dvi_kpathsea_finish = miktex_kpathsea_finish;
#endif

    _dvi_kpathsea_instance = _dvi_kpathsea_new();
    if (!_dvi_kpathsea_instance)
    {
        DVI_LOG_ERR("kpathsea_new failed");
        return 0;
    }

    return 1;
}

void
dvi_kpathsea_shutdown(void)
{
    _dvi_kpathsea_finish(_dvi_kpathsea_instance);
}

const char *
dvi_kpathsea_path_name_get(const char *name)
{
    char *n;

    _dvi_kpathsea_set_program_name(_dvi_kpathsea_instance, "kpsewhich", NULL);
    _dvi_kpathsea_init_prog (_dvi_kpathsea_instance, "LIBDVI", 300, NULL, NULL);
    n = _dvi_kpathsea_find_file(_dvi_kpathsea_instance, name, kpse_tfm_format, 1);
    return n;
}



/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
