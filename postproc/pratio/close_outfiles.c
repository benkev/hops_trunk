/************************************************************************/
/*									*/
/* Cleanup routine.							*/
/*									*/
/*	Inputs:		none						*/
/*									*/
/*	Output:		none						*/
/*									*/
/* Created 15 October 1997 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>

void
close_outfiles()
    {
    extern FILE *fp_rl_ll, *fp_lr_ll, *fp_rl_rr, *fp_lr_rr, *fp_rr_ll;

    fclose (fp_rl_ll);
    fclose (fp_lr_ll);
    fclose (fp_rl_rr);
    fclose (fp_lr_rr);
    fclose (fp_rr_ll);
    }
