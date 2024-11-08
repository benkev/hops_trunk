#
# $Id: Makefile.am 265 2011-05-20 15:39:47Z gbc $
#

SUBDIRS = .

AM_CPPFLAGS = @HOPS_INC@

bin_PROGRAMS = linkdata
bin_SCRIPTS = hops_data_links.pl

linkdata_SOURCES = alloc_lists.c file_list.c make_flinks.c \
		   parent_root.c cleanup.c linkdata.c make_link.c \
		   parse_cmdline.c dir_list.c make_clinks.c \
		   make_rlinks.c read_afiles.c type_3.c 

linkdata_LDADD = @AFIO_LIB@ @UTIL_LIB@
linkdata_DEPENDENCIES = @AFIO_DEP@ @UTIL_DEP@

#
# eof
#
