#
# $Id$
#

if HAVE_PYTHON

SRC_FILES = hopstest/__init__.py \
	hopstest/hopstest.py

EXTRA_DIST = $(SRC_FILES) setup.py.in

dist_bin_SCRIPTS =  hopstestsuite.py

all-local:
	(mkdir -p ./build)
	(rsync $(srcdir)/ -a .)
	(chmod -R u+rwx .)
	($(PYTHON) setup.py build --verbose)

install-exec-local:
	($(PYTHON) setup.py install \
		--prefix $(DESTDIR)$(prefix) \
		--record $(DESTDIR)$(pythondir)/$(PKG_NAME)/install_hopstest_files.txt \
		--verbose)
	(rm -r $(shell readlink -f $(builddir))/build)
	(rm $(shell readlink -f $(builddir))/Makefile.am)
	(rm $(shell readlink -f $(builddir))/Makefile.in)

uninstall-local:
	cat $(DESTDIR)$(pythondir)/$(PKG_NAME)/install_hopstest_files.txt | xargs rm -rf

endif 

#
# eof
#
