/* FOLD - program to fold together files containing closure */
/* phase and averaged amplitudes for CALTECH input.  Uses   */
/* routines from Numerical Recipies for matrix inversion.   */

#include <stdio.h>  
#include <math.h>
#include <stdlib.h>
#include "adata.h"
#include "nr.h"
#include "nrutil.h"

#define SIZE 100

char progname[8] = "fold";
int msglev = 1;

struct abbrev {char base[4];};

struct abbrev bas[100],cls[100];
fringesum ampscan[100],clscan[100];

main(argc,argv)
int argc;
char *argv[];
{
  FILE *fpinamp,*fpincls,*fpout;
  char linea[250],lineb[250],*testt,*testb;
  char ampname[50],clsname[50];

  int bramp,brcls,i,k,m,j,initflg;
  int scan_time;
  float *temp,*a,*x,*y,*sig,*w; /* components of matrix eq. */
  float **u,**v,chisq,clsph,time;
  fpout=stdout;

  /* set up matrix/vector elements: start with SIZE, expand later */
  a=vector(1,SIZE);
  x=vector(1,SIZE);
  y=vector(1,SIZE);
  sig=vector(1,SIZE);
  w=vector(1,SIZE);
  u=matrix(1,SIZE,1,SIZE);
  v=matrix(1,SIZE,1,SIZE);
  temp=vector(1,SIZE);
  
  /* parse command line */
  
  if (argc<2) {
    printf("\nfold: input format is: fold ampfilename clsfilename\n");
    printf("fold: ampfilename contains averaged amplitudes\n");
    printf("fold: clsfilename contains averaged closure phases\n");
    return;
  }
  else if (argc>4) {
    printf("\ntoo many arguments\n"); return;}

  else {
    sscanf(argv[1],"%s",ampname);
    sscanf(argv[2],"%s",clsname);
    if ((fpinamp=fopen(ampname,"r")) == NULL) {
      printf("error : can't open %s\n",ampname);
      return;
    }
    if ((fpincls=fopen(clsname,"r")) == NULL) {
      printf("error : can't open %s\n",clsname);
      return;
    }
  }

/* Initialize loop variables */
  initflg=1;
  i=0;
  j=0;
  if (brcls=fgets(lineb,250,fpincls) != 0) parse_fsumm(lineb,&clscan[0]);
  else {
    printf("No closure phases in file - setting all phases = 0\n"); 
    return;
  }
  
  /* Read each line */
  do
    {
      if (bramp=fgets(linea,250,fpinamp) != 0) 
        parse_fsumm(linea,&ampscan[i]);
      
      if (initflg == 1)
        {
          initflg = 0;
          scan_time = ampscan[i].scan_time;
        }

      /* Check to see if we're moving to the next scan time */
      if ((abs(ampscan[i].scan_time - scan_time)>=190) || (bramp==0))
        {
          /* Now we start reading from the closure phase      */
          /* file to find corresponding entries for the       */
          /* amps we've got                                   */
	  if (abs(clscan[0].scan_time - scan_time)<190) {
	    do {
              j++;

              if (brcls=fgets(lineb,250,fpincls) !=0) 
                parse_fsumm(lineb,&clscan[j]);

              if ((abs(clscan[j].scan_time - scan_time) >=190) || (brcls==0))
                {
                  /* match up amp and phase together */

                  for (k=0;k<i;k++) { 
                    strcpy(bas[k+1].base,ampscan[k].baseline); 
                    bas[k+1].base[2]='\0';
                  }

                  for (k=0;k<j;k++) { 
                    y[k+1]=clscan[k].resid_phas; 
                    sig[k+1]=1.0/clscan[k].snr; 
                    x[k+1]=1.0*k+1.0; 
                    strncpy(cls[k+1].base,clscan[k].baseline,2); 
                    cls[k+1].base[2]=clscan[k].quality;
                    cls[k+1].base[3]='\0';
                  }
                
                  
                  for (m=1;m<=j;m++) funcs(x[m],a,i);
                  svdfit(x,y,sig,j,a,i,u,v,w,&chisq,funcs);

                  /* test(i,j,a,x); */
                  /* this test routine will show derived baseline */
                  /* phases, triangle closures, and the signs of  */
                  /* baseline phase contributions to closure.     */

                  /* now assign phases to each baseline and output new scans */
                  time = 0.0;
                  for (m=1;m<=i;m++) {
                    ampscan[m-1].resid_phas = a[m];
                    time+=((float) ampscan[m-1].scan_time);
                  }
                  time = time/i;
                  for (m=1;m<=i;m++) {
                    ampscan[m-1].scan_time = (int) time;
                    write_vsumm(ampscan[m-1],fpout);
                  }
                  break;
                }
	    } while (1==1); 
	    if (brcls==0) printf("EOF on closure file reached\n");
	    else { parse_fsumm(lineb,&clscan[0]); j=0;}
          }
          else {
            for (k=0;k<i;k++) {
              ampscan[k].resid_phas = 0.0;
              write_vsumm(ampscan[k],fpout);
              /* printf("\n"); */
            }
          }
          
          i=0; parse_fsumm(linea,&ampscan[i]);
	}
      if (bramp==0) break;
      scan_time=ampscan[i].scan_time;
      i++;
    } while (1==1);
  fclose(fpinamp); fclose(fpincls);
} 



int sign(tri,base)
char *tri,*base;
{
  int comp,i;
  char tbase[3],ttri[5],bbase[3];

  tbase[1] = *base; tbase[0] = *(base+1);tbase[2]='\0';
  bbase[1] = *(base+1); bbase[0] = *base;bbase[2]='\0';
  for (i=0;i<3;i++) ttri[i] = *(tri+i); ttri[3] = ttri[0];ttri[4]='\0';

  comp=0;
  if (strstr(ttri,bbase)!=NULL) comp=1;
  if (strstr(ttri,tbase)!=NULL) comp=-1;
  return(comp);
}

void funcs(x,afunc,ma)
float afunc[],x;
int ma;
{
  int j,k;

  j=x+0.01; /* convert x to int */
  for(k=1;k<=ma;k++) afunc[k]=sign(cls[j].base,bas[k].base);
}


void test(p,q,afunc,x)
int p,q;
float afunc[],x[];
{
  int m,k;
  float *temp,clsph;

  temp=vector(1,SIZE);

  for (m=1;m<=q;m++) { 
    printf("For the Tri : %s the baseline contributions are :\n"
           ,cls[m].base);
    for (k=1;k<=p;k++) 
      printf("Base : %s     Contribution : %d\n"
             ,bas[k].base,sign(cls[m].base,bas[k].base));
  }
  
  /* now test the output of svdfit by adding closure */
  for (m=1;m<=p;m++)
    printf("Baseline #%d,  phase = %.2f,  stat = %s\n",
           m,afunc[m],bas[m].base);
  
  for (m=1;m<=q;m++){funcs(x[m],temp,p);
                     clsph=0;
                     for (k=1;k<=p;k++) clsph+=temp[k]*afunc[k];
                     printf("Closure on Tri %s is %.2f\n",
                            cls[m].base,clsph);
                   }
  free_vector(temp,1,SIZE);
}
              

