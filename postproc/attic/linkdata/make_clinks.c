/************************************************************************/
/*									*/
/* Creates directories as needed in the destination directory, and	*/
/* makes symbolic links to the data files.  The number of failed	*/
/* links is returned, negative if a severe error occurs.		*/
/*									*/
/*	Inputs:		cdata		Array of parsed corel lines	*/
/*			n		Number of lines in array	*/
/*			find_roots	If true, link parent root as	*/
/*					well				*/
/*									*/
/*	Output:		return		0 = OK, >0 bad, <0 awful	*/
/*									*/
/* Created 20 October 1993 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include "adata.h"

int
make_clinks (cdata, n, find_roots)
corelsum *cdata;
int n, find_roots;
    {
    int i, nfail;
    char *filename, *rootname;
    char *corelname(), *parent_root();

    if (n > 0) msg ("Linking %d corel filenames ...", 2, n);

    nfail = 0;
    for (i=0; i<n; i++)
	{
					/* Limited tolerance to stop garbage */
					/* screen output */
	if (nfail > 50)
	    {
	    msg ("Too many failures", 3);
	    break;
	    }
					/* Do it for the specified file */
	if ((filename = corelname (cdata+i)) == NULL)
	    {
	    msg ("Failed to generate a corel filename", 2);
	    nfail++;
	    continue;
	    }

	if (make_link (filename) != 0)
	    {
	    msg ("Failed to make link for file '%s'", 2, filename);
            nfail++;
            continue;
            }
					/* Informational */
	if (i%10 == 0)
	    { printf ("linkdata: Corel file %d ...\r", i); fflush (stdout); }

					/* Figure out the root filename */
					/* and link it too if requested */
	if (! find_roots) continue;
	rootname = parent_root (filename, cdata[i].source);
	if (make_link (rootname) != 0)
            {
            msg ("Failed to make link for file '%s'", 2, filename);
            nfail++;
            }
	}

    msg ("Links to %d corel files now exist", 2, i - nfail);

    return (nfail);
    }
