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
#include <string.h>

#include <stdio.h> /* for kpathsea */

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


struct _Dvi_Kpathsea
{
#ifdef _WIN32
    HMODULE mod;
#endif
    char *prog_name;

    kpathsea (*kpathsea_new)(void);
    void (*kpathsea_set_program_name)(kpathsea kpse,
                                      const char *argv0,
                                      const char *progname);
    void (*kpathsea_init_prog)(kpathsea kpse,
                               const char *prefix,
                               unsigned int dpi,
                               const char *mode,
                               const char *fallback);
    char *(*kpathsea_find_file)(kpathsea kpse,
                                const char *name,
                                kpse_file_format_type format,
                                int must_exist);
    char *(*kpathsea_find_glyph)(kpathsea kpse,
                                 const char *font_name,
                                 unsigned int dpi,
                                 kpse_file_format_type format,
                                 kpse_glyph_file_type *glyph_file);
    FILE *(*kpathsea_open_file)(kpathsea kpse,
                                const char *name,
                                kpse_file_format_type format);
    void (*kpathsea_finish)(kpathsea kpse);
};


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

#define DVI_FCT_MIKTEX_GET(sym) \
    kps->sym = (_dvi_##sym##_t)GetProcAddress(kps->mod, "miktek" #sym)

#define DVI_FCT_TEXLIVE_GET(sym) \
    kps->sym = (_dvi_##sym##_t)GetProcAddress(kps->mod, #sym)

#define DVI_FCTS_GET(tex)                   \
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
    (kps->kpathsea_new && \
     kps->kpathsea_set_program_name && \
     kps->kpathsea_init_prog && \
     kps->kpathsea_find_file && \
     kps->kpathsea_find_glyph && \
     kps->kpathsea_open_file && \
     kps->kpathsea_finish)

static unsigned char
_dvi_kpathsea_functions_set(Dvi_Kpathsea *kps)
{
    char *dll;

#ifdef KPATHSEA_DLL
    kps->mod = LoadLibrary(KPATHSEA_DLL);
    if (kps->mod)
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
        FreeLibrary(kps->mod);
        kps->mod = NULL;
    }
#endif
    dll = _dvi_kpathsea_dll_find();
    if (!dll)
    {
        DVI_LOG_ERR("DLL not found.");
        return 0;
    }
    DVI_LOG_INFO("kpathsea DLL  found: %s.", dll);
    kps->mod = LoadLibrary(dll);
    free(dll);
    if (!kps->mod)
        return 0;

    DVI_FCTS_GET(MIKTEX);
    if (!DVI_FCTS_TEST)
    {
        DVI_FCTS_GET(TEXLIVE);
        if (!DVI_FCTS_TEST)
        {
            FreeLibrary(kps->mod);
            kps->mod = NULL;
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

Dvi_Kpathsea *
dvi_kpathsea_new(const char *prog_name)
{
    Dvi_Kpathsea *kps;

    kps = (Dvi_Kpathsea *)calloc(1, sizeof(Dvi_Kpathsea));
    if (!kps)
        return NULL;

    kps->prog_name = strdup(prog_name);
    if (!kps->prog_name)
        goto free_kps;

    if (!_dvi_kpathsea_functions_set(kps))
        goto free_prog_name;

    return kps;

  free_prog_name:
    free(kps->prog_name);
  free_kps:
    free(kps);

    return NULL;
}

void
dvi_kpathsea_free(Dvi_Kpathsea *kps)
{
    if (!kps)
        return;

    if (kps->mod)
        FreeLibrary(kps->mod);
    free(kps->prog_name);
    free(kps);
}

char *
dvi_kpathsea_path_name_get(Dvi_Kpathsea *kps, const char *name)
{
    kpathsea kpse;
    char *n;

    kpse = kps->kpathsea_new();
    if (!kpse)
    {
        DVI_LOG_ERR("_dvi_kpathsea_new");
    }

    kps->kpathsea_set_program_name(kpse, "dvi.exe", NULL);
    kps->kpathsea_init_prog (kpse, "dvi.exe", 300, NULL, NULL);
    DVI_LOG_ERR("fonte: %s", name);
    n = kps->kpathsea_find_file(kpse, name, kpse_tfm_format, 1);
    DVI_LOG_ERR(" * name : %s", n);
    kps->kpathsea_finish(kpse);
    return n;
}



/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
