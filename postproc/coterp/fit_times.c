/************************************************************************/
/*									*/
/* 	Decomposes baseline based coherence times into stationized	*/
/*	quantities.							*/ 
/*									*/
/*	Inputs:		s		structure holding stations,	*/
/*					baselines, baseline based  	*/
/*					coherence times.		*/
/*									*/
/*	Output:		s		same structure but with station	*/
/*					based coherence times filled in.*/
/*									*/
/*			return value	0 if fit ok, -# if otherwise.	*/
/*									*/
/* Created Feb 10 1996 by SSD.						*/
/************************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "nrutil.h"
#include "coterp.h"

typedef struct { char sta; } stch;
typedef struct { char bas[3]; } bach;

stch station[100];
bach base[100];

int
fit_times (s)
scansum *s;
{
  
  int low,i,k,m,*ia,itst,loop,nb,ns,check,fitflag;
  int year, day, hour, min, sec;
			/* components of matrix eq. */
  double *ay,*az,*x,*y,*z,*sig;
  double **alpha,**covar,chisq,ochisq,alamda,temp,olamda;
  void funcs();
  void test();

  low=0;  
  nb=s->nb; 
			/* if only one baseline and two stations */
                        /* we need to add dummy station to allow */
                        /* fitting routine to converge.  So we add */
                        /* station "." which is never seen by routine */
                        /* that calls mrq_times.                */

  ns=s->nstat; if (ns==2) { ns+=1;low=1;}
  check=0;
			/* print out scan-time		*/

  /* int_to_time(s->scantime,&year,&day,&hour,&min,&sec); */
  /*printf("Working on scan %2d %03d-%02d%02d%02d\n",year, */
  /*                      day,hour,min,sec); */

			/* set up matrix/vector elements: */
  ay=dvector(1,ns);
  az=dvector(1,ns);
  ia=ivector(1,ns); 
  x=dvector(1,nb);
  y=dvector(1,nb);
  z=dvector(1,nb); 
  sig=dvector(1,nb);
  covar=dmatrix(1,ns,1,ns);
  alpha=dmatrix(1,ns,1,ns);

			/* convert inputs to vectors with new indicies */  

  for (i=1;i<=ns;i++) {
	if ((low==1)&&(i==ns)) station[i].sta='.';
	else station[i].sta = s->station[i-1].stn_id;
	}

  for (i=1;i<=nb;i++)
    {
      if (s->basetimes[i-1].srch_cotime == 0) y[i]=1.0;
      else y[i]=1.0/s->basetimes[i-1].srch_cotime;

      if (s->basetimes[i-1].noloss_cotime == 0) z[i]=1.0;
      else z[i]=1.0/s->basetimes[i-1].noloss_cotime;
      sig[i]=0.005;
      x[i]=1.0*i;
      strcpy(base[i].bas,s->basetimes[i-1].baseline);
    }
  
  for (i=1;i<=ns;i++) 
    {
      ia[i]=1;		/* Fit all stations */
      ay[i]=0.15;	/* First cut at first guess */
      az[i]=0.15;
    }

   /* test(ns,nb,ay,az,x,y,z); */
  
                        /* Start fitting iterations with mrqmin */
                        /* Loop twice - for nl and search cotimes. */

  for (loop=0;loop<=1;loop++)
    { 
      /* printf("In loop %d\n",loop); */
      k=1;    
      
                        /* do initialization call of mrqmin with alamda<0 */
      alamda=-1;
      if (loop==0)
        check=mrqfix(x,y,sig,nb,ay,ia,ns,covar,alpha,&chisq,funcs,&alamda);
      else 
        check=mrqfix(x,z,sig,nb,az,ia,ns,covar,alpha,&chisq,funcs,&alamda);
      if (check == -2) return(-2);
      
      itst=0;
      for (;;) 
        {
          olamda=alamda;
          ochisq=chisq;
          if (loop==0)
            check=mrqfix(x,y,sig,nb,ay,ia,ns,covar,alpha,&chisq,funcs,&alamda);
          else 
            check=mrqfix(x,z,sig,nb,az,ia,ns,covar,alpha,&chisq,funcs,&alamda);
          if (check == -2) return(-2);
          k++;    
                        /* compare present and old chisq */ 
          if (chisq>ochisq) 
            {
              /* printf("chisq went UP from %.2le to %.2le  ",ochisq,chisq); */
              itst=0;
            }
          
      			/* if chisq isn't changing because the search grid */
			/* is being made coarser (lamda) in the first 5 */
      			/* iterations, then reset itst.   */

          if ((chisq==ochisq)&&(olamda<=alamda)&&(k<5)) 
            {
              /* printf("chisq unchanged, lambda bigger  "); */
              itst=0;
            }
          
                        /* otherwise, if fractional change in chisq < 10%, */
			/* increment counter. */

          else if (fabs(chisq-ochisq)/chisq<0.1) 
            {
              /* printf("chisq went DOWN from %.2e to %.2e  ",ochisq,chisq);  */
              itst++;
            }

      			/* If test counter is > 4 fit has been found so we */
			/* leave fitting loop. */
          if (chisq <= 1e-10) {
		/* printf("chisq below limit\n"); */
		fitflag=0;break;
				}
          if (itst<4) 
            {
              /* printf("itst=%d,  chisq=%.2e\n",itst,chisq);  */
              continue;
            }
          
      			/* If number of iterations is >20 or chisq is */ 
			/* unacceptable, return bad fit */

          if (k>20) 
            { 
              /* printf("Breaking out of iter loop :k=%d,  chisq=%.2lf\n",k,chisq); */
              fitflag=-1; break;
            }
          
      			/* start ending procedure - final call to mrqmin */
			/* with alamda=0.0 */
          alamda=0.0;
          if (loop==0)
            check=mrqfix(x,y,sig,nb,ay,ia,ns,covar,alpha,&chisq,funcs,&alamda);
          else 
            check=mrqfix(x,z,sig,nb,az,ia,ns,covar,alpha,&chisq,funcs,&alamda);
          if (check == -2) return(-2);
          
          fitflag=0;
          break;        
        }         		
                        /* Leave Iteration Loop */
      
    } 
                        /* Assign station based cotimes - remember to   */
                        /* use inverse since times found are inverted   */

  for (i=1;i<=ns;i++)
    {
      if ((low==1)&&(i==ns)) continue;
      s->station[i-1].srch_cotime = 1.0/ay[i];
      s->station[i-1].noloss_cotime = 1.0/az[i];
    } 
  
   /* test(ns,nb,ay,az,x,y,z); */
  return(fitflag);
                        /* Free vector/matrix memory */
  free_dvector(ay,1,ns);
  free_dvector(az,1,ns);
  free_ivector(ia,1,ns);
  free_dvector(x,1,nb);
  free_dvector(y,1,nb);
  free_dvector(z,1,nb);
  free_dvector(sig,1,nb);
  free_dmatrix(covar,1,ns,1,ns);
  free_dmatrix(alpha,1,ns,1,ns);
}

  
void test(p,q,ay,az,x,y,z)
     double ay[],az[],x[],y[],z[];
     int p,q;
{
  int m,k,comp;
  double *temp,nltime,stime;
  void funcs();
  
  temp=dvector(1,p);
  
  for (m=1;m<=q;m++) 
    {
      printf("For the Base: %s station contributions are :\n"
             ,base[m].bas);
      for (k=1;k<=p;k++)
        {
          if (strchr(base[m].bas,station[k].sta)!=NULL) 
            {
              printf("Station : %c\n",station[k].sta);
            }
        }
    } 
  /* now test the output of mrqfix by adding closure */
  for (m=1;m<=p;m++)
    printf("Stat #%d, s_cot = %.2e, nl_cot = %.2e, stat = %c\n",
           m,1.0/ay[m],1.0/az[m],station[m].sta);
  
  for (m=1;m<=q;m++)
    {
      funcs(x[m],ay,&stime,temp,p); stime=1.0/stime;
      funcs(x[m],az,&nltime,temp,p); nltime=1.0/nltime;
      printf("Base %s : S_cot: new = %.2e orig = %.2e  NL_cot: new = %.2e orig=%.2e\n",
             base[m].bas,stime,1.0/y[m],nltime,1.0/z[m]);
    }
  free_dvector(temp,1,p);
}

void funcs(x,a,y,dyda,na)
     double *y,dyda[],a[],x;
     int na;
{
  int j,k,*stats;
  double num;
  stats=ivector(1,na);
  num=0.0; 
  j=x+0.01; /* convert x to int */
  for(k=1;k<=na;k++)
    {
      if (strchr(base[j].bas,station[k].sta)!=NULL) 
        {
          stats[k]=1;
          num+=a[k]*a[k];
        }
      else stats[k]=0;
    } 
  *y=sqrt(num); 
  for(k=1;k<=na;k++)
    {
      if (stats[k]==1) dyda[k]=2*a[k]/sqrt(num);
      else dyda[k]=0.0;
    }
  free_ivector(stats,1,na);
}


