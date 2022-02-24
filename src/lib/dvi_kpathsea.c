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

#include <stdlib.h>
#include <string.h>

#include <stdio.h> /* for kpathsea */

#ifdef HAVE_KPATHSEA_H
# include <kpathsea.h>
#elif defined HAVE_KPATHSEA_KPATHSEA_H
# include <kpathsea/kpathsea.h>
#endif

#ifdef _WIN32
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# include <windows.h>
# undef WIN32_LEAN_AND_MEAN
#endif

#include "Dvi.h"
#include "dvi_private.h"
#include "dvi_log.h"
#include "dvi_kpathsea.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */

#ifdef _WIN32

typedef void* kpathsea;

typedef enum
{
  kpse_gf_format,
  kpse_pk_format,
  kpse_any_glyph_format,        /* ``any'' meaning gf or pk */
  kpse_tfm_format,
  kpse_afm_format,
  kpse_base_format,
  kpse_bib_format,
  kpse_bst_format,
  kpse_cnf_format,
  kpse_db_format,
  kpse_fmt_format,
  kpse_fontmap_format,
  kpse_mem_format,
  kpse_mf_format,
  kpse_mfpool_format,
  kpse_mft_format,
  kpse_mp_format,
  kpse_mppool_format,
  kpse_mpsupport_format,
  kpse_ocp_format,
  kpse_ofm_format,
  kpse_opl_format,
  kpse_otp_format,
  kpse_ovf_format,
  kpse_ovp_format,
  kpse_pict_format,
  kpse_tex_format,
  kpse_texdoc_format,
  kpse_texpool_format,
  kpse_texsource_format,
  kpse_tex_ps_header_format,
  kpse_troff_font_format,
  kpse_type1_format,
  kpse_vf_format,
  kpse_dvips_config_format,
  kpse_ist_format,
  kpse_truetype_format,
  kpse_type42_format,
  kpse_web2c_format,
  kpse_program_text_format,
  kpse_program_binary_format,
  kpse_miscfonts_format,
  kpse_web_format,
  kpse_cweb_format,
  kpse_enc_format,
  kpse_cmap_format,
  kpse_sfd_format,
  kpse_opentype_format,
  kpse_pdftex_config_format,
  kpse_lig_format,
  kpse_texmfscripts_format,
  kpse_lua_format,
  kpse_fea_format,
  kpse_cid_format,
  kpse_mlbib_format,
  kpse_mlbst_format,
  kpse_clua_format,
  kpse_ris_format,
  kpse_bltxml_format,
  kpse_last_format /* one past last index */
} kpse_file_format_type;

typedef enum
{
  kpse_glyph_source_normal,  /* the searched-for font: already existed */
  kpse_glyph_source_alias,   /* : was an alias for an existing file */
  kpse_glyph_source_maketex, /* : was created on the fly */
  kpse_glyph_source_fallback /* : wasn't found, but the fallback font was */
} kpse_glyph_source_type;


typedef struct
{
  const char *name;              /* font name found */
  unsigned int dpi;              /* size found, for glyphs */
  kpse_file_format_type format;  /* glyph format found */
  kpse_glyph_source_type source; /* where we found it */
} kpse_glyph_file_type;

#endif

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

#ifdef _WIN32

static HMODULE _dvi_kpathsea_module = NULL;

static char *
_dvi_kpathsea_dll_find(void)
{
    char path[32767];
    char file[MAX_PATH];
    char *iter;
    char *iter2;

    if (!GetEnvironmentVariable("PATH", path, sizeof(path)))
        return NULL;

    iter = path;
    while ((iter2 = strchr(iter, ';')))
    {
        WIN32_FIND_DATA data;
        HANDLE h;
        ptrdiff_t l;

        l = iter2 - iter;
        memcpy(file, iter, l);
        memcpy(file + l, "\\*kpathsea*.dll", sizeof("\\*kpathsea*.dll"));
        h = FindFirstFile(file, &data);
        if (h != INVALID_HANDLE_VALUE)
        {
            char res[MAX_PATH];

            memcpy(res, iter, l);
            res[l] = '\\';
            memcpy(res + l + 1, data.cFileName, strlen(data.cFileName) + 1);
            FindClose(h);
            return strdup(res);
        }

        FindClose(h);
        iter = ++iter2;
    }

    return NULL;
}

