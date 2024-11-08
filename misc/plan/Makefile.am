#
# $Id: Makefile.am 3459 2021-11-22 16:55:08Z gbc $
#

SUBDIRS = .

TESTS_ENVIRONMENT = srcdir=$(srcdir) builddir=$(builddir)
dist_noinst_HEADERS = dsolv3.h network.h

if HAVE_GSL
bin_PROGRAMS   = EGparang position_dsolv position_dsolv2 position_dsolv3 \
	network_dsolv3
TESTS = check_dsolv.sh check_multifit_test.sh check_dsolv3.sh
check_PROGRAMS = multifit_pgrm multifit_data gmst_chk
else
bin_PROGRAMS   = EGparang position_dsolv position_dsolv2
TESTS = check_dsolv.sh
check_PROGRAMS = gmst_chk
EXTRA_PROGRAMS = position_dsolv3 network_dsolv3
endif

dist_check_SCRIPTS  = check_dsolv.sh check_multifit_test.sh check_dsolv3.sh
dist_bin_SCRIPTS = plotAlistResids.py position_dsolv_ave.sh

EGparang_SOURCES = EGparang.f

position_dsolv_SOURCES = dsolv.f
position_dsolv2_SOURCES = dsolv2.f

gmst_chk_SOURCES = gmst_chk.c

if HAVE_GSL
position_dsolv3_SOURCES = dsolv3.c dsolv3_main.c load_alist.c
position_dsolv3_CPPFLAGS = @HOPS_INC@
position_dsolv3_LDADD = @GSL_LIBS@ @AFIO_LIB@ @UTIL_LIB@ @MSG_LIB@

network_dsolv3_SOURCES = network.c netsolv.c brute.c
network_dsolv3_LDADD = @GSL_LIBS@

multifit_pgrm_SOURCES = multifit_test.c
multifit_pgrm_CPPFLAGS = -DMULTIFIT_PGRM
multifit_pgrm_LDADD = @GSL_LIBS@

multifit_data_SOURCES = multifit_test.c
multifit_data_CPPFLAGS = -DMULTIFIT_DATA
multifit_data_LDADD = @GSL_LIBS@
else

endif

CLEANFILES = *cmp* *out* result* expect*

#
# eof
#
