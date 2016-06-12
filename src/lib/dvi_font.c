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

#include "Dvi.h"
#include "dvi_private.h"
#include "dvi_log.h"
#include "dvi_map.h"
#include "dvi_kpathsea.h"
#include "dvi_font.h"
#include "dvi_stack.h"
#include "dvi_document_private.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */

#define DVI_TFM_WORD_READ \
    do \
    { \
    b0 = *tfm_cur_loc++; \
    b1 = *tfm_cur_loc++; \
    b2 = *tfm_cur_loc++; \
    b3 = *tfm_cur_loc++; \
    } while (0)

/**
 * @brief Input TFM data or return false.
 *
 * @param z The scaling factor.
 * @return 1 on success, 0 otherwise.
 *
 * This function absorbs the necessary information from the TFM file.
 *
 * @internal
 */
static unsigned char
_dvi_font_in_tfm(Dvi_Fonts *fontes, int z,
                 double conv, double tfm_conv,
                 int *tfm_check_sum, int *tfm_design_size)
{
    /* const unsigned char *tfm_cur_loc; */
    /* unsigned int nf; */
    /* int k; /\* index for loop [34] *\/ */
    /* int lh; /\* length of the header data, in four-byte words [34] *\/ */
    /* int nw; /\* number of words in the width table [34] *\/ */
    /* int width_ptr; /\* number of known character widths [30] *\/ */
    /* int wp; /\* new value of width_ptr after succesful input [34] *\/ */
    /* int alpha; */
    /* int beta; */
    /* unsigned char b0; */
    /* unsigned char b1; */
    /* unsigned char b2; */
    /* unsigned char b3; */
    /* unsigned char res = 1; */

    /* nf = fontes->nf; */
    /* width_ptr = 0; */
    /* tfm_cur_loc = dvi_map_base_get(fontes->fonts[nf].map); */

    /* /\* Read past the header data [35] *\/ */
    /* DVI_TFM_WORD_READ; */
    /* lh = (b2 << 8) + b3; /\* b2 * 256 + b3 *\/ */

    /* DVI_TFM_WORD_READ; */
    /* fontes->fonts[nf].bc = (b0 << 8) + b1; /\* b0 * 256 + b1 *\/ */
    /* fontes->fonts[nf].ec = (b2 << 8) + b3; /\* b2 * 256 + b3 *\/ */
    /* if (fontes->fonts[nf].ec < fontes->fonts[nf].bc) */
    /*     fontes->fonts[nf].bc = fontes->fonts[nf].ec + 1; */

    /* if ((width_ptr + fontes->fonts[nf].ec - fontes->fonts[nf].bc + 1) > DVI_MAX_WIDTHS) */
    /* { */
    /*     DVI_LOG_ERR("Table width not large enough."); */
    /*     return 0; */
    /* } */

    /* wp = width_ptr + fontes->fonts[nf].ec - fontes->fonts[nf].bc + 1; */
    /* DVI_TFM_WORD_READ; */
    /* nw = (b0 << 8) + b1; /\* b0 * 256 + b1 *\/ */
    /* if ((nw == 0) || (nw > 256)) */
    /* { */
    /*     DVI_LOG_ERR("Invalid TFM file."); */
    /*     return 0; */
    /* } */

    /* for (k = 1; k <= (3 + lh); k++) */
    /* { */
    /*     if ((tfm_cur_loc - dvi_map_base_get(fontes->fonts[nf].map)) >= (ptrdiff_t)dvi_map_length_get(fontes->fonts[nf].map)) */
    /*     { */
    /*         DVI_LOG_ERR("Invalid TFM file."); */
    /*         return 0; */
    /*     } */

    /*     DVI_TFM_WORD_READ; */
    /*     if (k == 4) */
    /*     { */
    /*         if (b0 < 128) */
    /*             *tfm_check_sum = ((b0 * 256 + b1) *256 + b2) * 256 + b3; */
    /*         else */
    /*             *tfm_check_sum = (((b0 - 256) * 256 + b1) *256 + b2) * 256 + b3; */
    /*     } */
    /*     else if (k == 5) */
    /*     { */
    /*         if (b0 < 128) */
    /*             *tfm_design_size = dvi_round(tfm_conv * (((b0 * 256 + b1) *256 + b2) * 256 + b3)); */
    /*         else */
    /*         { */
    /*             DVI_LOG_ERR("Invalid TFM file."); */
    /*             return 0; */
    /*         } */
    /*     } */
    /* } */

    /* /\* Store character-width indices at the end of the width table [36] *\/ */
    /* if (wp > 0) */
    /* { */
    /*     for (k = width_ptr; k < wp; k++) */
    /*     { */
    /*         DVI_TFM_WORD_READ; */
    /*         if (b0 > nw) */
    /*         { */
    /*             DVI_LOG_ERR("Invalid TFM file."); */
    /*             return 0; */
    /*         } */
    /*         width[k] = b0; */
    /*     } */
    /* } */

    /* /\* Read and convert the width values, setting up the in_width table [37] *\/ */
    /* /\* Replace z by z' and compute alpha and beta [38] *\/ */
    /* alpha = 16; */
    /* while (z >= 040000000) */
    /* { */
    /*     z /= 2; */
    /*     alpha += alpha; */
    /* } */
    /* beta = 256 / alpha; */
    /* alpha *= z; */

    /* for (k = 0; k < nw; k++) */
    /* { */
    /*     DVI_TFM_WORD_READ; */
    /*     in_width[k] = (((((b3 * z) / 0400) + (b2 * z)) / 0400) + (b1 * z)) / beta; */
    /*     if (b0 > 0) */
    /*     { */
    /*         if (b0 < 255) */
    /*         { */
    /*             DVI_LOG_ERR("Invalid TFM file."); */
    /*             return 0; */
    /*         } */
    /*         else */
    /*             in_width[k] = in_width[k] - alpha; */
    /*     } */
    /* } */

    /* /\* move the widths from in_width to width, and append pixel_width values [40] *\/ */
    /* if (in_width[0] != 0) */
    /* { */
    /*     DVI_LOG_ERR("Invalid TFM file."); */
    /*     return 0; */
    /* } */

    /* width_base[nf] = width_ptr - fontes->fonts[nf].bc; */
    /* if (wp > 0) */
    /* { */
    /*     for (k = width_ptr; k < wp; k++) */
    /*     { */
    /*         if (width[k] == 0) */
    /*         { */
    /*             width[k] = 0x7fffffff; /\* 017777777777 in dvitype *\/ */
    /*             pixel_width[k] = 0; */
    /*         } */
    /*         else */
    /*         { */
    /*             width[k] = in_width[width[k]]; */
    /*             pixel_width = dvi_round(conv * width[k]); */
    /*         } */
    /*     } */
    /* } */

    /* width_ptr = wp; */

    return 1;
}


