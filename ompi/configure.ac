#
# runt
#
AC_PREREQ(2.59)
AC_INIT(hmpi, 0.0, gbc@haystack.mit.edu)
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([hello_ompi.c])
AC_CONFIG_HEADERS([hops_ompi.h])

AC_PROG_CC
AX_OPENMP

AC_ARG_VAR(MPICC, [Full path to Open MPI C Compiler])

AC_MSG_NOTICE([Configure in ompi])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

#
# eof
#
