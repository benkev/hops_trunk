#
# $Id$
#

SRC_FILES = hopstestb/__init__.py \
	hopstestb/hopstestb.py

EXTRA_DIST = $(SRC_FILES) setup.py.in

if HAVE_PYTHON

dist_bin_SCRIPTS =  hopstestsuiteb.py

all-local:
	(mkdir -p ./build)
	(rsync -av --exclude 'Makefile.*' --exclude *.swp \
		--exclude CMake* --exclude \*.in $(srcdir)/ .)
	(chmod -R u+w .)
	($(PYTHON) setup.py build --verbose)

clean-local:
	rm -rf ./build ./dist hopstestb.egg-info CMakeLists.txt
	[ -f Makefile.am ] || rm -rf ./hopstest ./hopstestb

install-exec-local:
	[ -d $(DESTDIR)$(pythondir) ] || mkdir -p $(DESTDIR)$(pythondir)
	(PYTHONPATH=$(DESTDIR)$(pythondir) $(PYTHON) setup.py install \
		--prefix $(DESTDIR)$(prefix) \
		--record $(DESTDIR)$(pythondir)/install_hopstest_files.txt \
		--verbose)
	@PYTHON_UNZIP_EGGS@ && (cd $(DESTDIR)$(pythondir) ; \
		rm -rf EGG-INFO hopstestb ; \
		for egg in `ls hopstestb*.egg` ; do unzip $$egg ; done ; \
		rm -rf EGG-INFO) || true

uninstall-local:
	-cat $(DESTDIR)$(pythondir)/install_hopstest_files.txt | xargs rm -rf
	rm -rf $(DESTDIR)$(pythondir)/install_hopstest_files.txt
	rm -rf $(DESTDIR)$(pythondir)/hopstestb

DISTCLEANFILES = setup.py

endif

#
# eof
#
