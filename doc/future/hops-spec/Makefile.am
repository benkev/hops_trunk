#
# Makefile for the new HOPS spec
# 
SUBDIRS = .

docsdir = $(pkgdatadir)/docs

hops_spec_files = hops-spec.bib hops-spec.pdf hops-spec.tex \
	code/data_objects.cxx code/data_objects.hh
	code/data_operators.hh code/visibilities.hh

tex_sources = $(srcdir)/hops-spec.bib \
	$(srcdir)/hops-spec.tex \
        $(srcdir)/code/data_objects.cxx $(srcdir)/code/data_objects.hh \
        $(srcdir)/code/data_operators.hh $(srcdir)/code/visibilities.hh

# if/when these are to be shared...
# hops_dev_files = ...
#
if HAVE_DOCS
docs_DATA = hops-spec.pdf
EXTRA_DIST = $(hops_dev_files)
else
docs_DATA =
EXTRA_DIST =
endif

# A : at the end pulls in the default path
TEXINPUTS=.:$(srcdir):

# biber
hops-spec.pdf: $(tex_sources) force
	TEXINPUTS=$(TEXINPUTS) pdflatex hops-spec
	biber hops-spec
	TEXINPUTS=$(TEXINPUTS) pdflatex hops-spec
	TEXINPUTS=$(TEXINPUTS) pdflatex hops-spec

force: Makefile
	@echo forcing rebuild
	@rsync -a $(srcdir)/code .
	@rsync -a $(srcdir)/hops-spec.bib .
	@rsync -a $(srcdir)/../hops-development/hops-development.bib .
	touch force

clean-local:
	rm -rf ./code

CLEANFILES = *.aux *.out *.log *.out *.pdf *.toc *.bib *.blg *.bbl *.bcf *.xml
CLEANFILES += force

#
# eof
#