/**
 * @endcond
 */

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


void
dvi_font_define(const Dvi_Document *doc,
                const unsigned char **cur_loc,
                int e)
{
    char *name;
    const unsigned char *iter;
    unsigned int nf;
    unsigned int f;
    int tfm_check_sum = 0;
    int tfm_design_size = 0;
    int check_sum;
    int scaled_size;
    int design_size;
    int magnification;
    unsigned char a;
    unsigned char l;

    nf = doc->fontes->nf;
    iter = *cur_loc;

    for (f = 0; f < nf; f++)
    {
        if (doc->fontes->fonts[f].num == e)
            break;
    }

    /*
     * Read the font parameters into position for font nf
     * and print the font name [61]
     */
    check_sum = dvi_read_sint32(iter);
    iter += 4;
    scaled_size = dvi_read_sint32(iter);
    iter += 4;
    design_size = dvi_read_sint32(iter);
    iter += 4;
    if ((scaled_size <= 0) || (design_size <= 0))
        magnification = 1000;
    else
        magnification =
            dvi_round((1000.0 * dvi_document_conv_get(doc) * scaled_size) /
                      (dvi_document_true_conv_get(doc) * design_size));

    a = dvi_read_uint8(iter);
    iter++;
    l = dvi_read_uint8(iter);
    iter++;
    if ((a == 0) && (l == 0))
    {
        name = NULL;
        DVI_LOG_ERR("[Fntdef] font %d has no name", e);
    }
    else
    {
        name = (char *)malloc((a + l + 1) * sizeof(char));
        if (name)
            memcpy(name, iter, a);
        else
            DVI_LOG_ERR("[Fntdef] Can not allocate memory to store the name of font %d", e);
    }
    iter += a;
    if (name)
    {
        memcpy(name + a, iter, l);
        name[a + l] = '\0';
        if ((scaled_size <= 0) || (design_size <= 0))
            DVI_LOG_INFO("[Fntdef] Font %s found, not scaled.",
                         name);
        else
            DVI_LOG_INFO("[Fntdef] Font %s found, scaled at %d.",
                         name, magnification);
    }
    iter += l;

    doc->fontes->fonts[nf].num = e;
    doc->fontes->fonts[nf].check_sum = check_sum;
    doc->fontes->fonts[nf].scaled_size = scaled_size;
    doc->fontes->fonts[nf].design_size = design_size;
    doc->fontes->fonts[nf].name = name;

    if (f == nf)
    {
        /* Load the new font, unless there are problems [62] */

        size_t len;
        char *ext;
        const char *tfm_path;

        /* Add .tfm to the file name if no extension [66] */
        if (name)
        {
            ext = strrchr(name + a, '.');
            if (!ext)
            {
                len = strlen(name);
                name = realloc(name, len + 5);
                if (name)
                {
                    name[len] = '.';
                    name[len + 1] = 't';
                    name[len + 2] = 'f';
                    name[len + 3] = 'm';
                    name[len + 4] = '\0';
                }
                doc->fontes->fonts[nf].name = name;
            }
        }

        tfm_path = dvi_kpathsea_path_name_get(doc->fontes->fonts[nf].name);
        if (!tfm_path)
        {
            DVI_LOG_INFO("[Fntdef] TFM font %s can not be found",
                         doc->fontes->fonts[nf].name);
            goto free_name;
        }
        else
        {
            DVI_LOG_INFO("[Fntdef] TFM font found in %s", tfm_path);
            doc->fontes->fonts[nf].map = dvi_map_new(tfm_path);
            if (!doc->fontes->fonts[nf].map)
            {
                DVI_LOG_ERR("[Fntdef] Font %s can not be opened.",
                            doc->fontes->fonts[nf].name);
                goto free_name;
            }
            else if ((doc->fontes->fonts[nf].scaled_size <= 0) ||
                     (doc->fontes->fonts[nf].scaled_size >= 01000000000))
            {
                DVI_LOG_ERR("[Fntdef] Font %s has bad scale %d.",
                            doc->fontes->fonts[nf].name,
                            doc->fontes->fonts[nf].scaled_size);
                goto del_map;
            }
            else if ((doc->fontes->fonts[nf].design_size <= 0) ||
                     (doc->fontes->fonts[nf].design_size >= 01000000000))
            {
                DVI_LOG_ERR("[Fntdef] Font %s has bad design size %d.",
                            doc->fontes->fonts[nf].name,
                            doc->fontes->fonts[nf].design_size);
                goto del_map;
            }
            else if (_dvi_font_in_tfm(doc->fontes,
                                      doc->fontes->fonts[nf].scaled_size,
                                      doc->conv,
                                      doc->tfm_conv,
                                      &tfm_check_sum,
                                      &tfm_design_size))
            {
                /* Finish loading the new font info [63] */
                doc->fontes->fonts[nf].space = scaled_size % 6;
                if ((check_sum != 0) &&
                    (tfm_check_sum != 0) &&
                    (check_sum != tfm_check_sum))
                    DVI_LOG_WARN("[Fntdef] check sums do not agree (%d != %d).",
                                 check_sum, tfm_check_sum);
                if (abs(tfm_design_size - design_size) > 2)
                    DVI_LOG_WARN("[Fntdef] design sizes do not agree (%d, %d).",
                                 design_size, tfm_design_size);
                DVI_LOG_INFO("[Fntdef] Font %s [%d] loaded at size %d DVI units.",
                             doc->fontes->fonts[nf].name, e, scaled_size);
                design_size = dvi_round((100.0 * dvi_document_conv_get(doc) * scaled_size) / (dvi_document_true_conv_get(doc) * design_size));
                if (design_size != 100)
                    DVI_LOG_INFO("[Fntdef] Font %s magnified at %d.",
                                 doc->fontes->fonts[nf].name, design_size);
                doc->fontes->nf++;
            }
        }

        *cur_loc = iter;

        return;

      del_map:
        dvi_map_del(doc->fontes->fonts[nf].map);
      free_name:
        if (doc->fontes->fonts[nf].name)
            free(doc->fontes->fonts[nf].name);

        return;
    }
    else
    {
        /* f < nf : it is unlikely that we reach this code */
        DVI_LOG_INFO("[Fntdef] Font %d already defined.", e);
        /* Check that the current font definition matches the old one [60] */
        if (doc->fontes->fonts[f].check_sum != doc->fontes->fonts[nf].check_sum)
            DVI_LOG_ERR("[Fntdef] Check sum of font %d does not match previous definition.", e);
        if (doc->fontes->fonts[f].scaled_size != doc->fontes->fonts[nf].scaled_size)
            DVI_LOG_ERR("[Fntdef] Scaled size of font %d does not match previous definition.", e);
        if (doc->fontes->fonts[f].design_size != doc->fontes->fonts[nf].design_size)
            DVI_LOG_ERR("[Fntdef] Scaled size of font %d does not match previous definition.", e);
        if (strcmp(doc->fontes->fonts[f].name, doc->fontes->fonts[nf].name) != 0)
            DVI_LOG_ERR("[Fntdef] Name of font %d does not match previous definition.", e);
    }
}

void
dvi_font_del(Dvi_Fonts *fontes)
{
    unsigned int i;

    if (!fontes)
        return;

    for (i = 0; i < fontes->nf; i++)
    {
        dvi_map_del(fontes->fonts[i].map);
        free(fontes->fonts[i].name);
    }

    free(fontes->fonts);
    free(fontes);
}


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
