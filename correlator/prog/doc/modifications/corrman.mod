2000/04/07, 15:48; jball; corrman; maktabls(), slicem(), and corrman.h ; Added check that all active streams have same effective sample rate, else Cu_Resource_Error.
2000/04/26, 08:41; jball; corrman; main; "Baseline not found" error recovery fixed
2000/05/15, 13:46; jball; corrman; readsoc(); Fixed error on run > 10 minutes starting on even 10-minute boundary
2000/05/17, 17:38; jball; corrman; readsoc(); cFNum 0xbadd is now rejected for use in timing (badd is worse than bad)
2000/05/25, 16:29; jball; corrman; maktabls(); Subtract 1 from day-of-the-year to calculate startAPTime, etc. (systics)
2000/05/31, 17:54; jball; corrman; maktabls(); Added extra info to error message "... channel ... not being correlated"
2000/06/02, 13:56; jball; corrman; mktabls(), etc.; Added error checks on nlags, nblocks, etc., and addidional info in error print.
2000/06/03, 08:38; jball; corrman; maktabls(),main; Added debug prints for end of task, etc.
2000/06/05, 08:15; jball; corrman; main; Added error check on CORRELATOR_ABORT message
2000/06/08, 17:17; jball; corrman; maktabls(); Fixed error:  Incorrect xbar array for all boards except 0.  Added debug print for xbars.  Cleared xbars inside board loop.  Added check for correct segment for xbars.
2000/06/14, 08:45; jball; corrman; main; Now waits until TASK_STATUS message (from bosses in crates) before sending CU_CONFIGURATION message to conductor.
2000/06/15, 14:06; jball; corrman; readsoc(), etc.; startAPCFNum is now calculated as the equivalent CFNum for the first AP even if that AP is partial. Also, the ctime correction was removed (cbos does it correctly).
2000/06/19, 14:44; jball; corrman; various; Corrman is now receiving type-130 records from boss. Several related errors were fixed, and the type-130 summary is now being sent to opera for the double-click Details box.
2000/06/20, 08:53; jball; corrman; main, etc; Add w130 or w150 to the command line to write type-130 or type-150 records to the file.
2000/06/20, 17:56; jball; corrman; various; Fixed error in start and/or stop times when one station starts and/or ends at different times than other stations.
2000/06/21, 17:25; jball; corrman; readsoc(), etc.; Corrman now receives type-131 records (link status) from bosses and periodically sends the info to opera.
2000/06/22, 14:50; jball; corrman; maktabls(); Added error checks to insure that ap_length is an integer factor of 60 seconds and bocf_period is an integer factor of ap_length. Generate error messages if not.
2000/06/23, 09:19; jball; corrman; mktabls(); The start time in the type-100 records is now set to the start of AP number 0. (Formerly it was sometimes inside this AP.)
2000/06/27, 08:07; jball; corrman; reads(); Added debuggery to show socket-buffer usage above 32k (the default size). And buffer is now 256k. But this does not seem to help the "6-station" problem.
2000/07/03, 12:38; jball; corrman; maktabls; Major rewrite of snakes to read + and - symbolic cells from cvex to allow inter-chip (long) snakes.
2000/07/03, 13:44; jball; corrman; maktabls; Fixed incorrect type-101 records for auto and real cases: was counting snake cells instead of snake blocks.
2000/07/06, 12:49; jball; corrman; maktabls(); Fixed several errors in doing inter-chip snakes.
2000/07/06, 12:49; jball; corrman; chnlm(); Added more checks to insure that we're not configuring SUs and SU channels that are not actually in use.
2000/07/07, 14:58; jball; corrman; maktabls(); Number of blocks in type-100 and type-101 records is now rounded up to an even number. Thus it will be incorrect for some autocorrelations. To be fixed when CJL returns.
2000/07/18, 14:39; jball; corrman; mktabls(); Number of blocks is now the correct value (no more round up to even).
2000/07/24, 13:20; jball; corrman; sopera(); Timing and logic changes to give type-130 info to opera (for double-click Details) approximately every 3 seconds or every AP, whichever is longer.
2000/07/26, 16:18; jball; corrman; main; sopera cycle changed from 3 seconds to 6 seconds--opera was overloaded.
2000/07/26, 16:18; jball; corrman; readsoc(); Subtract cFOffset from type-141 cFNum to get to start of AP for timing checks (ctime and dt).
2000/07/27, 11:57; jball; corrman; readsoc(); Check 141->flags, if non-0, don't use for ctime checks.
2000/08/09, 09:06; jball; corrman; mofile; Added baseline information on error messages.
2000/08/21, 11:58; jball; corrman; main; Bug fix: Call sopera() only about every 6 seconds.
2000/08/24, 09:25; jball; corrman; mktabls(); Bug fix: Now does cross polarizations.
2000/09/26, 15:00; jball; corrman; many; Cleaned up some DEBUG messages.
2000/09/30, 21:36; jball; corrman; sopera; Now sends gray command to opera for the SUUCNKLS frame if no data from bos for (about) 6 seconds.
2000/10/20, 09:32; jball; corrman; maktabls, mofile; Lots of cleanup changes, but the only effect should be start and end times from SUT (rather than from ovex only).
2000/10/27, 16:04; jball; corrman; sopera, etask; Send -99 (blue) to opera if no data from boses after 2 ap-lengths.
2000/11/09, 14:21; jball; corrman; main; Should now support two crates (with cr2 on the command line).
2000/11/21, 08:27; jball; corrman; maktabls; Added error checks to detect incorrect cross-polarization setup.
2000/11/22, 10:49; jball; corrman; maktabls; Error messages on removed tape reduced to warnings.
2000/12/05, 09:19; jball; corrman; maktabls; "Channel not being correlated" message upgraded to msglev 1.
2000/12/13, 14:53; jball; corrman; main; Bug fix: Free slice(s) on CORRELATOR_ABORT even if no baselines assigned.
2000/12/26, 12:14; jball; corrman; readsoc; Bug fix: WARNING message incorrectly when baseline ends early.
2001/02/13, 16:37; jball; corrman; readsoc; Type-131 (link status) data are now or'd for 20 records, then sent to opera.
2001/02/21, 10:56; jball; corrman; main; Bug fix: CRATE_STATUS to link_status is now correct for two-crate mode.
2001/03/30, 12:21; jball; corrman; mofile; startAPTime (to bosses) is NOT backed up to the start of an AP.
2001/03/30, 15:37; jball; corrman; mofile; endAPTime (to bosses) is NOT backed up to start of an AP, and stop date (in type-100 records) and endAPCFNum (to bosses) is pushed forward to the end of the last (partial) AP.
2001/04/04, 12:33; jball; corrman; sopera; Bug fix: Channel numbers sent to opera for UCNKLS display are now correct.
2001/04/10, 16:44; jball; corrman; maktabls, chnlm; Bad bug fix: Now sets channels correctly even if FREQ section differs from one SU to another.
2001/04/20, 13:05; jball; corrman; maktabls; Major rewrite: Now handles lots of special cases correctly.
2001/04/23, 14:35; jball; corrman; sopera; Bug fix: Another error in channel numbers as sent to opera.
2001/04/30, 08:53; jball; corrman; readsoc; Tries to detect forks from type-141 flags, WARNING message if so.
2001/05/09, 17:10; jball; corrman; maktabls, mofile, etc.; Bug fixes: Confusion among 3 different baseline-numbering schemes.
2001/05/15, 10:27; jball; corrman; main; Bug fix: Abort (break) on one stream no longer affects other streams.
2001/05/18, 13:41; jball; corrman; sopera, etc.; Bug fix: s130 sent to opera for display is now correct for Xpol.
2001/06/07, 16:18; jball; corrman; chnlm; Check segment to allow duplicate \~chn numbers in different segments.
2001/06/12, 08:15; jball; corrman; sopera; Bug fix: Correct type-130 info now sent to opera on startup.
2001/07/02, 09:30; jball; corrman; maktabls; Now checks that all stations on task line are in at least one cvex baseline; warning message if not.
2001/10/04, 15:54; jball; corrman; chnlm; Assign all channels with the same chan_name to the same ~chan (presumably in other segments).
2001/10/09, 17:01; jball; corrman; maktabls; Now verifies all bandwidths the same and calculates oversample to send to bosses in crates.
2001/12/07, 09:13; jball; corrman; main; Command-line parameters w140...w147 now sent to boses in sendRecs.
2002/12/23, 17:10; jball; corrman; main; Bug fix: Single-station crossed polarization correlation was broken. (Still no mix of cross and autos in one pass.)
