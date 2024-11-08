/* Function used for fitting amplitude vs. seg. length data. */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
void ampf(float x,float a[],float *y,float dyda[], int na)
{
  float s1,s2;
  /* set y and dyda here based on functional form      */
  /* a[1]=height of plateau,                           */
  /* a[2]=slope of loss region in log space (negative) */
  /* a[3]=coherence time                               */

  s1=a[1];                      /* Value of amp in Plateau region */
  s2=a[1]+a[2]*log10(x/a[3]);   /* Value of amp in Coherence loss region */

    if (x<a[3])                   /* Derivatives in Plateau region */
    {
      *y=s1;
      dyda[1]=1.0;
      dyda[2]=0.0;
      dyda[3]=0.0;
    }
    if (x>=a[3])                  /* Derivatives in loss region */
    {
      *y=s2;
      dyda[1]=1.0;
      dyda[2]=log10(x/a[3]);
      dyda[3]=-a[2]/(a[3]*log(10.0));
    }
}
