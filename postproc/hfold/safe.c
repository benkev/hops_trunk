/* FOLD - program to fold together files containing closure */
/* phase and averaged amplitudes for CALTECH input.  Uses   */
/* routines from Numerical Recipies for matrix inversion.   */

#include <stdio.h>  
#include <math.h>
#include <stdlib.h>
#include "adata.h"
#include "nr.h"
#include "nrutil.h"

char progname[8] = "fold";
int msglev = 1;

  /* Structure to hold sorting information.*/

struct abbrev {
  char flag;            /* flags scan for matrix inclusion         */
  char base[3];         /* baseline                                */
  int index;            /* index refering back to actual scan #    */
  float ph;             /* phase                                   */
  int numt;             /* number of triangles baseline a part of  */
};

fringesum ampscan[22],clscan[22];
struct abbrev ampst[22],clstr[22];

main(argc,argv)
int argc;
char *argv[];
{
  FILE *fpinamp,*fpincls,*fpout;
  char linea[250],lineb[250];
  char ampname[50],clsname[50],mode,base[3];

  int bramp,brcls,i,k,m,j,initflg,q;
  int index,row,col,p,found,b[3];
  int scan_time,z,ma,leave,erased;
  float min;
  float **mat,**imat,*bvect,*cvect,*tvect; /* components of matrix eq. */


  base[2]='\0';
  fpout=stdout;

  mat=matrix(1,40,1,40);       /* matrix with closure phase constraints */
  imat = matrix(1,40,1,40);    /* inverse of mat */
  bvect=vector(1,40);          /* vector representing unknown basln phases */
  cvect=vector(1,40);          /* vector containing known clsphs */
  tvect=vector(1,40);          /* generic temporary vector */

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
  else {printf("No closure phases in file - setting all phases = 0\n"); return;}
  
  
  /* Read each line */
  
  do
    {
      if (bramp=fgets(linea,250,fpinamp) != 0) parse_fsumm(linea,&ampscan[i]);
      
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
              if (brcls=fgets(lineb,250,fpincls) !=0) parse_fsumm(lineb,&clscan[j]);
              if ((abs(clscan[j].scan_time - scan_time) >=190) || (brcls==0)){
                /* match up amp and phase together */
                printf("Amp Scans :\n");	
                for (k=0;k<i;k++) write_fsumm(ampscan[k],fpout);
                printf("Closure Scans :\n");
                for (k=0;k<j;k++) write_fsumm(clscan[k],fpout);
                break;
              }
	    } while (1==1); 
	    if (brcls==0) printf("EOF on closure file reached\n");
	    else { parse_fsumm(lineb,&clscan[0]); j=0;}
          }
          else {printf("No closure triangles this scan.  Setting phases = 0.\n");
		for (k=0;k<i;k++) write_fsumm(ampscan[k],fpout);}
          i=0; parse_fsumm(linea,&ampscan[i]);
	}
      if (bramp==0) break;
      scan_time=ampscan[i].scan_time;
      i++;
    } while (1==1);
  fclose(fpinamp); fclose(fpincls);
} 



