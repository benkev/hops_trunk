#
# $Id: Makefile.am 1337 2016-05-24 20:59:37Z gbc $
#

SUBDIRS = .

# this is a code fragment for debugging
EXTRA_DIST = gbcdump.cpp

noinst_HEADERS =

bin_PROGRAMS = dumpvis dumpswin dumpinput
check_PROGRAMS=

dumpinput_SOURCES = dumpinput.c
dumpinput_CPPFLAGS = $(DIFX_CFLAGS)
dumpinput_LDADD = $(DIFX_LIBS)

# something i wrote
dumpswin_SOURCES = dumpswin.c
dumpswin_CPPFLAGS = $(DIFX_CFLAGS)
dumpswin_LDADD = $(DIFX_LIBS)

# something roger wrote
dumpvis_SOURCES = dumpvis.c
dumpvis_CPPFLAGS = $(DIFX_CFLAGS)
dumpvis_LDADD = $(DIFX_LIBS)

#
# eof
#
