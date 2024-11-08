1994/08/02, 13:28; cjl; aedit; trngl_present; bug fix, string not terminated properly.
1994/08/02, 13:38; cjl; aedit; flag_by_type; Removed - obsolete.
1994/09/08, 13:40; cjl; aedit; dozens_of_them; Major overhaul for version 4.1, with support for closure data. Far too many changes to list.
1994/09/21, 12:15; cjl; aedit; usearray.h param_list.h fill_parms; Added amplitude incoherently averaged over segments to parameter list
1994/12/06, 14:13; cjl; aedit; cbind; Changed to new pgplot library, put cbind in aedit instead of library
1994/12/06, 14:13; cjl; aedit; write_prmline; Increased precision of output format
1994/12/06, 14:13; cjl; aedit; fill_parms usearray.h param_list.h extract_parms; Added utc_central to list of extractable parameters
1994/12/09, 10:12; cjl; aedit; clear; Improved handling of clear close and clearing of active plots
1994/12/09, 10:57; cjl; aedit; auto_hardcopy set_device; Sent auto hardcopy plot files to /tmp for robustness
1994/12/15, 14:12; cjl; aedit; dup_check; Allowed match on reversed baseline strings
1995/01/12, 15:11; cjl; aedit; plot.c; Added check for filters rejecting all plot data
1995/01/26, 11:00; cjl; aedit; plot.c; Fixed bug in plotting of u and v
1995/01/26, 13:04; cjl; aedit; flags.h get_plot_data; Fixed bug in flag bit manipulation for station plots
1995/02/06, 09:16; cjl; aedit; allocate_parms; Changed strerror to sys_errlist for portability
1995/02/13, 09:23; cjl; aedit; plot_fqex; Fixed small bug in source/symbol assignment
1995/03/02, 08:48; cjl; aedit; many; Changed parameters to double precision, added 2 new parameters
1995/03/02, 08:48; cjl; aedit; allocate_parms; Fixed bug causing bus error on repeated use of param command
1995/03/02, 09:27; cjl; aedit; execute write_param write_prmline; Added optional precision argument to pwrite command
1995/03/03, 16:17; cjl; aedit; aedit parse_cmdline; Implemented -b option
1995/03/08, 11:16; cjl; aedit; write_prmline; Expanded overflowing char array
1995/03/15, 15:14; cjl; aedit; fill_parms; Fixed some silly bugs in utc calculations
1995/03/17, 14:44; cjl; aedit; fill_closure; Fix ambiguity zero divide in cases of only 1 freq channel
1995/04/03, 11:30; cjl; aedit; write_data write_families; Fixed bug caused by hardcoded sort key
1995/04/04, 11:28; cjl; aedit; pstag_process write_data write_tdata; Modified to use afio library header routine
1995/04/13, 10:07; cjl; aedit; write_prmline; Removed accidental ANSI trigraph from string literal
1995/05/03, 11:42; cjl; aedit; many; Implemented multi-band psplotting, and psfile generation
1995/11/30, 09:56; cjl; aedit; set_procrange; Fixed bug for case of 0 args
1995/12/20, 15:50; cjl; aedit; init_inputs; procdate fields need initializing
1996/06/10, 13:29; cjl; aedit; fill_parms; Added track numbers and number of aps to extracted params
1996/06/11, 13:46; cjl; aedit; fill_closure; Prevent zero divide for zero-length triangle
1996/06/11, 15:27; cjl; aedit; fill_closure; Fixed minor rounding bug in scan offset calculation
1996/08/01, 14:32; cjl; aedit; plot_fqex; added editing capability after plotting each page
1996/09/23, 11:07; cjl; aedit; ps_scanlabel; Corrected bug which left seconds at zero
1996/09/23, 11:07; cjl; aedit; fill_parms usearray.h param_list.h; Added some more extractable parameters
1996/11/04, 10:52; cjl; aedit; many; Several changes to make closure calculations more robust and consistent
1996/11/07, 10:48; cjl; aedit; fill_tdata fill_closure; Improved rejection of triangles with mismatched baseline parameters
1997/03/03, 08:37; cjl; aedit; axis; Replaced scale reset with a no-op and a warning
1997/03/31, 10:51; cjl; aedit; fill_tdata; corrected logic for expansion of tdata array
1997/04/02, 10:44; cjl; aedit; fill_closure fill_tdata; fixed bug when triangles rejected for epoch/freq mismatch
1997/04/02, 10:44; cjl; aedit; execute fill_closure; Added option to force triangle generation even when epoch/freq mismatch
1997/09/15, 14:12; cjl; aedit; sorter plot_fqex; Fixed seg faults under linux - bad indirection and modifying constant arg
1998/03/11, 15:15; cjl; aedit; dup_check; fixed string termination bug
2000/03/20, 14:29; cjl; aedit; mk3_qf, others; Temporary fix to enable mk3-style quality codes in mk4
2000/04/20, 09:27; cjl; aedit; fplot; Added mk4 fringe plot capability
2000/04/20, 13:26; cjl; aedit; cursor_select edit zoom fplot; Added hardcopy and file save options to mk4 fringe plot popup
2000/04/21, 09:07; cjl; aedit; extract_parms fill4_parms; Implemented parameter extraction for Mk4, for subset of parameters
2000/04/21, 14:25; cjl; aedit; plot_quality; temporary fix to enable correct plot colors
2000/04/21, 16:48; cjl; aedit; set_timerange set_procrange; Fixed y2k bugs
2000/05/02, 14:29; cjl; aedit; summary.h clear_fsumm clear_source pr_fsumm summ_fringe; Accommodate qcode G in summary listings
2000/08/31, 17:16; cjl; aedit; sizelimits.h; Set maxversion to 5
2001/02/01, 12:08; cjl; aedit; 13 routines modified, 6 new routines; New reproc command, psfile capability implemented for Mk4
2001/02/01, 12:08; cjl; aedit; several; Improved support for G codes
2001/02/02, 15:02; cjl; aedit; modified 7 routines, one new routine; Added psplot capability for Mk4
2001/02/05, 13:29; cjl; aedit; pstag_process; Fixed bug which caused processing of only the first subgroup
2001/02/15, 11:53; cjl; aedit; 15 modified, 1 added; Added polarization support for type 2 data, not for type 3 yet
2001/02/15, 12:55; cjl; aedit; none; Updated various help files, added new ones for polarization and reproc
2001/05/16, 09:20; cjl; aedit; tfilter; Eliminate error return due to polarization filter
2001/07/18, 15:02; cjl; aedit; many; Support for long source names
2001/08/14, 15:14; cjl; aedit; dup_check; Added test for polarization
2002/01/02, 09:21; cjl; aedit; many; Added proper support for H-codes throughout
2008/06/19, 09:43; rjc; aedit; set_sources; Allow 32 char source names
2008/12/05, 14:30; rjc; aedit; makekey sizelimits; allow 500 sources (was 10, catch overruns of array
