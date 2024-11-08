#
# $Id$
#
# GBC_HOPS_PRINTER([normal notice arguments])
#-----------------------------------------
AC_DEFUN([GBC_HOPS_PRINTER],
[AC_MSG_NOTICE([automatic printer configuration is not fully implemented])
 h=`hostname`
 if [test -z "$PRINTER"] ; then
     if [test "$h" = smm-pc] ; then PRINTER=hp4600 ; fi
     if [test "$h" = gefera] ; then PRINTER=hp4600 ; fi
 fi
 AC_SUBST([PRINTER])
])

#
# eof
#
