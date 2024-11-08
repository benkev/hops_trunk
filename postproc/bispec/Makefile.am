#
# $Id: Makefile.am 3399 2021-10-26 20:05:16Z barrettj $
#

SUBDIRS = .

AM_CPPFLAGS = @HOPS_INC@

noinst_HEADERS = bispec.h

bin_PROGRAMS = bispec

bispec_SOURCES = bispec.c make_bispec.c parse_cmdline.c

bispec_LDADD = @MSG_LIB@ @DFIO_LIB@ @AFIO_LIB@ @UTIL_LIB@
bispec_DEPENDENCIES = @MSG_DEP@ @DFIO_DEP@ @AFIO_DEP@ @UTIL_DEP@

#
# eof
#
