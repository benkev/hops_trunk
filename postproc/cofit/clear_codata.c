/************************************************************************/
/*									*/
/* Trivial routine to clear cosummary structure array			*/
/*									*/
/*	Inputs:		codata		undefined state			*/
/*									*/
/*	Output:		codata		initialized			*/
/*									*/
/* Created October 5, 1995 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include "cofit.h"

void clear_codata (struct cosummary *codata)
    {
    int i, j;

    for (i=0; i<MAX_BNO; i++)
	{
	codata[i].datum = NULL;
	codata[i].plateau = 0.0;
	codata[i].breakpoint = 0.0;
	codata[i].slope = 0.0;
	for (j=0; j<MAX_NSEGLEN; j++)
	    {
	    codata[i].ampl[j] = 0.0;
	    codata[i].snr[j] = 0.0;
	    codata[i].fitsnr[j] = 0.0;
	    codata[i].seglen[j] = 0.0;
	    codata[i].navg[j] = 0;
	    }
	}
    return;
    }
