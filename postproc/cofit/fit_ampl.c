/************************************************************************/
/*									*/
/* Fits a theoretical curve of amplitude versus segmentation time	*/
/*									*/
/*	Inputs:		codatum		structure of scan data		*/
/*			npt		number of array elements	*/
/*									*/
/*	Output:		codatum		struct. with filled in amp. fit	*/
/*			return value	0=OK, -1=noconvergence		*/
/*					-2=Num. Rec. Error		*/
/* Created October 6 1995 by CJL/SSD					*/
/* Completed Feb 10 1996 by SSD.					*/
/************************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "nr.h"
#include "nrutil.h"
#include "cofit.h"

int fit_ampl (struct cosummary *codatum, int npt)
{
  
  int br,i,k,m,j,*ia,itst,npar,fitflag,check;
  float *a,*aamp,*sseglen,*sig; 	/* components of matrix eq. */
  float **alpha,**covar,chisq,ochisq,alamda,temp,*deriv,olamda;
  /* void ampf(); */
  npar=3;

  					/* set up matrix/vector elements: */
  deriv=vector(1,npt);
  a=vector(1,npar);
  ia=ivector(1,npar);  
  aamp=vector(1,npt); 
  sseglen=vector(1,npt);
  sig=vector(1,npt);
  covar=matrix(1,npar,1,npar);
  alpha=matrix(1,npar,1,npar);

  					/* convert inputs to vectors with new indicies */  
  for (i=0;i<npt;i++)
    {
      aamp[i+1]=codatum->ampl[i];
      sseglen[i+1]=codatum->seglen[i];
      sig[i+1]=codatum->ampl[i]/codatum->snr[i];
      printf("index=%d  seglen=%4g  amp=%f  snr=%f\n",i,codatum->seglen[i], codatum->ampl[i],codatum->snr[i]);
    }

					/* first guesses of parameters */
  a[1]=aamp[1];
  a[2]=(aamp[npt]-aamp[1])/(log10(sseglen[npt]/sseglen[1]));

  /* printf("aamp[1]=%.2f  aamp[npt]=%.2f  sseglen[npt]=%.2f  sseglen[1]=%.2f\n",
	aamp[1],aamp[npt],sseglen[npt],sseglen[1]); */

  a[3]=20.0;   
  /* printf("Guesses :  Pl=%.2f\tSlope=%.2f\tTcoh=%.2f\n",a[1],a[2],a[3]); */

  ia[1]=1;ia[2]=1; ia[3]=1;		/* fit all parameters */
  
  /* for (k=1;k<=i;k++) { printf("%.2f  %.2f  %.2f\n",sseglen[k],aamp[k],sig[k]);} */
  /* printf("Number of points = %d\tNumber of parameters = %d\n",i,npar); */ 
 
  k=1;					/* initialize iteration counter */
  
  					/* do initialization call of mrqmin with alamda<0 */
  alamda=-1;
  check=mrqfix(sseglen,aamp,sig,i,a,ia,npar,covar,alpha,&chisq,ampf,&alamda);
  if (check == -2) return(-2);
  
  					/* Start fitting iterations with mrqmin */
  itst=0;
  for (;;) 
    {
      olamda=alamda;
      ochisq=chisq;
      check=mrqfix(sseglen,aamp,sig,i,a,ia,npar,covar,alpha,&chisq,ampf,&alamda);
      if (check == -2) return(-2);

      /* printf("Iter# %d: Pl=%.3f Sl=%.3f Tcoh=%.3f"
        " ChiSq=%.3f Ala=%e\n", k,a[1],a[2],a[3],chisq,alamda); */

      k++;     
      if (chisq>ochisq) itst=0;		/* compare present and old chisq */

      					/* if chisq isn't changing because the search */
					/* grid is being made coarser (lamda) in the */
      					/* first 5 iterations, then reset itst.   */

      if ((chisq==ochisq)&&(olamda<=alamda)&&(k<5)) itst=0;

      					/* otherwise, if fractional change in */ 
					/* chisq < 10%, increment counter. */

      else if (fabs(chisq-ochisq)/chisq<0.1) itst++;

      					/* If test counter is > 4 fit has been */
					/* found so we leave fitting loop. */

      if (itst<4) 
	{
	  /* printf("itst=%d\n",itst); */
	  continue;
	}

      					/* If number of iterations is >20 or chisq */
					/* is unacceptable, return bad fit */

      if ((k>20)||(chisq>25 && !codatum->ign_chisq))
        {
            codatum->iterations = k;
            codatum->chisq = chisq;
            fitflag=-1; break;
        }

      					/* start ending procedure - final call to */
					/* mrqmin with alamda=0.0 */
      alamda=0.0;
      /* printf("Setting alambda=0.0 for final call to mrqmin\n"); */
      check=mrqfix(sseglen,aamp,sig,i,a,ia,npar,covar,alpha,&chisq,ampf,&alamda);
      if (check == -2) return(-2);

       printf("Iter# %d:   Pl =%+.3f   Slp =%+.3f   Tcoh =%+.3f\n",
	k,a[1],a[2],a[3]);
       printf("Iter# %d: s(Pl)=%+.3f s(Slp)=%+.3f s(Tcoh)=%+.3f\n",
        k,sqrt(covar[1][1]),sqrt(covar[2][2]),sqrt(covar[3][3]));  
       printf("Iter# %d: ChiSq=%.2e Alamda=%.2e\n",
        k, chisq,alamda); 

      fitflag=0;
      break;        
    }         				/* Leave Iteration Loop */

  
  					/* Prepare Output : fill in codatum structure */
					/*  and return fit/nofit flag */
  codatum->plateau = a[1];
  codatum->slope  = a[2];
  codatum->breakpoint = a[3];

  /* printf("Testing nrerror\n"); */
  /* nrerror("this is just a test"); */

  // return(fitflag);

					  /* Free vector/matrix memory */
  free_vector(deriv,1,npt);
  free_vector(a,1,npar);
  free_ivector(ia,1,npar);
  free_vector(aamp,1,npt);
  free_vector(sseglen,1,npt);
  free_vector(sig,1,npt);
  free_matrix(covar,1,npar,1,npar);
  free_matrix(alpha,1,npar,1,npar);
  return(fitflag);
}




