/************************************************************************/
/*									*/
/* Deals with everything on the command line.				*/
/*									*/
/*	Inputs:		argc, argv	command line args		*/
/*									*/
/*	Output:		infile		input data filename		*/
/*									*/
/* Created October 28 1997 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include <unistd.h>

#define FALSE 0

int
parse_cmdline (int argc, char** argv, char* infile)
    {
    char c;
    extern char *optarg;
    extern int optind, msglev, default_cotime, clean;
					/* Default output */
    strcpy (infile, "none");
                                        /* parse command line and read in */
                                        /* filename */
    while ((c = getopt (argc, argv, "cm:t:")) != -1)
        {
        switch (c)
            {
					/* Suppress scratch file deletion */
	    case 'c':
		clean = FALSE;
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

	    case 't':
		if (sscanf (optarg, "%d", &default_cotime) != 1)
		    {
		    msg ("Invalid -t argument '%s'", 2, optarg);
		    return (1);
		    }
		if ((default_cotime < 1) || (default_cotime > 999))
		    {
		    msg ("Illegal coherence time specified, %d\n", 2,
			default_cotime);
		    return (1);
		    }
		break;

            case '?':
            default:
                syntax("$HeadURL: svn+ssh://vault.haystack.mit.edu/svn/hops/trunk/postproc/pratio/parse_cmdline.c $");
                return (1);
            }
        }
					/* Get data file name */
    if ((argc - optind) != 1)
	{
	msg ("Incorrect number of command line arguments", 2);
	return (1);
	}
    strcpy (infile, argv[argc-1]);
    return (0);
    }
