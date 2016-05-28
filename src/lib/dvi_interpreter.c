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
#include "dvi_opcodes.h"
/* #include "dvi_font.h" */
#include "dvi_document_private.h"
#include "dvi_interpreter.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/


#ifdef _WIN32
# define DVI_FMT_HHU "%hu"
# define DVI_FMT_ZU "%Iu"
#else
# define DVI_FMT_HHU "%hhu"
# define DVI_FMT_ZU "%zu"
#endif

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


/**
 * @brief Process the preamble.
 *
 * @param[in] cur_loc The current location in the file.
 * @param[out] id DVI version supported by the DVI file.
 * @param[out] num The numerator of the fraction of decimicron per DVI unit.
 * @param[out] den The denominator of the fraction of decimicron per DVI unit.
 * @param[out] mag The desired magnification.
 * @param[out] comment The introductory comment in the DVI file.
 * @return 1 on success, 0 otherwise.
 *
 * This function processes the preamble. @p comment is a newly
 * allocated string and must be freed when not used anymore.

 * @note The global variable @c new_mag is considered to be 0, see [41]
 *
 * Design: [17] [109] [110]
 * @li Verify that the first byte is the preamble command [109]
 * @li Read and check the Id of the DVI file [109]
 * @li Read and check the numerator and denominator of the fraction [110]
 * @li Read and check the desired magnification [110]
 * @li Retrieve the introductory comment [109]
 *
 * @see dvi_document_new()
 *
 * @internal
 */
unsigned char
dvi_interpreter_preamble_parse(const Dvi_Document *doc,
                               unsigned char *id, /* unsigned byte [109] */
                               int *num,          /* signed quad [110] */
                               int *den,          /* signed quad [110] */
                               int *mag,          /* signed quad [110] */
                               char **comment)
{
    const unsigned char *cur_loc;
    unsigned char k;
    char *c;

    DVI_LOG_DBG("[Preamble] start processing.");

    cur_loc = dvi_document_base_get(doc);

    if (dvi_read_uint8(cur_loc) != DVI_OPCODE_PRE)
    {
        DVI_LOG_ERR("[Preamble] wrong opcode: %u (should be %u)",
                    dvi_read_uint8(cur_loc), DVI_OPCODE_PRE);
        return 0;
    }

    cur_loc++;

    *id = dvi_read_uint8(cur_loc);
    if ((*id != 2) && (*id != 3) && (*id != 5))
    {
        DVI_LOG_ERR("[Preamble] wrong identifer byte: %u (should be2, 3 or 5)", *id);
        return 0;
    }

    cur_loc++;

    *num = dvi_read_sint32(cur_loc);
    if (*num <= 0)
    {
        DVI_LOG_ERR("[Preamble] numerator is non positive: %d", *num);
        return 0;
    }

    cur_loc += 4;

    *den = dvi_read_sint32(cur_loc);
    if (*den <= 0)
    {
        DVI_LOG_ERR("[Preamble] denominator is non positive: %d", *den);
        return 0;
    }

    DVI_LOG_INFO("[Preamble] resolution: %d/%d", *num, *den);

    cur_loc += 4;

    /*
     * In [110], a check is done on 'new_mag'. But we are in mode 'the_works",
     * so, as stated in [41], we consider 'new_mag' to be equal to 0.
     */
    *mag = dvi_read_sint32(cur_loc);
    if (*mag <= 0)
    {
        DVI_LOG_ERR("[Preamble] magnification is non positive: %d", *mag);
        return 0;
    }

    DVI_LOG_INFO("[Preamble] magnification: %d", *mag);

    cur_loc += 4;

    *comment = NULL;
    k = dvi_read_uint8(cur_loc);
    if (k > 0)
    {
        /* the comment in a DVI file is not a null terminated string */
        c = (char *)malloc((k + 1) * sizeof(char *));
        if (c)
        {
            memcpy(c, ++cur_loc, k);
            c[k] = '\0';
            *comment = c;
            DVI_LOG_INFO("[Preamble] comment: %s", *comment);
        }
        else
        {
            *comment = NULL;
            DVI_LOG_ERR("[Preamble] Can not allocate memory for DVI file comment");
        }
    }
    else
    {
        *comment = NULL;
        DVI_LOG_INFO("[Preamble] DVI file has no comment");
    }

    DVI_LOG_DBG("[Preamble] processing finished.");

    return 1;
}

