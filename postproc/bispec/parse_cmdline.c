/************************************************************************/
/*									*/
/* Deals with everything on the command line.				*/
/*									*/
/*	Inputs:		argc, argv	command line args		*/
/*									*/
/*	Output:		fpbase		open stream for baseline data	*/
/*			fptri		open stream for triangle data	*/
/*			outfile		name of output bispec. file	*/
/*			return value	0=OK, 1=BAD			*/
/*									*/
/* Created August 23 1996 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "bispec.h"
#include "mk4_util.h"

int
parse_cmdline (int argc,
               char **argv,
               FILE **fpbase,
               FILE **fptri,
               char *outfile)
    {
    char c;
    int fd;
    extern char *optarg;
    extern int optind, msglev;
					/* Default output */
    strcpy (outfile, "none");
                                        /* parse command line and read in */
                                        /* filename */
    while ((c = getopt (argc, argv, "m:o:")) != -1)
        {
        switch (c)
            {
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
					/* Create or truncate output file */
		fd = creat (outfile, 0666);
		if (fd < 0) 
		    {
		    msg ("Failed to create output file", 2);
		    return (1);
		    }
		close (fd);
                break;

            case '?':
            default:
                syntax("$HeadURL: svn+ssh://vault.haystack.mit.edu/svn/hops/trunk/postproc/bispec/parse_cmdline.c $");
                return (1);
            }
        }
					/* Get data file names and open them */
    if ((argc - optind) != 2)
	{
	msg ("Incorrect number of command line arguments", 2);
	return (1);
	}
    if ((*fpbase = fopen (argv[argc-2], "r")) == NULL)
	{
	msg ("Could not open baseline file '%s'", 2, argv[argc-2]);
	return (1);
	}
    if ((*fptri = fopen (argv[argc-1], "r")) == NULL)
	{
	msg ("Could not open triangle file '%s'", 2, argv[argc-1]);
	return (1);
	}
    return (0);
    }