# define DVI_FCT_MIKTEX_GET(sym) \
    _dvi_##sym = (_dvi_##sym##_t)GetProcAddress(_dvi_kpathsea_module, "miktek" #sym)

# define DVI_FCT_TEXLIVE_GET(sym) \
    _dvi_##sym = (_dvi_##sym##_t)GetProcAddress(_dvi_kpathsea_module, #sym)

# define DVI_FCTS_GET(tex)                   \
    do \
    { \
        DVI_FCT_##tex##_GET(kpathsea_new); \
        DVI_FCT_##tex##_GET(kpathsea_set_program_name); \
        DVI_FCT_##tex##_GET(kpathsea_init_prog); \
        DVI_FCT_##tex##_GET(kpathsea_find_file); \
        DVI_FCT_##tex##_GET(kpathsea_find_glyph); \
        DVI_FCT_##tex##_GET(kpathsea_open_file); \
        DVI_FCT_##tex##_GET(kpathsea_finish); \
    } while (0)

#define DVI_FCTS_TEST \
    (_dvi_kpathsea_new && \
     _dvi_kpathsea_set_program_name && \
     _dvi_kpathsea_init_prog && \
     _dvi_kpathsea_find_file && \
     _dvi_kpathsea_find_glyph && \
     _dvi_kpathsea_open_file && \
     _dvi_kpathsea_finish)

static unsigned char
_dvi_kpathsea_functions_set(void)
{
    char *dll;

# ifdef KPATHSEA_DLL
    _dvi_kpathsea_module = LoadLibrary(KPATHSEA_DLL);
    if (_dvi_kpathsea_module)
    {
        DVI_FCTS_GET(MIKTEX);
        if (!DVI_FCTS_TEST)
        {
            DVI_FCTS_GET(TEXLIVE);
            if (!DVI_FCTS_TEST)
                goto free_mod;
        }

        return 1;

      free_mod:
        FreeLibrary(_dvi_kpathsea_module);
        _dvi_kpathsea_module = NULL;
    }
# endif
    dll = _dvi_kpathsea_dll_find();
    if (!dll)
    {
        DVI_LOG_ERR("DLL not found.");
        return 0;
    }

    DVI_LOG_INFO("kpathsea DLL found: %s.", dll);
    _dvi_kpathsea_module = LoadLibrary(dll);
    if (!_dvi_kpathsea_module)
    {
        DVI_LOG_ERR("kpathsea DLL %s can not be loaded.", dll);
        free(dll);
        return 0;
    }
    free(dll);

    DVI_FCTS_GET(MIKTEX);
    if (!DVI_FCTS_TEST)
    {
        DVI_FCTS_GET(TEXLIVE);
        if (!DVI_FCTS_TEST)
        {
            FreeLibrary(_dvi_kpathsea_module);
            _dvi_kpathsea_module = NULL;
            return 0;
        }
    }

    return 1;
}

#endif

/**
 * @endcond
 */

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/

unsigned char
dvi_kpath_sea_init(void)
{
#ifdef _WIN32
    if (!_dvi_kpathsea_functions_set())
        return 0;
#else
    _dvi_kpathsea_new = kpathsea_new;
    _dvi_kpathsea_set_program_name = kpathsea_set_program_name;
    _dvi_kpathsea_init_prog = kpathsea_init_prog;
    _dvi_kpathsea_find_file = kpathsea_find_file;
    _dvi_kpathsea_find_glyph = kpathsea_find_glyph;
    _dvi_kpathsea_open_file = kpathsea_open_file;
    _dvi_kpathsea_finish = kpathsea_finish;
#endif

    _dvi_kpathsea_instance = _dvi_kpathsea_new();
    if (!_dvi_kpathsea_instance)
    {
        DVI_LOG_ERR("kpathsea_new failed");
#ifdef _WIN32
        if (_dvi_kpathsea_module)
            FreeLibrary(_dvi_kpathsea_module);
#endif
        return 0;
    }

    return 1;
}

void
dvi_kpathsea_shutdown(void)
{
    _dvi_kpathsea_finish(_dvi_kpathsea_instance);
#ifdef _WIN32
    if (_dvi_kpathsea_module)
    {
        DVI_LOG_ERR("on libere le module");
        FreeLibrary(_dvi_kpathsea_module);
    }
#endif
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
