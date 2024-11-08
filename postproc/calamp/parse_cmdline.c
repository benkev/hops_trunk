/************************************************************************/
/*                                                                      */
/* Deals with everything on the command line.                           */
/*                                                                      */
/*      Inputs:         argc, argv      command line args               */
/*                                                                      */
/*      Output:         fpout           output file pointer             */
/*                      return value    0=OK, 1=BAD                     */
/*                                                                      */
/* Created 25 September 1996 by CJL                                     */
/*                                                                      */
/************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "calamp.h"
#include "mk4_util.h"
#include "mk4_afio.h"

int caltime = CALTIME;
int force_tsys = FALSE;

int
parse_cmdline (int argc,
               char **argv,
               FILE **fpout)
    {
    char c, outfile[100];
    extern char *optarg;
    extern int optind, msglev, need_same_source;
                                        /* Default */
    *fpout = stdout;
                                        /* parse command line and read in */
                                        /* filename */
    while ((c = getopt (argc, argv, "c:m:o:st")) != -1)
        {
        switch (c)
            {
                                        /* Cal time tolerance control */
            case 'c':
                if (sscanf (optarg, "%d", &caltime) != 1)
                    {
                    msg ("Invalid -c flag argument '%s'", 2, optarg);
                    msg ("Cal time remains at %d", 2, caltime);
                    }
                if (caltime < 0 || caltime > 1000000)
                    {
                    msg ("Invalid -c flag argument '%s'", 2, optarg);
                    caltime = CALTIME;
                    msg ("Cal time remains at %d", 2, caltime);
                    }
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
                                        /* Require same source for cal */
            case 's':
                need_same_source = TRUE;
                break;
                                        /* Force program to ignore Ta info */
            case 't':
                force_tsys = TRUE;
                break;
            case '?':
            default:
                syntax("$HeadURL: svn+ssh://vault.haystack.mit.edu/svn/hops/trunk/postproc/calamp/parse_cmdline.c $");
                return (1);
            }
        }
    return (0);
    }


