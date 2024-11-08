#include "nrutil.h"

#include <stdio.h>

/* returns -2 on error, apparently */

int mrqfix(float x[], float y[], float sig[], int ndata, float a[],
    int ia[], int ma, float **covar, float **alpha, float *chisq,
    void (*funcs)(float, float [], float *, float [], int),
    float *alamda)
{
  // void covsrt(),mrqcof(),svbksb();
  // int svdcmpp();
  extern void covsrt(float **covar, int ma, int ia[], int mfit);
  extern void mrqcof(float x[], float y[], float sig[], int ndata, float a[], int ia[], int ma, float **alpha, float beta[], float *chisq, void (*funcs)(float, float [], float *, float [], int));
  extern void svbksb(float **u, float w[], float **v, int m, int n, float b[], float x[]);
  extern int svdcmpp(float **a, int m, int n, float w[], float **v);

  int j,k,l,m,decomp;
  float wmax,wmin,**u,**v,*w,*tempin,*tempsol;
  static int mfit;
  static float ochisq,*atry,*beta,*da,**oneda;
  
  if (*alamda < 0.0) {
    atry=vector(1,ma);
    beta=vector(1,ma);
    da=vector(1,ma);
    for (mfit=0,j=1;j<=ma;j++)
      if (ia[j]) mfit++;
    oneda=matrix(1,mfit,1,1);
    *alamda=0.001;
    mrqcof(x,y,sig,ndata,a,ia,ma,alpha,beta,chisq,funcs);
    ochisq=(*chisq);
    for (j=1;j<=ma;j++) atry[j]=a[j];
  }
  for (j=0,l=1;l<=ma;l++) {
    if (ia[l]) {
      for (j++,k=0,m=1;m<=ma;m++) {
        if (ia[m]) {
          k++;
          covar[j][k]=alpha[j][k];
        }
      }
      covar[j][j]=alpha[j][j]*(1.0+(*alamda));
      oneda[j][1]=beta[j];
    }
  }
  /* for (j=1;j<=ma;j++){printf("covar[%d][%d]=%e\n",j,j,covar[j][j]);} */

 /*  New linear solving routine based on SVD */
 /*printf("right before matrix and vector setting\n"); */ 
	u=matrix(1,mfit,1,mfit);
	v=matrix(1,mfit,1,mfit);
	w=vector(1,mfit);
        tempin=vector(1,mfit);
        tempsol=vector(1,mfit);
/*printf("right before covar copying\n"); */

 /*  Copy covar into u matrix */

	for (j=1;j<=mfit;j++) {
		for(k=1;k<=mfit;k++) u[j][k]=covar[j][k];
		}
/* printf("right before decompose\n"); */

/*   SV Decompose the covar matrix */
/*   If unsuccessful then return -2 */
        decomp = 0;
	decomp=svdcmpp(u,mfit,mfit,w,v);
        if (decomp == -2) {printf("returning -2 inside mrqfix\n"); return(-2); }
/* printf("right before singular stuff\n"); */
/*   Take care of any singular values */

	wmax=0.0;
  	for(j=1;j<=mfit;j++) if (w[j] > wmax) wmax=w[j];
	wmin=wmax*1.0e-6;
	for(j=1;j<=mfit;j++) if (w[j] < wmin) w[j]=0.0;
	
/*    Now backsubstitute getting solution vector and inverse matrix */
/* printf("right before backsub"); */
	for(j=1;j<=mfit;j++) tempin[j]=oneda[j][1];
 	svbksb(u,w,v,mfit,mfit,tempin,tempsol);
/* printf("right after first backsub\n"); */

	for(j=1;j<=mfit;j++) oneda[j][1]=tempsol[j];

	for(j=1;j<=mfit;j++) {
		tempin[1]=0.0;tempin[2]=0.0;tempin[3]=0.0;tempin[j]=1.0;
		svbksb(u,w,v,mfit,mfit,tempin,tempsol);
		for(k=1;k<=mfit;k++) covar[k][j]=tempsol[k];
	}

	free_matrix(u,1,mfit,1,mfit);
	free_matrix(v,1,mfit,1,mfit);
	free_vector(w,1,mfit);
	free_vector(tempsol,1,mfit);
 	free_vector(tempin,1,mfit); 

  for (j=1;j<=mfit;j++) da[j]=oneda[j][1];
  if (*alamda == 0.0) {
    covsrt(covar,ma,ia,mfit);
    free_matrix(oneda,1,mfit,1,1);
    free_vector(da,1,ma);
    free_vector(beta,1,ma);
    free_vector(atry,1,ma);
    return(0);	/* presuming this is a valid return */
  }
  for (j=0,l=1;l<=ma;l++)
    if (ia[l]) atry[l]=a[l]+da[++j];
  
  if (atry[3]>=0.0)
    {
      mrqcof(x,y,sig,ndata,atry,ia,ma,covar,da,chisq,funcs);
      if (*chisq < ochisq) {
        *alamda *= 0.1;
        ochisq=(*chisq);
        for (j=0,l=1;l<=ma;l++) {
          if (ia[l]) {
            for (j++,k=0,m=1;m<=ma;m++) {
              if (ia[m]) {
                k++;
                alpha[j][k]=covar[j][k];
              }
            }
            beta[j]=da[j];
            a[l]=atry[l];
          }
        }
      } else {
        *alamda *= 10.0;
        *chisq=ochisq;
      }
  } else {
    *alamda *= 10.0;
    *chisq=ochisq;
  }
return(0);
}
/* (C) Copr. 1986-92 Numerical Recipes Software *"<,. */
