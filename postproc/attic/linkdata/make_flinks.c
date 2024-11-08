/************************************************************************/
/*                                                                      */
/* Creates directories as needed in the destination directory, and      */
/* makes symbolic links to the data files.  The number of failed        */
/* links is returned, negative if a severe error occurs.                */
/*                                                                      */
/*      Inputs:         fdata           Array of parsed fringe lines    */
/*                      n               Number of lines in array        */
/*                      find_roots      If true, link parent root as    */
/*                                      well                            */
/*                                                                      */
/*      Output:         return          0 = OK, >0 bad, <0 awful        */
/*                                                                      */
/* Created 20 October 1993 by CJL                                       */
/*                                                                      */
/************************************************************************/
#include <stdio.h>
#include "adata.h"

int
make_flinks (fdata, n, find_roots, find_type3)
fringesum *fdata;
int n, find_roots;
    {
    int i, nfail;
    char *filename, *rootname, *type3name;
    char *fringename(), *parent_root(), *type_3();

    if (n > 0) msg ("Linking %d fringe filenames ...", 2, n);

    nfail = 0;
    for (i=0; i<n; i++)
        {
        if ((filename = fringename (fdata+i)) == NULL)
            {
            msg ("Failed to generate a fringe filename", 2);
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
            { printf ("linkdata: Fringe file %d ...\r", i); fflush (stdout); }

                                        /* Figure out the root filename */
                                        /* and link it too if requested */
        if (find_roots)
            {
            rootname = parent_root (filename, fdata[i].source);
            if (make_link (rootname) != 0)
                {
                msg ("Failed to make link for file '%s'", 2, filename);
                nfail++;
                }
            }
                                        /* Same thing for type 3 files */
        if (find_type3)
            {
            type3name = type_3 (filename, "ref");
            if (make_link (type3name) != 0)
                {
                msg ("Failed to make link for file '%s'", 2, filename);
                nfail++;
                }
            type3name = type_3 (filename, "rem");
            if (make_link (type3name) != 0)
                {
                msg ("Failed to make link for file '%s'", 2, filename);
                nfail++;
                }
            }
        }

    msg ("Links to %d fringe files now exist", 2, i - nfail);

    return (nfail);
    }
