
/* cs.h DRAFT Revised 1999 June 11, JAB */ 
#include "mk4_sizes.h"

struct Channel_Stats { /* Channel statistics from correlator */ 
  /* Reference:  T1_R130 in T1.h */ 
  int8_t           SUId; /* Units are % errors */ 
  int8_t          ChnId; 
  int8_t          CFNum; 
  int8_t         ChkSum;  
  int8_t       InBdLink; 
  int8_t       InBdSync; }; 

struct Corr_Stats { /* Correlation statistics */ 
  int8_t headTapPastEnd; /* Units are % errors */ 
  int8_t  headTap3Carry; 
  int8_t tailTapPastEnd; 
  int8_t  tailTap3Carry; }; 

struct Correlator_Stats { /* Correlator statistics, corrman --> opera */ 
      /* One of these per baseline */ 
  struct header bin_head; 
  int8_t su[2]; /* su[SUIndex] = SU numbers ends of baseline */ 
      /* SUIndex = 0 for reference station or 1 for remote station */ 
  int8_t numChannels; 
  int8_t ChnData[MAX_CHAN_RT]; /* Got this channel data, TRUE or FALSE */ 
  struct Channel_Stats chs[2][MAX_CHAN_RT]; 
      /* That's chs[SUIndex][channel] */ 
  struct Corr_Stats cos[MAX_CHAN_RT]; }; 

