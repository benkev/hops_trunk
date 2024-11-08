#
# $Id$
#

SRC_FILES = vpal/__init__.py \
	vpal/utility.py \
	vpal/ffres2pcp_lib.py \
	vpal/fourphase_lib.py \
	vpal/report_lib.py \
	vpal/proxy_cable_cal.py \
	vpal/pcc_delay_fitting.py \
	vpal/pcc_plotting_utils.py \
	vpal/fringe_file_manipulation.py \
	vpal/phasecal.py \
	vpal/processing.py

EXTRA_DIST = $(SRC_FILES) setup.py.in

if HAVE_PYTHON

all-local:
	(mkdir -p ./build)
	(rsync -av --exclude 'Makefile.*' --exclude *.swp \
		  --exclude CMake* --exclude deprecated_modules \
		  --exclude \*.in $(srcdir)/ .)
	(chmod -R u+w .)
	($(PYTHON) setup.py build --verbose)

clean-local:
	rm -rf ./build ./dist vpal.egg-info
	[ -f Makefile.am ] || rm -rf vpal deprecated_modules CMakeLists.txt

install-exec-local:
	[ -d $(DESTDIR)$(pythondir) ] || mkdir -p $(DESTDIR)$(pythondir)
	(PYTHONPATH=$(DESTDIR)$(pythondir) $(PYTHON) setup.py install \
		--prefix $(DESTDIR)$(prefix) \
		--record $(DESTDIR)$(pythondir)/install_vpal_files.txt \
		--verbose)
	@PYTHON_UNZIP_EGGS@ && (cd $(DESTDIR)$(pythondir) ; \
		rm -rf EGG_INFO vpal ; \
		for egg in `ls vpal*.egg` ; do unzip $$egg ; done ; \
		rm -rf EGG-INFO) || true

uninstall-local:
	cat $(DESTDIR)$(pythondir)/install_vpal_files.txt | xargs rm -rf
	rm -rf $(DESTDIR)$(pythondir)/install_vpal_files.txt
	rm -rf $(DESTDIR)$(pythondir)/vpal

DISTCLEANFILES = setup.py

endif

#
# eof
#
