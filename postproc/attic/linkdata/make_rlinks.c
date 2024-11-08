/************************************************************************/
/*									*/
/* Creates directories as needed in the destination directory, and	*/
/* makes symbolic links to the data files.  The number of failed	*/
/* links is returned, negative if a severe error occurs.		*/
/*									*/
/*	Inputs:		rdata		Array of parsed root lines	*/
/*			n		Number of lines in array	*/
/*			destdir		Where to put links		*/
/*									*/
/*	Output:		return		0 = OK, >0 bad, <0 awful	*/
/*									*/
/* Created 20 October 1993 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include "adata.h"

int
make_rlinks (rdata, n)
rootsum *rdata;
int n;
    {
    int i, nfail;
    char *filename, *rootname();

    if (n > 0) msg ("Linking %d root filenames ...", 2, n);

    nfail = 0;
    for (i=0; i<n; i++)
	{
	if (nfail > 50)
	    {
	    msg ("Too many failures", 3);
	    break;
	    }

	if ((filename = rootname (rdata+i)) == NULL)
	    {
	    msg ("Failed to generate a root filename", 2);
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
            { printf ("linkdata: Root file %d ...\r", i); fflush (stdout); }
	}

    msg ("Links to %d root files now exist", 2, i - nfail);

    return (nfail);
    }
