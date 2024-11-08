/************************************************************************/
/*									*/
/* This program reads a list of incoherent detections, and a 		*/
/* corresponding list of triangle records (formed in a dumb fashion in	*/
/* aedit), and creates a shell script designed to appropriately segment */
/* the three baselines contributing to each triangle record.  The	*/
/* script also forms the segmented bispectra via aedit, and averages	*/
/* them, placing the output in a final file containing optimum 		*/
/* estimates of closure phase (ready for the fold program).		*/
/*									*/
/*	Inputs:		baselist		Input A-file data	*/
/*			trilist			All triangles based on	*/
/*						input data file		*/
/*									*/
/*	Output:		filename		Output A-file data	*/
/*						containing closures	*/
/*									*/
/* Created August 22 1996 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>  
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include "adata.h"
#include "mk4_util.h"
#include "mk4_afio.h"
#include "bispec.h"
#include "msg.h"

// char progname[8] = "bispec";
// int msglev = 1;
int datatype = 0;
int bspace = 500;
int tspace = 500;

int
main (int argc, char* argv[])
    {
    int i, nf, nt, ret;
    fringesum *bdata;
    trianglesum *tdata;
    char outfile[256], line[512];
    FILE *fpbase, *fptri;
    set_progname("bispec");
    set_msglev(1);
					/* Interpret command line */
    if (parse_cmdline (argc, argv, &fpbase, &fptri, outfile) != 0) 
	exit (1);
					/* Allocate some space to start */
    bdata = (fringesum *) calloc (bspace, sizeof (fringesum));
    if (bdata == NULL)
	{
	perror ("calloc");
	msg ("Could not allocate memory for baseline data array.", 3);
	exit (1);
	}
    tdata = (trianglesum *) calloc (tspace, sizeof (trianglesum));
    if (tdata == NULL)
	{
	perror ("calloc");
	msg ("Could not allocate memory for triangle records.", 3);
	exit (1);
	}
					/* Read in the baseline data */
    nf = 0;
    while (fgets (line, 511, fpbase) != NULL)
	{
        if (afile_comment(line)) continue;
	if ((ret = parse_fsumm (line, bdata+nf)) != 0)
	    { msg ("Error return %d from parse_fsumm()", 2, ret); exit (1); }
	nf++;
	if (nf >= bspace)
	    {
	    bspace += 100;
	    bdata = (fringesum *)realloc (bdata, bspace*sizeof(fringesum));
	    if (bdata == NULL)
		{ msg("Fatal error allocating memory for data - Abort!", 3); exit(1); }
	    }
	}
					/* Read in the triangle data */
    nt = 0;
    while (fgets (line, 511, fptri) != NULL)
	{
        if (afile_comment(line)) continue;
	if ((ret = parse_tsumm (line, tdata+nt)) != 0)
	    { msg ("Error return %d from parse_tsumm()", 2, ret); exit (1); }
	nt++;
	if (nt >= tspace)
	    {
	    tspace += 100;
	    tdata = (trianglesum *)realloc (tdata, tspace*sizeof(trianglesum));
	    if (bdata == NULL)
		{ msg("Fatal error allocating memory for data - Abort!", 3); exit(1); }
	    }
	}
					/* Loop over triangle records */
    for (i=0; i<nt; i++)
	if (make_bispec (tdata+i, bdata, nf, outfile) != 0)
	    {
	    msg ("Error in make_bispec", 2);
	    exit (1);
	    }
					/* That's it */
    exit (0);
    }
