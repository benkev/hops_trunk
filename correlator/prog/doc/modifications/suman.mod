2000/04/04, 14:54; rjc; suman; Config_SUs; changed configuration for DCM to use ovex values (rather than initialization values)
2000/04/05, 10:15; rjc; suman; Config_SUs; Add configuration of SUIM (so stnclk is right)
2000/04/05, 10:15; rjc; suman; Config_SUs; Change DCB configure message to code "c" (for configure)
2000/04/05, 11:14; rjc; suman; build_CFG_CRM; add extra modes to TC_and_CC_clock to support multiplexing
2000/04/05, 12:42; rjc; suman; common.h; Added state 6c.
2000/04/06, 17:12; rjc; suman; build_CFG_DIM; Give more explanatory error message for ivex eqlzr error
2000/04/27, 14:53; rjc; suman; Config_SUs; Send operator an ALL_MOUNTED message when all tapes are mounted.
2000/05/10, 13:04; rjc; suman; proc_A_S_TAPE_MOUNTED; Enable low tape whenever a new tape is mounted.
2000/05/10, 13:42; rjc; suman; sinmul Makefile; Moved sinmul into suman directory; changed debug print level.
2000/05/23, 16:00; rjc; suman; suman; Issue ABORT_SU and FLUSH_Q when breaking a scan.
2000/05/30, 16:55; rjc; suman; makefile; Change to local sinmul copy, since May 10th mod was to wrong makefile.
2000/06/07, 13:16; rjc; suman; build_SUMISC; optimize pre-correlation tape backup for speed & acceleration
2000/06/23, 14:24; jball; suman; settabls.c; Changed DIM idle code from 84 to 81 (fixed logic low).
2000/06/27, 14:14; rjc; suman; build_SUMISC; fine-tune tape backup for synchronization
2000/06/27, 14:14; rjc; suman; suman; cosmetic changes to debug printout
2000/06/29, 16:33; rjc; suman; proc_RSP_TAPE_POSITIONED_AT_TOT; don't update current position with footage from pos to TOT
2000/07/18, 15:35; rjc; suman; suman; add timeout for barcode read request to mcb
2000/07/18, 15:35; rjc; suman; cook307; Don't cook numbers for channels that are masked off
2000/07/19, 10:05; jball; suman; global.h; Added global.suinfo[] to contain more setup information for SUs and channels.
2000/07/19, 10:05; jball; suman; settabls(); Now saves TRM track information in global.suinfo[]. 
2000/07/19, 10:05; jball; suman; build_CFG_DIM; Now saves additional SU information in global.suinfo[]. 
2000/07/19, 10:05; jball; suman; proc_RSP_SU_STATUS; Fixed TRM status information for channels (previously sometimes wrong track).
2000/07/19, 13:33; jball; suman; build_CFG_TRM; Fixed which TRM tracks to configure.
2000/07/19, 14:02; rjc; suman; suman; reordered shutdown code to fix intermittent pcal bug
2000/07/21, 12:57; rjc; suman; Config_SUs; abort configure on get_vex error returns
2000/07/26, 16:21; jball; suman; settabls(),proc_RSP_SU_STATUS(); Fixed some confusion between ovex channel numbers and SU channel numbers.
2000/07/26, 16:21; jball; suman; settabls(); ChannelMask now masks SU channels instead of ovex channels.
2000/07/26, 16:21; jball; suman; SyncTapes; Deleted code that set channel status incorrectly.
2000/07/27, 14:44; jball; suman; settabls(); Now inverts the time sequence of the barrel-unroll table, which is what seems to be needed to use the standard roll tables in ovex.
2000/07/31, 17:01; jball; suman; settabls(); Reverse unroll is commented out, back to normal forward unroll.
2000/08/03, 13:41; jball; suman; Tot_to_Ascii(); Added check for time out of range, "TimeRangeError" if so.
2000/08/07, 16:33; jball; suman; build_CFG_DIM; Set tape_track to -1 (not in use) whenever CrossBar[] >= 64.
2000/08/07, 16:33; jball; suman; build_CFG_PCAL; Eliminated unneeded extra call to get_vex() and other minor cleanups.
2000/08/07, 16:33; jball; suman; proc_CON_msg() (in suman.c); Commented out the extra free()s.
2000/08/08, 09:00; jball; suman; proc_CON_msg; Set all free()d pointers to NULL.
2000/08/08, 15:34; rjc; suman; proc_RSP_AVERAGE_STATISTICS; don't create 304's unless SYNCING or SYNCED
2000/08/09, 13:07; rjc; suman; proc_ON_msg; Prevent configuration if previous task still active.
2000/08/10, 09:55; rjc; suman; suman; Don't send redundant (and duff, at that) stop tape message.
2000/08/11, 16:00; rjc; suman; proc_RSP_PHASECAL_DATA; Refuse to capture phase cal data into t307 recs unless correlating
2000/08/11, 00:00; jball; suman; proc_CON_msg; After END_TASK, set StationFileName to null (to prevent overwriting). Same after REMOVE_STATION.
2000/08/14, 16:03; rjc; suman; Tot2date; Don't round seconds to nearest second on conversion.
2000/08/15, 10:49; jball; suman; proc_CON_msg(),etc.; Made clean_up() to free malloc()ed, etc. Called clean_up from both END_TASK and REMOVE_STATION.
2000/08/16, 11:14; rjc; suman; Pos_to_TOT, proc_RSP_TAPE_POSITIONED_AT_TOT, common.h; trap 0xbadd footage as ptot timeout, solving sticky TOT bug
2000/08/16, 15:22; jball; suman; get_index() in xtras.c; Now protects against overflow in ...tape_footage[]. 
2000/08/17, 11:12; rjc; suman; proc_A_S_TAPE_DISMOUNTED; clear out mounting flag when tape dismounts
2000/08/17, 16:33; rjc; suman; proc_A_S_TAPE_MOUNTED,Init_SUs,proc_MCB_msg,proc_A_S_TAPE_DISMOUNTED,suman; fix bugs affecting manual tape mounting
2000/08/18, 10:07; rjc; suman; proc_A_S_TAPE_MOUNTED; reject tape mounted message if a tape is already mounted
2000/08/18, 10:25; rjc; suman; proc_RSP_TAPE_FRAME_HEADER_DATA; shorten printed line; filter out day 0
2000/08/21, 11:12; rjc; suman; proc_OPR_msg; cleaned up error messages
2000/08/21, 14:45; rjc; suman; Config_SUs, ClockRecovery, common.h; set tape drive's bitsync based upon actual track bitrate
2000/08/25, 12:29; jball; suman; Config_SUs; Use also lvex start and stop times.
2000/08/25, 15:30; jball; suman; Config_SUs; Take lvex start and stop times back out. Sorry.
2000/08/31, 17:31; jball; suman; build_CFG_SUIM; Set bit 10 in ST1status and ST3status to the same as bit 10 in TX0control and TX2control (0 for 1 bit/sample, 1 for 2 bits/sample).
2000/09/07, 17:04; jball; suman; settabls(); Bug fix: Wrong CRM-xbar-out tracks in case sign and mag tracks have same parity (i.e., both even or both odd).
2000/09/14, 10:45; jball; suman; tapehead(), etc.; Tapehead (new function) adds on-disk tape-head-offset database. Many mods to incorporate this into suman.
2000/09/14, 16:45; jball; suman; tapehead, proc_A_S_TAPE_DISMOUNT; Bug fix: Close tape-head-offset file when tape is removed from drive.
2000/09/15, 15:47; jball; suman; suman; Bug fix: Suman now checks automatic head tracking before writing head position to database. This is to prevent writing on fast forward or reverse.
2000/09/18, 08:56; jball; suman; tapehead, etc.; Tape-head-offset database now includes last five SU numbers used to read each tape.
2000/09/19, 13:28; jball; suman; several; Try again: Suman now pays attention to start and stop times in lvex (as well as ovex).
2000/09/20, 16:07; jball; suman; Config_SUs; At start of each scan, check fill_tape_hd_db, if NO, then peak regardless.
2000/09/22, 08:22; jball; suman; settabls; Bug fix: When roll is ON, configure all tracks even if assigned as home tracks to an inactive channel.
2000/09/25, 11:47; jball; suman; Config_SUs; Start and stop times are now forced to integer seconds.
2000/09/25, 11:55; jball; suman; Config_SUs; Added calculated acceleration time to lvex start time, and round up.
2000/10/02, 12:36; rjc; suman; proc_RSP_SU_STATUS,build_SUMISC; Use task_id for SU array code
2000/10/10, 13:38; jball; suman; suman; Write tape database at END_TASK moved to before STOP_TAPE and verify tape moving and auto-track enabled.
2000/10/11, 12:56; jball; suman; Config_SUs; Check for negative durations, write error message if so.
2000/10/11, 14:24; jball; suman; OK_to_configure, Config_SUs, su_db.h; Added OK_to_config() to config PCAL to avoid unnecessary PCAL messages.
2000/10/13, 15:40; jball; suman; Sync_Tapes, proc_ROT_msg, su_db.h; Bug fix: Send correct polynomials to station even if late start.
2000/10/16, 15:20; jball; suman; suman, Config_SUs, tapehead, System_Startup, su_db.h; Changed tape-head database system. Latest values are now averaged in memory.
2000/10/17, 10:08; jball; suman; Sync_Tapes, proc_ROT_msg; Clean up, and fix error message if run out of polynomials.
2000/10/18, 13:11; jball; suman; proc_ROT_msg; Bug fix: Out-of-polynomials message is now correct.
2000/10/20, 14:35; jball; suman; suman; FLUSHQI on END_TASK is now ROT scheduled 5 seconds after the most-recent ROT broadcast.
2000/10/23, 10:32; jball; suman; suman, proc_ROT_msg, common.h, global.h, prototypes.h; Add ROT_time_tag offset to now to scheduled time of FLUSHQI, plus cleanup.
2000/10/24, 09:56; jball; suman; Config_SUs, build_SCAN; Test: Back out of configure phase cal only when needed, AND advance startROT to odd second if even.
2000/10/25, 16:38; jball; suman; Config_SUs, proc_RSP_PHASECAL_DATA, proc_RSP_msg; Added several array-index-out-of-bounds checks and error messages.
2000/10/26, 10:00; jball; suman; proc_CON_msg, build_SCAN; Tests: Add back even to odd second, delete extra FLUSHQI on END_TASK.
2000/10/26, 13:38; jball; suman; proc_CON_msg; Deleted extra abort command on END_TASK.
2000/10/26, 17:15; jball; suman; proc_CON_msg; Test: On END_TASK, first FLUSHQI, then ABORT delayed 3 seconds.
2000/10/26, 17:15; jball; suman; Config_SUs; Test: Put back in CONFIG_PCAL only when needed (OK_to_Configure()).
2000/10/27, 17:30; jball; suman; Sync_Tapes; Comment out SU_FINISH command at end of scan.
2000/10/27, 17:31; jball; suman; proc_CON_msg; On END_TASK, comment out FLUSHQI IMMEDIATE and ABORT after delay, replace with SU_FINISH IMMEDIATE and FLUSHQI after delay.
2000/10/30, 14:28; jball; suman; build_DPU_COMMAND; Bug fix: Offset2Next set to correct value.
2000/10/30, 14:28; jball; suman; Sync_Tapes; Stop-tape command ROT queued for stop_rot back in.
2000/10/31, 09:10; jball; suman; Sync_Tapes; Added back in: SU_FINISH at stop_rot plus 2 seconds.
2000/10/31, 14:10; jball; suman; proc_CON_msg; On END_TASK, if not within 10 seconds of stop_rot, then FLUSHQI immediately and ABORT after 5 seconds.
2000/11/02, 16:45; jball; suman; proc_CON_msg; On END_TASK more than 10 seconds from normal end, add stop phase cal ROT scheduled 3 seconds later.
2000/11/06, 15:06; jball; suman; Config_SUs; Check for unreasonable numbers for tape-head offsets, write warning message if so.
2000/11/06, 16:53; jball; suman; Pos_to_TOT, proc_CON_msg; Bug fix: head_position in the sut was sometimes being treated as unsigned. It can be negative.
2000/11/16, 12:04; rjc; suman; Config_SUs; Fixed units bug in tape drive bit sync calculation.
2000/11/16, 12:34; jball; suman; proc_CON_msg; Bug fix: REMOVE_STATION case, deleted check on suman_state.
2000/11/16, 14:48; jball; suman; remove_station_from_list; Bug fix: Remove station also from "link" list.
2000/11/17, 13:16; jball; suman; proc_CON_msg; Send CANT_MOUNT_TAPE to conductor as response to REMOVE_STATION.
2000/12/06, 08:18; jball; suman; proc_RSP_AVERAGE_STATISTICS; Bug fix: Track numbers in type-304 records are now correct even for tracks through DIM1.
2000/12/08, 09:33; rjc; suman; build_CFG_SUIM; change SUIM serial link setup time for 32 Ms/s operation
2000/12/08, 16:08; jball; suman; proc_CON_msg; Bug fix: REMOVE_STATION now clears also Needs entry.
2000/12/12, 14:34; rjc; suman; Config_SUs; Fix units bug affecting choice of DPU bitsync octave.
2001/01/08, 10:39; jball; suman; proc_CON_msg; If su_list[] is 2, the corresponding SU is UNinitialized.
2001/01/10, 17:19; jball; suman; proc_RSP_PHASECAL_DATA; For each type 307, calculate (approx) phase-cal amplitude (1 per channel) and write to SUT.
2001/01/10, 17:19; jball; suman; various; Initialize new phase-cal info in SUT on startup and end task.
2001/01/11, 14:19; jball; suman; su_db.h; Added pctime = time when last phase-cal data were received.
2001/01/11, 14:19; jball; suman; System_Startup, proc_A_S_TAPE_DISMOUNTED; Set pctime to 0 (not in use).
2001/01/11, 14:19; jball; suman; proc_CON_msg; On END_TASK, set pctime to 0 (not in use).
2001/01/11, 14:19; jball; suman; Sync_Tapes; Set pctime to now (on tape startup).
2001/01/11, 14:19; jball; suman; proc_RSP_PHASECAL_DATA; On receiving phase-cal data, set pctime to now.
2001/01/11, 14:19; jball; suman; periodic_services; If pctime is more than 2*PCAL_ACC_PER/FramesPerSOT old, then write blue to pcal (for opera's Details).
2001/01/11, 14:34; jball; suman; proc_RSP_PHASECAL_DATA; Bug fix: If channel is inactive, then don't write phase-cal data; set val_count 0.
2001/01/26, 12:24; jball; suman; proc_RSP_SAMPLER_STATS; New function to receive sampler-statistics messages from SU.
2001/01/26, 12:24; jball; suman; build_message, Sync_Tapes, proc_RSP_msg, proc_CON_msg, prototypes.h, makefile ; Ask for and receive sampler statistics from SU. Doesn't work yet.
2001/01/31, 17:32; rjc; suman; build_CFG_SUIM; Set bit 2 of state cntr regs, to enable state statistics.
2001/02/05, 11:29; jball; suman; Config_SUs; If VLBA and valid lvex, then tape direction from sign of start_speed.
2001/02/09, 08:39; jball; suman; proc_MCB_msg, Sync_Tapes, Read_DPU_State, build_SUMISC, su_db.h, common.h, Tunables.h; Offset tape-start ROT by desired footage minus present footage (converted to SysClocks).
2001/02/13, 15:44; jball; suman; Sync_Tapes; Offset start_rot by an integer-second version of the same offset applied to tape_start_rot.
2001/02/13, 15:44; jball; suman; Sync_Tapes, build_SCAN; Replace even-to-odd-second offset with one conditioned on 16-step roll and sample_rate < 8 Ms/s.
2001/02/15, 15:26; jball; suman; Sync_Tapes; If autopeak_interval from lvex, use it instead of fixed AUTO_TRACK_INTERVAL.
2001/02/21, 10:48; jball; suman; Sync_Tapes; Added write to tape database (at start of each scan).
2001/02/22, 12:43; jball; suman; Sync_Tapes, Confit_SUs, Pos_to_TOT, suman, xtras, su_db.h; Bug fix: All tape footages and head positions are signed numbers.
2001/02/23, 16:29; rjc; suman; Sync_Tapes; Force 16 phase barrel roll to odd second for any sample rate.
2001/02/26, 10:33; jball; suman; Config_SUs; Bug fix: Initial head offset from database can be negative.
2001/02/27, 16:54; jball; suman; Config_SUs, suman; Bug fix: Reads correct numbers from tape-head database on startup.
2001/03/02, 13:34; jball; suman; Config_SUs; If lvex start time is significantly earlier than ovex start time, add to tape footage (from lvex) to advance to ovex time.
2001/03/07, 13:00; jball; suman; Config_SUs, Pos_to_TOT, proc_A_S_TAPE_DISMOUNTED, su_db.h; Record and use tape_offset between lvex or ovex and as-read footage for a specific TOT.
2001/03/12, 16:22; jball; suman; proc_RSP_SU_STATUS; Better check on DMM, CRM, DCM, and TRM health.
2001/03/14, 16:48; jball; suman; Xmit_FFs, suman, build_CFG_SUIM, prototypes.h, common.h ; New Xmit_FFs() processes TRANSMIT_FFS message from bosses to briefly send 0xff.
2001/03/27, 15:07; jball; suman; Sync_Tapes; Bug fix: Download correct polynomials even if tape-start time is within 5 bocfs before an integer minute.
2001/03/28, 13:52; jball; suman; build_CFG_DIM; Bug fix: Report all 32 tracks in type-304 records through dim1.
2001/03/29, 11:06; jball; suman; Init_SUs; Delete ENAB_DCM_DMM_VALIDITY_CONTROL bit from FeatureBitMask.
2001/03/30, 15:42; jball; suman; Sync_Tapes; In CMD_SET_TX_START_ROT message (c.f., build_SCAN()), back up startROT by 2 seconds, advance finishROT and TapeServoMaxTOT by 1 second.
2001/04/04, 17:02; rjc; suman; build_poly; subtract out integer second offsets from SU's delay polynomial
2001/04/06, 17:02; rjc; suman; build_SUMISC; fixed units for NominalROTTOT offset of multiple seconds
2001/04/10, 16:45; jball; suman; proc_RSP_PHASECAL_DATA; Now catches 0-count case sets SUT for opera to display.
2001/04/25, 14:08; jball; suman; Init_SUs, Config_SUs; Added 3-choice
2001/04/27, 11:21; jball; suman; proc_RSP_TAPE_FRAME_HEADER_DATA; Bug fix: VLBA negative day number.
2001/05/03, 12:25; jball; suman; build_CFG_DCM, build_CFG_SUIM; On initialization, calculate station-clock frequency using ivex header_duration and calculate CFlength using HeaderMultiplier.
2001/05/11, 10:12; jball; suman; proc_RSP_TAPE_FRAME_HEADER_DATA, Config_SUs, su_db.h, task_stream.h; Bug fix: Now reads info from correct root file when multiple streams.
2001/05/14, 12:26; jball; suman; build_POLY, su_db.h; Bug fix: Now no reference to (incorrect) ovex after the first interval.
2001/06/20, 14:25; jball; suman; main, xtras; Added state to ResourceList[].status to wait for another stream to finish with tape.
2001/06/21, 14:32; jball; suman; main, proc_A_S_TAPE_DISMOUNT, System_Startup, proc_RSP_msg, Config_SUs; Rework tape-mounting procedure; eliminate ResourceList.
2001/06/22, 13:07; jball; suman; proc_OPR_msg; Rewrote section that processes message from typing into opera's banana box.
2001/07/02, 16:47; jball; suman; prototypes.h, sinmul, OK_to_config, su_db.h, Config_SUs, build_CFG_PCAL; Phasecal configuration now on a per channel basis (from svex).
2001/07/09, 15:43; jball; suman; proc_RSP_TAPE_FRAME_HEADER_DATA; Bug fix: Wrong TOT to SUT from VLBA-format tapes.
2001/07/16, 09:54; jball; suman; proc_CON_msg; Bug fix: On UNinitialize, reset also arrays used by OK_to_configure.
2001/07/20, 14:46; rjc; suman; build_SUMISC,build_CFG_DCM; Patchup time-related parameters for time-related parameters for speedup.
2001/07/23, 09:02; jball; suman; Config_SUs; Error message if ovex drive number is 0 (calibration) or negative.
2001/07/30, 15:20; rjc; suman; build_SUMISC; Include speedup factor in calculation of ScanData2ScanInterval
2001/08/08, 14:06; jball; suman; Config_SUs, proc_RSP_PHASECAL_DATA, su_db.h; Phase-cal configuration now uses ovex channel_out_assignment and phase_cal_detect (in part)
2001/08/10, 09:52; jball; suman; sinmul, cook307, su_db.h; Phase-cal frequencies can now be specified in ovex (when omitted from svex)
2001/08/10, 16:48; jball; suman; proc_RSP_PHASECAL_DATA; Bug fix: Type-308 duration is now correct (formerly 5 seconds fixed)
2001/08/13, 11:04; jball; suman; suman, proc_RSP_PHASECAL_DATA, Sync_Tapes; Bug fix: Omitted ovex if_def phase-cal frequency interval now correctly shuts off phase cal.
2001/08/14, 14:05; jball; suman; sinmul; Error message if phase-cal frequency is outside bandwidth.
2001/08/20, 15:33; jball; suman; main, Sync_Tapes, su_db.h, Tunables.h; Phase-cal now configured to give 10-second read cycle and slightly less than 9-second "duration" (integration period)
2001/08/24, 16:16; rjc; suman; Sync_Tapes; Take into account speedup when calculating start_rot.
2001/08/29, 17:03; jball; suman; build_POSnPEAKHEADS, Config_SUs, build_message, c3msgs.h; Part of implementation of "peak to track" (find track) in SU.
2001/08/31, 13:29; rjc; suman; Config_SUs,Tunables.h; Changed headpeak timeout to 90s.
2001/09/12, 10:54; rjc; suman; Init_SUs, c3msgs; Make default filtering of automatic head
2001/09/18, 09:17; jball; suman; Sync_Tapes; Reorganized and added speed_up more correctly.
2001/09/26, 15:37; jball; suman; Sync_Tapes, Tunables.h; Readjusted startup timing. TAPE_HEADSTART now 10 sec.
2001/10/01, 16:23; rjc; suman; Sync_Tapes; Apply speedup to obey ROT backup for polynomials.
2001/10/04, 15:38; jball; suman; build_POLY; Set ModelParams to oversample.
2001/10/04, 15:38; jball; suman; settabls; Configure all SU output channels that have the same input channel.
2001/10/04, 15:38; jball; suman; proc_RSP_SAMPLER_STATS, cook307, proc_RSP_AVERAGE_STATISTICS; Adjust times in file types 304, 306, and 308 to be start times (i.e., subtract duration from end time).
2001/10/04, 15:50; jball; suman; Sync_Tapes; Adjust PCAL period using speed_up.
2001/10/10, 13:39; jball; suman; settabls; If ivex...eqlzr is 0, then use this hsc regardless of calculated frequency.
2001/10/14, 11:08; jball; suman; Config_SUs, build_POLY, cook307, sinmul; Bug fix: Confusion between channels and ovex channel index.
2001/10/25, 15:15; jball; suman; Config_SUs; Bug fix: Channel count with channel mask now correct for monitor track.
2001/11/01, 10:54; jball; suman; Config_SUs, Sync_Tapes, suman, tapehead, proc_A_S_TAPE_DISMOUNTED, prototypes.h, tapeDB.h; Tapehead no longer records copies and extrapolations in tape database.
2001/11/02, 09:05; jball; suman; Config_SUs, tapehead, prototypes.h, tapeDB.h; Improvements in "hints" for tape-head peaking.
2001/12/12, 13:24; jball; suman; build_POLY, Pos_to_TOT, build_SUMISC, suman, Config_SUs, task_stream.h; Calculate k_div in Pos_to_TOT based on all stations in a scan, use it in build_POLY.
2002/03/05, 10:54; jball; suman; Config_SUs, Sync_Tapes; Moved shut-down-pcal sequence from Config_SUs to Sync_tapes.
2002/03/21, 15:28; jball; suman; proc_CON_msg() in suman.c; Added checks in case END_TASK to avoid crashes on abort.
2002/06/12, 09:48; jball; suman; Config_SUs; Mark5 start byte must be non-negative
2002/06/12, 13:15; jball; suman; proc_RSP_AVERAGE_STATISTICS, su_db.h; Multiple Mark5s with skipo in SU_db[] 
2002/06/12, 13:15; jball; suman; proc_RSP_SAMPLER_STATS, su_db.h; Check for byte slip, warning message if so
2002/06/25, 11:46; jball; suman; Sync_tapes, Config_SUs; Mark5 disks now start after delay to ROT specs
2002/07/30, 13:05; jball; suman; Config_SUs; Swap order of 6a and 6b to configure DCM before phase cal. Also delay a few cycles before starting phase cal.
2002/08/16, 11:45; jball; suman; Pos_to_TOT; Offset disk_start_tot by NomROTTOToff, so it is actually a start ROT.
2002/08/26, 13:54; jball; suman; Config_SUs, Mark5Com; Improve the default case if Mark 5 can't decode start of scan.
2002/08/29, 14:30; jball; suman; Config_SUs; More careful check for correct disk mounting Mark 5.
2002/10/04, 14:21; jball; suman; Pos_to_TOT; Bug fix: Mark-5 early start should now be OK.
2002/10/07, 17:08; jball; suman; proc_RSP_AVERAGE_STATISTICS; Increased the size of the skip error box (to 1.2 Mbytes, SS) to reduce the frequency of small skips.
2002/11/06, 09:21; jball; suman; Config_SUs; Added abort su operations to STATE_1.
2002/12/27, 17:01; jball; suman; Pos_to_TOT, Mark5Com ; Mark 5: Use data_check to get mode, num tracks, play rate, to set new I/O board
2003/01/16, 10:37; jball; suman; Pos_to_TOT, Mark5Com; Adapt to new version-2.2 of the Mark-5 writeup.
2003/03/10, 12:38; jball; suman; Pos_to_TOT; Set play_rate from ovex, not from tfperiod from data_check.
2003/03/10, 12:38; jball; suman; Mark5Com; Allow incorrect byte period to process flawed Westford disks.
2003/03/13, 17:04; jball; suman; Pos_to_TOT; In case TOT5Com() failure, bump pointer by 65536 bytes, try again, up to 10 times.
2003/03/24, 14:59; rjc; suman; Pos_to_TOT; fix bug in k_div calculation (0..4) instead of (1,2,4,8,16).
2003/03/27, 16:55; jball; suman; Pos_to_TOT, Mark5Com; Various patch ups to work with mark-3-mode-C format data. Now do a track_check on the monitor_track instead of a data_check.
2003/05/17, 12:56; jball; suman; Config_SUs; Added Mark-5 'status?' query.
2003/05/22, 12:55; jball; suman; Mark5Com, Pos_to_TOT; Use revised track_check in case of flawed Mark-5 disks.
2003/05/23, 11:33; jball; suman; Pos_to_TOT; Small fix to Mark-5 bytes/sec used in skip.
2003/08/12, 11:05; jball; suman; Pos_to_TOT; Case Mark 5, allow up to 9-second early start (formerly 3 seconds)
2003/08/29, 17:07; rjc; suman; build_SUMISC; set drive type to Mk4(within the SU) when MK5A is being used
2004/01/30, 14:51; jball; suman; Pos_to_TOT; Mark-5 case, now allow srart_rot to be no more than 3 or 4 seconds early.
2004/02/16, 09:21; jball; suman; suman.c; In case Mark 5, skip calls to tapehead() and don't use results.
2004/02/28, 09:18; jball; suman; Sync_Tapes, suman.c; Implementing realtime_mode (remembered in task_stream[]) 
2004/02/29, 11:20; jball; suman; Pos_to_TOT; In case track_check fails (eVLBI case), bytepsec (used in skip) defaults to mark4:32 mode
2004/03/02, 12:55; jball; suman; Mark5Com, Pos_to_TOT; In case 'track_check?' fails (net2out case), do 'mode?' query, use answers to calculate bytepsec (SS bytes per second) for skip 
2004/03/18, 09:10; jball; suman; Mark5Com, Config_SUs; Check both Mark-5 banks for lvex serial numbers, and skip missing slave disk in check
2004/03/19, 10:09; jball; suman; Config_SUs; Fixed bug in serial-number checking (with bank switching)
2004/12/07, 09:26; jball; suman; Pos_to_TOT; Subtract NomROTTOToff from disk_start_tot for Mark5.
2005/02/01, 11:08; rjc; suman; suman.c; fixed vtquerying SU's that are unitialized
2005/02/02, 09:47; rjc; suman; Config_SUs; vtx bug fix was actually in Config_SUs, not suman.c
2005/03/02, 16:32; jball; suman; Pos_to_TOT; Added error-return checking on several Mark5Com() returns. Tell conductor if error.
2007/09/07, 18:28; rjc; suman; Config_SUs; fixed bug do to bedded loop on i, caused infinite mount loop
2008/03/13, 12:50; rjc; suman; config_doms; put configure_scan before request_pcals to avoid rejecting pcal
2008/05/27, 12:00; rjc; suman; send_cal_requests; Allow 16 pcal tones to be specified
2008/12/05, 14:31; rjc; suman; send_cal_requests; don't request pcal from mk5b if no tones are active
