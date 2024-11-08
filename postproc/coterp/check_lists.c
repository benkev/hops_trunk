/************************************************************************/
/*									*/
/* Examines the coherence fitted list and the masterlist that have	*/
/* just been read in and sorted.  The former must be a strict subset	*/
/* of the latter.  The number of scans in fitlist is returned		*/
/*									*/
/*	Inputs:		fitlist		data array of "calibrator" data	*/
/*			nf		size of fitlist			*/
/*			masterlist	main data array			*/
/*			nm		size of masterlist		*/
/*									*/
/*	Output:		return value	number of scans in fitlist	*/
/*					-1 means error			*/
/*									*/
/* Created November 30 1995 by CJL					*/
/*									*/
/************************************************************************/
#include "coterp.h"

int
check_lists (fitlist, nf, masterlist, nm)
fringedata *masterlist, *fitlist;
int nf, nm;
    {
    int findex, mindex, nscan, f, m, match, oldtimetag;
    char fname[256];
    fringesum *fdatum, *mdatum;
					/* Loop through both sorted */
					/* lists, checking for subset */
					/* and counting scans */
    nscan = m = oldtimetag = 0;
    for (f=0; f<nf; f++)
	{
	findex = fitlist[f].order;
	fdatum = &(fitlist[findex].fdata);
					/* Count scans */
	if (fdatum->time_tag != oldtimetag) 
	    {
	    oldtimetag = fdatum->time_tag;
	    nscan++;
	    }
					/* Look for scan match */
	strcpy (fname, fringename (fdatum));
	match = FALSE;
	for ( ; m<nm; m++)
	    {
	    mindex = masterlist[m].order;
	    mdatum = &(masterlist[mindex].fdata);
	    if (strcmp (fname, fringename (mdatum)) == 0)
		{
		match = TRUE;
		break;
		}
	    }
					/* Abort if not found */
	if (! match)
	    {
	    msg ("Error, fitted list must be subset of masterlist", 2);
	    return (-1);
	    }
	}

    return (nscan);
    }
