1999/12/20, 16:34; cjl; fourfit; set_pointers; Fixed memory allocation bug - use maxap instead of num_ap
1999/12/20, 16:35; cjl; fourfit; make_postplot; Adjusted scales etc to make plot fit A4 paper
1999/12/20, 18:10; cjl; fourfit; fourfit fringe_search output display_fplot; Added quit option to fringe plot display
1999/12/21, 18:00; cjl; fourfit; norm; Switched lag order, conjugated correlation vector, due to mk4 conventions
1999/12/28, 11:51; cjl; fourfit; make_postplot; Adjusted units and precisions in clock numbers
2000/01/13, 13:56; cjl; fourfit; time_range; Changed reference time for geodesy so closure is preserved
2000/01/14, 11:53; cjl; fourfit; fill_202; Corrected apriori clock at frt to include integrated clockrate
2000/01/14, 11:53; cjl; fourfit; make_postplot; Correct sign of clock quantities on plot
2000/02/14, 09:54; cjl; fourfit; fill_205; added math.h to fix fmod call
2000/02/14, 09:54; cjl; fourfit; fill_206; added math.h to fix fmod call
2000/02/14, 13:47; cjl; fourfit; tpstat_interp; Put in trap for corrupt type 304 records
2000/02/14, 14:29; cjl; fourfit; compute_mode,fill_208; Insert totphase_ref in 208, apply BEC's memo more rigorously
2000/03/07, 13:11; cjl; fourfit; make_flist; D Graham inspired fix to eliminate f.p. equality test
2000/03/07, 13:11; cjl; fourfit; norm; Flipped sign of imaginary part for USB only, not LSB
2000/03/08, 13:15; cjl; fourfit; fill_210; New routine
2000/03/09, 09:29; cjl; fourfit; output display_fplot; removed traditional ascii fringe plot due to obsolescence
2000/03/10, 16:55; cjl; fourfit; compute_qf; Enabled some error codes
2000/03/10, 16:56; cjl; fourfit; fill_202; Enabled copy of VSNs from lvex into 202 record
2000/03/13, 10:41; cjl; fourfit; fill_210; Fixed function data type bug
2000/03/15, 16:07; cjl; fourfit; many; Partial implementation of phasecal, some new routines
2000/03/20, 14:30; cjl; fourfit; pcal_interp; Flipped sign and added 180 deg to conform to mk3 pcal convention
2000/03/21, 10:26; cjl; fourfit; several; Completed modifications for phasecal
2000/04/07, 16:08; cjl; fourfit; time-range parse_cmdline; Added -T option for reftime relative to global scan start, for closure
2000/05/15, 09:49; cjl; fourfit; several; Changed name of program to fourfit from mk4fit (also doc and mod files)
2000/06/07, 10:53; cjl; fourfit; fill_206; Fix a bug in number of APs, found by L. Petrov
2000/06/23, 16:21; rjc; fourfit; time_range; adjust scheduled start time to be start of AP that it occurs in
2000/07/17, 11:35; rjc; fourfit; make_plotdata; Zero out all of X and Y arrays prior to XPower FFT
2000/07/24, 11:25; cjl; fourfit; time_range; Use new fourfit_reftime vex parameteras override
2000/07/25, 09:38; cjl; fourfit; fill_204; Set version number to 1.0
2000/07/27, 16:39; rjc; fourfit; fringe_search; Enhanced diagnostics for memory allocation failures.
2000/11/15, 11:02; cjl; fourfit; ap_mean; Fixed incorrect declaration of linterp()
2000/11/15, 11:02; cjl; fourfit; many; Implemented -P flag to permit handling of multi-polarization files, pol selection
2000/11/15, 11:02; cjl; fourfit; pcal_interp; Added trap for missing type-3 files, which crashed linux version
2000/11/15, 11:02; cjl; fourfit; many; Implemented weights for fractional APs, affecting many routines and calculations
2000/11/16, 15:22; cjl; fourfit; set_pointers; Reduced print priority of polarization processing report
2001/01/17, 12:24; rjc; fourfit; vrot; Modified sb phase correction (no-op for mults. of 8 lags)
2001/01/19, 15:07; rjc; fourfit; norm; correct normalization for 2-bit data
2001/02/14, 16:28; cjl; fourfit; fill_207, make_postplot; Support polarization properly on plot and error rate statistics
2001/02/14, 16:30; cjl; fourfit; rotate_pcal; Fixed bug in ap by ap mode for remote station
2001/02/14, 16:32; rjc; fourfit; interp; Don't use fractional sbd when doing the sbd tabular interpolation.
2001/02/27, 11:47; rjc; fourfit; apply_filter,compute_cf,filter.h,statistics.h,param_struct.h; Use new status & flag bits from t120 records.
2001/04/12, 10:58; rjc; fourfit; make_postplot; Print input filename at bottom of page.
2001/04/13, 13:49; rjc; fourfit; pcal_interp; Fixed bug that applied wrong pcal in 50% of pol. scans.
2001/04/18, 15:24; rjc; fourfit; tpstat_interp; fix factor of 8 bug in perr rates; print perrs at -m1 level
2001/04/26, 13:36; cjl; fourfit; fill_fringe_info fill_212 fill_230 parse_cmdline; Added type 212 and optional 230 records to fourfit output file
2001/05/25, 10:32; rjc; fourfit; fourfit; changed version number to 3.0
2001/05/25, 11:25; rjc; fourfit; fill_204; copy version # into type 204 record
2001/05/25, 11:54; rjc; fourfit; search; use in trapping for too many autocorrelation lags
2001/05/31, 11:59; cjl; fourfit; fill_212; Fix indexing bug which caused error for nonzero start offsets in control file
2001/06/18, 14:53; rjc; fourfit; <many>; Allow pcal tone freqs to be specified for each channel.
2001/07/06, 13:43; cjl; fourfit; fill_206; Added computation of weights, changed status array ap_frac to double
2001/07/17, 13:56; cjl; fourfit; fill_230; Support new sideband weights
2001/07/18, 11:17; cjl; fourfit; several; Added support for long source names
2001/07/19, 10:00; cjl; fourfit; make_plotdata; Fixed bug for high resolution autocorrelation xpower spectra
2001/08/14, 15:34; cjl; fourfit; precorrect; Changed message level of missing pcal warning
2001/08/16, 15:31; cjl; fourfit; fill_230; Rotate output to remove phasecal
2001/10/04, 15:11; rjc; fourfit; ap_mean; Extend data array so pcal from very start thru end usable.
2001/10/11, 15:45; rjc; fourfit; make_postplot; In case of SSB, only plot xpower spectrum of present sideband.
2001/10/25, 14:12; rjc; fourfit; compute_qf; Enable H code for pcal amplitude below 0.5% in voltage.
2001/11/06, 14:36; cjl; fourfit; make_postplot calc_rms clear_plotdata organize_data stcount_interp plot_struct.h pass_struct.h; Implemented state count data interpolation and plotting
2001/11/06, 14:38; cjl; fourfit; make_postplot; Fix various bugs, rearrange some items
2001/11/13, 14:33; rjc; fourfit; make_postplot; Re-arranged labels for state count and parity plots.
2001/11/13, 14:33; rjc; fourfit; calc_normalization(new), norm, et al.; Use sampler statistics if available and requested in control file.
2001/11/21, 17:05; rjc; fourfit; stcount_interp; Fix bug involving discard of multiple wrong-duration points.
2001/12/04, 10:40; rjc; fourfit; fill_param; Use Nyquist sample rate (as does correlator)
2001/12/11, 15:19; rjc; fourfit; stcount_interp; Trap out 0 valid counts in t306 sampler statistics.
2002/01/02, 09:20; cjl; fourfit; compute_qf; Altered H-code assignment to work when only one station is in normal pcal mode
2002/01/04, 11:31; rjc; fourfit; make_flist; Allow polarization sidebands to differ in freq by 10%.
2002/01/15, 13:39; rjc; fourfit; make_passes; Fix start and stop times to be relative to current hour.
2002/01/22, 16:10; rjc; fourfit; interp; Remove pc_offset along with average phase in pc rate calculation.
2002/02/15, 09:12; cjl; fourfit; fill_208; Fixed long-standing error which underestimated singleband delay and associated errors
2002/04/19, 13:52; rjc; fourfit; norm, apply_passband, cf_file package; Allow discard of all data not within a specified passband
2002/06/12, 13:03; rjc; fourfit; mk4_data; Increased MAX_APXLAG to be consistent with MAXAP and MAXLAG (2^21)
2002/07/19, 16:37; rjc; fourfit; stcount_interp; byte slip detector for 2 bit data based on state counts. msglev=1 warning only
2002/11/14, 11:15; rjc; fourfit; ap_mean; Handle properly the case of only a single pcal point in a scan.
2003/03/21, 11:07; rjc; fourfit; calc_normalization; Correctly handle norm. for 1 bit autocorrelations
2003/04/04, 16:51; rjc; fourfit; calc_normalization; Fix default scaling for 2 bit autocorrelations
2003/04/14, 10:45; rjc; fourfit; apply_passband; Fixed indexing for passband selection for LSB case.
2003/05/01, 11:13; rjc; fourfit; make_postplot; Change tracks printout so overstriking doesn't occur.
2003/06/12, 16:20; rjc; fourfit; norm,make_plotdata,fft1,make_postplot,mk4_data.h; Change temporary storage defs. so that large lags or aps works.
2003/06/13, 09:46; rjc; fourfit; compute_qf; include mk4_data.h so that dependencies work out correctly
2004/02/17, 15:11; rjc; fourfit; norm; Print message when sliver and zero records are discarded.
2004/05/21, 11:23; rjc; fourfit; stcount_interp; Extended state counts by 30s at each end of scan.
2004/05/21, 11:43; rjc; fourfit; make_postplot; Added field stating whether or not sample count normalization used.
2004/10/05, 09:41; rjc; fourfit; make_flist; Change freq offset to allow as much as 50% difference in channels.
2005/01/14, 14:52; rjc; fourfit; control.h; Changed the NULLINT token value to be -12345 (since start -1 wouldn't work)
2005/02/09, 10:23; rjc; fourfit; tpstat_interp,pcal_interp,compute_model,make_passes,time_range; Fixed use of scan statement by changing time base to everywhere
2005/02/14, 16:37; rjc; fourfit; fill_200; Make consistent with change of seconds origin bookkeeping).
2005/04/06, 10:00; rjc; fourfit; stcount_interp; Make same change to time base as was done on 2.14 for other routines
2005/04/12, 17:00; rjc; fourfit; calc_normalization; Don't normalize by state stats. when just a sliver in high state.
2005/04/20, 14:31; rjc; fourfit; parser; On wildcarded baseline, apply pc_freqs to both stations.
2005/05/27, 09:34; rjc; fourfit; summarize_result; Deleted this now obsolete subroutine
2005/06/06, 09:18; rjc; fourfit; stcount_interp; Fixed stc extrapolation bug affecting scans without channel 0
2005/07/07, 10:44; rjc; fourfit; make_postplot; changed level limits from 60..76 to 54..72% to properly center them
2005/08/10, 11:09; rjc; fourfit; make_flist; Change allowable freq offset back to 25%
2005/09/15, 14:12; rjc; fourfit; fill_param; Use actual sample rate (rather than Nyquist rate from BW)
2005/10/27, 13:34; rjc; fourfit; compute_qf; Also mark high pcal amps (> 200) with an H error code
2005/11/09, 14:33; rjc; fourfit; fourfit.doc; Added brief description of passband filtering option.
2006/02/06, 10:32; rjc; fourfit; compute_qf; Changed H code error upper threshold from 200 to 500
2006/04/27, 10:55; rjc; fourfit; parse_cmdline; Added + argument to getopt call to enforce linux/hppa compatibility
2007/01/19, 11:49; rjc; fourfit; interp; fixed bug wherein ap_by_ap pcal was ignored for remote station
2007/01/26, 10:22; rjc; fourfit; rotate_pcal; added prototype for c_mag() to stop segmentation fault
2007/03/12, 09:21; rjc; fourfit; pcal_interp; allow lsb pcal, but give precedence to usb pcal
2007/05/24, 10:19; rjc; fourfit; fft1; increase Data dimension to to stop seg faults
2008/04/18, 13:05; rjc; fourfit; pcal_interp; Fixed bug affecting pcal for LSB in dual polarization case
2008/10/31, 10:36; rjc; fourfit; rotate_pcal; fixed sign of remote station pcal phase in ap_by_ap mode
2009/02/24, 13:29; rjc; fourfit; search; trap for too many mbd points, caused by freq seq. redimensiont to 1024
2009/03/10, 09:39; rjc; fourfit; pcal_interp; fixed bug for mk5b pcal normalization - was too big by about 1.7x
2009/03/10, 09:39; rjc; fourfit; make_postplot; cosmetic change to printout to color code pcal amplitudes
