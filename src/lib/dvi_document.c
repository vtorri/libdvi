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
#include "dvi_log.h"
#include "dvi_font.h"
#include "dvi_interpreter.h"
#include "dvi_document.h"
#include "dvi_document_private.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/


/**
 * @brief Create a new DVI document.
 *
 * @param[in] filename The DVI file name.
 * @return A newly allocated #Dvi_Document object, or @c NULL on failure.
 *
 * This function creates a #Dvi_Document object.
 *
 * Design: [107]
 * @li Open the file @p filename
 * @li Process the preamble [107] [109] [110] (see dvi_interpreter_preamble_parse())
 * @li Compute the conversion factors [107] [109] [110]
 * @li Process the postspostamble [107] [100]
 * @li Process the postamble [107] [103]
 * @li Process the font definition of the postamble [107] [103] [106] [59] [61]
 */
DAPI Dvi_Document *
dvi_document_new(const char *filename)
{
    Dvi_Document *doc;
    int num;           /* preamble num, signed quad [110] */
    int den;           /* preamble num, signed quad [110] */
    int mag;           /* preamble num, signed quad [110] */
    int postamble_loc; /* postpostamble postamble loc, signed quad [100] */
    int pnum;          /* postamble num, signed quad [103] */
    int pden;          /* postamble num, signed quad [103] */
    int pmag;          /* postamble num, signed quad [103] */
    int final_bop;     /* postamble final bop location, signed quad [100] */
    unsigned short max_s; /* postamble stack depth, unsigned two bytes [103] */
    unsigned short total_pages; /* postamble nbr of pages; unsigned two bytes [103] */
    unsigned char id;  /* unsigned byte [109] */
    unsigned char pid; /* postpostamble id, unsigned byte [100] */

    if (!filename || !*filename)
    {
        DVI_LOG_ERR("Filename is NULL or empty");
        return NULL;
    }

    doc = (Dvi_Document *)calloc(1, sizeof(Dvi_Document));
    if (!doc)
    {
        DVI_LOG_ERR("Can not allocate memory for document object");
        return NULL;
    }

    doc->filename = strdup(filename);
    if (!doc->filename)
    {
        DVI_LOG_ERR("Can not allocate memory for file name %s",
                    filename);
        goto free_doc;
    }

    doc->map = dvi_map_new(filename);
    if (!doc->map)
        goto free_filename;

    /*
     * A DVI file must contain at least:
     *  - a preamble (1 + 1 + 4 + 4 + 4 + 1 bytes = 15 bytes)
     *  - a postamble (1 + 4 + 4 + 4 + 4 + 4 + 4 + 2 + 2 bytes = 29 bytes)
     *  - a postpostamble (1 + 4 + 1 + at least 4 bytes = 10 bytes)
     * that is a total of 54 bytes. So we exit if size if less than 54 bytes
     */
    if (dvi_map_length_get(doc->map) < 54)
    {
        DVI_LOG_ERR("File %s is not big enough and hence not a DVI file",
                    filename);
        goto del_map;
    }

    /* process the preamble: [109] and [110] */
    if (!dvi_interpreter_preamble_parse(doc,
                                        &id, &num, &den, &mag, &doc->comment))
    {
        DVI_LOG_ERR("Malformed preamble in DVI file");
        goto del_map;
    }

    /* compute the conversion factors: [110] */
    doc->true_conv = (num / 254000.0) * (DVI_RESOLUTION / den);
    doc->conv = doc->true_conv * (mag / 1000.0);
    doc->tfm_conv = (25400000.0 / num) * (den / 473628672.0) / 16.0;
    DVI_LOG_INFO("[Preamble] Conversion factor: %16.8f pixels per DVI units.", doc->conv);

    /* process the postpostamble: [100]] */
    if (!dvi_interpreter_postpostamble_parse(doc, &pid, &postamble_loc))
    {
        DVI_LOG_ERR("Malformed postpostamble in DVI file");
        goto free_comment;
    }

    if (id != pid)
    {
        DVI_LOG_ERR("Identifier in preamble and postpostamble mismatch (%hhu != %hhu)",
                    id, pid);
        goto free_comment;
    }

    /* process the postamble: [100] */
    /* we jump to the POST command */
    if (!dvi_interpreter_postamble_parse(doc,
                                         postamble_loc,
                                         &final_bop,
                                         &pnum,
                                         &pden,
                                         &pmag,
                                         &max_s,
                                         &total_pages))
    {
        DVI_LOG_ERR("Malformed postamble in DVI file");
        goto free_comment;
    }

    if (num != pnum)
    {
        DVI_LOG_ERR("Numerators in preamble and postamble mismatch (%d != %d)",
                    num, pnum);
        goto free_comment;
    }

    if (den != pden)
    {
        DVI_LOG_ERR("Denominators in preamble and postamble mismatch (%d != %d)",
                    den, pden);
        goto free_comment;
    }

    if (mag != pmag)
    {
        DVI_LOG_ERR("Magnifications in preamble and postamble mismatch (%d != %d)",
                    mag, pmag);
        goto free_comment;
    }

    doc->stack  = dvi_stack_new(max_s);
    if (!doc->stack)
    {
        DVI_LOG_ERR("Can not allocate memory for the stack");
        goto free_comment;
    }

    return doc;

  free_comment:
    free(doc->comment);
  del_map:
    dvi_map_del(doc->map);
  free_filename:
    free(doc->filename);
  free_doc:
    free(doc);

    return NULL;
}

DAPI void
dvi_document_del(Dvi_Document *doc)
{
    if (!doc)
        return;

    dvi_font_del(doc->fontes);
    dvi_stack_del(doc->stack);
    free(doc->comment);
    dvi_map_del(doc->map);
    free(doc->filename);
    free(doc);
}
