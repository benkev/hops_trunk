/************************************************************************/
/*									*/
/* This routine dumps the scan-averaged polarization ratio A-file	*/
/* records to their respective output files.  The files are already 	*/
/* opened for writing by open_outfiles().				*/
/*									*/
/*	Inputs:		outdata		Records to write		*/
/*									*/
/*	Output:		return value	0=OK, else bad			*/
/*									*/
/* Created 28 October 1997 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include "adata.h"

int
write_ratios (fringesum* outdata)
    {
    int i;
    char type;
    FILE *fp;
    extern FILE *fp_rl_ll, *fp_lr_ll, *fp_rl_rr, *fp_lr_rr, *fp_rr_ll;

    for (i=0; i<5; i++)
	{
					/* No data for this ratio */
	if (outdata[i].length == 0) continue;
					/* Find type and assign to correct */
					/* file pointer */
	type = outdata[i].datatype[1];
	if (type == 'm') fp = fp_rl_ll;
	else if (type == 'n') fp = fp_lr_ll;
	else if (type == 'o') fp = fp_rl_rr;
	else if (type == 'p') fp = fp_lr_rr;
	else if (type == 'q') fp = fp_rr_ll;
					/* Do it */
	if (write_fsumm (outdata+i, fp) != 0)
	    {
	    msg ("Error writing output data records", 3);
	    return (-1);
	    }
	}

    return (0);
    }
