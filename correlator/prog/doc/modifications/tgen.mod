2000/04/07, 11:30; jball; tgen; readv() and tgen (main); Purged tgen of all references to ovex->exper_num; used evex instead.
2000/04/24, 10:29; jball; tgen; tgen; Speedup: Now cats evex, ovex, lvex to composite file to give to get_vex()
2000/06/05, 11:17; jball; tgen; main & readv(); Added fflush() to give error messages in proper order.
2000/06/05, 12:57; jball; tgen; readv(); On error from get_vex(), tgen now prints the scan number and name that caused the error.
2000/06/07, 12:24; jball; tgen; several; More cleanup of error and debug messages.
2000/07/10, 14:43; jball; tgen; main; Replaced ivex->system_tempo in tape-speed calculation.
2000/07/25, 12:11; jball; tgen; main; Fixed error: ivex not being read to get system_tempo.
2000/07/25, 14:54; jball; tgen; main; Fudged tape footage calculation to reduce number of warning messages. Fix later.
2000/07/26, 16:16; jball; tgen; main; Tape footage now allows 5 second under or 25 second over (because early_start is not usually in ovex), else warning message.
2001/01/23, 08:59; jball; tgen; readv; Reject station whenever ovex drive_no is negative.
2001/01/25, 14:23; jball; tgen; main, readv; Added command-line option whether to use or ignore lvex file.
2001/02/09, 08:42; jball; tgen; main; Compare ovex with lvex tape speed, WARNING if not the same.
2001/03/19, 14:01; jball; tgen; readv; Reject station if ovex drive number is 0 (calibration) or negative.
2001/07/19, 10:18; jball; tgen; main; Bug Fix: Correct station order even if multi pass.
2001/07/25, 14:45; jball; tgen; main; Bug fix: Multi-pass scans are now correct even with no station list on the command line.
2001/07/26, 13:09; jball; tgen; main; Different calculation of duration for "footage time" warning message to reduce the frequency of this message.
2001/07/31, 11:51; jball; tgen; main; Now prints "New tape station VSN slot" when appropriate.
2002/11/04, 14:00; jball; tgen; main; Update to deal with Mark-5 disks instead of tapes.
