#
# fourfit sources borrowed from fourfit.c
#
fourfit_sources = \
	$(fourfitdir)/set_defaults.c \
	$(fourfitdir)/parse_cmdline.c \
	$(fourfitdir)/check_rflist.c \
	$(fourfitdir)/organize_data.c \
	$(fourfitdir)/make_passes.c \
	$(fourfitdir)/fringe_search.c \
	$(fourfitdir)/get_corel_data.c \
	$(fourfitdir)/parse_control_file.c \
	$(fourfitdir)/refringe_list.c \
	$(fourfitdir)/time_range.c \
	$(fourfitdir)/set_pointers.c \
	$(fourfitdir)/pcal_interp.c \
	$(fourfitdir)/fill_param.c \
	$(fourfitdir)/skip_data.c \
	$(fourfitdir)/clear_pass.c \
	$(fourfitdir)/generate_cblock.c \
	$(fourfitdir)/apply_filter.c \
	$(fourfitdir)/precorrect.c \
	$(srcdir)/hook_search.c \
	$(fourfitdir)/ion_search.c \
	$(fourfitdir)/read_control_file.c \
	$(fourfitdir)/init_tokens.c \
	$(fourfitdir)/lex.c \
	$(fourfitdir)/create_fsm.c \
	$(fourfitdir)/parser.c \
	$(fourfitdir)/make_flist.c \
	$(fourfitdir)/nullify_cblock.c \
	$(fourfitdir)/clear_freq_corel.c \
	$(fourfitdir)/default_cblock.c \
	$(fourfitdir)/criteria_match.c \
	$(fourfitdir)/copy_cblock_parts.c \
	$(fourfitdir)/gate_off.c \
	$(srcdir)/hook_freq_spacing.c \
	$(fourfitdir)/search_windows.c \
	$(fourfitdir)/delay_rate.c \
	$(fourfitdir)/fft1.c \
	$(fourfitdir)/update.c \
	$(fourfitdir)/interp.c \
	$(fourfitdir)/create_fname.c \
	$(fourfitdir)/parabola.c \
	$(srcdir)/hook_vrot.c \
	$(fourfitdir)/get_bfstring.c \
	$(fourfitdir)/skip_index.c \
	$(fourfitdir)/calc_rms.c \
	$(fourfitdir)/fill_fringe_info.c \
	$(fourfitdir)/display_fplot.c \
	$(fourfitdir)/plot_complex.c \
	$(fourfitdir)/plot_graph.c \
	$(fourfitdir)/grid.c \
	$(fourfitdir)/put_char_at.c \
	$(fourfitdir)/sprint_char_arr.c \
	$(fourfitdir)/fill_200.c \
	$(fourfitdir)/fill_201.c \
	$(fourfitdir)/fill_202.c \
	$(fourfitdir)/fill_203.c \
	$(fourfitdir)/fill_204.c \
	$(fourfitdir)/fill_205.c \
	$(fourfitdir)/fill_206.c \
	$(fourfitdir)/fill_207.c \
	$(fourfitdir)/fill_208.c \
	$(fourfitdir)/fill_210.c \
	$(fourfitdir)/fill_222.c \
	$(fourfitdir)/compute_qf.c \
	$(fourfitdir)/compute_model.c \
	$(fourfitdir)/make_postplot.c \
	$(fourfitdir)/ap_mean.c \
	$(fourfitdir)/linterp.c \
	$(fourfitdir)/rotate_pcal.c \
	$(fourfitdir)/fill_212.c \
	$(fourfitdir)/fill_230.c \
	$(fourfitdir)/stcount_interp.c \
	$(fourfitdir)/calc_normalization.c \
	$(fourfitdir)/apply_passband.c \
	$(fourfitdir)/apply_notches.c \
	$(fourfitdir)/max555.c \
	$(fourfitdir)/interp555.c \
	$(fourfitdir)/pcalibrate.c \
	$(fourfitdir)/sampler_delays.c \
	$(fourfitdir)/norm_xf.c \
	$(fourfitdir)/norm_fx.c \
	$(srcdir)/hook_make_plotdata.c \
	$(srcdir)/hook_output.c \
	$(fourfitdir)/read_sdata.c \
	$(fourfitdir)/diff_file_phase.c \
	$(fourfitdir)/generate_graphs.c \
	$(fourfitdir)/generate_text.c \
	$(fourfitdir)/create_lockfile.c \
	$(fourfitdir)/wait_for_write_lock.c \
	$(fourfitdir)/fourfit_signal_handler.c \
	$(fourfitdir)/est_pc_manual.c \
	$(fourfitdir)/fourfit.c

BUILT_SOURCES =  search.c freq_spacing.c vrot.c make_plotdata.c output.c
search.c: $(fourfitdir)/search.c
	$(srcdir)/hook_import.sh $(srcdir)/../fourfit search.c

freq_spacing.c: $(fourfitdir)/freq_spacing.c
	$(srcdir)/hook_import.sh $(srcdir)/../fourfit freq_spacing.c

vrot.c: $(fourfitdir)/vrot.c
	$(srcdir)/hook_import.sh $(srcdir)/../fourfit vrot.c

make_plotdata.c: $(fourfitdir)/make_plotdata.c
	$(srcdir)/hook_import.sh $(srcdir)/../fourfit make_plotdata.c

output.c: $(fourfitdir)/output.c
	$(srcdir)/hook_import.sh $(srcdir)/../fourfit output.c

#
# eof
#
