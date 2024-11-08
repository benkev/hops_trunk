/************************************************************************/
/*									*/
/* This routine is responsible for reading data into the internal	*/
/* memory of coterp.  The routine also takes care of			*/
/* automatically expanding the data arrays if they get full, using a 	*/
/* standard UNIX memory allocation package				*/
/*									*/
/*	Inputs:		data		Data array			*/
/*			filename	Name of file to read		*/
/*									*/
/*	Output:		return value	0 for success, -1 for failure	*/
/*									*/
/* Created November 29 1995 by CJL, borrowed from cofit			*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "coterp.h"

int
read_data (data, filename, nrec, space)
char *filename;
fringedata **data;
int *nrec, *space;
    {
    FILE *fp;
    struct stat statbuf;
    int nkb, nbadtype, nfail, version, type, pret;
    char c, line[512];
					/* Open up input file */
    if (stat (filename, &statbuf) != 0) 
	{
	if (errno == ENOENT) msg ("File '%s' does not exist", 3, filename);
	else msg ("Problem accessing file '%s'", 3, filename);
	return (-1);
	}

    else if ((fp = fopen (filename,"r")) == NULL) 
	{
	msg ("Problem opening '%s'", 3, filename);
	return (-1);
	}
					/* Read through entire file */
    msg ("Reading data from file '%s' ...", 1, filename);
    nbadtype = nfail = 0;
    while (fgets (line, 511, fp) != NULL) 
	{
					/* Ignore comment lines */
        if (afile_comment(line)) continue;
					/* What type of line is this? */
					/* Handle case of old format */
	aline_id (line, &version, &type);
					/* Better be 0, 1, 2, 3 or 4 */
	if (type != 2)
	    {
	    msg ("Found line other than type-2, skipping", 2);
	    continue;
	    }
					/* call parser */
	pret = parse_fsumm (line, &((*data)[*nrec].fdata));

					/* Count parsing failures */
	if (pret != 0)
	    {
	    msg ("line parse failed, pret = %d", -1, pret);
	    nfail++;
	    continue;
	    }
	c = (*data)[*nrec].fdata.datatype[0];
	if ((c != 'K') && (c != 'S'))
	    {
	    msg ("line is incorrect data type", 1);
	    nfail++;
	    continue;
	    }
					/* Valid record, expand array if necessary */
	*nrec += 1;
	if (*nrec == *space)
	    {
	    *space += 100;
	    *data = (fringedata *) realloc(*data, *space*sizeof(fringedata));
	    if (*data == NULL)
		{
		perror("realloc");
		msg("Fatal error allocating memory for data - Abort!", 3);
		exit(1);
		}
	    nkb = (*space*sizeof(fringedata))/1024;
	    printf ("Expanded array memory to %d Kb ...\r", nkb); fflush (stdout);
	    }

	}				/* End of main read loop */

    nkb = (*space*sizeof(fringedata))/1024;
    msg ("Total space occupied by data arrays = %d Kb", 1, nkb);

    if (nfail > 0)
	msg ("Warning, %d lines failed to parse in '%s' (ignored)", 2, 
			nfail, filename);

    fclose(fp);
    return(0);
    }
