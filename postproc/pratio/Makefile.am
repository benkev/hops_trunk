#
# $Id: Makefile.am 3399 2021-10-26 20:05:16Z barrettj $
#

SUBDIRS = .

AM_CPPFLAGS = @HOPS_INC@

bin_PROGRAMS = pratio

pratio_SOURCES = calc_vec_avg.c open_outfiles.c ratio.c \
		 transfer_cotimes.c close_outfiles.c \
		 parse_cmdline.c reset_pol_field.c write_ratios.c \
		 compute_ratios.c pratio.c set_pol_field.c 

pratio_LDADD = @MSG_LIB@ @AFIO_LIB@ @UTIL_LIB@
pratio_DEPENDENCIES = @MSG_DEP@ @AFIO_DEP@ @UTIL_DEP@

#
# eof
#
