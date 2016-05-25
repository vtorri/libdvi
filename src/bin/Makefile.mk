
bin_PROGRAMS = src/bin/dvi

src_bin_dvi_SOURCES = \
src/bin/dvi.c

src_bin_dvi_CPPFLAGS = \
-I$(top_srcdir)/src/lib

src_bin_dvi_CFLAGS = @DVI_CFLAGS@

src_bin_dvi_LDADD = \
src/lib/libdvi.la
