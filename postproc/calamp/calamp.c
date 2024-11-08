/************************************************************************/
/*                                                                      */
/* Framework of program to calibrate amplitudes in A-files              */
/*                                                                      */
/*      Inputs:         fitlist                 Fitted "cal" list       */
/*                      masterlist              All scans represented   */
/*                                                                      */
/*      Output:         masterlist.cot          coherence times entered */
/*                                                                      */
/* Created September 25 1996 by CJL                                     */
/* Diddled 1996 October 3, JAB                                          */
/* Changed and added station codes Nov. 21, 1997, DAB                   */ 
/*                                                                      */
/************************************************************************/
#include <stdio.h>  
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include "calamp.h" 
#include "mk4_util.h" 
#include "mk4_afio.h" 
#include "msg.h"

#define TRUE 1
#define FALSE 0
#define NCAL 1000
 
// char progname[7] = "calamp";
// int msglev = 2;
int need_same_source = FALSE;

/* Two-char station codes */
char * sta[] = { "  ", "ha", "on", "hc", "kp", "qb", "ov", "bn", "pt", "pv", "mh", "se", "fd", "la", "mk" };
int sta_size = 15;
/* One-char station codes */
char stl[]  =  {  ' ',  'K',  'S',  'h',  'k',  'q',  'o',  'B',  'P',  'x',  'Z',  'd',  'F',  'X',  'U' };

struct Dal dal;

int
main (int argc, char* argv[])
    {
    int time, version, type, first, dur, read_calfile();
    char stn1, stn2, source[32];
    float elev1, elev2;
    char line[512];
    fringesum datum;
    FILE *fp, *fpout;
    extern int optind;
    set_progname("calamp");
    set_msglev(2);
                                /* Process option flags on command line */
    if (parse_cmdline (argc, argv, &fpout) != 0)
        {
        syntax("$HeadURL: svn+ssh://vault.haystack.mit.edu/svn/hops/trunk/postproc/calamp/calamp.c $");
        exit (1);
        }
                                        /* Open input A-file */
    if ((fp = fopen (argv[optind], "r")) == NULL)
        {
        msg ("Cannot open input A-file '%s'", 2, argv[1]);
        exit (1);
        }
                                        /* read and store the cal info from */
                                        /* a disk file */
    if (read_calfile(argv[optind+1]) != 0) /* JAB */ 
        {
        msg ("Failure reading calibration information from '%s'", 
                                                3, argv[optind+1]);
        exit (1);
        }
                                        /* Read through the input A-file */
    first = TRUE;
    while (fgets (line, 511, fp) != NULL)
        {
                                        /* Ignore comment lines */
        if (afile_comment(line)) continue;
                                        /* What type of line is this? */
        aline_id (line, &version, &type);
                                        /* Better be 0, 1, 2, 3 or 4 */
        if (type != 2)
            {
            msg ("Found line other than type-2, skipping", 2);
            continue;
            }
                                        /* call parser, get relevant fields */
        if (parse_fsumm (line, &datum) != 0)
            {
            msg ("Failed to parse line in input file '%s', skipping", 2, argv[1]);
            continue;
            }
        time = datum.time_tag;
        stn1 = datum.baseline[0];
        stn2 = datum.baseline[1];
        elev1 = datum.ref_elev;
        elev2 = datum.rem_elev;
        dur = datum.duration;
        strcpy (source, datum.source);
                                        /* Modify amplitude field */
        datum.amp *= calfact(time, stn1, stn2, elev1, elev2, dur, source); /* JAB */
                                        /* Check the answer */
        if (datum.amp <= 0.0)
            {
            msg ("Cannot calibrate scan '%s', skipping", 2, fringename (&datum));
            continue;
            }
                                        /* Write header first time through */
        if (first)
            {
            if (afile_header (version, 2, fpout) != 0)
                msg ("Error writing output file header", 2);
            first = FALSE;
            }
                                        /* Write out modified line */
        if (write_fsumm (&datum, fpout) != 0)
            {
            msg ("Error writing to output file", 2);
            exit (1);
            }
        }
                                        /* Success */
    exit (0);
    }
