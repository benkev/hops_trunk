#include "nrutil.h"

void mrqcof(x,y,sig,ndata,a,ia,ma,alpha,beta,chisq,funcs)
double **alpha,*chisq,a[],beta[],sig[],x[],y[];
int ia[],ma,ndata;
void (*funcs)();
{
	int i,j,k,l,m,mfit=0;
	double ymod,wt,sig2i,dy,*dyda;

	dyda=dvector(1,ma);
	for (j=1;j<=ma;j++)
		if (ia[j]) mfit++;
	for (j=1;j<=mfit;j++) {
		for (k=1;k<=j;k++) alpha[j][k]=0.0;
		beta[j]=0.0;
	}
	*chisq=0.0;
	for (i=1;i<=ndata;i++) {
		(*funcs)(x[i],a,&ymod,dyda,ma);
		sig2i=1.0/(sig[i]*sig[i]);
		dy=y[i]-ymod;
		for (j=0,l=1;l<=ma;l++) {
			if (ia[l]) {
				wt=dyda[l]*sig2i;
				for (j++,k=0,m=1;m<=l;m++)
					if (ia[m]) alpha[j][++k] += wt*dyda[m];
				beta[j] += dy*wt;
			}
		}
		*chisq += dy*dy*sig2i;
	}
	for (j=2;j<=mfit;j++)
		for (k=1;k<j;k++) alpha[k][j]=alpha[j][k];
	free_dvector(dyda,1,ma);
}
/* (C) Copr. 1986-92 Numerical Recipes Software *"<,. */
