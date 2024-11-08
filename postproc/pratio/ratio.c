/************************************************************************/
/*									*/
/* Takes up to four records, each representing the same segment in 	*/
/* various correlation products, and computes the vector ratios as	*/
/* needed and possible, placing the results in an output array		*/
/*									*/
/*	Inputs:		data		Up to 4 correlation products	*/
/*					in A-file records		*/
/*			nd		Number of records present	*/
/*									*/
/*	Output:		ratios		Array of results (re, imag)	*/
/*									*/
/* Created 28 October 1997 by CJL					*/
/*									*/
/************************************************************************/
#include <math.h>
#include "adata.h"

void
ratio (fringesum* data, int nd, double ratios[5][3])
    {
    int i, ll, rr, rl, lr;
    double amp, phase, weight;
					/* Init output */
    for (i=0; i<5; i++) ratios[i][2] = 0.0;
					/* Find out which ones are present */
    ll = rr = rl = lr = -1;
    for (i=0; i<nd; i++)
	{
	if (data[i].datatype[1] == 'w') ll = i;
	else if (data[i].datatype[1] == 'x') rr = i;
	else if (data[i].datatype[1] == 'y') rl = i;
	else if (data[i].datatype[1] == 'z') lr = i;
	}
					/* Form all possible ratios */
    if (rl >= 0 && ll >= 0)
	{
	weight = (data[rl].length + data[ll].length) / 2.0;
	amp = weight * data[rl].amp / data[ll].amp;
	phase = (data[rl].resid_phas - data[ll].resid_phas) 
			/ (180.0 / 3.141592654);
					/* real, imaginary and weight */
	ratios[0][0] = amp * cos (phase);
	ratios[0][1] = amp * sin (phase);
	ratios[0][2] = weight;
	}
    if (lr >= 0 && ll >= 0)
	{
	weight = (data[lr].length + data[ll].length) / 2.0;
	amp = weight * data[lr].amp / data[ll].amp;
	phase = (data[lr].resid_phas - data[ll].resid_phas) 
			/ (180.0 / 3.141592654);
					/* real, imaginary and weight */
	ratios[1][0] = amp * cos (phase);
	ratios[1][1] = amp * sin (phase);
	ratios[1][2] = weight;
	}
    if (rl >= 0 && rr >= 0)
	{
	weight = (data[rl].length + data[rr].length) / 2.0;
	amp = weight * data[rl].amp / data[rr].amp;
	phase = (data[rl].resid_phas - data[rr].resid_phas) 
			/ (180.0 / 3.141592654);
					/* real, imaginary and weight */
	ratios[2][0] = amp * cos (phase);
	ratios[2][1] = amp * sin (phase);
	ratios[2][2] = weight;
	}
    if (lr >= 0 && rr >= 0)
	{
	weight = (data[lr].length + data[rr].length) / 2.0;
	amp = weight * data[lr].amp / data[rr].amp;
	phase = (data[lr].resid_phas - data[rr].resid_phas) 
			/ (180.0 / 3.141592654);
					/* real, imaginary and weight */
	ratios[3][0] = amp * cos (phase);
	ratios[3][1] = amp * sin (phase);
	ratios[3][2] = weight;
	}
    if (rr >= 0 && ll >= 0)
	{
	weight = (data[rr].length + data[ll].length) / 2.0;
	amp = weight * data[rr].amp / data[ll].amp;
	phase = (data[rr].resid_phas - data[ll].resid_phas) 
			/ (180.0 / 3.141592654);
					/* real, imaginary and weight */
	ratios[4][0] = amp * cos (phase);
	ratios[4][1] = amp * sin (phase);
	ratios[4][2] = weight;
	}
    }
