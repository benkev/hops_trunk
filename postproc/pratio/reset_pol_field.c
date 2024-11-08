/************************************************************************/
/*									*/
/* Simply sets the datatype field to reflect the polarization type	*/
/* specified in ptype.  infile and outfile must be different.  This is	*/
/* in lieu of proper polarization support in a later version of the 	*/
/* A-file format.							*/
/*									*/
/*	Inputs:		infile		Name of untypes input file	*/
/*			ptype		Character for pol type (wxyz)	*/
/*									*/
/*	Output:		outfile		modified version of infile	*/
/*			return value	0=OK, else bad			*/
/*									*/
/* Created 15 October 1997 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include "adata.h"

int
reset_pol_field (char* infile, char* outfile, char ptype)
    {
    FILE *fp, *fpout;
    char line[512];
    int i, ret;
    fringesum datum;

					/* Open input file */
    if ((fp = fopen (infile, "r")) == NULL)
	{
	msg ("Error, file '%s' does not exist", 3, infile);
	return (1);
	}
					/* Open output file */
    if ((fpout = fopen (outfile, "w")) == NULL)
	{
	msg ("Error, could not create file '%s'", 3, outfile);
	return (1);
	}
					/* Read data, resetting data type */
    while (fgets (line, 511, fp) != NULL)
	{
        if (afile_comment(line)) continue;
	if ((ret = parse_fsumm (line, &datum)) != 0)
	    { 
	    msg ("Error return %d from parse_fsumm()", 2, ret); 
	    return (1);
	    }
	datum.datatype[1] = ptype;
        if (write_fsumm (datum, fpout) != 0)
	    {
	    msg ("Error writing to file '%s'", 2, outfile);
	    return (1);
	    }
	}

    fclose (fp);
    fclose (fpout);
    return (0);
    }
