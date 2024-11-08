#
# $Id: Makefile.am 4078 2023-10-01 15:08:09Z gbc $
#

SUBDIRS = .

AM_CPPFLAGS = @HOPS_INC@ -DPS2PDF=\"@PS2PDF@\"

TESTS_ENVIRONMENT = abs_top_srcdir=$(abs_top_srcdir)
TESTS             = chk_fplot.sh

noinst_HEADERS = fplot.h

bin_PROGRAMS = fplot
dist_check_SCRIPTS = chk_fplot.sh

if HAVE_X11

fplot_SOURCES = fplot.c parse_cmdline.c skip_poln.c

fplot_LDADD = @MSG_LIB@ @DFIO_LIB@ @UTIL_LIB@ @X_FPLOT_LIB@ @X11_LIB@
fplot_DEPENDENCIES = @MSG_DEP@ @DFIO_DEP@ @UTIL_DEP@

else !HAVE_X11
fplot_SOURCES = you_lose.c
endif !HAVE_X11

you_lose.c: Makefile.am
	echo 'main(){return(puts("Missing X11 or PGPLOT\n"));}' > $@

CLEANFILES = you_lose.c *.pdf *.ps

#
# eof
#
