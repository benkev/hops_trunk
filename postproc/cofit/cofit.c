/************************************************************************/
/*									*/
/* This program reads in data which has been segmented by fringex, then	*/
/* averaged incoherently by the average program.  It looks for data	*/
/* on any given baseline which was segmented using a variety of segment	*/
/* lengths, and fits the resulting amplitudes and SNRs as a function of	*/
/* segment length.  These fits constitute coherence analysis, the	*/
/* results of which are plotted and written out in the form of 1 A-file	*/
/* line per baseline/scan, with coherence time fields filled in.	*/
/*									*/
/*	Inputs:		filename		Input A-file data	*/
/*									*/
/*	Output:		filename		Output A-file data	*/
/*									*/
/* Created October 5 1995 by CJL					*/
/*									*/
/************************************************************************/
#include <math.h>
#include <stdio.h>  
#include <stdlib.h>
#include <string.h>  
#include <errno.h>
#include "mk4_util.h"
#include "cpgplot.h"
#include "cofit.h"
#include "msg.h"

// char progname[8] = "cofit";
// int msglev = 1;
int datatype = 0;
int space = 500;

int main (int argc, char *argv[])
    {
    int i, navg, nout, scan_boundary, order, oldtime, bno, nsegtime, plot;
    avg_data *data;
    char oldbl[3], oldpl[3];
    struct cosummary codata[MAX_BNO];
    fringesum *datum;
    FILE *fpout;
    extern int optind;
    int ignore_csq = 0;
    set_progname("cofit");
    set_msglev(1);
    environment();
					/* Allocate some space to start */
    data = (avg_data *) calloc (space, sizeof (avg_data));
    if (data == NULL)
	{
	perror ("calloc");
	msg ("Could not allocate memory for main data array.", 3);
	exit (1);
	}
					/* Interpret command line */
    if (parse_cmdline (argc, argv, &fpout, &plot, &ignore_csq) != 0) 
	exit (1);
					/* Read in the data */
					/* After parse_cmdline, optind points */
					/* at the arguments following command */
					/* line flags.  Read all files in. */
    navg = 0;
    if (optind < argc)
	{
	for ( ; optind<argc; optind++) 
	    if (read_data (&data, argv[optind], &navg) != 0) exit (1);
	}
    else if (read_data (&data, "stdin", &navg) != 0) exit (1);
					/* Sort data into proper order */
    if (sort_data (data, navg) != 0)
	exit (1);
					/* Loop over all data */
    nout = 0;
    scan_boundary = TRUE;
    for (i=0; i<navg; i++)
	{
	order = data[i].order;
	datum = &(data[order].fdata);
					/* Initialize each scan */
	if (scan_boundary)
	    {
	    oldtime = datum->time_tag;
	    strcpy (oldbl, "  ");
	    strcpy (oldpl, "  ");
	    bno = -1;
	    scan_boundary = FALSE;
	    }
					/* New baseline */
					/* (always true for new scan) */
                                        /* new pol is effectively a new bl */
	if (strcmp (oldbl, datum->baseline) != 0 ||
            strcmp (oldpl, datum->polarization) != 0) 
	    {
	    bno++;
	    if (bno > MAX_BNO)
		{
		msg ("Too many baselines, abort!", 3);
		exit (1);
		}
	    nsegtime = 0;
	    codata[bno].datum = datum;
	    strcpy (oldbl, datum->baseline);
	    strcpy (oldpl, datum->polarization);
					/* Remember to force timetag to be */
					/* actual scan time for subsequent */
					/* processing */
	    codata[bno].datum->time_tag -= codata[bno].datum->scan_offset;
	    codata[bno].datum->offset += codata[bno].datum->scan_offset;
	    codata[bno].datum->scan_offset = 0;
            codata[bno].ign_chisq = ignore_csq;
	    }
					/* Record amplitude, SNR and */
					/* segment time for fitting/plotting */
	if (nsegtime > MAX_NSEGLEN)
	    msg ("Too many seg. time values, ignoring some data", 3);
	else
	    {
	    codata[bno].ampl[nsegtime] = datum->amp;
	    codata[bno].snr[nsegtime] = datum->snr;
	    codata[bno].seglen[nsegtime] = datum->duration;
	    codata[bno].navg[nsegtime] = datum->srch_cotime;
	    nsegtime++;
	    }
					/* Is this a scan boundary (or EOF)? */
	if ((i+1) == navg) scan_boundary = TRUE;
	else
	    {
	    order = data[i+1].order;
	    if (data[order].fdata.time_tag != oldtime) scan_boundary = TRUE;
	    }
					/* If yes, process previous scan */
					/* and clear for next scan */
	if (scan_boundary)
	    {
	    fit_codata (codata);
	    if (plot) plot_codata (codata);
	    nout += write_codata (codata, fpout);
	    clear_codata (codata);
	    }
	}
    msg ("Wrote %d coherence-analyzed output records", 1, nout);

    if (plot) cpgend();
    exit (0);
    }
