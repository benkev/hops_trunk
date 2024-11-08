/************************************************************************/
/*									*/
/* Opens up the four input files containing the four polarization	*/
/* correlation products, sets the datatype field, and writes them all	*/
/* back out to a temporary file.					*/
/*									*/
/*	Inputs:		infile		root name for input files	*/
/*									*/
/*	Output:		return value	0=OK, else bad			*/
/*									*/
/* Created 14 October 1997 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include "adata.h"

int
set_pol_field (char* infile)
    {
    FILE *fp, *fpout;
    int i, ret;
    char ptype, filename[256], outfile[256], line[512];
    fringesum datum;

					/* Open output file */
    sprintf (outfile, "%s.LLRRRLLR", infile);
    if ((fpout = fopen (outfile, "w")) == NULL)
	{
	msg ("Error, could not create file '%s'", 3, outfile);
	return (1);
	}
    for (i=0; i<4; i++)
	{
					/* Sort out which file to read */
	switch (i)
	    {
	    case 0:
		ptype = 'w';
		sprintf (filename, "%s.LL", infile);
		break;
	    case 1:
		ptype = 'x';
		sprintf (filename, "%s.RR", infile);
		break;
	    case 2:
		ptype = 'y';
		sprintf (filename, "%s.RL", infile);
		break;
	    case 3:
		ptype = 'z';
		sprintf (filename, "%s.LR", infile);
	    default:
		break;
	    }
					/* Open it, OK if missing */
	if ((fp = fopen (filename, "r")) == NULL)
	    {
	    msg ("Warning, file '%s' does not exist", 2, filename);
	    continue;
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
					/* Dump to temp file */
	    write_fsumm (&datum, fpout);
	    }

	fclose (fp);
	}

    fclose (fpout);
    return (0);
    }
