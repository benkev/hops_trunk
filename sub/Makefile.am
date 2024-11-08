#
# $Id: Makefile.am 4124 2023-10-07 15:23:11Z gbc $
#

if HAVE_DEVEL
if HAVE_PYTHON
SUBDIRS = . msg afio dfio util vex @M4PYDIR@
TESTS = importcheck.py
else
SUBDIRS = . msg afio dfio util vex
TESTS =
endif
else
SUBDIRS = . msg afio dfio util vex
TESTS =
endif
DIST_SUBDIRS = $(SUBDIRS)
check_SCRIPTS = importcheck.py
dist_check_SCRIPTS = importcheck.py

#
# eof
#
