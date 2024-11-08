#
# doxygen generated documents
#
if HAVE_DOXY
doxydoxy = hops-sub.doxy hops-fourfit.doxy hops-include.doxy \
	hops-alist.doxy hops-aedit.doxy
doxydirs = hops-sub-doxy hops-fourfit-doxy hops-include-doxy \
	hops-alist-doxy hops-aedit-doxy
hopspdfs = hops-sub-doxy.pdf hops-fourfit-doxy.pdf hops-include-doxy.pdf \
	hops-alist-doxy.pdf hops-aedit-doxy.pdf
products = $(hopspdfs) hops-html.tgz

EXTRA_DIST = $(doxydoxy) $(products)

all:	$(doxydirs) $(products)

%-doxy : %.doxy
	rm -rf $@
	doxygen $<
	cd $@/latex && ( echo '' | make )
	ls -l $@/latex/refman.pdf

%-doxy.pdf : %-doxy
	cp -p $</latex/refman.pdf $@

hops-html.tgz:	Makefile $(doxydirs) $(hopspdfs)
	tar cf $@.tar $(hopspdfs)
	for d in $(doxydirs) ; do tar -rf $@.tar $$d/html ; done
	gzip $@.tar
	mv $@.tar.gz $@
	ls -l $@

clean-local:
	rm -rf $(doxydirs)

CLEANFILES = $(products)

else
hops-sub-doxy:
	@echo doxygen was not found
endif
#
# eof
#
