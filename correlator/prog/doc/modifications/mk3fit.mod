1994/09/07, 10:22; rjc; mk3fit; many; Implemented ad hoc phase correction (polynomial or sinewave)
1994/09/07, 13:54; rjc; mk3fit; apply_filter; Expanded printout of summary statistics for data rejection
1994/09/08, 09:11; rjc; mk3fit; make_passes; Require zero delay (and baseline) for autocorrelation status.
1994/09/08, 11:28; cjl; mk3fit; get_corel_data; Made it more tolerant of return values from read_corel
1994/09/14, 10:57; rjc; mk3fit; calc_pcal; Don't reject large DELAYZ phase-cals in case of Mark 3
1994/09/14, 12:46; cjl; mk3fit; get_corel_data; Fixed bug in error return handling of read_corel
1994/09/27, 11:36; rjc; mk3fit; parser,init_tokens; Allow station also allow global pc_mode specification
1994/10/07, 11:02; rjc; mk3fit; draw_graphs; fixed bug in phase cal plots when start of data is modified
1994/10/07, 11:05; rjc; mk3fit; make_fringeplot; allow phase cal amplitudes to be > 99
1994/11/21, 14:17; rjc; mk3fit; precorrect,search; allow phase cal offsets in all 3 modes
1994/11/21, 14:17; rjc; mk3fit; make_fringeplot; changed phase cal Mode designation to letter keys (N, A, and M)
1994/12/02, 09:20; rjc; mk3fit; delay_rate,make_plotdata; Fix AP index bug affecting long scans
1994/12/02, 09:20; rjc; mk3fit; fill_4500; Fix bug: make be sb_win, not mb_win
1994/12/08, 11:29; cjl; mk3fit; get_xref fill_freq_table; Fix bug by properly ignoring -1 entries for index number in xref table
1994/12/08, 14:22; rjc; mk3fit; parse_cmdline; Properly prototyped bf_string() as a char
1994/12/09, 14:57; rjc; mk3fit; draw_graphs,make_plotdata,clear_plotdata; Fixed phase cal phase plots when near 180 degrees.
1994/12/22, 16:08; rjc; mk3fit; apply_filter,organize_data,make_fringeplot; Implemented pulsar mode for Mark 3A correlations only.
1994/12/28, 13:10; rjc; mk3fit; norm; Apply fractional bit amplitude correction (and fix formulas)
1995/01/03, 13:46; cjl; mk3fit; control.h param_struct.h bcd_qmsec.c; Changed long to int for portability on alpha machines
1995/01/04, 12:10; cjl; mk3fit; calc_rms; Patched th_timerms_phase to be always nonzero
1995/01/11, 12:22; cjl; mk3fit; organize_data; Fix bug in Mk3/Mk3a determination for pulsar mode
1995/01/18, 12:24; cjl; mk3fit; summarize_result; show delay rate instead of fringe rate
1995/01/18, 16:10; cjl; mk3fit; organize_data; Fix long-standing bug in extraction of baseline number from corel records
1995/01/19, 11:30; rjc; mk3fit; make_fringeplot; Fix recent bug that causes fr/asec in NS and EW to be swapped.
1995/01/19, 11:31; cjl; mk3fit; apply_filter.c search.c; Added filter for too few bits in AP, skip pcal calcs for a freq if no APs
1995/01/20, 14:34; rjc; mk3fit; fill_4300.c; Fixed detection of qf=2 (low amp channel) scans.
1995/01/25, 16:48; cjl; mk3fit; fill_freq_table; Altered sideband pair detection to allow multiple channels with same freq
1995/01/26, 09:34; cjl; mk3fit; fill_freq_table partner_freqs; Split off USB/LSB matching in new routine partner_freqs()
1995/01/30, 10:07; cjl; mk3fit; fft1 delay_rate; Changed array sizes to use MAXACCPER in data.h
1995/01/31, 11:46; cjl; mk3fit; output; Fix bug in write_fringe call (found by M. Britton, UCSB)
1995/04/05, 10:21; cjl; mk3fit; refringe_list; modified to use aline_id()
1995/04/19, 12:04; cjl; mk3fit; organize_data; Inserted trap for baseline number above 63
1995/11/22, 16:49; cjl; mk3fit; refringe_list; Fixed two bugs, line buffer overflow and wrong indirection for fdata
1996/06/10, 13:37; cjl; mk3fit; make_fringeplot; Added pass numbers to fringe plot
1996/08/20, 10:38; rjc; mk3fit; grid,interp,make_fringeplot,search,search_windows; Allow multiband search windows to wrap around the natural window
1996/08/28, 13:44; rjc; mk3fit; search; Fixed bug introduced in wrap around code for case of 0 width mbd window.
1996/09/23, 13:24; rjc; mk3fit; time_range; Disallow data which starts its AP before start of scan.
1996/10/03, 16:01; rjc; mk3fit; interp; Fixed bug that prevented interpolation beyond wide open window (gave E-code)
1996/11/13, 16:22; rjc; mk3fit; draw_graphs; Changed calculation of d.r. spectrum plot symbols upper bound.
1996/12/16, 13:09; cjl; mk3fit; search; Add initialization of pcal arrays
1997/03/18, 17:25; cjl; mk3fit; search; expanded mbdelay transform array to 1024
1997/04/02, 16:12; cjl; mk3fit; fill_4000; Fixed bug in identifying drive numbers
1998/04/22, 15:06; cjl; mk3fit; make_fringeplot; Added LO offset to fringeplot
2000/05/15, 09:50; cjl; mk3fit; several; Changed name of program to mk3fit (also doc and mod files)
