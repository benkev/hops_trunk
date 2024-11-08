#
# Makefile for future development stuff
#
SUBDIRS = .

docsdir = $(pkgdatadir)/docs

hops_dev_files = hops-development.tex introduction.tex \
	designdetails.tex designoutline.tex \
	detailedtimeline.tex hops-development.bib
tex_sources = $(srcdir)/hops-development.tex $(srcdir)/introduction.tex \
	$(srcdir)/designdetails.tex $(srcdir)/designoutline.tex \
	$(srcdir)/detailedtimeline.tex $(srcdir)/hops-development.bib

if HAVE_DOCS
docs_DATA = hops-development.pdf
EXTRA_DIST = $(hops_dev_files)
else
docs_DATA =
EXTRA_DIST =
endif

# A : at the end pulls in the default path
TEXINPUTS=.:$(srcdir):

# biber
hops-development.pdf: $(tex_sources) Makefile
	TEXINPUTS=$(TEXINPUTS) pdflatex hops-development
	rsync -a $(srcdir)/hops-development.bib .
	TEXINPUTS=$(TEXINPUTS) biber hops-development
	TEXINPUTS=$(TEXINPUTS) pdflatex hops-development
	TEXINPUTS=$(TEXINPUTS) pdflatex hops-development

force: Makefile
	@echo forcing rebuild
	touch force

CLEANFILES = *.aux *.out *.log *.out *.pdf *.toc *.bib *.blg *.bbl *.bcf *.xml

#
# eof
#
