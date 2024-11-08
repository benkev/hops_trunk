/************************************************************************/
/*									*/
/* Takes a file containing the fringex output for a baseline/scan, all	*/
/* correlation products, sorted nicely by aedit, and computes vector	*/
/* averaged ratios and weights, putting the numbers in a small output	*/
/* array.								*/
/*									*/
/*	Inputs:		filename	sorted fringex output		*/
/*									*/
/*	Output:		vec_avg		double array to hold results	*/
/*			return value	0=OK, else error		*/
/*									*/
/* Created October 28 1997 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include <math.h>
#include "adata.h"

int
calc_vec_avg (char* filename, double vec_avg[5][3])
    {
    char line[256];
    double totamp, phase, ratios[5][3];
    double rl_ll_sum[2], lr_ll_sum[2], rl_rr_sum[2], lr_rr_sum[2], rr_ll_sum[2];
    double n_lr_ll, n_rl_ll, n_lr_rr, n_rl_rr, n_rr_ll;
    fringesum data[10];
    int nd, timetag, ret;
    FILE *fp;
					/* Init */
    rl_ll_sum[0] = rl_ll_sum[1] = n_rl_ll = 0.0;
    lr_ll_sum[0] = lr_ll_sum[1] = n_lr_ll = 0.0;
    rl_rr_sum[0] = rl_rr_sum[1] = n_rl_rr = 0.0;
    lr_rr_sum[0] = lr_rr_sum[1] = n_lr_rr = 0.0;
    rr_ll_sum[0] = rr_ll_sum[1] = n_rr_ll = 0.0;
					/* Read sorted file and form */
					/* all the ratios we can for each */
					/* time segment, adding them into */
					/* vector totals */
    if ((fp = fopen (filename, "r")) == NULL)
	{
	msg ("Error opening %s", 3, filename);
	return (1);
	}
    nd = 0;
    while (fgets (line, 511, fp) != NULL)
	{
        if (afile_comment(line)) continue;
	if ((ret = parse_fsumm (line, data+nd)) != 0)
	    { 
	    msg ("Error return %d from parse_fsumm()", 2, ret); 
	    exit (1); 
	    }
					/* New segment, not first? */
	if ((nd > 0) && (data[nd].time_tag != timetag))
	    {
					/* Form ratios */
	    ratio (data, nd, ratios);
					/* Add into vector sums */
	    if (ratios[0][0] >= 0.0) 
		{
		rl_ll_sum[0] += ratios[0][0]; 
		rl_ll_sum[1] += ratios[0][1];
		n_rl_ll += ratios[0][2];
		}
	    if (ratios[1][0] >= 0.0) 
		{
		lr_ll_sum[0] += ratios[1][0]; 
		lr_ll_sum[1] += ratios[1][1];
		n_lr_ll += ratios[1][2];
		}
	    if (ratios[2][0] >= 0.0) 
		{
		rl_rr_sum[0] += ratios[2][0]; 
		rl_rr_sum[1] += ratios[2][1];
		n_rl_rr += ratios[2][2];
		}
	    if (ratios[3][0] >= 0.0) 
		{
		lr_rr_sum[0] += ratios[3][0]; 
		lr_rr_sum[1] += ratios[3][1];
		n_lr_rr += ratios[3][2];
		}
	    if (ratios[4][0] >= 0.0) 
		{
		rr_ll_sum[0] += ratios[4][0]; 
		rr_ll_sum[1] += ratios[4][1];
		n_rr_ll += ratios[4][2];
		}
					/* Reset for next segment */
	    memcpy (data, data+nd, sizeof (fringesum));
	    nd = 0;
	    }

	timetag = data[nd].time_tag;
	nd++;
	}
    fclose (fp);
					/* Clean up last segment */
    if (nd > 0)
	{
					/* Form ratios */
	ratio (data, nd, ratios);
					/* Add into vector sums */
	if (ratios[0][2] >= 0.0) 
	    {
	    rl_ll_sum[0] += ratios[0][0]; 
	    rl_ll_sum[1] += ratios[0][1];
	    n_rl_ll += ratios[0][2];
	    }
	if (ratios[1][2] >= 0.0) 
	    {
	    lr_ll_sum[0] += ratios[1][0]; 
	    lr_ll_sum[1] += ratios[1][1];
	    n_lr_ll += ratios[1][2];
	    }
	if (ratios[2][2] >= 0.0) 
	    {
	    rl_rr_sum[0] += ratios[2][0]; 
	    rl_rr_sum[1] += ratios[2][1];
	    n_rl_rr += ratios[2][2];
	    }
	if (ratios[3][2] >= 0.0) 
	    {
	    lr_rr_sum[0] += ratios[3][0]; 
	    lr_rr_sum[1] += ratios[3][1];
	    n_lr_rr += ratios[3][2];
	    }
	if (ratios[4][2] >= 0.0) 
	    {
	    rr_ll_sum[0] += ratios[4][0]; 
	    rr_ll_sum[1] += ratios[4][1];
	    n_rr_ll += ratios[4][2];
	    }
	}
					/* Compute averages and store weights */
    if ((vec_avg[0][2] = n_rl_ll) > 0.0)
	{
	totamp = sqrt (rl_ll_sum[0]*rl_ll_sum[0] + rl_ll_sum[1]*rl_ll_sum[1]);
	phase = atan2 (rl_ll_sum[1], rl_ll_sum[0]);
	vec_avg[0][0] = totamp / n_rl_ll;
	vec_avg[0][1] = phase * 180.0 / 3.141592654;
	}
    if ((vec_avg[1][2] = n_lr_ll) > 0.0)
	{
	totamp = sqrt (lr_ll_sum[0]*lr_ll_sum[0] + lr_ll_sum[1]*lr_ll_sum[1]);
	phase = atan2 (lr_ll_sum[1], lr_ll_sum[0]);
	vec_avg[1][0] = totamp / n_lr_ll;
	vec_avg[1][1] = phase * 180.0 / 3.141592654;
	}
    if ((vec_avg[2][2] = n_rl_rr) > 0.0)
	{
	totamp = sqrt (rl_rr_sum[0]*rl_rr_sum[0] + rl_rr_sum[1]*rl_rr_sum[1]);
	phase = atan2 (rl_rr_sum[1], rl_rr_sum[0]);
	vec_avg[2][0] = totamp / n_rl_rr;
	vec_avg[2][1] = phase * 180.0 / 3.141592654;
	}
    if ((vec_avg[3][2] = n_lr_rr) > 0.0)
	{
	totamp = sqrt (lr_rr_sum[0]*lr_rr_sum[0] + lr_rr_sum[1]*lr_rr_sum[1]);
	phase = atan2 (lr_rr_sum[1], lr_rr_sum[0]);
	vec_avg[3][0] = totamp / n_lr_rr;
	vec_avg[3][1] = phase * 180.0 / 3.141592654;
	}
    if ((vec_avg[4][2] = n_rr_ll) > 0.0)
	{
	totamp = sqrt (rr_ll_sum[0]*rr_ll_sum[0] + rr_ll_sum[1]*rr_ll_sum[1]);
	phase = atan2 (rr_ll_sum[1], rr_ll_sum[0]);
	vec_avg[4][0] = totamp / n_rr_ll;
	vec_avg[4][1] = phase * 180.0 / 3.141592654;
	}

    return (0);
    }
