
### Version

m4_define([v_maj], [0])
m4_define([v_min], [0])
m4_define([v_mic], [1])

m4_define([libdvi_version], [v_maj.v_min.v_mic])

m4_define([lt_cur], m4_eval(v_maj + v_min))
m4_define([lt_rev], v_mic)
m4_define([lt_age], v_min)

AC_INIT([libdvi], [libdvi_version], [vincent dot torri at gmail dot com])
AC_PREREQ([2.60])
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

# AC_PROG_C** defines CFLAGS to -g -O2 if CFLAGS is empty. Let's use our CFLAGS

AC_MSG_CHECKING([whether CFLAGS is empty])
if test  "x$CFLAGS" = "x" ; then
   no_cflags="yes"
else
   no_cflags="no"
fi
AC_MSG_RESULT([${no_cflags}])

CFLAGS_SHELL=${CFLAGS}
CFLAGS=""

AC_USE_SYSTEM_EXTENSIONS


### If the host is ceGCC, we exit

AC_CANONICAL_HOST

have_win32="no"
case "$host_os" in
   cegcc* | mingw32ce*)
      AC_MSG_ERROR([Windows CE is not supported. Exiting...])
      ;;
   mingw*)
      have_win32="yes"
      ;;
esac

AM_INIT_AUTOMAKE([1.11 dist-xz subdir-objects])
AM_SILENT_RULES([yes])

LT_PREREQ([2.4])
LT_INIT([win32-dll disable-static])

VMAJ=v_maj
AC_SUBST([VMAJ])

version_info="lt_cur:lt_rev:lt_age"
AC_SUBST([version_info])

### configure options

AC_ARG_WITH([kpathsea-dll-path],
   [AS_HELP_STRING([--with-kpathsea-dll-path], [set path to kpathsea DLL on Windows @<:@default=auto@:>@])],
   [kpathsea_dll=$with_kpathsea_dll_path
   ],
   [kpathsea_dll="auto"])

if ! test "x${kpathsea_dll}" = "xauto" && test "x${have_win32}" = "xyes" ; then
   AC_DEFINE_UNQUOTED([KPATHSEA_DLL], ["${kpathsea_dll}"], [Define to the given value of the kpathsea DLL on Windows])
fi

AC_ARG_ENABLE([release],
   [AS_HELP_STRING([--enable-release], [set release mode @<:@default=no@:>@])],
   [
    if test "x${enableval}" = "xyes" ; then
       enable_release="yes"
    else
       enable_release="no"
    fi
   ],
   [enable_release="no"])

if test "x${enable_release}" = "xyes" ; then
   AC_DEFINE([NDEBUG], [1], [Define to 1 if release mode is wanted])
else
   AC_DEFINE([_DEBUG], [1], [Define to 1 if debug mode is wanted])
fi

### Needed information

### Checks for programs

AC_PROG_CC_C99
AM_PROG_CC_C_O

AC_EXEEXT

AC_ARG_VAR([KPSEWHICH], ["kpsewhich tool"])
AC_CHECK_PROG([KPSEWHICH], [kpsewhich${EXEEXT}], [kpsewhich${EXEEXT}], ["none"])
if ! test "x${KPSEWHICH}" = "xnone" ; then
   AC_DEFINE_UNQUOTED([KPSEWHICH], ["${KPSEWHICH}"], ["Define to the value of the kpsewhich binary"])
fi

### Checks for libraries

if test "x${have_win32}" = "xno" ; then
   AC_SEARCH_LIBS([kpathsea_new], [kpathsea], [have_kpathsea="yes"], [have_kpathsea="no"])
fi

### Checks for header files

if test "x${have_win32}" = "xno"  && test "x${have_kpathsea}" = "xyes" ; then
   AC_CHECK_HEADERS([kpathsea.h], [have_kpathsea="yes"], [have_kpathsea="no"])
   if test "x${have_kpathsea}" = "xno" ; then
      AC_CHECK_HEADERS([kpathsea/kpathsea.h], [have_kpathsea="yes"], [have_kpathsea="no"])
   fi
fi

### Checks for types

### Checks for structures

### Checks for compiler characteristics

DVI_ATTRIBUTE_UNUSED

if test "x${no_cflags}" = "xyes" ; then
   DVI_CFLAGS="-pedantic -Wall -Wextra -Wshadow -Wdeclaration-after-statement -Wmissing-prototypes -Wstrict-prototypes -Wredundant-decls -Wpointer-arith -Wno-variadic-macros -Wno-missing-field-initializers -Winline -Wwrite-strings"

   if test "x${have_win32}" = "xyes" ; then
      DVI_CFLAGS="${DVI_CFLAGS} -Wno-pedantic-ms-format"
   fi

   if test "x${enable_release}" = "xno" ; then
      DVI_CFLAGS="${DVI_CFLAGS} -g3 -ggdb3 -O2"
   fi
fi

AC_SUBST([DVI_CFLAGS])

### Checks for linker characteristics

### Checks for library functions

### Checks for system services

AC_SYS_LARGEFILE

AC_CONFIG_FILES([
Makefile
])

AC_OUTPUT

echo
echo "$PACKAGE $VERSION"
echo
echo "Configuration Options Summary:"
echo
echo "  OS...................: ${host_os}"
echo
echo "  Release mode.........: ${enable_release}"
echo
if ! test "x${KPSEWHICH}" = "xnone" ; then
echo "  kpsewhich............: ${KPSEWHICH}"
else
echo "  kpsewhich............: no"
fi
if test "x${have_win32}" = "xno" ; then
echo "  kpathsea feature.....: ${have_kpathsea}"
else
echo "  kpathsea feature.....: ${kpathsea_dll}"
fi
echo
echo "Compilation............: make (or gmake)"
echo "  Environment flags....:"
echo "    CPPFLAGS...........: $CPPFLAGS"
echo "    CFLAGS.............: $CFLAGS"
echo "    LDFLAGS............: $LDFLAGS"
echo "  libdviflags..........:"
echo "    DVI_CFLAGS.........: ${DVI_CFLAGS}"
echo
echo "Installation...........: make install (as root if needed, with 'su' or 'sudo')"
echo "  prefix...............: $prefix"
echo
