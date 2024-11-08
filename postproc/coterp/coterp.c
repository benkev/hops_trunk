/************************************************************************/
/*									*/
/* This program transfers coherence time information from a coherence	*/
/* fitted list of baseline scans (fitlist) to a global list of all	*/
/* baseline scans (masterlist).  The fitlist must be a subset of the	*/
/* masterlist.  The coterp program uses three main algorithms.  First	*/
/* it converts the fitlist coherence information into a station-based	*/
/* form using a least-squares approach.  It then applies a theoretical	*/
/* elevation dependence for station-based coherence times in order to	*/
/* solve for the time signature of the coherence times.  Finally, it	*/
/* interpolates these times to the unfitted scans in the masterlist, 	*/
/* correcting for station elevations in the process.  The end result	*/
/* is the original masterlist with optimal coherence times in the 	*/
/* relevant A-file fields.  This masterlist can then be used as input	*/
/* to the fringe search module of the mm-VLBI analysis package.		*/
/*									*/
/*	Inputs:		fitlist			Fitted "cal" list	*/
/*			masterlist		All scans represented	*/
/*									*/
/*	Output:		masterlist.cot		coherence times entered	*/
/*									*/
/* Created November 29 1995 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>  
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include "coterp.h"
#include "msg.h"

// char progname[8] = "coterp";
// int msglev = 1;
int mspace = 500;
int fspace = 200;

main (argc,argv)
int argc;
char *argv[];
    {
    int nf, nm, err, ret, nscan, plot;
    char outfile[256];
    fringedata *masterlist, *fitlist;
    fringesum *datum;
    scansum *scsum;
    FILE *fpout;
    extern int optind;
    set_progname("coterp");
    set_msglev(1);
					/* Init */
    err = FALSE;
					/* Allocate some space to start */
    fitlist = (fringedata *) calloc (fspace, sizeof (fringedata));
    masterlist = (fringedata *) calloc (mspace, sizeof (fringedata));
    if ((fitlist == NULL) || (masterlist == NULL))
	{
	perror ("calloc");
	msg ("Could not allocate memory for main data arrays.", 3);
	exit (1);
	}
					/* Interpret command line */
    if (parse_cmdline (argc, argv, &fpout, &plot) != 0) 
	{
	syntax("$HeadURL: svn+ssh://vault.haystack.mit.edu/svn/hops/trunk/postproc/coterp/coterp.c $");
	exit (1);
	}
					/* Read in the data */
					/* After parse_cmdline, optind points */
					/* at the arguments following command */
					/* line flags.  There must be two files */
    nf = nm = 0;
    ret = read_data (&fitlist, argv[optind], &nf, &fspace);
    ret += read_data (&masterlist, argv[optind+1], &nm, &mspace);
    if (ret != 0)
	{
	msg ("Failed to read data from %s, %s", 2, argv[optind], argv[optind+1]);
	exit (1);
	}
					/* Sort data into proper order */
    ret = sort_data (fitlist, nf);
    ret += sort_data (masterlist, nm);
    if (ret != 0)
	{
	msg ("Failed to sort data", 2);
	exit (1);
	}
					/* Check validity and count scans */
    if ((nscan = check_lists (fitlist, nf, masterlist, nm)) < 0) 
	{
	msg ("Error, fitted list must be subset of masterlist", 2);
	exit (1);
	}
					/* Allocate scsum array */
    scsum = (scansum *)calloc (nscan+1, sizeof (scansum));
    if (scsum == NULL)
	{
	msg ("Could not allocate memory for scan array", 2);
	exit (1);
	}
					/* Create station-based coherence times */
    if (stationize (fitlist, nf, scsum) != 0) 
	{
	msg ("Error figuring out station-based coherence times", 2);
/*	exit (1); */
	}
					/* Plot the station-based times */
    if (plot) plot_sttimes (scsum);
					/* Interpolate station-based coherence */
					/* times to master list, and convert */
					/* back to baseline-based times, with */
					/* elevation corrections */
    if (interp (masterlist, nm, scsum) != 0) 
	{
	msg ("Error interpolating coherence times to master list", 2);
	exit (1);
	}
					/* Write the data out with coherence time */
					/* fields filled in */
    if (write_data (masterlist, nm, fpout) != 0) 
	{
	msg ("Error writing out interpolated data", 2);
	exit (1);
	}
					/* Success */
    exit (0);
    }
