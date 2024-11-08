/************************************************************************/
/*									*/
/* This program is responsible for forming optimally averaged ratios of	*/
/* the various cross-correlations performed for polarization-sensitive	*/
/* CMVA work.  It relies heavily on existing programs like aedit, and	*/
/* calls them when needed.  Much of the functionality centers around	*/
/* taking care of tedious bookkeeping.					*/
/*									*/
/*	Inputs:		infile  		Input file name		*/
/*									*/
/*	Output:		outfile 		Output file name	*/
/*									*/
/* Created October 14 1997 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>  
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include "adata.h"
#include "msg.h"

#define TRUE 1
#define FALSE 0

// char progname[8] = "pratio";
// int msglev = 1;
int default_cotime = 10;
int clean = TRUE;

main (int argc, char* argv[])
    {
    int i, nf, nbs, ret, scantime;
    fringesum data[10], outdata[10];
    char infile[256], filename[256], line[512], aedit_command[256];
    char baseline[3];
    FILE *fp;

    set_progname("pratio");
    set_msglev(1);
					/* Interpret command line */
    if (parse_cmdline (argc, argv, infile) != 0) 
	exit (1);
					/* Put tagged data into temp file */
    if (set_pol_field (infile) != 0) exit (1);
					/* Sort it using aedit */
    sprintf (filename, "%s.LLRRRLLR", infile);
    sprintf (aedit_command, "aedit -m 3 -b \"batch;read %s;\
                                sort base;sort time;write %s.sort1\"",
							filename, infile);
    if (system (aedit_command) != 0)  
	{ 
	msg ("aedit_command error", 2); 
	return (1); 
	}
    if (clean) unlink (filename);
					/* Open input and output files */
    sprintf (filename, "%s.sort1", infile);
    if ((fp = fopen (filename, "r")) == NULL)
	{
	msg ("Error opening %s", 3, filename);
	exit (1);
	}
    open_outfiles (infile);
					/* Loop through sorted data, */
					/* splitting into baseline/scans */
    nf = 0;
    scantime = 0;
    nbs = 0;
    while (fgets (line, 511, fp) != NULL)
        {
        if (afile_comment(line)) continue;
        if ((ret = parse_fsumm (line, data+nf)) != 0)
	    { 
	    msg ("Error return %d from parse_fsumm()", 2, ret); 
	    exit (1); 
	    }
					/* New baseline/scan, not first? */
	if ((nf > 0) && ((data[nf].time_tag > scantime)
			|| (strncmp (data[nf].baseline, baseline, 2) != 0)))
	    {
					/* Do the work for this baseline/scan */
	    if (transfer_cotimes (data, nf) != 0)
		msg ("Error in transfer_cotimes", 1);
	    else if (compute_ratios (infile, data, nf, outdata) != 0)
		msg ("Error in compute_ratios", 1);
	    else if (write_ratios (outdata) != 0)
		msg ("Error in write_ratios", 1);
	    else
		{
		nbs++;
		msg ("Processed baseline/scan %d", 2, nbs);
		}
					/* Reset for the next baseline/scan */
	    memcpy (data, data+nf, sizeof (fringesum));
	    nf = 0;
	    }
	scantime = data[nf].time_tag;
	strncpy (baseline, data[nf].baseline, 2);
        nf++;
	if (nf >= 10)
	    {
	    msg ("Overflow, duplicated data?", 3);
	    exit (1);
	    }
	}
					/* Clean up last baseline/scan */
    if (nf > 0)
	{
	if (transfer_cotimes (data, nf) != 0)
	    msg ("Error in transfer_cotimes", 1);
	else if (compute_ratios (infile, data, nf, outdata) != 0)
	    msg ("Error in compute_ratios", 1);
	else if (write_ratios (outdata) != 0)
	    msg ("Error in write_ratios", 1);
	else
	    {
	    nbs++;
	    msg ("Processed baseline/scan %d", 2, nbs);
	    }
	}
    fclose (fp);
    if (clean) unlink (filename);

    close_outfiles ();
					/* That's it */
    exit (0);
    }
