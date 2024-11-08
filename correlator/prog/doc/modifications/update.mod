2000/03/29, 10:44; rjc; conductor; con_structs action_background identify_transition action_start_rot get_next_event iniitialize_substates log_state; log time stamp and duration of each state into /correlator/tmp/cond_state_log
2000/03/28, 10:59; rjc; sutrx; sutrx my_recv; Send red (bummer) box message for all tombstone errors.
2000/03/23, 09:38; cjl; util; ismk4; Made more robust in the face of post-Y2K Mk3 data
2000/03/23, 09:32; cjl; include; type_207.h; Added padding to version 1 structure for alignment of subsequent 208s
2000/03/23, 09:31; cjl; fplot; all; Added mk4 support
2000/03/21, 10:26; cjl; mk4fit; several; Completed modifications for phasecal
2000/03/20, 14:30; cjl; mk4fit; pcal_interp; Flipped sign and added 180 deg to conform to mk3 pcal convention
2000/03/20, 14:29; cjl; aedit; mk3_qf, others; Temporary fix to enable mk3-style quality codes in mk4
2000/03/15, 16:07; cjl; mk4fit; many; Partial implementation of phasecal, some new routines
2000/03/13, 10:41; cjl; mk4fit; fill_210; Fixed function data type bug
2000/03/13, 10:41; cjl; dfio; addr_208 copy_208; Fixed bug for version 1 records - prior to this fix v1 208 records were bad
2000/03/10, 16:56; cjl; mk4fit; fill_202; Enabled copy of VSNs from lvex into 202 record
2000/03/10, 16:55; cjl; mk4fit; compute_qf; Enabled some error codes
2000/03/09, 09:29; cjl; mk4fit; output display_fplot; removed traditional ascii fringe plot due to obsolescence
2000/03/08, 13:15; cjl; mk4fit; fill_210; New routine
2000/03/08, 12:56; cjl; dfio; many; Added support for type 210 record
2000/03/07, 14:06; rjc; genaroot; lp_year; Cleaned up code; also fixed to accept year as 1900 relative.
2000/03/07, 13:45; cjl; linkdata; alloc_lists dir_list; Increased allowed size of scan directory name
2000/03/07, 13:11; cjl; mk4fit; norm; Flipped sign of imaginary part for USB only, not LSB
2000/03/07, 13:11; cjl; mk4fit; make_flist; D Graham inspired fix to eliminate f.p. equality test
2000/03/06, 15:52; rjc; genaroot; doy_mmdd; Fixed bug that gave wrong result for February 29th of any leapyear.
2000/02/14, 14:29; cjl; mk4fit; compute_mode,fill_208; Insert totphase_ref in 208, apply BEC's memo more rigorously
2000/02/14, 13:47; cjl; mk4fit; tpstat_interp; Put in trap for corrupt type 304 records
2000/02/14, 09:54; cjl; mk4fit; fill_206; added math.h to fix fmod call
2000/02/14, 09:54; cjl; mk4fit; fill_205; added math.h to fix fmod call
2000/02/14, 09:29; cjl; dfio; clear_208,addr_208,copy_208; Added support for version 1
2000/02/09, 08:50; cjl; vex; cvex_info; Added auto_corr statement handling in CORR_CONFIG
2000/02/07, 16:24; rjc; conductor; take_action.c,action_kill_station.c; Correct logic for remove station while awaiting valid times.
2000/02/07, 15:22; rjc; conductor; action_read_streams.c; Implemented a new tsf command called hold, to put stream into hold state.
2000/01/19, 17:57; rjc; conductor; action_kill_station.c; fixed bug in remove_station functionality
2000/01/14, 11:54; cjl; util; time_to_double; New routine to simplify mk4 time computations in some places
2000/01/14, 11:53; cjl; mk4fit; make_postplot; Correct sign of clock quantities on plot
2000/01/14, 11:53; cjl; mk4fit; fill_202; Corrected apriori clock at frt to include integrated clockrate
2000/01/13, 14:58; rjc; conductor; action_background.c; Put source & suffix both in cond_status message (for opera to display).
2000/01/13, 13:56; cjl; mk4fit; time_range; Changed reference time for geodesy so closure is preserved
2000/01/10, 13:31; cjl; afio; many; Fix Y2K problem due to 2-digit year in A-file format
2000/01/05, 14:01; rjc; genaroot; vex_calc.c,$CALC/calc8/catmm.f,$CALC/calc8/control_init.f; Enable atmosphere, ignores zenatm input values, uses Niell dry model, Saastamoinen zenith value
1999/12/28, 15:43; cjl; genaroot; clock_adjust; changed units on clockrate adjustment
1999/12/28, 11:52; cjl; genaroot; vex_calc; put zatm in nanosec, not sec
1999/12/28, 11:51; cjl; mk4fit; make_postplot; Adjusted units and precisions in clock numbers
1999/12/28, 11:19; cjl; genaroot; clock_adjust; Fixed logic error, do_rate was always false
1999/12/22, 15:19; cjl; vex; get_pcm_config; Fixed bug in increment of ncfg
1999/12/21, 18:00; cjl; mk4fit; norm; Switched lag order, conjugated correlation vector, due to mk4 conventions
1999/12/20, 18:10; cjl; mk4fit; mk4fit fringe_search output display_fplot; Added quit option to fringe plot display
1999/12/20, 16:35; cjl; mk4fit; make_postplot; Adjusted scales etc to make plot fit A4 paper
1999/12/20, 16:34; cjl; mk4fit; set_pointers; Fixed memory allocation bug - use maxap instead of num_ap
1999/12/20, 13:43; cjl; dfio; display_221; Added q option to quit a set of displays
1998/05/20, 09:37; cjl; cofit; many; Robustness enhancement, addition of -c flag
1998/05/19, 10:06; cjl; fringex; fringex; Fixed indirection in write_fsumm call
1998/05/19, 10:06; cjl; fringex; calc_seg; Added missing return(0) at end
1998/05/19, 07:23; cjl; bfio; read_root; Used memset for t1000 init, copybuf/memcpy cause undiagnosed segfault under linux
1998/05/19, 07:05; cjl; alist; alist; Fixed bad indirection in write_r/c/fsumm calls
1998/04/22, 15:06; cjl; fourfit; make_fringeplot; Added LO offset to fringeplot
1998/04/06, 14:08; cjl; fringex; set_loops; Suppressed sub-second segmenting for -i all option
1998/03/11, 15:15; cjl; aedit; dup_check; fixed string termination bug
1997/12/15, 17:03; cjl; fringex; clear_fxp; Fixed incorrect indirection in clear_fsumm call
1997/11/21, 11:29; dboboltz; calamp; calamp.c, calamp.h, read_calfile.c; Fixed inf. loop in read_calfile.c; Added & chaged station codes station codes
1997/11/21, 10:22; cjl; bfio; data.h; Increased MAX_T2000 to 100, eliminates bug for spectral line
1997/09/15, 14:12; cjl; aedit; sorter plot_fqex; Fixed seg faults under linux - bad indirection and modifying constant arg
1997/09/02, 08:50; cjl; calamp; read_calamp; Deal with inline comments in cal data
1997/08/05, 14:57; cjl; util; get_filelist root_belong syntax; Fixed some int/ptr compilation warnings
1997/08/05, 14:37; cjl; shell_scripts; mod; Switched to tcsh, and added some portability features
1997/04/02, 16:12; cjl; fourfit; fill_4000; Fixed bug in identifying drive numbers
1997/04/02, 10:44; cjl; mmreduce; make_vis; added force option to aedit close command
1997/04/02, 10:44; cjl; aedit; fill_closure fill_tdata; fixed bug when triangles rejected for epoch/freq mismatch
1997/04/02, 10:44; cjl; aedit; execute fill_closure; Added option to force triangle generation even when epoch/freq mismatch
1997/03/31, 10:51; cjl; aedit; fill_tdata; corrected logic for expansion of tdata array
1997/03/18, 17:25; cjl; fourfit; search; expanded mbdelay transform array to 1024
1997/03/03, 08:37; cjl; aedit; axis; Replaced scale reset with a no-op and a warning
1996/12/16, 13:09; cjl; fourfit; search; Add initialization of pcal arrays
1996/12/06, 13:11; cjl; util; time_to_int; quick fix for millenium
1996/12/06, 09:44; cjl; ccfio; ccfile.h; Increased space in HEAD section of structure
1996/12/03, 15:14; cjl; cdump; execute; Commented out line whick seeks to zero on opcode 3
1996/11/13, 16:22; rjc; fourfit; draw_graphs; Changed calculation of d.r. spectrum plot symbols upper bound.
1996/11/07, 10:48; cjl; aedit; fill_tdata fill_closure; Improved rejection of triangles with mismatched baseline parameters
1996/11/04, 10:52; cjl; aedit; many; Several changes to make closure calculations more robust and consistent
1996/10/03, 16:01; rjc; fourfit; interp; Fixed bug that prevented interpolation beyond wide open window (gave E-code)
1996/10/01, 08:38; cjl; adump; strip_bline parse_bfields; Added coherence times to extractable parameters
1996/09/23, 13:24; rjc; fourfit; time_range; Disallow data which starts its AP before start of scan.
1996/09/23, 11:07; cjl; aedit; ps_scanlabel; Corrected bug which left seconds at zero
1996/09/23, 11:07; cjl; aedit; fill_parms usearray.h param_list.h; Added some more extractable parameters
1996/08/28, 13:44; rjc; fourfit; search; Fixed bug introduced in wrap around code for case of 0 width mbd window.
1996/08/20, 10:38; rjc; fourfit; grid,interp,make_fringeplot,search,search_windows; Allow multiband search windows to wrap around the natural window
1996/08/01, 14:32; cjl; aedit; plot_fqex; added editing capability after plotting each page
1996/07/26, 14:19; cjl; coterp; parse_cmdline; Added no-op fprintf to init FILE struct, apparent HP bug
1996/06/11, 15:28; cjl; average; triangle_avg baseline_avg; Fixed bug causing time tag errors in scan averaging
1996/06/11, 15:27; cjl; aedit; fill_closure; Fixed minor rounding bug in scan offset calculation
1996/06/11, 13:46; cjl; aedit; fill_closure; Prevent zero divide for zero-length triangle
1996/06/10, 13:37; cjl; fourfit; make_fringeplot; Added pass numbers to fringe plot
1996/06/10, 13:29; cjl; aedit; fill_parms; Added track numbers and number of aps to extracted params
1996/03/13, 16:40; cjl; afio; parse_tsumm; Long-standing bug in reading version 4
1996/02/07, 16:35; cjl; afio; write_fsumm; added more precision to delay_rate
1995/12/20, 15:50; cjl; aedit; init_inputs; procdate fields need initializing
1995/12/07, 14:06; cjl; util; clear_fstruct; added intparm and floatparm
1995/12/07, 14:01; cjl; include; fstruct.h; added a couple of auxiliary parameters (for fringex, but generally useful)
1995/11/30, 09:56; cjl; aedit; set_procrange; Fixed bug for case of 0 args
1995/11/24, 15:11; cjl; util; msg; Changed to write to stderr instead of stdout
1995/11/22, 16:49; cjl; fourfit; refringe_list; Fixed two bugs, line buffer overflow and wrong indirection for fdata
1995/11/08, 11:49; cjl; alist; Support for A-file format v4
1995/11/08, 11:48; cjl; include; adata.h; Implemented version 4 of the format
1995/11/08, 11:48; cjl; afio; all; Implemented version 4 of the format
1995/10/31, 15:19; cjl; alist; summarize_fringe; Added calculation of offset field
1995/10/19, 14:00; cjl; linkdata; linkdata; Installed warning for relative datadir pathname
1995/10/04, 16:28; cjl; average; average read_data sort_data; Allowed average to read from stdin, expanded sorting to seg times
1995/08/18, 16:39; dole; fringex; ; corrected scan_time calc. and put in correct form of avsec
1995/08/17, 16:21; cjl; bfio; read_fringe; Enhance error reporting
1995/08/04, 13:39; cjl; util; check_name others; Support type-3 and type-4 files for Mk4
1995/07/26, 12:10; cjl; shell_scripts; efind; Modified to allow afile numbers beyond 9999
1995/07/20, 10:10; cjl; fringex; all; Modified output fields, changed calling sequence, some internal reorganization
1995/05/04, 16:09; cjl; shell_scripts; dribble; Fixed error reporting for rename failure
1995/05/03, 11:42; cjl; aedit; many; Implemented multi-band psplotting, and psfile generation
1995/04/25, 13:01; cjl; bfio; read_root; Trapped HP floating point format files
1995/04/19, 12:04; cjl; fourfit; organize_data; Inserted trap for baseline number above 63
1995/04/13, 10:07; cjl; aedit; write_prmline; Removed accidental ANSI trigraph from string literal
1995/04/05, 10:22; cjl; linkdata; read_afiles; modified to use aline_id()
1995/04/05, 10:21; cjl; fourfit; refringe_list; modified to use aline_id()
1995/04/05, 10:09; cjl; adump; many; Upgrade for type-3 lines, format version 3
1995/04/05, 10:06; cjl; afio; parse_tsumm clear_tsumm; Fix minor version 3 bug
1995/04/04, 12:52; cjl; alist; summarize_fringe alist; Support version 3 afile format, make it default version
1995/04/04, 11:28; cjl; aedit; pstag_process write_data write_tdata; Modified to use afio library header routine
1995/04/04, 11:15; cjl; afio; afile_header; New routine to timestamp/label A-files
1995/04/03, 11:30; cjl; aedit; write_data write_families; Fixed bug caused by hardcoded sort key
1995/03/30, 16:34; cjl; cdump; rec_formats.h; fixed number of items in 2600 record
1995/03/29, 12:24; cjl; include; adata.h; Added fields for version 3
1995/03/29, 12:23; cjl; afio; parse_csumm parse_fsumm parse_rsumm parse_tsumm write_csumm write_fsumm write_rsumm write_tsumm; Added support for version 3 A-file format
1995/03/28, 15:58; cjl; modformat; modformat; Fixed bug in handling end of description
1995/03/27, 12:51; cjl; shell_scripts; monitor; Added more error checking, enabled logging
1995/03/27, 12:51; cjl; shell_scripts; drool; New script to keep transfer area tidy
1995/03/27, 12:51; cjl; shell_scripts; dribble; Added handling of missing .sent files
1995/03/26, 23:54; cjl; shell_scripts; dribble; Added date stamps to some messages
1995/03/22, 16:28; cjl; afio; parse_rsumm parse_csumm write_rsumm write_csumm; Trivial mods for format version 3
1995/03/22, 09:46; cjl; rename; fill_rootnames read_corelnames; Implemented proper error returns for dribble mode
1995/03/17, 14:44; cjl; aedit; fill_closure; Fix ambiguity zero divide in cases of only 1 freq channel
1995/03/17, 14:43; cjl; shell_scripts; dribble; Many tweaks after testing with real data flow
1995/03/15, 15:14; cjl; aedit; fill_parms; Fixed some silly bugs in utc calculations
1995/03/08, 11:16; cjl; aedit; write_prmline; Expanded overflowing char array
1995/03/03, 16:17; cjl; aedit; aedit parse_cmdline; Implemented -b option
1995/03/03, 15:48; cjl; adump; all; Initial implementation
1995/03/02, 09:27; cjl; aedit; execute write_param write_prmline; Added optional precision argument to pwrite command
1995/03/02, 08:48; cjl; aedit; many; Changed parameters to double precision, added 2 new parameters
1995/03/02, 08:48; cjl; aedit; allocate_parms; Fixed bug causing bus error on repeated use of param command
1995/02/24, 12:52; cjl; shell_scripts; dribble; Added error strings to status file
1995/02/20, 13:01; cjl; alist; summarize_fringe; changed phase_type to datatype with sensible default
1995/02/20, 12:56; cjl; include; adata.h; changed phase_type to datatype in type 2 record
1995/02/20, 12:52; cjl; afio; clear_fsumm parse_fsumm write_fsumm; Changed phase_type into datatype, a 2-char string
1995/02/17, 10:00; cjl; afio; write_fsumm; Changed default 'parent' string to '0' instead of null
1995/02/17, 09:53; cjl; afio; parse_fsumm; Added a diagnostic message
1995/02/13, 09:23; cjl; aedit; plot_fqex; Fixed small bug in source/symbol assignment
1995/02/08, 08:57; cjl; rename; many; Major update to support -d option (dribble mode)
1995/02/07, 14:52; cjl; shell_scripts; monitor; A new script to pop up a hpterm window on an X screen at the correlator
1995/02/07, 14:52; cjl; shell_scripts; dribble; Added automatic test fourfitting of transferred data
1995/02/06, 11:24; cjl; modformat; modformat; Fix bug which trips up Sun but not HP
1995/02/06, 10:20; cjl; ccfio; many; Changed strerror to sys_errlist for portability
1995/02/06, 09:56; cjl; ddstat; many; Replace strerror with sys_errlist for portability
1995/02/06, 09:46; cjl; rename; fill_cmdfile; Remove symlink.h (not present on Suns)
1995/02/06, 09:43; cjl; unix_to_ci; unix_to_ci; Remove symlink.h (not present on Suns)
1995/02/06, 09:30; cjl; ddstat; chk_sub_expdir; Remove symlink.h, not present on Suns
1995/02/06, 09:16; cjl; aedit; allocate_parms; Changed strerror to sys_errlist for portability
1995/02/06, 09:14; cjl; linkdata; make_link read_afiles; Changed strerror to sys_errlist for portability
1995/01/31, 11:46; cjl; fourfit; output; Fix bug in write_fringe call (found by M. Britton, UCSB)
1995/01/30, 10:07; cjl; fourfit; fft1 delay_rate; Changed array sizes to use MAXACCPER in data.h
1995/01/26, 13:04; cjl; aedit; flags.h get_plot_data; Fixed bug in flag bit manipulation for station plots
1995/01/26, 11:00; cjl; aedit; plot.c; Fixed bug in plotting of u and v
1995/01/26, 09:34; cjl; fourfit; fill_freq_table partner_freqs; Split off USB/LSB matching in new routine partner_freqs()
1995/01/25, 16:48; cjl; fourfit; fill_freq_table; Altered sideband pair detection to allow multiple channels with same freq
1995/01/24, 08:49; cjl; alist; parse_cmdline alist summarize_fringe; Added -fr and -ff command line flags to ease transition from HP1000
1995/01/20, 14:34; rjc; fourfit; fill_4300.c; Fixed detection of qf=2 (low amp channel) scans.
1995/01/19, 16:48; cjl; util; report_times; Raised message levels to 3
1995/01/19, 16:48; cjl; include; fstruct.h; Raised file limit to 100000
1995/01/19, 11:31; cjl; fourfit; apply_filter.c search.c; Added filter for too few bits in AP, skip pcal calcs for a freq if no APs
1995/01/19, 11:30; rjc; fourfit; make_fringeplot; Fix recent bug that causes fr/asec in NS and EW to be swapped.
1995/01/18, 16:10; cjl; fourfit; organize_data; Fix long-standing bug in extraction of baseline number from corel records
1995/01/18, 12:24; cjl; fourfit; summarize_result; show delay rate instead of fringe rate
1995/01/18, 10:52; cjl; bfio; read_root; Fix error in type-1000 record initialization
1995/01/12, 15:11; cjl; aedit; plot.c; Added check for filters rejecting all plot data
1995/01/11, 12:22; cjl; fourfit; organize_data; Fix bug in Mk3/Mk3a determination for pulsar mode
1995/01/09, 12:15; cjl; shell_scripts; dribble; Slight mods to match ARW's dribble mode conventions
1995/01/05, 12:55; cjl; afio; write_fsumm.c; Fixed bug in total_sbresid ... a fabs() call was removed
1995/01/04, 14:05; cjl; ddstat; ddstat.h; Changed "long" to "int" (alpha portability)
1995/01/04, 13:55; cjl; include; various; Changed all "long" declarations to "int"
1995/01/04, 13:54; cjl; x_fplot; get_gc.c; Changed "long" declarations to "int"
1995/01/04, 13:45; cjl; bfio; swap.c; fix bug in float_reverse
1995/01/04, 13:01; cjl; include; data.h; Add padding in data_fringe struct for data alignment
1995/01/04, 12:10; cjl; fourfit; calc_rms; Patched th_timerms_phase to be always nonzero
1995/01/03, 13:46; cjl; fourfit; control.h param_struct.h bcd_qmsec.c; Changed long to int for portability on alpha machines
1994/12/28, 13:10; rjc; fourfit; norm; Apply fractional bit amplitude correction (and fix formulas)
1994/12/22, 16:08; rjc; fourfit; apply_filter,organize_data,make_fringeplot; Implemented pulsar mode for Mark 3A correlations only.
1994/12/15, 14:12; cjl; aedit; dup_check; Allowed match on reversed baseline strings
1994/12/09, 14:57; rjc; fourfit; draw_graphs,make_plotdata,clear_plotdata; Fixed phase cal phase plots when near 180 degrees.
1994/12/09, 10:57; cjl; aedit; auto_hardcopy set_device; Sent auto hardcopy plot files to /tmp for robustness
1994/12/09, 10:12; cjl; aedit; clear; Improved handling of clear close and clearing of active plots
1994/12/08, 14:22; rjc; fourfit; parse_cmdline; Properly prototyped bf_string() as a char
1994/12/08, 11:29; cjl; fourfit; get_xref fill_freq_table; Fix bug by properly ignoring -1 entries for index number in xref table
1994/12/08, 09:26; cjl; cdump; many; Added pcal amp/phase display for corel files
1994/12/06, 14:13; cjl; aedit; write_prmline; Increased precision of output format
1994/12/06, 14:13; cjl; aedit; fill_parms usearray.h param_list.h extract_parms; Added utc_central to list of extractable parameters
1994/12/06, 14:13; cjl; aedit; cbind; Changed to new pgplot library, put cbind in aedit instead of library
1994/12/02, 09:20; rjc; fourfit; fill_4500; Fix bug: make be sb_win, not mb_win
1994/12/02, 09:20; rjc; fourfit; delay_rate,make_plotdata; Fix AP index bug affecting long scans
1994/11/21, 14:17; rjc; fourfit; precorrect,search; allow phase cal offsets in all 3 modes
1994/11/21, 14:17; rjc; fourfit; make_fringeplot; changed phase cal Mode designation to letter keys (N, A, and M)
1994/10/07, 11:05; rjc; fourfit; make_fringeplot; allow phase cal amplitudes to be > 99
1994/10/07, 11:02; rjc; fourfit; draw_graphs; fixed bug in phase cal plots when start of data is modified
1994/09/27, 11:36; rjc; fourfit; parser,init_tokens; Allow station also allow global pc_mode specification
1994/09/22, 16:50; cjl; afio; summarize_corel; Trapped null character for corel quality code in ancient data
1994/09/21, 12:15; cjl; aedit; usearray.h param_list.h fill_parms; Added amplitude incoherently averaged over segments to parameter list
1994/09/21, 09:41; cjl; ccfio; write_source; Adjusted output format for declination
1994/09/14, 12:48; cjl; bfio; read_corel; modified error printouts to be more informative, less annoying
1994/09/14, 12:46; cjl; fourfit; get_corel_data; Fixed bug in error return handling of read_corel
1994/09/14, 10:57; rjc; fourfit; calc_pcal; Don't reject large DELAYZ phase-cals in case of Mark 3
1994/09/08, 13:40; cjl; aedit; dozens_of_them; Major overhaul for version 4.1, with support for closure data. Far too many changes to list.
1994/09/08, 11:28; cjl; fourfit; get_corel_data; Made it more tolerant of return values from read_corel
1994/09/08, 09:11; rjc; fourfit; make_passes; Require zero delay (and baseline) for autocorrelation status.
1994/09/07, 16:56; aeer; fringex; fringex.c; added -q option to allow segments less than 1 sec
1994/09/07, 13:54; rjc; fourfit; apply_filter; Expanded printout of summary statistics for data rejection
1994/09/07, 10:22; rjc; fourfit; many; Implemented ad hoc phase correction (polynomial or sinewave)
1994/09/02, 16:47; cjl; bfio; all; Added byte-reversal routines to accommodate DEC alpha machines
1994/08/02, 13:38; cjl; aedit; flag_by_type; Removed - obsolete.
1994/08/02, 13:28; cjl; aedit; trngl_present; bug fix, string not terminated properly.
1994/08/02, 10:59; cjl; modformat; modformat; Added program name handling
1994/08/02, 10:37; cjl; mod; ; Added program name to output
1994/08/02, 10:18; cjl; modformat; modformat; Added filter for grep output format
1994/08/02, 08:52; cjl; afio; clear_trisumm, parse_trisumm, write_trisumm, adata.h; Created routines to support closure triangle A-file format
1994/07/25, 09:23; cjl; mod; ; Initial version released
2004/02/26, 10:27; rjc; update; build; Get rid of make for rotman on hppa machine from build script.
2005/06/21, 16:50; rjc; update; make_tarball; included into gtars some library references for the plplot packae
2005/11/14, 16:30; rjc; update; build; added solo and soloist to build script
