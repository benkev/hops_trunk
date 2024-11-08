/************************************************************************/
/*									*/
/* Opens the files which are to contain the final polarization ratio	*/
/* records								*/
/*									*/
/*	Inputs:		infile		filename root for this run	*/
/*									*/
/*	Output:		fp1-6		via extern			*/
/*			return value	0=OK, else bad			*/
/*									*/
/* Created 15 October 1997 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>

FILE *fp_rl_ll, *fp_lr_ll, *fp_rl_rr, *fp_lr_rr, *fp_rr_ll;

int
open_outfiles (char* infile)
    {
    char filename[256];

    sprintf (filename, "%s.rl_ll", infile);
    if ((fp_rl_ll = fopen (filename, "w")) == NULL) return (1);
    sprintf (filename, "%s.lr_ll", infile);
    if ((fp_lr_ll = fopen (filename, "w")) == NULL) return (1);
    sprintf (filename, "%s.rl_rr", infile);
    if ((fp_rl_rr = fopen (filename, "w")) == NULL) return (1);
    sprintf (filename, "%s.lr_rr", infile);
    if ((fp_lr_rr = fopen (filename, "w")) == NULL) return (1);
    sprintf (filename, "%s.rr_ll", infile);
    if ((fp_rr_ll = fopen (filename, "w")) == NULL) return (1);

    return (0);
    }
