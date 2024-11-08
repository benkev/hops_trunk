/************************************************************************/
/*                                                                      */
/* Program to convert a specification of a subset of data into a        */
/* directory structure filled with symbolic links to the real data.     */
/* This "dummy" directory structure can then be used to perform any     */
/* desired operation on the selected subset of data.  The subset        */
/* specification takes the form of a standard A-file, presumably        */
/* filtered according to various criteria by AEDIT.  The overall result */
/* is a flexible tool for selecting and operating on data, be it        */
/* generation of export tapes, refringing, or recorrelating operations, */
/* or applications not yet anticipated.                                 */
/*                                                                      */
/*      Inputs:         Filename(s)     Input A-file(s)                 */
/*                                                                      */
/*      Output:         Directory       Filled in with subdirs/links    */
/*                                                                      */
/* Created October 18 1993 by CJL                                       */
/*                                                                      */
/************************************************************************/
#include <stdio.h>
#include "adata.h"

#define FALSE 0
#define TRUE 1

char progname[9] = "linkdata";
int msglev = 1;

int find_roots = TRUE;
int find_type3 = TRUE;
char destdir[256];

main (argc, argv)
int argc;
char *argv[];
    {
    int argind, count[3];
    int foulup = FALSE;
    rootsum *rdata;
    corelsum *cdata;
    fringesum *fdata;
    extern char datadir[];
                                        /* Set up environment, esp. datadir */
    environment();
    if (datadir[0] != '/')
        {
        msg ("Warning ... DATADIR appears to be a relative pathname", 2);
        msg ("This is not good practise, especially for linkdata", 2);
        }
                                        /* Strip off any option flags and set externs */
    if (parse_cmdline (argc, argv, &argind) != 0)
        {
        syntax();
        exit (1);
        }
                                        /* Read in afile data from all files */
    if (read_afiles (argv+argind, argc-argind, &rdata, &cdata, &fdata, count) != 0)
        exit (1);
                                        /* Make space for list of dirs/files created */
                                        /* to allow for reversibility of ops */
                                        /* Lists declared extern in alloc_lists() */
/*    if (alloc_lists (count[0] + count[1] + count[2]) != 0) exit (1);  */

                                        /* Create the root links, making subdirs */
                                        /* as we go */
    if (make_rlinks (rdata, count[0]) != 0)
        foulup = TRUE;
                                        /* Corel and fringe files need parent roots */
                                        /* Maybe find them if not already listed */
    else if (make_clinks (cdata, count[1], find_roots) != 0)
        foulup = TRUE;
    else if (make_flinks (fdata, count[2], find_roots, find_type3) != 0)
        foulup = TRUE;
                                        /* Tidy up the mess if failure */
    if (foulup)
        {
        cleanup (destdir);
        exit (1);
        }

    exit (0);
    }
