
dnl Macros for checking if the compiler supports some __attribute__ uses

dnl Usage: DVI_ATTRIBUTE_UNUSED
dnl call AC_DEFINE for DVI_UNUSED if __attribute__((unused)) is available

AC_DEFUN([DVI_ATTRIBUTE_UNUSED],
[
AC_MSG_CHECKING([for __attribute__ ((unused))])
AC_COMPILE_IFELSE(
   [AC_LANG_PROGRAM(
       [[
void foo(int x __attribute__ ((unused))) {}
       ]],
       [[
       ]])],
   [have_attribute_unused="yes"],
   [have_attribute_unused="no"])
AC_MSG_RESULT([${have_attribute_unused}])

if test "x${have_attribute_unused}" = "xyes" ; then
   AC_DEFINE([DVI_UNUSED], [__attribute__ ((unused))], [Macro declaring a function argument to be unused.])
else
   AC_DEFINE([DVI_UNUSED], [], [__attribute__ ((unused)) is not supported.])
fi
])

dnl End of dvi_attribute.m4
