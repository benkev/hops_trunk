#include "nrutil.h"

void svdvar(v,ma,w,cvm)
float **cvm,**v,w[];
int ma;
{
	int k,j,i;
	float sum,*wti;

	wti=vector(1,ma);
	for (i=1;i<=ma;i++) {
		wti[i]=0.0;
		if (w[i]) wti[i]=1.0/(w[i]*w[i]);
	}
	for (i=1;i<=ma;i++) {
		for (j=1;j<=i;j++) {
			for (sum=0.0,k=1;k<=ma;k++) sum += v[i][k]*v[j][k]*wti[k];
			cvm[j][i]=cvm[i][j]=sum;
		}
	}
	free_vector(wti,1,ma);
}
/* (C) Copr. 1986-92 Numerical Recipes Software *1(."<,. */
