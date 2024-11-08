#
# $Id: Makefile.am 3399 2021-10-26 20:05:16Z barrettj $
#

SUBDIRS = .

AM_CPPFLAGS = @HOPS_INC@

noinst_HEADERS = coterp.h cpgplot.h nrutil.h

bin_PROGRAMS = coterp

if HAVE_PGPLOT
if HAVE_X11
if HAVE_PNG

coterp_SOURCES = check_lists.c elfactor.c interp.c sort_data.c \
		 stationize.c coterp.c fit_cotime.c read_data.c \
		 sorter.c write_data.c parse_cmdline.c \
		 plot_sttimes.c dpythag.c mrqcof.c svbksb.c \
		 dsvdcmpp.c mrqfix.c covsrt.c fit_times.c nrutil.c 

# original order:
#coterp_LDADD = @PGP_LIB@ @PNG_LIB@ @AFIO_LIB@ @UTIL_LIB@ @X11_LIB@
# more logical order:
coterp_LDADD = @AFIO_LIB@ @UTIL_LIB@ @MSG_LIB@ @PGP_LIB@ @PNG_LIB@ @X11_LIB@
coterp_DEPENDENCIES = @MSG_DEP@ @AFIO_DEP@ @UTIL_DEP@

else !HAVE_PNG
coterp_SOURCES = you_lose.c
endif !HAVE_PNG
else !HAVE_X11
coterp_SOURCES = you_lose.c
endif !HAVE_X11
else !HAVE_PGPLOT
coterp_SOURCES = you_lose.c
endif !HAVE_PGPLOT

you_lose.c: Makefile.am
	echo 'main(){return(puts("Missing PNG, X11 or PGPLOT\n"));}' > $@

CLEANFILES = you_lose.c

#
# eof
#
