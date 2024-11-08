#
# $Id: Makefile.am 1152 2015-07-01 16:00:44Z gbc $
#

SUBDIRS = .

fourfitdir=$(srcdir)/../fourfit

AM_CPPFLAGS = @HOPS_INC@ -I$(fourfitdir)

TESTS_ENVIRONMENT = srcdir=$(srcdir) ffdir=$(fourfitdir)
TESTS             = hook_import.sh

noinst_HEADERS =

if HAVE_DEVEL
bin_PROGRAMS = fearfit
bin_SCRIPTS = fearfit-driver.pl \
	      fearfit-make-cf.sh fearfit-scan-cf.sh fearfit-join-cf.pl
else
bin_PROGRAMS =
bin_SCRIPTS = 
endif
check_PROGRAMS =
EXTRA_DIST = $(bin_SCRIPTS) hook_import.sh fearfit.mk

if HAVE_FFTW
if HAVE_PGPLOT
if HAVE_X11
if HAVE_PNG

if HAVE_DEVEL
# defines fourfit_sources from fourfit's Makefile.am
include $(srcdir)/fearfit.mk

# this file is built in the source directory whenever
# its dependencies change--it is also svn backed up.
$(srcdir)/fearfit.mk: $(srcdir)/hook_import.sh $(fourfitdir)/Makefile.am
	cd $(srcdir) && ./hook_import.sh $(fourfitdir)

fearfit_SOURCES = $(fourfit_sources)
fearfit_CPPFLAGS = -DFF_PROGNAME=\"fearfit\" \
		   -DFF_VER_NO=\"@PACKAGE_VERSION@\" \
		   $(AM_CPPFLAGS)
fearfit_LDADD = @DFIO_LIB@ @VEX_LIB@ @AFIO_LIB@ @UTIL_LIB@ \
		@PGP_LIB@ @PNG_LIB@ @X_FPLOT_LIB@ @X11_LIB@ @FFTW3_LIBS@
fearfit_DEPENDENCIES = @DFIO_DEP@ @VEX_DEP@ @AFIO_DEP@ @UTIL_DEP@
endif

else !HAVE_PNG
fearfit_SOURCES = you_lose.c
endif !HAVE_PNG
else !HAVE_X11
fearfit_SOURCES = you_lose.c
endif !HAVE_X11
else !HAVE_PGPLOT
fearfit_SOURCES = you_lose.c
endif !HAVE_PGPLOT
else
fearfit_SOURCES = you_lose.c
endif !HAVE_FFTW

you_lose.c: Makefile.am
	echo 'main(){return(puts("Missing PNG, X11 or PGPLOT\n"));}' > $@

old_sources = norm.c

CLEANFILES = you_lose.c fearfit.mk $(BUILT_SOURCES) $(OLD_SOURCES)

#
# eof
#
