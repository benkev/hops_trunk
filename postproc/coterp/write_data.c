/************************************************************************/
/*									*/
/* Writes the output file, putting in a time stamp, and the standard	*/
/* A-file header line							*/
/*									*/
/*	Inputs:		masterlist	main data array with co. times	*/
/*					filled in			*/
/*			nm		size of masterlist		*/
/*			fpout		Output data stream		*/
/*									*/
/*	Output:		return value	0 for success 1 for failure	*/
/*									*/
/* Created 1 December 1995 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include "coterp.h"

int
write_data (masterlist, nm, fpout)
fringedata *masterlist;
int nm;
FILE *fpout;
    {
    int i;
                                        /* Time stamp the file and put in */
                                        /* the header */
    if (afile_header (masterlist[0].fdata.version, 2, fpout) != 0)
	msg ("Error writing header", 2);
					/* Write out in original sort order */
    for (i=0; i<nm; i++)
	{
	if (write_fsumm (&(masterlist[i].fdata), fpout) != 0)
	    {
	    msg ("Error writing out data", 2);
	    return (1);
	    }
	}
    fclose (fpout);
	
    return (0);
    }

