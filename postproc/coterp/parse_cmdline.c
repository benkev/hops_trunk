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
/* Created 27 February 1996 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "cpgplot.h"
#include "coterp.h"

double max_cotime;

int
parse_cmdline (argc, argv, fpout, plot)
int argc;
char **argv;
FILE **fpout;
int *plot;
    {
    char c, outfile[100], device[100];
    int i;
    double cotime;
    extern char *optarg;
    extern int optind, msglev;
					/* Defaults */
    *fpout = stdout;
    *plot = FALSE;
    max_cotime = 392.0;
                                        /* parse command line and read in */
                                        /* filename */
    while ((c = getopt (argc, argv, "c:d:m:o:x")) != -1)
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
					/* BUG 7/26/96 found by CJL */
					/* As best I can tell, there is a bug */
					/* in the standard C library in the area */
					/* of file IO.  This file, fpout, gets */
					/* written to late in the program with a */
					/* call to afile_header(), which uses a */
					/* fprintf call.  The line of code below */
					/* seems to initialize the FILE structure */
					/* elements, which otherwise contains */
					/* null pointers and a zero __cnt variable */
					/* The fprintf in afile_header seg-faults */
					/* without this initialization.  I speculate */
					/* that initialization of the FILE struct */
					/* is delayed by the system until it is needed, */
					/* but that there is a loophole which allows */
					/* actual IO to be attempted before the */
					/* initialization has taken place, under */
					/* certain circumstances.  This workaround is */
					/* obviously system-dependent, but at least */
					/* should be benign on other systems */
		fprintf (*fpout, "");
                break;

	    case 'x':
		strcpy (device, "/XW");
                *plot = TRUE;
                break;

            case '?':
            default:
                syntax("$HeadURL: svn+ssh://vault.haystack.mit.edu/svn/hops/trunk/postproc/coterp/parse_cmdline.c $");
                return (1);
            }
        }
					/* Input files on command line handled */
					/* in main routine.

					/* Open plot device */
    if (*plot)
	{
	if (cpgbeg (0, device, 1, 1) != 1)
	    {
	    msg ("Could not open pgplot device '%s', abort.", 3, device);
	    exit (1);
	    }
	cpgsubp (4, 3);
	}

    return (0);
    }


