/************************************************************************/
/*									*/
/* This routine is responsible for figuring out which correlation	*/
/* products have valid coherence times, and inserting			*/
/* appropriate coherence times into records which lack them.  It is	*/
/* vital that the coherence times for a baseline/scan are the same for	*/
/* all correlation products, because the segmenting has to match for	*/
/* forming the ratios.							*/
/*									*/
/*	Inputs:		data		Afile records of corr'n prods	*/
/*			nf		Number of entries in data array	*/
/*									*/
/*	Output:		data		Suitably modified		*/
/*			return value	0=OK, else error		*/
/*									*/
/* Created 15 October 1997 by CJL					*/
/*									*/
/************************************************************************/
#include "adata.h"

int
transfer_cotimes (fringesum* data, int nf)
    {
    int i, noloss_cotime, ntime, tot_time;
    extern int default_cotime;
					/* Only error is too many entries */
    if (nf > 4)
	{
	msg ("Error, too many correlation products", 2);
	return (1);
	}
					/* Compute average of valid */
					/* coherence times */
    tot_time = ntime = 0;
    for (i=0; i<nf; i++)
	{
	if (data[i].noloss_cotime > 0)
	    {
	    tot_time += data[i].noloss_cotime;
	    ntime++;
	    }
	}
					/* No valid times, use default */
    if (ntime == 0)
	{
printf ("nf = %d\n", nf);
	msg ("Substituting %d secs for unknown cotime", 1,
					default_cotime);
	noloss_cotime = default_cotime;
	}
    else noloss_cotime = tot_time / ntime;
					/* Copy to all records */
    for (i=0; i<nf; i++) 
	data[i].noloss_cotime = noloss_cotime;

    return (0);
    }