/**
 * @brief Process the postpostamble.
 *
 * @param[in] base The base (start) address of the DVI file.
 * @param[in] length The size in bytes of the DVI file.
 * @param[out] id DVI version supported by the DVI file.
 * @param[out] postamble_loc The location of the postamble relative to
 * the base address.
 * @return 1 on success, 0 otherwise.
 *
 * This function processes the postpostamble, reading from the last
 * byte, then backward.
 *
 * Design: [20] [105]
 * @li Read and check the values 223 [105]
 * @li Read and check the Id value [105]
 * @li Read and check the postamble location
 *
 * @see dvi_document_new()
 *
 * @internal
 */
unsigned char
dvi_interpreter_postpostamble_parse(const Dvi_Document *doc,
                                    unsigned char *id,  /* unsigned byte [105] */
                                    int *postamble_loc) /* signed quad [105] */
{
    const unsigned char *cur_loc;
    const unsigned char *base;
    size_t length;
    size_t nbr;

    DVI_LOG_DBG("[Postpostamble] start processing.");

    /*
     * Parsing the file backward from the end to get POST and POSTPOST
     * parameters
     */
    base = dvi_document_base_get(doc);
    length = dvi_document_length_get(doc);
    cur_loc = base + (length - 1);
    while (*cur_loc == 223)
    {
        cur_loc--;
        /*
         * Another safety check: we must not go too far:
         *  - a preamble (1 + 1 + 4 + 4 + 4 + 1 + k bytes = 15 + k bytes)
         *  - a postamble (1 + 4 + 4 + 4 + 4 + 4 + 4 + 2 + 2 bytes = 29 bytes)
         *  - a postpostamble (1 + 4 + 1 bytes = 6 bytes)
         * that is a total of 50 + k bytes. So we exit if we go beyond
         * 50 + k bytes.
         */
        if ((cur_loc - base) < (50 + base[14]))
        {
            DVI_LOG_ERR("[Postpostamble]: too much trailing bytes");
            return 0;
        }
    }

    nbr = length - (cur_loc - base);
    DVI_LOG_DBG("[Postpostamble]: number of trailing bytes: " DVI_FMT_ZU, nbr);
    /*
     * TeX appends 4 to 7 trailing bytes, but any number of trailing bytes
     * is allowed. [20]
     */
    if (nbr < 4)
    {
        DVI_LOG_ERR("[Postpostamble]: wrong number of trailing bytes: " DVI_FMT_ZU, nbr);
        return 0;
    }

    *id = dvi_read_uint8(cur_loc);

    cur_loc -= 4;
    *postamble_loc = dvi_read_sint32(cur_loc);
    if ((*postamble_loc < 0) || (*postamble_loc > ((cur_loc -base) - 33)))
    {
        DVI_LOG_ERR("[Postpostamble]: wrong post pointer %zu (should be %d)",
                    cur_loc - base, *postamble_loc);
        return 0;
    }

    cur_loc--;
    if (dvi_read_uint8(cur_loc) != DVI_OPCODE_POST_POST)
    {
        DVI_LOG_ERR("[Postpostamble]:  wrong opcode for postpostamble %hhu (should be %hhu)",
                    dvi_read_uint8(cur_loc), DVI_OPCODE_POST_POST);
        return 0;
    }

    DVI_LOG_DBG("[Postpostamble] processing finished.");

    return 1;
}

