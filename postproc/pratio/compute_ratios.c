/************************************************************************/
/*									*/
/* This routine does all the hard work in pratio.  It takes the A-file	*/
/* lines for a single baseline/scan, sanitized with uniform coherence	*/
/* times, and writes them to a temporary file.  Fringex is then run on	*/
/* the file to produce uniformly segmented datasets for each 		*/
/* correlation product present.  These datasets are merged and sorted	*/
/* using aedit, then read in a segment at a time.  For each segment,	*/
/* all possible polarization ratios are formed, and added into vector	*/
/* sums.  The scan average of each ratio is computed, and written into	*/
/* output A-file records with encoded datatype fields.			*/
/*									*/
/*	Inputs:		infile		Root of filename for this run	*/
/*			data		Input afile records		*/
/*			nf		Number of input records		*/
/*									*/
/*	Output:		outdata		output ratio records		*/
/*			return value	0=OK, else bad			*/
/*									*/
/* Created 15 October 1997 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include "adata.h"

int
compute_ratios (char* infile, fringesum* data, int nd, fringesum* outdata)
    {
    int i;
    char filename[256], aecomfile[256], fxcommand[256], aeoutfile[256];
    char aecommand[256], inname[256], outname[256], outname2[256], str[256];
    double ratios[5][3];
    double vec_avg[5][3];
    FILE *aecom, *fp;
    extern int clean;

					/* Only one product, abandon */
    if (nd == 1)
	{
	msg ("Cannot compute ratios from one correlation product", 1);
	return (1);
	}
					/* Open the aedit runfile */
    sprintf (aecomfile, "%s.aecom", infile);
    if ((aecom = fopen (aecomfile, "w")) == NULL)
	{
	msg ("Could not open aedit command file '%s'", 2, filename);
	return (1);
	}
					/* Perform separate runs of fringex */
					/* on each correlation product */
    for (i=0; i<nd; i++)
	{
					/* Which correlation product? */
	if (data[i].datatype[1] == 'w') sprintf (str, "ll");
	else if (data[i].datatype[1] == 'x') sprintf (str, "rr");
	else if (data[i].datatype[1] == 'y') sprintf (str, "rl");
	else if (data[i].datatype[1] == 'z') sprintf (str, "lr");
					/* Define temp filenames */
	sprintf (inname, "%s.fxin.%s", infile, str);
	sprintf (outname, "%s.fxout.%s", infile, str);
	sprintf (outname2, "%s.fxfix.%s", infile, str);
					/* Open and write fringex input file */
	if ((fp = fopen (inname, "w")) == NULL) 
	    {
	    msg ("Could not open file '%s'", 2, inname);
	    return (1);
	    }
	if (write_fsumm (data+i, fp) != 0) 
	    {
	    msg ("Error writing file '%s'", 2, inname);
	    return (1);
	    }
	fclose (fp);
					/* Execute fringex */
	sprintf (fxcommand, "fringex -i noloss -r %s > %s", inname, outname);
	if (system (fxcommand) != 0) 
	    {
	    msg ("Error executing fringex on file '%s'", 2, inname);
	    return (1);
	    }
					/* Tag the fringex output by datatype */
	if (reset_pol_field (outname, outname2, data[i].datatype[1]) != 0)
	    {
	    msg ("Error resetting polarization type field in '%s'", 2, outname2);
	    return (1);
	    }
					/* Clean up and arrange for aedit */
					/* to read the results */
	if (clean) unlink (inname);
	if (clean) unlink (outname);
	fprintf (aecom, "read %s\n", outname2);
	}
					/* Complete aedit command file */
    sprintf (aeoutfile, "%s.aeout", infile);
    fprintf (aecom, "sort timetag; write %s; exit", aeoutfile);
    fclose (aecom);
					/* Execute aedit merge and sort */
    sprintf (aecommand, "aedit -m 3 -r %s", aecomfile);
    if (system (aecommand) != 0)
	{
	msg ("Error executing aedit on file '%s'", 2, aeoutfile);
	return (1);
	}
    if (clean) 
	{
	unlink (aecomfile);
	sprintf (filename, "%s.fxfix.ll", infile); unlink (filename);
	sprintf (filename, "%s.fxfix.rr", infile); unlink (filename);
	sprintf (filename, "%s.fxfix.lr", infile); unlink (filename);
	sprintf (filename, "%s.fxfix.rl", infile); unlink (filename);
	}
					/* Compute and sum the ratios, and */
					/* form weighted averages */
    calc_vec_avg (aeoutfile, vec_avg);
    if (clean) unlink (aeoutfile);
					/* Templates */
    memcpy (outdata, data, sizeof (fringesum));
    memcpy (outdata+1, data, sizeof (fringesum));
    memcpy (outdata+2, data, sizeof (fringesum));
    memcpy (outdata+3, data, sizeof (fringesum));
    memcpy (outdata+4, data, sizeof (fringesum));
    outdata[0].datatype[1] = 'm';
    outdata[1].datatype[1] = 'n';
    outdata[2].datatype[1] = 'o';
    outdata[3].datatype[1] = 'p';
    outdata[4].datatype[1] = 'q';
					/* Fill in numbers */
    for (i=0; i<5; i++)
	{
	outdata[i].amp = vec_avg[i][0];
	outdata[i].resid_phas = vec_avg[i][1];
	outdata[i].length = vec_avg[i][2];
	}

    return (0);
    }
