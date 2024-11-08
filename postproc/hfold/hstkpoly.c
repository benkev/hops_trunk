/* FOLD - program to fold together files containing closure */
/* phase and averaged amplitudes for CALTECH input.  Uses   */
/* routines from Numerical Recipies for matrix inversion.   */

#include <stdio.h>  
#include <math.h>
#include <stdlib.h>
#include "nr.h"
#include "nrutil.h"

#define SIZE 100
/* #define NPOLY 6 */

void funcs(x,afunc,ma)
     float afunc[],x;
     int ma;
{
  int k;
  
  /* printf("inside the funcs function\n"); */
  afunc[1]=1.0;
  /* printf("after the first dec\n"); */
  for(k=2;k<=ma;k++) {afunc[k]=afunc[k-1]*(x-51.0);}
}

main(argc,argv)
int argc;
char *argv[];
{
  FILE *fpin;
  char line[250];
  char datname[50];

  int br,i,ndat,m,NPOLY;
  float *temp,*a,*x,*y,*sig,*w; /* components of matrix eq. */
  float **u,**v,chisq,el,gain;

  /* set up matrix/vector elements: start with SIZE, expand later */
  a=vector(1,SIZE);
  x=vector(1,SIZE);
  y=vector(1,SIZE);
  sig=vector(1,SIZE);
  w=vector(1,SIZE);
  u=matrix(1,SIZE,1,SIZE);
  v=matrix(1,SIZE,1,SIZE);
  temp=vector(1,SIZE);
  NPOLY = 6;

  if (argc < 2) {
    printf("\ngain: input format is: gain datafile\n");
    printf("gain: datafile contains gain vs elevation points\n");
    return;
  }
  else {
    sscanf(argv[1],"%s",datname);
    if ((fpin=fopen(datname,"r")) == NULL) {
      printf("error : can't open %s\n",datname);
      return;
    }
    if (argc==3) sscanf(argv[2],"%d",&NPOLY);
  }

  i=0;
  while (br=fgets(line,250,fpin) !=0)
    {
      i++;
      
      sscanf(line,"%f %f",&el,&gain);
      y[i]=gain; 
      sig[i]=0.1; 
      x[i]=el;
      /* printf("%.2f\t%.2f\n",x[i],y[i]); */
    }
  ndat = i;
  /* funcs(5,a,NPOLY); */
  for (m=1;m<=1;m++) funcs(x[m],a,NPOLY);
  for (i=1;i<=NPOLY;i++) printf("basis %d at x=5 is %.2f\n",i,a[i]);
  svdfit(x,y,sig,ndat,a,NPOLY,u,v,w,&chisq,funcs);
  printf("Poly fit of degree %d :\n",NPOLY);
  for (i=1;i<=NPOLY;i++) printf("Coeff %d is %.8e\n",i,a[i]);
  printf("Chi-Squared is %.2f\n",chisq);
}




