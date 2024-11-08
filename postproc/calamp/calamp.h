#ifndef CALAMP_H
#define CALAMP_H

/* calamp.h 
 * Revised:  1996 October 23, JAB 
 * Changed MAXRUNS from 1024 to 5000 Nov 21, 1997, DAB
 */ 

#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <math.h> /* Add -lm to the cc line */ 

#define  MAXRUNS   5000 /* Maximum number of cal lines (each station) */ 
#define  MAXSTATIONS 12 /* Maximum number of stations */ 
#define  MAXPARMS     5 /* Maximum parameters in gain curve */ 
#define  MAXNAME     32 /* Maximum length of a source name */ 
#define  GK -8.4313e-3  /* -(4*pi/lambda)^2 for 86 GHz and lambda 
                         * in mils only (!) 
                         */ 
#define  CALTIME    600 /* Maximum time offset to cal, seconds, default */ 

#include "adata.h"      /* Colin's */  

#define TRUE  1 
#define FALSE 0 


struct Dal { /* To contain all the cal information */ 
  int ctime[MAXSTATIONS][MAXRUNS];     /* UT in hops format */ 
  double ctigs[MAXSTATIONS][MAXRUNS];  /* Tsys or S*Tsys/Tant */ 
  double parms[MAXSTATIONS][MAXRUNS][MAXPARMS]; /* Gain curve parameters */ 
  char sources[MAXSTATIONS][MAXRUNS][MAXNAME]; /* Source names */ 
}; 

extern float calfact (int, char, char, float, float, int, char *);
extern int   parse_cmdline (int, char **, FILE **);
extern int   read_calfile (char *finam);

#endif