/**
 * @brief Process the postamble.
 *
 * @param[in] cur_loc The current location in the file.
 * @param[out] final_bop The location of the final beginning of page,
 * relative to the base address.
 * @param[out] num The numerator of the fraction of decimicron per DVI unit.
 * @param[out] den The denominator of the fraction of decimicron per DVI unit.
 * @param[out] mag The desired magnification.
 * @param[out] max_s The maximum stack depth.
 * @param[out] total_page The total number of pages.
 * @return 1 on success, 0 otherwise.
 *
 * This function processes the postpostamble, reading from the last
 * byte, then backward.
 *
 * Design: [19] [103]
 * @li Read the final beginning of page
 * @li Read and check the numerator and denominator of the fraction [103]
 * @li Read and check the desired magnification [103]
 * @li Read the maximum stack depth [103]
 * @li Read the total number of page [103]
 * @li Process the font definitions [103] [106] [59] [61]
 *
 * @see dvi_interpreter_fnt_def_parse()
 * @see dvi_document_new()
 *
 * @internal
 */
unsigned char
dvi_interpreter_postamble_parse(Dvi_Document *doc,
                                int postamble_loc,
                                int *final_bop,              /* signed quad [101] */
                                int *num,                    /* signed quad [103] */
                                int *den,                    /* signed quad [103] */
                                int *mag,                    /* signed quad [103] */
                                unsigned short *max_s,       /* unsigned two bytes [103] */
                                unsigned short *total_pages) /* unsigned two bytes [103] */
{
    const unsigned char *cur_loc;

    DVI_LOG_DBG("[Postamble] start processing.");

    cur_loc = dvi_document_base_get(doc) + postamble_loc;

    if (dvi_read_uint8(cur_loc) != DVI_OPCODE_POST)
    {
        DVI_LOG_ERR("[Postamble]: wrong opcode for postamble (is %hhu, should be %hhu)",
                    dvi_read_uint8(cur_loc), DVI_OPCODE_POST);
        return 0;
    }

    cur_loc++;

    /* final bop */

    *final_bop = dvi_read_sint32(cur_loc);

    cur_loc += 4;
    *num = dvi_read_sint32(cur_loc);
    if (*num <= 0)
    {
        DVI_LOG_ERR("[Postamble] numerator is non positive: %d", *num);
        return 0;
    }

    cur_loc += 4;
    *den = dvi_read_sint32(cur_loc);
    if (*den <= 0)
    {
        DVI_LOG_ERR("[Postamble] denominator is non positive: %d", *den);
        return 0;
    }

    cur_loc += 4;
    *mag = dvi_read_sint32(cur_loc);
    if (*mag <= 0)
    {
        DVI_LOG_ERR("[Postamble] magnification is non positive: %d", *mag);
        return 0;
    }

    cur_loc += 4;
    /* l parameter (height+depth of tallest page, ignored, see [19] and [103] */

    cur_loc += 4;
    /* u parameter (width of widest page, ignored, see [19] and [103] */

    cur_loc += 4;
    *max_s = dvi_read_uint16(cur_loc);

    cur_loc += 2;
    *total_pages = dvi_read_uint16(cur_loc);

    cur_loc += 2;

    if (!dvi_interpreter_fnt_def_parse(doc, cur_loc))
        return 0;

    DVI_LOG_DBG("[Postamble] processing finished.");

    return 1;
}

/**
 * @brief Process the font definitions ot the postamble
 *
 * @param[in] doc The document.
 * @param[in] cur_loc The current location in the file.
 * @return 1 on success, 0 otherwise.
 *
 * This function processes the font definitions of the postamble,
 * beginning at @p cur_loc. The number of fonts is stored in
 * @p fonts_count and the definition of each font is stored in the
 * array @p fonts_def. If no font is found (which is very unlikely),
 * @p fonts_count is set to 0 and @p fonts_def is set to @c NULL and 1
 * is returned. This function returns 1 on success, 0 otherwise.
 *
 * Design: [19] [103] [106]
 * @li Read each byte and chec if a fnt_def* opcode is found [106]
 * @li If so, define a font [106] [59] [61]
 *
 * @see dvi_interpreter_postamble_parse()
 *
 * @internal
 */
