/************************************************************************/
/*                                                                      */
/* This handles the command line.  It  looks for UNIX-style option      */
/* flags (which must come first), sets a couple of externs accordingly, */
/* and passes the remaining arguments back in the form of a count of    */
/* arguments used up by the flags.                                      */
/*                                                                      */
/*      Inputs:         argc, argv              command line arguments  */
/*                                              in standard form        */
/*                                                                      */
/*      Output:         argind                  points to 1st arg after */
/*                                              flags                   */
/*                      return value            0=OK, !=0 is error      */
/*                                                                      */
/* 20 October 1993 by CJL                                               */
/*                                                                      */
/************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#define FALSE 0

parse_cmdline (argc, argv, argind)
int argc;
char **argv;
int *argind;
    {
    char c;
    extern int find_roots, find_type3;
    extern char destdir[], datadir[];
    extern char *optarg;
    extern int optind;
                                        /* Interpret command line flags */
    while ((c = getopt (argc, argv, "rsd:")) != -1) 
        {
        switch (c) 
            {
            case 'r':
                find_roots = FALSE;
                break;

            case 's':
                find_type3 = FALSE;
                break;

            case 'd':
                strcpy (destdir, optarg);
                if (strcmp (destdir, datadir) == 0)
                    {
                    msg ("Error, destination directory must not be same", 3);
                    msg ("as main data directory '%s'", 3, datadir);
                    return (1);
                    }
                break;

            case '?':
                return (1);
                break;
            }
        }

    *argind = optind;
                                        /* No filename args means error */
    if (argc == *argind) return (1);

    return (0);
    }
