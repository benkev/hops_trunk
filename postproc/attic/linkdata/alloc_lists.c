/************************************************************************/
/*                                                                      */
/* Allocates space for lists of directories and files, filled when      */
/* directories and links are created, and used for removal of same if   */
/* and when the program aborts.                                         */
/*                                                                      */
/*      Inputs:         nfiles          Maximum # of files expected     */
/*                                                                      */
/*      Output:         dlist, flist    allocated externs               */
/*                      return value    0=OK, 1=BAD                     */
/*                                                                      */
/* Created 21 October 1993 by CJL                                       */
/*                                                                      */
/************************************************************************/
#include <stdlib.h>
#include <stdio.h>

char **dlist;
char **flist;
int ndir = 0, nfil = 0;

int
alloc_lists (nfiles)
int nfiles;
    {
    int i;

    dlist = (char **) malloc (nfiles * sizeof (char *));
    flist = (char **) malloc (nfiles * sizeof (char *));
    if ((dlist == NULL) || (flist == NULL))
        {
        msg ("Allocation failure in alloc_lists()", 3);
        return (1);
        }
    for (i=0; i<nfiles; i++)
        {
                                        /* Up to 2 directories created per file */
        dlist[i*2] = (char *) malloc (32);
        dlist[i*2 + 1] = (char *) malloc (32);
        flist[i] = (char *) malloc (32);
        if ((dlist[i] == NULL) || (flist[i] == NULL))
            {
            msg ("Allocation failure in alloc_lists()", 3);
            return (1);
            }
        }
    return (0);
    }
    
