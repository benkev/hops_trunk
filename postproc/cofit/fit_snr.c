/************************************************************************/
/*									*/
/* Fits a theoretical curve of snr versus segmentation time		*/
/*									*/
/*	Inputs:		codatum		structure of scan  data		*/
/*			npt		number of array elements	*/
/*									*/
/*	Output:		codatum		struct. filled in with snr fit	*/
/*			return value	seglen of function peak		*/
/*									*/
/* Created October 6 1995 by CJL/SSD					*/
/*									*/
/************************************************************************/
#include "cofit.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int
fit_snr(struct cosummary *codatum, int npt)
    {
					/* Temporary routine that fits parabola */
					/* to highest three points and interpolates */

	float loglen[100];
	int i,k;
	float peak,time,logtime;
	float r,s,t,a,b,c,x,y,z;

					/* First find the peak in snr */
					/* Convert to log for time axis since */
					/* we expect parabola in log-lin plot */

	peak=-1.0;
	for(i=0;i<npt;i++) {
			    loglen[i]=log10(codatum->seglen[i]);				
/* printf("seglen=%.2f, loglen=%.2f, snr = %.2f\n",
		(codatum->seglen[i]),loglen[i],(codatum->snr[i])); */

			    if ((codatum->snr[i])>peak) 
				{
				  peak=(codatum->snr[i]);
				  k = i;
				}
			   }

					/* Assign variables to coordinates of highest */
					/* three points.                              */

	a=loglen[k]; z= (codatum->snr[k]);

	if (k==0) {
            b=loglen[k+1]; y = (codatum->snr[k+1]);
	    c=loglen[k+2]; x = (codatum->snr[k+2]);
            codatum->snr_peak[0] = 0;
            codatum->snr_peak[1] = 1;
            codatum->snr_peak[2] = 2;
        }

	if (k==(npt-1)) {
            b=loglen[k-1]; y = (codatum->snr[k-1]);
            c=loglen[k-2]; x = (codatum->snr[k-2]);
            codatum->snr_peak[0] = npt-3;
            codatum->snr_peak[1] = npt-2;
            codatum->snr_peak[2] = npt-1;
        }

	if ((k>0)&&(k<(npt-1))) {
            b=loglen[k-1]; y = (codatum->snr[k-1]);
            c=loglen[k+1]; x = (codatum->snr[k+1]);
            codatum->snr_peak[0] = k-1;
            codatum->snr_peak[1] = k;
            codatum->snr_peak[2] = k+1;
        }

					/* Form of parabola is y=rx^2 + sx + t */

	r = (a*(x-y) +b*(z-x) + c*(y-z))/(a*a*(b-c)+b*b*(c-a)+c*c*(a-b));
	s = ((y-x) - r*(b*b-c*c))/(b-c);
	t = z - r*a*a - s*a;

	/* printf("r=%.2f s=%.2f t=%.2f\n",r,s,t); */

					/* Peak is at x = -s/(2r) */

	logtime = -s/(2*r);
	time = floor(pow(10.0,logtime));

					/* Is this value reasonable? */
					/* Sanity checks.            */
					/* Look for concave up parabola, time */
					/* too small or too big.	*/

	if ((r>=0.0)||(time<1)||(time>2*(codatum->seglen[npt-1]))) return(-1);
	
	for(i=0;i<npt;i++)
		codatum->fitsnr[i] = r*pow(loglen[i],2.0) + s*loglen[i] + t;
	 
	return((int) time);
    }      
