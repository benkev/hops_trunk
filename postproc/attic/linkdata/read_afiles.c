/************************************************************************/
/*									*/
/* Takes a list of filenames and reads them as A-files, building up	*/
/* 3 dynamically allocated arrays of parsed A-file data, one each for	*/
/* root, corel and fringe data.  Also maintains a count of each type	*/
/* of data.								*/
/*									*/
/*	Inputs:		argv, argc	list of files in standard fmt.	*/
/*									*/
/*	Output:		rdata, cdata, 	arrays to be allocated		*/
/*			fdata		and filled			*/
/*			count		Count of elements in each array	*/
/*			return value	0=OK, 1=bad			*/
/*									*/
/* Created 20 October 1993 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "adata.h"

int
read_afiles (argv, argc, rdata, cdata, fdata, count)
char **argv;
int argc;
rootsum **rdata;
corelsum **cdata;
fringesum **fdata;
int count[3];
    {
    int i, version, type, rspace, cspace, fspace;
    char line[512];
    FILE *fp;
    struct stat statbuf;
/*  extern int errno, sys_nerr; */
/*  extern char *sys_errlist[]; */
					/* Start with some memory */
    rspace = 100; cspace = fspace = 500;
    *rdata = (rootsum *) calloc (rspace, sizeof (rootsum));
    *cdata = (corelsum *) calloc (cspace, sizeof (corelsum));
    *fdata = (fringesum *) calloc (fspace, sizeof (fringesum));
					/* Check that this worked, */
					/* bigtime abort if not */
    if ((*rdata == NULL) || (*cdata == NULL) || (*fdata == NULL))
	{
	msg ("Memory allocation failure in read_afiles()", 3);
/*	if (errno <= sys_nerr) msg ("%s", 2, sys_errlist[errno]); */
	return (1);
	}
					/* Loop over all specified files.  If one */
					/* or more are bad, just skip, don't abort */
    count[0] = count[1] = count[2] = 0;
    for (i=0; i<argc; i++)
	{
	if (stat (argv[i], &statbuf) != 0)
	    {
	    if (errno == ENOENT) msg ("File '%s' does not exist", 2, argv[i]);
	    else msg ("Problem accessing file '%s'", 2, argv[i]);
	    continue;
	    }
	if ((fp = fopen (argv[i], "r")) == NULL)
	    {
	    msg ("Problem opening '%s'", 2, argv[i]);
	    continue;
	    }
					/* Read and parse the files */
	msg ("Reading data from file '%s' ...", 2, argv[i]);
	while (fgets (line, 511, fp) != NULL)
	    {
	    if (line[0] == '*') continue;           /* Ignore comment lines */

					/* What type of line is this? */
	    aline_id (line, &version, &type);
	    if ((type < 0) || (type > 2))           /* Better be 0, 1 or 2 */
		{
		msg ("Found line with inappropriate format, skipping.",2);
		continue;
		}
					/* call proper parser */
	    switch (type)
		{
		case 0:
		    if (parse_rsumm (line, *rdata + count[0]) >= 0) count[0]++;
		    break;
		case 1:
		    if (parse_csumm (line, *cdata + count[1]) >= 0) count[1]++;
		    break;
		case 2:
		    if (parse_fsumm (line, *fdata + count[2]) >= 0) count[2]++;
		}

					/* Expand arrays as required */
	    if (count[0] == rspace)
		{
		rspace += 100;
		*rdata = (rootsum *) realloc (*rdata, rspace*sizeof(rootsum));
		}
	    if (count[1] == cspace)
		{
		cspace += 500;
		*cdata = (corelsum *) realloc (*cdata, cspace*sizeof(corelsum));
		}
	    if (count[2] == fspace)
		{
		fspace += 500;
		*fdata = (fringesum *) realloc (*fdata, fspace*sizeof(fringesum));
		}
					/* Check that this worked, */
					/* bigtime abort if not */
	    if ((*rdata == NULL) || (*cdata == NULL) || (*fdata == NULL))
		{
		msg ("Memory allocation failure in read_afiles()", 3);
/*		if (errno <= sys_nerr) msg ("%s", 2, sys_errlist[errno]); */
		return (1);
		}
	    }		/* End while loop */
	fclose (fp);
	}		/* End for loop */
					/* Report what we did */
    msg ("Read %d root lines, %d corel lines and %d fringe lines", 2,
			count[0], count[1], count[2]);
    if (argc == 1) msg ("from %d input A-file", 2, argc);
    else msg ("from %d input A-files", 2, argc);

    return (0);
    }
