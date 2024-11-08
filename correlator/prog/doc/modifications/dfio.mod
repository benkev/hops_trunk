1999/12/20, 13:43; cjl; dfio; display_221; Added q option to quit a set of displays
2000/02/14, 09:29; cjl; dfio; clear_208,addr_208,copy_208; Added support for version 1
2000/03/08, 12:56; cjl; dfio; many; Added support for type 210 record
2000/03/13, 10:41; cjl; dfio; addr_208 copy_208; Fixed bug for version 1 records - prior to this fix v1 208 records were bad
2000/04/05, 12:40; rjc; dfio; read_mk4sdata; Fixed bug that wouldn't allow 16 channels.
2000/04/19, 10:59; cjl; dfio; addr_221 display_221; Fixed a null-termination problem in postscript fringe plot
2000/04/19, 11:31; cjl; dfio; many; Added messages at level -1 detailing allocated and freed memory blocks
2000/04/19, 15:25; cjl; dfio; display_221; Added immediate return mode for use in aedit popups
2000/04/20, 09:27; cjl; dfio; display_221; Added a mode to close gs window
2000/05/05, 09:08; cjl; dfio; open_datafile; Remove need for absolute pathname argument
2001/04/26, 13:39; cjl; dfio; copy_212 copy_230 addr_212 clear_212 write_mk4fringe read_mk4fringe write_record; Added support for 212 and 230 records in type 2 files
2001/05/25, 10:50; cjl; dfio; read_mk4fringe addr_212 copy_221; Fix data alignment problems, provide compatibility for misaligned files
2001/07/06, 13:42; cjl; dfio; copy_206 addr_206 clear_206 type_206.h; Added support for version 1 with chan and sband weights
2001/07/17, 13:56; cjl; dfio; clear_230 addr_230 copy_230; Support modifications in type_230.h
2001/09/18, 14:50; cjl; dfio; addr_221; Fix bug in testing for padded flag variable
2001/09/18, 15:18; cjl; dfio; addr_308 copy_308; Loop over all 32 elements of phasecal data array instead of just 16
2001/11/14, 10:58; cjl; dfio; read_mk4fringe; Added diagnostic messages level -2
2004/11/23, 11:40; rjc; dfio; display_221.c; Moved printing of eps trailer so that linux screen doesn't blank.
2005/05/27, 13:21; rjc; dfio; copy_200; fixed byte flip bug in linux version that affected expt. number
2009/03/11, 14:35; rjc; dfio; display_221; added redundant lines to get page buffer to clear for fourfit display
2009/03/11, 14:35; rjc; dfio; compress; fixed align32 macro to be no-op so 64 bitersion wouldn't abort
2009/03/11, 14:35; rjc; dfio; addr_221,copy_221; increased buffer workspace for compress
2009/05/13, 13:17; rjc; dfio; display_221; fixed bug that appeared doing fplot's in aedit, causing segmentation faults
2009/06/22, 16:22; rjc; dfio; copy_100, copy_101, copy_110, copy_120; Bug fix: dereference ptr in assignment
2009/06/22, 16:22; rjc; dfio; write_mk4corel; Bug fix: include fp as 2nd argument in 2 write_record calls
