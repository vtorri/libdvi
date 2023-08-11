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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef DVI_LOG_H
#define DVI_LOG_H

#include <stdarg.h>

#define DVI_LOG(l, ...) \
    dvi_log_print(l, __FILE__, __func__, __LINE__, __VA_ARGS__)

#define DVI_LOG_ERR(...) \
    DVI_LOG(DVI_LOG_LEVEL_ERR, __VA_ARGS__)

#define DVI_LOG_WARN(...) \
    DVI_LOG(DVI_LOG_LEVEL_WARN, __VA_ARGS__)

#define DVI_LOG_INFO(...) \
    DVI_LOG(DVI_LOG_LEVEL_INFO, __VA_ARGS__)

#define DVI_LOG_DBG(...) \
    DVI_LOG(DVI_LOG_LEVEL_DBG, __VA_ARGS__)

typedef enum
{
    DVI_LOG_LEVEL_ERR,
    DVI_LOG_LEVEL_WARN,
    DVI_LOG_LEVEL_INFO,
    DVI_LOG_LEVEL_DBG,
    DVI_LOG_LEVEL_LAST
} Dvi_Log_Level;


typedef void (*Dvi_Log_Print_Cb)(Dvi_Log_Level level,
				 const char *file,
				 const char *fct,
				 int line,
				 const char *fmt,
				 void *data,
				 va_list args);

DVI_API void dvi_log_print_cb_set(Dvi_Log_Print_Cb cb, void *data);

DVI_API void dvi_log_print_cb_stderr(Dvi_Log_Level level,
                                     const char *file,
                                     const char *fct,
                                     int line,
                                     const char *fmt,
                                     void *data,
                                     va_list args);

DVI_API void dvi_log_print_cb_stdout(Dvi_Log_Level level,
                                     const char *file,
                                     const char *fct,
                                     int line,
                                     const char *fmt,
                                     void *data,
                                     va_list args);

DVI_API void dvi_log_print(Dvi_Log_Level level,
                           const char *file,
                           const char *fct,
                           int line,
                           const char *fmt, ...);

DVI_API void dvi_log_level_set(Dvi_Log_Level level);

DVI_API Dvi_Log_Level dvi_log_level_get(void);

#endif /* DVI_LOG_H */
