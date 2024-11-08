/************************************************************************/
/*									*/
/* This routine takes a filled in array of codata structures, and plots	*/
/* amplitude and snr data, with fits, as a function of segmentation	*/
/* time for each baseline.  It uses the PGPLOT subroutine library, and	*/
/* sends the output to the device specified in the original cpgbeg call	*/
/* The plot device is assumed to be open already.			*/
/*									*/
/*	Inputs:		codata		struct array filled with data	*/
/*									*/
/*	Output:		graphics					*/
/*									*/
/* Created October 5 1995 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include <math.h>
#include "mk4_util.h"
#include "cpgplot.h"
#include "cofit.h"

void plot_codata (struct cosummary codata[])
    {
    int i, base, nbase, npt, year, day, hour, min, sec, wid;
    int nxsub, nysub;
    float ch, amplmax, snrmax, amperr;
    float snrhigh[MAX_NSEGLEN], snrlow[MAX_NSEGLEN];
    float amphigh[MAX_NSEGLEN], amplow[MAX_NSEGLEN];
    float log_seglen[MAX_NSEGLEN], log_segmin, log_segmax;
    float log_bpoint, log_ncotime, log_scotime;
    float fit_seglen[3], fitampl[3];
    char label[50], bad_fit[50];
    struct cosummary *codatum;
					/* How many baselines are there? */
    nbase = 0;
    while (codata[nbase].datum != NULL) nbase++;
					/* Set character height */
    cpgsch (2.0);
					/* Loop over all baselines in codata */
    for (base=0; base<nbase; base++)
	{
					/* How many points are we plotting? */
	codatum = codata + base;
	for (npt=0; npt<MAX_NSEGLEN; npt++)
	    if (codatum->ampl[npt] == 0) break;
					/* Establish error bars and extrema */
	amplmax = snrmax = 0.0;
	for (i=0; i<npt; i++)
	    {
	    snrhigh[i] = codatum->snr[i] + 1.0;
	    snrlow[i] = codatum->snr[i] - 1.0;
	    if (snrlow[i] < 0.0) snrlow[i] = 0.0;
	    if (codatum->snr[i] > 0.0) 
		amperr = codatum->ampl[i] / codatum->snr[i];
	    else amperr = 0;
	    amphigh[i] = codatum->ampl[i] + amperr;
	    amplow[i] = codatum->ampl[i] - amperr;
	    if (amplow[i] < 0.0) amplow[i] = 0.0;
	    if (snrhigh[i] > snrmax) snrmax = snrhigh[i];
	    if (codatum->fitsnr[i] > snrmax) snrmax = codatum->fitsnr[i];
	    if (snrmax <= 0.0) snrmax = 3.0;
	    if (amphigh[i] > amplmax) amplmax = amphigh[i];
	    }
	if (codatum->plateau > amplmax) amplmax = codatum->plateau;

	for (i=0; i<npt; i++)
	    log_seglen[i] = log10 ((double)codatum->seglen[i]);
	log_segmin = log_seglen[0];
	log_segmax = log_seglen[npt-1];
					/* Move to next page/panel */
	cpgpage();
					/* Display snr data/fit in yellow/red */
	cpgsci (1);
	cpgsvp (0.1, 0.95, 0.1, 0.45);
	cpgswin (log_segmin-0.3, log_segmax+0.3, 0.0, snrmax*1.1);
	cpgbox ("BCLNST", 0.0, 0.0, "BCNST", 0.0, 0.0);
	cpgmtxt ("B", 2, 0.5, 0.5, "Segmentation time (sec)");
	cpgmtxt ("L", 1.7, 0.5, 0.5, "snr");
					/* Plot the data */
	cpgsci (11);
	cpgpt (npt, log_seglen, codatum->snr, 3);
	cpgerry (npt, log_seglen, snrlow, snrhigh, 0.5);

        cpgsci (2);
        cpgline (3, &log_seglen[codatum->snr_peak[0]],
            &codatum->snr[codatum->snr_peak[0]]);

	if (codatum->datum->srch_cotime > 0)
            {
	    cpgsci (2);
            log_scotime = log10 ((double)(codatum->orig_srch));
            if (log_scotime > (log_segmax + 0.3)) log_scotime = log_segmax+0.3;
                                        /* Orig srch_cotime */
            cpgsci (8); cpgqlw (&wid); cpgslw (3 * wid); cpgsls(2);
            cpgmove (log_scotime, 0.0);
            cpgdraw (log_scotime, snrmax*1.1);
            cpgslw (wid); cpgsls(1);
            cpgmtxt("LV", -3.0, 0.15, 0.0, "(orig)");
            /* cpgline (npt, log_seglen, codatum->fitsnr); */
                                        /* Cotime is gold color, bold */
            log_scotime = log10 ((double)(codatum->datum->srch_cotime));
            if (log_scotime > (log_segmax + 0.3)) log_scotime = log_segmax+0.3;
            cpgsci (8); cpgqlw (&wid); cpgslw (5 * wid);
            cpgmove (log_scotime, 0.0);
            cpgdraw (log_scotime, snrmax*1.1);
            cpgslw (wid);
            cpgmtxt("RV", -3.0, 0.15, 1.0, "srch");
            }
	else
	    {
	    cpgsci (1);
	    cpgqlw(&wid);cpgslw(2*wid);
            snprintf(bad_fit, sizeof(bad_fit)-2,
                "Bad Fit (%d time)", codatum->datum->srch_cotime);
	    cpgmtxt ("B", -1.5, 0.15, 0.0, bad_fit);
	    cpgslw (wid);
	    }
					/* Same thing for amplitudes */
	cpgsci (1);
	cpgsvp (0.1, 0.95, 0.45, 0.8);
	cpgswin (log_segmin-0.3, log_segmax+0.3, 0.0, amplmax*1.1);
	cpgbox ("BCLST", 0.0, 0.0, "BCMST", 0.0, 0.0);
	int_to_time (codatum->datum->time_tag, &year, &day, &hour, &min, &sec);
	sprintf (label, "File: %03d-%02d%02d%02d/%s.%c.%d.%s", day, hour, min, sec,
		codatum->datum->baseline, codatum->datum->freq_code,
		codatum->datum->extent_no, codatum->datum->root_id);
	cpgmtxt ("T", 0.5, 0.5, 0.5, label);
	cpgmtxt ("R", 2.2, 0.5, 0.5, "amp");
					/* Plot the fit */
	cpgsci (2);
	log_bpoint = log10 ((double)(codatum->breakpoint));
	fitampl[0] = fitampl[1] = codatum->plateau;
	fitampl[2] = codatum->plateau + codatum->slope*(log_segmax - log_bpoint);
	fit_seglen[0] = log_segmin;
	fit_seglen[1] = log_bpoint;
	fit_seglen[2] = log_segmax;
	/* cpgline (3, fit_seglen, fitampl); */
					/* <0 means fit failed */
	if (codatum->datum->noloss_cotime > 0)
	    {
	    cpgline (3, fit_seglen, fitampl);
	    cpgsci (3); cpgqlw (&wid); cpgslw (3 * wid); cpgsls(4);
	    cpgmove (log_bpoint, 0.0);
	    cpgdraw (log_bpoint, amplmax*1.1);
            cpgslw (wid); cpgsls(1);
            cpgmtxt("LV", -3.0, 0.15, 0.0, "(bp)");

	    log_ncotime = log10 ((double)(codatum->datum->noloss_cotime));
	    if (log_ncotime > (log_segmax + 0.3)) log_ncotime = log_segmax+0.3;
					/* Cotime is green, bold */
	    cpgsci (3); cpgqlw (&wid); cpgslw (5 * wid);
	    cpgmove (log_ncotime, 0.0);
	    cpgdraw (log_ncotime, amplmax*1.1);
	    cpgslw (wid); 
            cpgmtxt("RV", -3.0, 0.15, 1.0, "loss");

                                        /* and be helpful on content */
            cpgsci (4);
            snprintf(bad_fit, sizeof(bad_fit)-2, "%2.2s %2.2s",
                codatum->datum->baseline, codatum->datum->polarization);
            cpgmtxt("LV", -1.0, 0.75, 0.0, bad_fit);
	    }
	else
            {
            cpgsci (1);
            cpgqlw(&wid);cpgslw(2*wid);
            snprintf(bad_fit, sizeof(bad_fit)-2,
                "Bad Fit (%d loss)", codatum->datum->noloss_cotime);
            cpgmtxt ("B", -1.5, 0.1, 0.0, bad_fit);
            cpgslw (wid);
            }
					/* Amplitude data */
	cpgsci (11);
	cpgpt (npt, log_seglen, codatum->ampl, 3);
	cpgerry (npt, log_seglen, amplow, amphigh, 0.5);
	}

    return;
    }
