#
# $Id$
#

SUBDIRS = .

AM_CPPFLAGS = -I. @HEADER_INCPATH_STR@ @CMAKE_C_FLAGS@

include_HEADERS = @HEADER_BASENAMES_STR@

lib_LTLIBRARIES = lib@LIBNAME@.la

lib@LIBNAME@_la_SOURCES = @SOURCE_BASENAMES_STR@

lib@LIBNAME@_la_LIBADD = @AM_LIBLIST_STR@

lib@LIBNAME@_la_LDFLAGS = -avoid-version

#
# eof
#