#
# $Id: Makefile.am 77 2010-12-02 14:50:41Z gbc $
#

SUBDIRS = .

EXTRA_DIST = stubs/streamstor.pc \
	     stubs/xlrapi.h \
	     stubs/xlrdbcommon.h \
	     stubs/xlrtypes.h \
	     stubs/mark5dir.h \
	     stubs/Parse5A-2007-05-22.h \
	     stubs/Parse5A-2008-10-14.h \
	     stubs/Parse5A-2009-03-17.h

noinst_HEADERS = dls_utils.h dls_compat.h

bin_PROGRAMS = hops_mk5

# streamstor variables are defined via pkg-config
# info pkg-config for the autobarf macros
# see stubs/streamstor.pc for more....

AM_CXXFLAGS = $(STREAMSTOR_CFLAGS)

hops_mk5_SOURCES = mk5_main.cpp stubs/xlr_stubs.cpp \
		   GetBankStatus.cpp GetDeviceInfo.cpp \
		   GetDriveInfo.cpp GetDriveTemp.cpp \
		   GetSerial.cpp GetSMART.cpp \
		   GetDirectoryInfo.cpp
hops_mk5_LDADD = $(STREAMSTOR_LIBS)

#
# eof
#
