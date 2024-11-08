
/* T1aux.h; append to $INC/T1.h 
 * Revised:  1999 July 14, JAB */ 

/* Correlator End Task */ 
typedef struct T1_R150 { 
  T1_RecHdr recHdr; 
  /* Use recHdr.index for number of indices (snakes) hereby reported done. 
   * (See type_100.nindex) */ 
  char qcode[2]; /* To copy to type_100.qcode[] */ 
} T1_R150; 

