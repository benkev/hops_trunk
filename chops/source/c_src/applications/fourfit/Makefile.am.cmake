#
# $Id$
#

SUBDIRS = .

AM_CPPFLAGS = -I. @HEADER_INCPATH_STR@ @CMAKE_C_FLAGS@

AM_CPPFLAGS += ${FFTW3_CFLAGS} ${X_CFLAGS} -I${PGPLOT_DIR}

bin_PROGRAMS = @EXENAME@

@EXENAME@_SOURCES = @SOURCE_BASENAMES_STR@

@EXENAME@_LDADD = @AM_LIBLIST_STR@

@EXENAME@_LDADD += ${PGP_LIB} ${PNG_LIB} ${X_FPLOT_LIB} ${X11_LIB} ${FFTW3_LIBS}

#
# eof
#