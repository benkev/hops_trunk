#
# $Id: Makefile.am 817 2013-06-10 13:30:37Z gbc $
#

SUBDIRS = .

bin_PROGRAMS = vlbi2 vlbi7
check_PROGRAMS= m5b_tshift

m5b_tshift_SOURCES = m5b_tshift.c
m5b_tshift_CPPFLAGS = -D_FILE_OFFSET_BITS=64

vlbi2_SOURCES = mainv.c vlbiv.c four.c

vlbi7_SOURCES = vlbi7.c
vlbi7_CPPFLAGS = -D_FILE_OFFSET_BITS=64

vlbi2.tgz: $(vlbi2_SOURCES) $(vlbi7_SOURCES) doit.sh Makefile.am
	for s in $(vlbi2_SOURCES) doit.sh ; do cp $(srcdir)/$$s . ; done
	tar zcf $@ $(vlbi2_SOURCES) doit.sh
	for s in $(vlbi2_SOURCES) doit.sh ; do rm $$s ; done

#
# eof
#
