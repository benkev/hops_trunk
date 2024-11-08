/************************************************************************/
/*									*/
/* Writes out A-file lines for all baselines of a scan, as contained	*/
/* in the codata structure.  These A-file lines have the coherence	*/
/* time fields filled in, suitable for input to the fringe search	*/
/* package.								*/
/*									*/
/*	Inputs:		codata		representing 1 scan		*/
/*			fpout		open output file		*/
/*									*/
/*	Output:		A-file lines	written to fpout		*/
/*			return value	Number of lines written		*/
/*									*/
/* Created October 6 1995 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include "mk4_afio.h"
#include "mk4_util.h"
#include "cofit.h"

int write_codata (struct cosummary *codata, FILE *fpout)
    {
    int i,j;

    afile_header(codata[0].datum->version, 2, fpout);

    i = 0;j=0;
    while (codata[i].datum != NULL)
	{
					/* This tags data as having bogus duration */
	codata[i].datum->datatype[0] = 'K';
					/* Did fit_codata find good nl_cotime and 	*/
					/* srch_cotime?  If not then do not write. 	*/

					/* SSD 9mar08 took this out since want all scans */
/*	if ((codata[i].datum->noloss_cotime == -1)||(codata[i].datum->srch_cotime ==-1))
	     {
	     j++;i++;
	     continue;
	     }
*/
	write_fsumm (codata[i].datum, fpout);
	i++;
	}

    return (i-j);
    }
