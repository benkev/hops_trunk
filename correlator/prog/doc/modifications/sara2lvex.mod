2001/02/07, 08:44; jball; sara2lvex; all; New program reads sara output (VLBA) and ovex, writes lvex.
2001/02/21, 10:54; jball; sara2lvex; main; Added header info and full comments (i.e., with time and station).
2001/03/06, 20:58; jball; sara2lvex; main; Now puts one-char station code (from ovex) on def line.
2001/03/19, 10:15; jball; sara2lvex; main; Bug fix: Now rejects calibration and other garbage scans.
2001/03/27, 10:39; jball; sara2lvex; main; Add pass identifier calculated from head offset and track numbers.
2001/04/18, 14:53; jball; sara2lvex; main; Bug fix: Now works even if sara file contains stations not found in ovex.
2001/04/24, 12:36; jball; sara2lvex; main; Now bridges over brief off-source and LO-unlock events in a scan. (Previously it broke the scan at such an event to make two scans with the same name.)
2001/06/07, 10:42; jball; sara2lvex; main; Added special-case fix for KP C993x.
2001/12/07, 09:11; jball; sara2lvex; main; Now deals with subnetted (time jumps backwards) in Craig Walker's ovex.
