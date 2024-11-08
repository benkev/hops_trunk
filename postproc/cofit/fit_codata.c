/************************************************************************/
/*									*/
/* This routine takes a filled in array of codata structures, and fits	*/
/* theoretical curves to each baseline.  It uses these fits to either	*/
/* declare the coherence times undetermined, or to fill in the 		*/
/* coherence time elements of the data record.  It also fills in the	*/
/* fitted amplitude and SNR values for plotting purposes.		*/
/*									*/
/*	Inputs:		codata		struct array filled with data	*/
/*									*/
/*	Output:		codata		Fitting information added	*/
/*									*/
/* Created October 5 1995 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include <math.h>
#include "cofit.h"
#include "mk4_afio.h"
#include "mk4_util.h"

void fit_codata (struct cosummary codata[])
    {
    int base, nbase, npt, i, rv;
    double exponent, noloss_cotime;
    struct cosummary *codatum;
    extern double max_cotime;
					/* How many baselines are there? */
    nbase = 0;
    while (codata[nbase].datum != NULL) nbase++;
					/* Loop over all baselines in codata */
    for (base=0; base<nbase; base++)
	{
					/* How many points are we fitting? */
	codatum = codata + base;
	for (npt=0; npt<MAX_NSEGLEN; npt++)
	    if (codatum->ampl[npt] == 0) break;
					/* Delegate fitting to subroutines */
	msg ("Attempting fit for '%s'", -2, 
					fringename (codatum->datum));
	if ((rv = fit_ampl (codatum, npt)) != 0)
	    {
	    codatum->datum->noloss_cotime = -1;
	    msg ("Error (%d %d %g) fitting noloss cotime for '%s'", 2,
                rv, codatum->iterations, codatum->chisq,
                fringename (codatum->datum));
	    }

	else 
	    {
	    exponent = -0.05 * codatum->plateau / codatum->slope;
	    noloss_cotime = codatum->breakpoint * pow (10.0, exponent);
					/* Avoid short overflow in 3rd line below */
	    if (noloss_cotime < 1.0) noloss_cotime = 1.0;
	    codatum->datum->noloss_cotime = floor (noloss_cotime);
	    printf("for snr: exponent=%f   noloss_cotime = %f\n",
                exponent, noloss_cotime);
	    }

	if (normalize_snr (codatum, npt) != 0)
	    {
	    codatum->datum->srch_cotime = -1;
	    printf("problem in normalize_snr\n");
	    msg ("Error fitting srch_cotime for '%s'", 2,
					fringename (codatum->datum));
	    }
	if ((codatum->datum->srch_cotime = fit_snr (codatum, npt)) <=0) 
	    {
	    printf("problem in fit_snr (%d)\n", codatum->datum->srch_cotime);
	    codatum->datum->srch_cotime = -1;
	    msg ("Error fitting srch_cotime for '%s'", 2,
					fringename (codatum->datum));
	    }
	else
	    {
	    printf("srch_cotime = %d\n",codatum->datum->srch_cotime);
	    printf("noloss_cotime = %d\n",codatum->datum->noloss_cotime);
            codatum->orig_srch = codatum->datum->srch_cotime;
	    if (codatum->datum->srch_cotime < codatum->datum->noloss_cotime)
		codatum->datum->srch_cotime = codatum->datum->noloss_cotime;
	    }
					/* Impose upper limit */
	if (codatum->datum->noloss_cotime > max_cotime)
	    codatum->datum->noloss_cotime = max_cotime;
	if (codatum->datum->srch_cotime > max_cotime)
	    codatum->datum->srch_cotime = max_cotime;
					/* Flagging message */	
	if ((codatum->datum->srch_cotime == -1)||(codatum->datum->noloss_cotime == -1))
		msg("Cofit removing this scan from output ...",2);
	}
    return;
    }
