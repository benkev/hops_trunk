#
# documents
#
# SUBDIRS = .

docsdir = $(pkgdatadir)/docs

ff_man_files = algorithms.tex architecture.tex calling_graphs.tex \
	cf_keywords.tex command_line_options.tex file_based_pcal.tex \
	fourfit_users_manual.tex fringe_plots.tex history.tex \
	introduction.tex mk4_files.tex multitone_pcal.tex new_parameters.tex \
	program_control.tex program_output.tex svn_history.tex \
	fourfit_graph.pdf fringe_search_graph.pdf ionosphere2.png \
	ionosphere.png ion_search_graph.pdf output_graph.pdf \
	parse_control_file_graph.pdf
tex_sources = $(srcdir)/algorithms.tex $(srcdir)/architecture.tex \
	$(srcdir)/calling_graphs.tex $(srcdir)/cf_keywords.tex \
	$(srcdir)/command_line_options.tex $(srcdir)/file_based_pcal.tex \
	$(srcdir)/fourfit_users_manual.tex $(srcdir)/fringe_plots.tex \
	$(srcdir)/history.tex $(srcdir)/introduction.tex \
	$(srcdir)/mk4_files.tex $(srcdir)/multitone_pcal.tex \
	$(srcdir)/new_parameters.tex $(srcdir)/program_control.tex \
	$(srcdir)/program_output.tex $(srcdir)/svn_history.tex
tex_sync = fourfit_graph.pdf fringe_search_graph.pdf ionosphere2.png \
	ionosphere.png ion_search_graph.pdf \
	output_graph.pdf parse_control_file_graph.pdf

if HAVE_DOCS
docs_DATA = fourfit_users_manual.pdf
EXTRA_DIST = $(ff_man_files)
else
docs_DATA =
EXTRA_DIST =
endif

# A : at the end pulls in the default path
TEXINPUTS=.:$(srcdir):

fourfit_users_manual.pdf: $(tex_sources) force
	ls $(tex_sources)
	TEXINPUTS=$(TEXINPUTS) pdflatex fourfit_users_manual
	TEXINPUTS=$(TEXINPUTS) pdflatex fourfit_users_manual
	TEXINPUTS=$(TEXINPUTS) pdflatex fourfit_users_manual

force: Makefile
	for f in $(tex_sync) ; do cp -p $(srcdir)/$$f . ; done
	@echo forcing rebuild
	touch force

CLEANFILES = *.aux *.out *.log *.out *.pdf *.toc *.bib *.blg *.bbl *.bcf *.xml
CLEANFILES += *.png force

#
# eof
#
