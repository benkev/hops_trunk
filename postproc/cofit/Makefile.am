#
# $Id: Makefile.am 3399 2021-10-26 20:05:16Z barrettj $
#

SUBDIRS = .

AM_CPPFLAGS = @HOPS_INC@

noinst_HEADERS = cofit.h cpgplot.h nr.h nrutil.h

bin_PROGRAMS = cofit

if HAVE_PGPLOT
if HAVE_X11
if HAVE_PNG

cofit_SOURCES = clear_codata.c fit_codata.c parse_cmdline.c \
		sort_data.c cofit.c plot_codata.c read_data.c \
		sorter.c write_codata.c fit_ampl.c ampf.c \
		nrutil.c covsrt.c mrqcof.c mrqfix.c svbksb.c \
		svdcmpp.c pythag.c normalize_snr.c fit_snr.c 

cofit_LDADD = @PGP_LIB@ @PNG_LIB@ @MSG_LIB@ @AFIO_LIB@ @UTIL_LIB@ @X11_LIB@
cofit_DEPENDENCIES = @MSG_DEP@ @AFIO_DEP@ @UTIL_DEP@

else !HAVE_PNG
cofit_SOURCES = you_lose.c
endif !HAVE_PNG
else !HAVE_X11
cofit_SOURCES = you_lose.c
endif !HAVE_X11
else !HAVE_PGPLOT
cofit_SOURCES = you_lose.c
endif !HAVE_PGPLOT

you_lose.c: Makefile.am
	echo 'main(){return(puts("Missing PNG, X11 or PGPLOT\n"));}' > $@

CLEANFILES = you_lose.c

#
# eof
#
