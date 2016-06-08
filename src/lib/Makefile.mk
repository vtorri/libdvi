
lib_LTLIBRARIES = src/lib/libdvi.la

includes_HEADERS = src/lib/Dvi.h
includesdir = $(pkgincludedir)-@VMAJ@

src_lib_libdvi_la_SOURCES = \
src/lib/dvi_document.c \
src/lib/dvi_font.c \
src/lib/dvi_interpreter.c \
src/lib/dvi_kpathsea.c \
src/lib/dvi_log.c \
src/lib/dvi_main.c \
src/lib/dvi_map.c \
src/lib/dvi_process.c \
src/lib/dvi_stack.c \
src/lib/dvi_document.h \
src/lib/dvi_document_private.h \
src/lib/dvi_font.h \
src/lib/dvi_interpreter.h \
src/lib/dvi_kpathsea.h \
src/lib/dvi_log.h \
src/lib/dvi_map.h \
src/lib/dvi_opcodes.h \
src/lib/dvi_private.h \
src/lib/dvi_process.h \
src/lib/dvi_stack.h

src_lib_libdvi_la_CPPFLAGS = \
-DDVI_BUILD

src_lib_libdvi_la_CFLAGS = \
@DVI_CFLAGS@

src_lib_libdvi_la_LIBADD = -lm

src_lib_libdvi_la_LDFLAGS = -no-undefined -version-info @version_info@
