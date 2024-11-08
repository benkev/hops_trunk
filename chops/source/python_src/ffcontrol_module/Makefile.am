#
# $Id$
#

AM_CPPFLAGS = -I. -I$(srcdir)/include \
	-I$(srcdir)/../../c_src/mk4util/include \
	-I$(srcdir)/../../c_src/fourfit_libs/ffcontrol/include \
	-Wall -Wextra -DHAVE_CONFIG_H

pkginclude_HEADERS = include/ffcontrol_module_extern.h
pkglib_LTLIBRARIES = libffcontrolpyb.la

# note that these are rsync'd to build area due to setup.py
ffcontrolsources = src/construct_cblock.c \
	src/ffcontrol_module_extern.c \
	src/compute_control_file_hash.c
SRC_FILES = ffcontrol/__init__.py ffcontrol/ffcontrol.py

# and for some reason, some older automakes have trouble with
# library dependencies
BUILT_SOURCES = $(ffcontrolsources) $(SRC_FILES) importstamp
ffcontrol/ffcontrol.py:
	(cd $(abs_top_builddir); ./config.status $(subdir)/$@ )

$(ffcontrolsources): importstamp
importstamp: $(srcdir)/src $(srcdir)/include
	[ -f Makefile.am ] || {\
		[ -d src ] || mkdir src ;\
		for s in $(ffcontrolsources);\
			do [ -f $$s ] || cp -p $(srcdir)/$$s src ; done ;\
		[ -d include ] || mkdir include ;\
		for s in $(pkginclude_HEADERS);\
			do [ -f $$s ] || cp -p $(srcdir)/$$s src ; done ;}
	touch importstamp

nodist_libffcontrolpyb_la_SOURCES = $(ffcontrolsources)

libffcontrolpyb_la_LIBADD = ../../c_src/mk4util/libmk4utilb.la \
	../../c_src/fourfit_libs/ffcontrol/libffcontrolb.la

libffcontrolpyb_la_LDFLAGS = -avoid-version

EXTRA_DIST = $(SRC_FILES) setup.py.in $(ffcontrolsources)

if HAVE_PYTHON

all-local:
	(mkdir -p ./build)
	(rsync -a --exclude 'Makefile.*' --exclude *.swp \
		--exclude CMake* --exclude \*.in $(srcdir)/ .)
	(chmod -R u+w .)
	($(PYTHON) setup.py build --verbose)

clean-local:
	rm -rf ./build ./dist ffcontrol.egg-info
	[ -f Makefile.am ] || rm -rf ./include ./src CMakeLists.txt

install-exec-local:
	 -d $(DESTDIR)$(pythondir) ] || mkdir -p $(DESTDIR)$(pythondir)
	(PYTHONPATH=$(DESTDIR)$(pythondir) $(PYTHON) setup.py install \
		--prefix $(DESTDIR)$(prefix) \
		--record $(DESTDIR)$(pythondir)/install_ffcontrol_files.txt \
		--verbose)
	@PYTHON_UNZIP_EGGS@ && (cd $(DESTDIR)$(pythondir) ; \
		rm -rf EGG-INFO ffcontrol ; \
		for egg in `ls ffcontrol*.egg` ; do unzip $$egg ; done ; \
		rm -rf EGG-INFO) || true

uninstall-local:
	cat $(DESTDIR)$(pythondir)/install_ffcontrol_files.txt | xargs rm -rf
	rm -rf $(DESTDIR)$(pythondir)/install_ffcontrol_files.txt
	rm -rf $(DESTDIR)$(pythondir)/ffcontrol

CLEANFILES = importstamp

DISTCLEANFILES = setup.py ffcontrol/__init__.py

endif

#
# eof
#