unsigned char
dvi_interpreter_fnt_def_parse(Dvi_Document *doc,
                              const unsigned char *cur_loc)
{
    Dvi_Fonts *fontes;
    const unsigned char *iter;
    unsigned int nf;

    DVI_LOG_DBG("[Fntdef] start processing.");

    iter = cur_loc;
    nf = 0;

    do
    {
        if (*iter == DVI_OPCODE_NOP)
        {
            DVI_LOG_DBG("[Fntdef] found NOP opcode.");
            iter++;
        }
        else if ((*iter >= DVI_OPCODE_FNT_DEF1) &&
                 (*iter <= DVI_OPCODE_FNT_DEF4))
        {
            int scaled_size;
            int design_size;

            iter += 1 + (*iter - DVI_OPCODE_FNT_DEF1 + 1) + 4;
            /* Check if scaled_size and design_size are > 0 or not */
            scaled_size = dvi_read_uint32(iter);
            iter += 4;
            design_size = dvi_read_uint32(iter);
            iter += 4 + 1 + 1 + *(iter + 4) + *(iter + 5);
            if ((scaled_size > 0) &&
                (scaled_size < 01000000000) &&
                (design_size > 0) &&
                (design_size < 01000000000))
                nf++;
        }
        else
        {
            DVI_LOG_ERR("[Fntdef] opcode different from NOP or FNT_DEF* (found opcode " DVI_FMT_HHU ")",
                        *iter);
            return 0;
        }

    } while (*iter != DVI_OPCODE_POST_POST);

    if (nf == 0)
    {
        DVI_LOG_WARN("[Fntdef] No font found.");
        return 0;
    }

    fontes = (Dvi_Fonts *)calloc(1, sizeof(Dvi_Fonts));
    if (!fontes)
    {
        DVI_LOG_ERR("can not allocate memory for fonts");
        return 0;
    }

    fontes->fonts = (Dvi_Font_Tfm *)calloc(nf, sizeof(Dvi_Font_Tfm));
    if (!fontes->fonts)
    {
        DVI_LOG_ERR("can not allocate memory for TFM fonts");
        free(fontes);
        return 0;
    }

    doc->fontes = fontes;
    doc->fontes->nf = 0;

    do
    {
        if (*cur_loc == DVI_OPCODE_NOP)
            cur_loc++;
        else if ((*cur_loc >= DVI_OPCODE_FNT_DEF1) &&
                 (*cur_loc <= DVI_OPCODE_FNT_DEF4))
        {
            int e; /* external font number, see 'e' in [59] */

            if (*cur_loc == DVI_OPCODE_FNT_DEF1)
            {
                cur_loc++;
                e = dvi_read_sint8(cur_loc);
                cur_loc++;
            }
            else if (*cur_loc == DVI_OPCODE_FNT_DEF2)
            {
                cur_loc++;
                e = dvi_read_sint16(cur_loc);
                cur_loc += 2;
            }
            else if (*cur_loc == DVI_OPCODE_FNT_DEF3)
            {
                cur_loc++;
                e = dvi_read_sint24(cur_loc);
                cur_loc += 3;
            }
            else /* DVI_OPCODE_FNT_DEF4 */
            {
                cur_loc++;
                e = dvi_read_sint32(cur_loc);
                cur_loc += 4;
            }

            dvi_font_define(doc, &cur_loc, e);
        }

    } while (*cur_loc != DVI_OPCODE_POST_POST);

    DVI_LOG_DBG("[Fntdef] processing finished.");

    return 1;
}


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/


