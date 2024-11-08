/************************************************************************/
/*									*/
/* Deals with everything on the command line.				*/
/*									*/
/*	Inputs:		argc, argv	command line args		*/
/*									*/
/*	Output:		fpout		output file pointer		*/
/*			display		For graphical output		*/
/*			return value	0=OK, 1=BAD			*/
/*									*/
/* Created October 5 1995 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "mk4_util.h"
#include "cpgplot.h"
#include "cofit.h"

double max_cotime;

int parse_cmdline (int argc, char **argv,
    FILE **fpout, int *plot, int *ignore_csq)
    {
    char c, outfile[100], device[100];
    int i, cols = 4, rows = 3;
    double cotime;
    extern char *optarg;
    extern int optind, msglev;
					/* Defaults */
    *fpout = stdout;
    *plot = FALSE;
    max_cotime = 392.0;
                                        /* parse command line and read in */
                                        /* filename */
    while ((c = getopt (argc, argv, "c:d:m:o:sxg:")) != -1)
        {
        switch (c)
            {
	    case 'c':
		if (sscanf (optarg, "%lf", &cotime) != 1)
		    {
		    msg ("Invalid -c flag argument '%s'", 2, optarg);
		    msg ("Max. allowed cotime remains at %g sec", 2, max_cotime);
		    }
		else if (cotime <= 1.0)
		    {
		    msg ("Invalid -c flag (must be >= 1.0)", 2);
		    msg ("Max. allowed cotime remains at %g sec", 2, max_cotime);
		    }
		else max_cotime = cotime;
		break;
					/* File away the display string */
	    case 'd':
		strcpy (device, optarg);
		*plot = TRUE;
		break;
					/* Verbosity control */
            case 'm':
                if (sscanf (optarg, "%d", &msglev) != 1)
                    {
                    msg ("Invalid -m flag argument '%s'", 2, optarg);
                    msg ("Message level remains at %d", 2, msglev);
                    }
                if (msglev > 3) msglev = 3;
                if (msglev < -3) msglev = -3;
                break;
					/* Output override, default stdout above */
            case 'o':
                strcpy (outfile, optarg);
		if ((*fpout = fopen (outfile, "w")) == NULL)
                    {
                    msg ("Could not open output file '%s'", 3, outfile);
                    return (1);
                    }
                break;

            case 's':
                *ignore_csq = TRUE;
                break;

	    case 'x':
		strcpy (device, "/XW");
                *plot = TRUE;
                break;

            case 'g':
                if (sscanf (optarg, "%dx%d", &cols, &rows) != 2)
                    {
                    msg ("-g CxR is required", 3);
                    return(1);
                    }
                break;

            case '?':
            default:
                syntax("$HeadURL: svn+ssh://vault.haystack.mit.edu/svn/hops/trunk/postproc/cofit/parse_cmdline.c $");
                return (1);
            }
        }
					/* Input files on command line are */
					/* handled in the main routine. */
					/* Open plot device */
    if (plot)
	{
	if (cpgbeg (0, device, 1, 1) != 1)
	    {
	    msg ("Could not open pgplot device '%s', abort.", 3, device);
	    exit (1);
	    }
	cpgsubp (cols, rows);
	}

    return (0);
    }


