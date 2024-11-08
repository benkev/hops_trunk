1994/08/02, 08:52; cjl; afio; clear_trisumm, parse_trisumm, write_trisumm, adata.h; Created routines to support closure triangle A-file format
1994/09/22, 16:50; cjl; afio; summarize_corel; Trapped null character for corel quality code in ancient data
1995/01/05, 12:55; cjl; afio; write_fsumm.c; Fixed bug in total_sbresid ... a fabs() call was removed
1995/02/17, 09:53; cjl; afio; parse_fsumm; Added a diagnostic message
1995/02/17, 10:00; cjl; afio; write_fsumm; Changed default 'parent' string to '0' instead of null
1995/02/20, 12:52; cjl; afio; clear_fsumm parse_fsumm write_fsumm; Changed phase_type into datatype, a 2-char string
1995/03/22, 16:28; cjl; afio; parse_rsumm parse_csumm write_rsumm write_csumm; Trivial mods for format version 3
1995/03/29, 12:23; cjl; afio; parse_csumm parse_fsumm parse_rsumm parse_tsumm write_csumm write_fsumm write_rsumm write_tsumm; Added support for version 3 A-file format
1995/04/04, 11:15; cjl; afio; afile_header; New routine to timestamp/label A-files
1995/04/05, 10:06; cjl; afio; parse_tsumm clear_tsumm; Fix minor version 3 bug
1995/11/08, 11:48; cjl; afio; all; Implemented version 4 of the format
1996/02/07, 16:35; cjl; afio; write_fsumm; added more precision to delay_rate
1996/03/13, 16:40; cjl; afio; parse_tsumm; Long-standing bug in reading version 4
2000/01/10, 13:31; cjl; afio; many; Fix Y2K problem due to 2-digit year in A-file format
2000/04/14, 17:02; cjl; afio; parse_fsumm write_fsumm; Fixed procdate years for version 5
2001/02/02, 10:05; cjl; afio; parse_tsumm; Fixed minor error in declaration
