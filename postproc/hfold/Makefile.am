#
# $Id: Makefile.am 3399 2021-10-26 20:05:16Z barrettj $
#

SUBDIRS = .

AM_CPPFLAGS = @HOPS_INC@

noinst_HEADERS = nr.h nrutil.h

bin_PROGRAMS = hfold

hfold_SOURCES = fold.c nrutil.c pythag.c svbksb.c svdcmp.c svdvar.c svdfit.c
hfold_LDADD = @MSG_LIB@ @AFIO_LIB@ @UTIL_LIB@
hfold_DEPENDENCIES = @MSG_DEP@ @AFIO_DEP@ @UTIL_DEP@

#
# eof
#
