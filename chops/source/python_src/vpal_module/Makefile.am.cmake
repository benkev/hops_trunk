#
# $Id$
#

if HAVE_PYTHON

SRC_FILES = vpal/__init__.py \
    vpal/baseline_fringe_product_list.py \
    vpal/ffres2pcp_lib.py \
    vpal/fourphase_lib.py \
    vpal/fringe_file_objects.py \
    vpal/control_analysis_lib.py \
    vpal/mixedmode.py \
    vpal/plotting.py \
    vpal/processing.py \
    vpal/scripting.py \
    vpal/phasecal.py

EXTRA_DIST = $(SRC_FILES) setup.py.in

all-local:
	(mkdir -p ./build)
	(rsync $(srcdir)/ -a .)
	(chmod -R u+rwx .)
	($(PYTHON) setup.py build --verbose)

install-exec-local:
	($(PYTHON) setup.py install \
		--prefix $(DESTDIR)$(prefix) \
		--record $(DESTDIR)$(pythondir)/$(PKG_NAME)/install_vpal_files.txt \
		--verbose)
	(rm -r $(shell readlink -f $(builddir))/build)
	(rm $(shell readlink -f $(builddir))/Makefile.am)
	(rm $(shell readlink -f $(builddir))/Makefile.in)

uninstall-local:
	cat $(DESTDIR)$(pythondir)/$(PKG_NAME)/install_vpal_files.txt | xargs rm -rf

endif 

#
# eof
#
