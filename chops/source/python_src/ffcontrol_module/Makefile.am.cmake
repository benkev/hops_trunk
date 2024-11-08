#
# $Id$
#

AM_CPPFLAGS = -I. @HEADER_INCPATH_STR@ @CMAKE_C_FLAGS@

pkginclude_HEADERS = @HEADER_BASENAMES_STR@

pkglib_LTLIBRARIES = lib@LIBNAME@.la

lib@LIBNAME@_la_SOURCES = @SOURCE_BASENAMES_STR@

lib@LIBNAME@_la_LIBADD = @AM_LIBLIST_STR@

lib@LIBNAME@_la_LDFLAGS = -avoid-version

if HAVE_PYTHON

SRC_FILES = ffcontrol/__init__.py \
	ffcontrol/ffcontrol.py

EXTRA_DIST = $(SRC_FILES) setup.py.in

all-local:
	(mkdir -p ./build)
	(rsync $(srcdir)/ -a .)
	(chmod -R u+rwx .)
	($(PYTHON) setup.py build --verbose)

install-exec-local:
	($(PYTHON) setup.py install \
		--prefix $(DESTDIR)$(prefix) \
		--record $(DESTDIR)$(pythondir)/$(PKG_NAME)/install_ffcontrol_files.txt \
		--verbose)
	(rm -r $(shell readlink -f $(builddir))/src)
	(rm -r $(shell readlink -f $(builddir))/build)
	(rm $(shell readlink -f $(builddir))/Makefile.am)
	(rm $(shell readlink -f $(builddir))/Makefile.in)

uninstall-local:
	cat $(DESTDIR)$(pythondir)/$(PKG_NAME)/install_ffcontrol_files.txt | xargs rm -rf

endif 

#
# eof
#
