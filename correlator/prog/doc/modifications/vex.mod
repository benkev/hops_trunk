1999/12/22, 15:19; cjl; vex; get_pcm_config; Fixed bug in increment of ncfg
2000/02/09, 08:50; cjl; vex; cvex_info; Added auto_corr statement handling in CORR_CONFIG
2000/05/15, 14:16; cjl; vex; do_track; Changed fatal error to warning, improved message accuracy
2000/05/19, 11:17; cjl; vex; do_antenna; Improved support of axis types, and treat unrecognized type as error
2000/07/24, 11:26; cjl; vex; scan_info init_scan; Add fourfit_reftime parameter in SCHED section
2001/02/07, 13:27; cjl; vex; vex.h; Increased max number of comments from 2K to 8K
2001/02/14, 18:25; cjl; vex; lvex_info, get_logscan, pformat.txt, blk_stmt.txt; Added autopeak_interval keyword to lvex
2001/03/29, 12:28; cjl; vex; lvex_info get_logscan; Added pass keyword
2001/04/19, 13:50; cjl; vex; get_drive_init; Modified equaliztorequire three values
2001/04/24, 12:07; cjl; vex; write_vexfile; Improved editing function behaviour
2001/04/27, 14:47; cjl; vex; ivex_info ivex.h; Added header_duration in CORR_INIT section
2001/05/31, 10:47; cjl; vex; do_clock; Added trap for no valid clock_early statement
2001/10/25, 11:43; cjl; vex; rse_dec; Fix bug for -00 degress
2002/01/11, 19:13; cjl; vex; lvex_info get_logscan get_drive_init ivex.h lvex.h pformat.txt blk_stmt.txt; Implement Mk5 support in vex parsing
2002/03/25, 14:11; cjl; vex; do_das ovex.h; Added support for the Mark5A recorder type vex specification
2002/03/28, 14:21; cjl; vex; get_logscan; Changed to more portable scanf form for non-standard long long int conversion
2002/04/03, 15:22; cjl; vex; get_logscan; Added parsing of ndiscs, overlooked originally
2004/08/31, 14:21; rjc; vex; vex.h; Increased MAXDEF from 256 to 512 to allow 512 DEF's (e.g. expt's in global.evex)
2005/07/08, 11:21; rjc; vex; scan_info, sllvex_info; allow up to 200000 lines in merged vex files (tgen was exceeding)
2005/12/06, 11:32; rjc; vex; get_drive_init; Add code to recognize Mark5B drive type in global.ivex file
2008/05/27, 12:01; rjc; vex; do_phasecal_detect; Allow 16 pcal tones to be specified
2008/07/23, 09:28; rjc; vex; get_vex; Fixed bug that prevented editting of ovex while within genaroot
