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
          
          do
            {
              if (brcls=fgets(lineb,250,fpincls) != 0) parse_fsumm(lineb,&clscan[j]);
              
              if ((abs(clscan[j].scan_time - scan_time)>=190) || (brcls==0))
                {
                  /* now we have all cls and amp scans for this time */
                  /* and we set up matrix and invert                 */

                  for (k=0;k<i;k++) {ampst[k].flag = '-'; ampst[k].ph=-1.0;
				     ampst[k].base[0]=ampscan[k].baseline[0];
				     ampst[k].base[1]=ampscan[k].baseline[1];}
                  for (k=0;k<j;k++) {clstr[k].flag='*'; 
				     clstr[k].base[0]=clscan[k].baseline[0];
				     clstr[k].base[1]=clscan[k].baseline[1];
				     clstr[k].base[2]=clscan[k].quality;}
                  
                  /* 1st we dis-include amp scans which are not represented */
                  /* in the closure triangles                               */
                  q=0;
                  for (k=0;k<i;k++){
                    found=0;
                    for (z=0;z<j;z++) {
                      if (strchr(clstr[z].base,ampscan[k].baseline[0]) != NULL){
                        if (strchr(clstr[z].base,ampscan[k].baseline[1]) != NULL) found++;
                      } 
                    }
                    if (found>0) {q++; ampst[k].flag='*';}
                    ampst[k].numt=found;
                  } /* end of search for baselines among closure phase triangles */
                
                  
		  /* if too many clsph then erase j-q+2 of them making sure not to */
		  /* erase one that isn't redundant.                               */

		  if (j>q-2) {
                    k=0;  /* initialize triangle index */
                    erased=0;
		    do {
                      leave=0;
		      for (z=0;z<3;z++) b[z]=-1; 
                      m=0;
                      for (p=0;p<i;p++) { /* look through baselines */
                        if (ampst[p].flag=='*'){
                          for (z=0;z<3;z++){
                            if (z!=2) {base[0]=clstr[k].base[z];base[1]=clstr[k].base[z+1];}
                            else {base[0]=clstr[k].base[2];base[1]=clstr[k].base[0];}
                            if (twostat(base,ampst[p].base)!=0)
                              {b[m]=p; m++;
                               if (ampst[p].numt==1) leave =1;}
                          }
                        }
                      }
                      if (leave!=1) {clstr[k].flag='-'; erased++;
				     for (z=0;z<3;z++) if (b[z] != -1) ampst[b[z]].numt--;}
                      k++;
                    } while (erased<(j-q+2));
		  }
                  
                  if (j<q-2) {
                    


                  }
                  
		  /* set phases on last two valid amps = 0 */

		  z=2;k=i-1;
		  while (z>0){
		    if (ampst[k].flag=='*') {ampst[k].flag='-'; ampst[k].ph=0.0;z--;}
		    k--;
		  }
                  
		  /* fill matrix with closure coefficients. It should have */
		  /* size : [q-2][q-2]                                    */
		  row=0;
		  for (k=0;k<j;k++){
		    if (clstr[k].flag=='*')
                      {row++; 
                       cvect[row]=clscan[k].resid_phas; /* load clsph vector */
                       for(z=0;z<3;z++){
                         if (z!=2) {base[0]=clstr[k].base[z];base[1]=clstr[k].base[z+1];}
                         else {base[0]=clstr[k].base[2];base[1]=clstr[k].base[0];}
                         col=0;
                         for (p=0;p<i;p++){
                           if (ampst[p].flag=='*') 
                             {col++;
                              if (ma=twostat(base,ampst[p].base)!=0) mat[row][col]=ma;}
                         }
                       }
                     }
		  }

                  /* test so far by printing out vect and mat */
                  for (z=1;z>q-2;z++){
                    printf("%f     ",cvect[z]);
                    for (p=1;p>q-2;p++) printf("%f ",mat[z][p]);
                    printf("\n");
                  }

                  /* move current lineb into clscan[0] */
                  parse_fsumm(lineb,&clscan[0]); j=1;
                  break;
                }
              j++;
            } while (1==1);
          
          parse_fsumm(linea,&ampscan[0]); i=0;

        }
      if (bramp==0) break;  /* if EOF then break out of do loop */ 
      scan_time=ampscan[i].scan_time;
      i++;
      
    } while (1==1);
  fclose(fpinamp); fclose(fpincls);
}

/* twostat : compares two baselines and returns +/- 1 if both   */
/* match where sign signifies order.  returns zero if no match. */

int twostat(base1,base2)
char *base1,*base2;
{
  int comp;
  
  if ((*base1 == *base2) && (*(base1+1) == *(base2+1)))
    comp = 1;
  else if ((*base1 == *(base2+1)) && (*(base1+1) == *base2))
    comp = -1;
  else comp = 0;

  return(comp);
}











