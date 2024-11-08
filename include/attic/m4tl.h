
/* m4tl.h 
 * For m4tl(), which reads a prescribed record 
 * from the tape library on GRUMPY (A-900) and returns the 
 * information therefrom. 
 * Revised:  2000 July 21, JAB 
 */ 

#ifndef TRUE 
#define TRUE  1 
#define FALSE 0 
#endif

#ifndef GRUMPY 
#define GRUMPY "grumpy" /* Default */ 
struct TapeLibInfo { /* Tape Library Information */ 
  char label[8]; /* The tape label (VSN) */ 
  char chek[4]; /* CRCC check on label */ 
  char tape[10]; /* Experiment label */ 
  char exp[4]; /* Experiment number */ 
  char slot[6]; /* Where tape is */ 
  /* (These are NOT strings because there is no '\0' termination) */ 
  int thin; /* TRUE or FALSE */ }; 
#endif 
#ifndef SLOTS 
#define SLOTS "/correlator/tapelib/slots.txt" /* ASCII tape library */ 
#endif 
extern char * me; /* My name */ 
extern int msglev; /* For debuggery */ 
int m4tl (char * label, struct TapeLibInfo * * aans); 
int m4tl1(char * label, struct TapeLibInfo * * aans); 
int m4tl2(char * label, struct TapeLibInfo * * aans); 

