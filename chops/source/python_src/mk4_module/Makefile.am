#
# $Id$
#
# MAKEFLAGS = -k if desperate

AM_CPPFLAGS = -I. -I$(srcdir)/include \
	-I$(srcdir)/../../c_src/msg/include \
	-I$(srcdir)/../../c_src/mk4util/include \
	-I$(srcdir)/../../c_src/dfio/include \
	-I$(srcdir)/../../c_src/afiob/include \
	-Wall -Wextra -DHAVE_CONFIG_H

pkginclude_HEADERS = ./include/mk4_module_extern.h
pkglib_LTLIBRARIES = libmk4iob.la

# this one is real but imported for simplicity?
dfioorigsources = src/mk4_module_extern.c
# these are imported via rsync
dfiohacksources = src/addr_100.c \
	src/addr_101.c src/addr_110.c \
	src/addr_120.c src/addr_200.c \
	src/addr_201.c src/addr_202.c \
	src/addr_203.c src/addr_204.c \
	src/addr_205.c src/addr_206.c \
	src/addr_207.c src/addr_208.c \
	src/addr_210.c src/addr_212.c \
	src/addr_220.c src/addr_221.c \
	src/addr_222.c src/addr_230.c \
	src/addr_300.c src/addr_301.c \
	src/addr_302.c src/addr_303.c \
	src/addr_304.c src/addr_305.c \
	src/addr_306.c src/addr_307.c \
	src/addr_308.c src/addr_309.c \
	src/clear_100.c src/clear_101.c \
	src/clear_110.c src/clear_120.c \
	src/clear_200.c src/clear_201.c \
	src/clear_202.c src/clear_203.c \
	src/clear_204.c src/clear_205.c \
	src/clear_206.c src/clear_207.c \
	src/clear_208.c src/clear_210.c \
	src/clear_212.c src/clear_220.c \
	src/clear_221.c src/clear_222.c \
	src/clear_230.c src/clear_300.c  \
	src/clear_301.c src/clear_302.c \
	src/clear_303.c src/clear_304.c \
	src/clear_305.c src/clear_306.c \
	src/clear_307.c src/clear_308.c \
	src/clear_309.c src/clear_coord.c \
	src/clear_mk4corel.c src/clear_mk4fringe.c \
	src/clear_mk4sdata.c src/compress.c \
	src/copy_100.c src/copy_101.c \
	src/copy_110.c src/copy_120.c \
	src/copy_200.c src/copy_201.c  \
	src/copy_202.c src/copy_203.c \
	src/copy_204.c src/copy_205.c \
	src/copy_206.c src/copy_207.c \
	src/copy_208.c src/copy_210.c \
	src/copy_212.c src/copy_220.c \
	src/copy_221.c src/copy_222.c \
	src/copy_230.c src/copy_300.c \
	src/copy_301.c src/copy_302.c \
	src/copy_303.c src/copy_304.c \
	src/copy_305.c src/copy_306.c \
	src/copy_307.c src/copy_308.c \
	src/copy_309.c src/corel_alloc.c \
	src/display_221.c src/init_000.c \
	src/open_datafile.c src/read_mk4corel.c \
	src/read_mk4file.c src/read_mk4fringe.c \
	src/read_mk4sdata.c src/test_compress.c \
	src/write_err.c src/write_mk4corel.c \
	src/write_mk4fringe.c src/write_mk4sdata.c \
	src/write_record.c

# give automake a clue it is in for trouble
BUILT_SOURCES = $(dfioorigsources) $(dfiohacksources) importstamp

$(dfioorigsources): importstamp
$(dfiohacksources): importstamp

importstamp: $(srcdir)/src $(srcdir)/../../c_src/dfio/src
	[ -f Makefile.am ] || {\
		[ -d src ] || mkdir src ;\
		[ -f src/mk4_module_extern.c ] ||\
		cp -p $(srcdir)/src/mk4_module_extern.c src ;\
		for s in $(dfiohacksources);\
		do [ -f $$s ] || cp -p $(srcdir)/../../c_src/dfio/$$s src ; done ; }
	touch importstamp

nodist_libmk4iob_la_SOURCES =  $(dfioorigsources) $(dfiohacksources)

libmk4iob_la_LIBADD = \
	../../c_src/msg/libmsgb.la \
	../../c_src/afio/libafiob.la \
	../../c_src/mk4util/libmk4utilb.la

libmk4iob_la_LDFLAGS = -avoid-version

SRC_FILES = mk4b/__init__.py \
	mk4b/mk4b.py \
	afiob/__init__.py \
	afiob/afiob.py

# mk4_module_extern.c needed here as it the 'SOURCE' copy is nodist
EXTRA_DIST = $(SRC_FILES) setup.py.in $(srcdir)/src/mk4_module_extern.c

if HAVE_PYTHON

all-local:
	(mkdir -p ./build)
	(rsync -av --exclude 'Makefile.*' --exclude *.swp \
		--exclude CMake* --exclude \*.in $(srcdir)/ .)
	(chmod -R u+w .)
	($(PYTHON) setup.py build --verbose)

clean-local:
	rm -rf ./build ./dist mk4.egg-info
	[ -f Makefile.am ] || rm -rf src afio mk4 include CMakeLists.txt

install-exec-local:
	[ -d $(DESTDIR)$(pythondir) ] || mkdir -p $(DESTDIR)$(pythondir)
	(PYTHONPATH=$(DESTDIR)$(pythondir) $(PYTHON) setup.py install \
		--prefix $(DESTDIR)$(prefix) \
		--record $(DESTDIR)$(pythondir)/install_mk4_files.txt \
		--verbose)
	@PYTHON_UNZIP_EGGS@ && (cd $(DESTDIR)$(pythondir) ; \
		rm -rf EGG-INFO mk4b afiob ; \
		for egg in `ls mk4*.egg` ; do unzip $$egg ; done ; \
		rm -rf EGG-INFO) || true

uninstall-local:
	cat $(DESTDIR)$(pythondir)/install_mk4_files.txt | xargs rm -rf
	rm -rf $(DESTDIR)$(pythondir)/install_mk4_files.txt
	rm -rf $(DESTDIR)$(pythondir)/afiob
	rm -rf $(DESTDIR)$(pythondir)/mk4b

CLEANFILES = importstamp

DISTCLEANFILES = setup.py afiob/__init__.py mk4b/__init__.py

endif

#
# eof
#

