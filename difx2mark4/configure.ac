# Roger has retired...
#AC_INIT([difx2mark4], [1.8], [Roger Cappallo <rjc@haystack.mit.edu>])
# and John is a better contact....
AC_INIT([difx2mark4], [1.8], [John Barrett <barrettj@mit.edu>])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([src/difx2mark4.h])

AC_C_BIGENDIAN

AC_PROG_CC
AC_PROG_AWK
AM_SANITY_CHECK

AC_CHECK_LIB(m, erf,,[AC_MSG_ERROR("need libm")])

# one of these required
# PKG_CONFIG_PATH
# PKG_CONFIG_LIBDIR
AC_MSG_NOTICE([PKG_CONFIG_PATH is $PKG_CONFIG_PATH])
AC_MSG_NOTICE([PKG_CONFIG_LIBDIR is $PKG_CONFIG_LIBDIR])
PKG_CHECK_MODULES(DIFXIO, difxio >= 3.7.0)

LIBS="${LIBS} ${DIFXIO_LIBS}"
cflags="-D_LARGE_FILE_SOURCE -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64"
CFLAGS="$cflags ${CFLAGS} ${DIFXIO_CFLAGS} "

#
# apparently fftw usage disappeared at some point
# 
# AC_MSG_CHECKING([whether to enable fftw (default yes)])
# AC_ARG_WITH(fftw,
#     [  --without-fftw           disable fftw features],
#     with_fftw=$withval,
#     with_fftw=yes)
# AC_MSG_RESULT($with_fftw)
#
# if test "$with_fftw" == "yes"; then
# 	PKG_CHECK_MODULES(FFTW3, fftw3, [hasfftw=true], [hasfftw=false])
# 	if test "$hasfftw" == "true"; then
# 		AC_DEFINE([HAVE_FFTW], 1, [1=has FFTW])
# 	fi
# 	AM_CONDITIONAL(WITH_FFTW, $hasfftw)
# 
# 	LIBS="${LIBS} ${FFTW3_LIBS}"
# 	CFLAGS="${CFLAGS} ${FFTW3_CFLAGS}"
# 	AC_SUBST(FFTW3_CFLAGS)
# 	AC_SUBST(FFTW3_LIBS)
# else
# 	AM_CONDITIONAL(WITH_FFTW, "false")
# fi

AC_CONFIG_FILES([ Makefile src/Makefile ])
AC_OUTPUT

# eof
