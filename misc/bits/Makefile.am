#
# $Id: Makefile.am 786 2012-12-12 21:58:32Z rjc $
#

SUBDIRS = .

noinst_HEADERS = anoise.h vnoise.h

always = b4thresh

if HAVE_GSL
if HAVE_FFTW
binp = anoise
extp =
else #HAVE_FFTW
binp =
extp = anoise
endif #HAVE_FFTW
bin_PROGRAMS = $(binp)
check_PROGRAMS=bfind bnoise bpcal bstate bwrite vnoise b2vdif $(always) $(extp)
EXTRA_PROGRAMS=
else #HAVE_GSL
bin_PROGRAMS = 
check_PROGRAMS= $(always)
EXTRA_PROGRAMS=bfind bnoise bpcal bstate bwrite b2vdif vnoise anoise
endif #HAVE_GSL

TESTS_ENVIRONMENT = srcdir=$(srcdir)
TESTS = $(always)

LFILE = -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE
#GPROF = -pg -Dprofile=
#GFLAG = -pg
GPROF = -Dprofile=static
GFLAG =

bfind_SOURCES = bfind.c
bfind_CPPFLAGS = $(LFILE)

bnoise_SOURCES = bnoise.c
bnoise_CPPFLAGS = $(LFILE) $(GPROF)
bnoise_LDFLAGS = $(GFLAG)
bnoise_LDADD = @GSL_LIBS@

bpcal_SOURCES = bpcal.c
bpcal_CPPFLAGS = $(LFILE)

bstate_SOURCES = bstate.c
bstate_CPPFLAGS = $(LFILE)

bwrite_SOURCES = bwrite.c
bwrite_CPPFLAGS = $(LFILE)

b2vdif_SOURCES = b2vdif.c
b2vdif_CPPFLAGS = $(LFILE)

vnoise_SOURCES = vnoise.c vnoise_maker.c quant_hacker.c
vnoise_CPPFLAGS = $(LFILE) $(GPROF)
vnoise_LDFLAGS = $(GFLAG)
vnoise_LDADD = @GSL_LIBS@ @RT_LIBS@

b4thresh_SOURCES = b4thresh.c b4ensb.c b4antsum.c

anoise_SOURCES = anoise.c anoise_maker.c quant_hacker.c \
		 anoise_common.c anoise_station.c anoise_extend.c \
		 anoise_describe.c
anoise_CFLAGS = @FFTW3_CFLAGS@ $(AM_CFLAGS)
anoise_CPPFLAGS = $(LFILE) $(GPROF)
anoise_LDFLAGS = $(GFLAG)
anoise_LDADD = @GSL_LIBS@ @RT_LIBS@ @FFTW3_LIBS@

CLEANFILES = *vdif* *spec*

#
# eof
#
