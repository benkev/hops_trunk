#
# $Id: Makefile.am 3399 2021-10-26 20:05:16Z barrettj $
#

SUBDIRS = .

AM_CPPFLAGS = @HOPS_INC@

noinst_HEADERS = calamp.h

bin_PROGRAMS = calamp

calamp_SOURCES = calamp.c parse_cmdline.c calfact.c read_calfile.c

calamp_LDADD = @MSG_LIB@ @DFIO_LIB@ @AFIO_LIB@ @UTIL_LIB@
calamp_DEPENDENCIES = @MSG_DEP@ @DFIO_DEP@ @AFIO_DEP@ @UTIL_DEP@

#
# eof
#
