
/* mess.h contains definitions, tables, and programs for 
 * sending and receiving messages in the Mark-IV system. 
 * See also mess.c.  Revised:  2005 December 23, JAB 
 */ 
#ifndef  MESSS 
#define  MESSS 

#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/msg.h> 
#include <netinet/in.h> /* For htonl and ntohl */ 

#ifdef msg 
#undef msg /* Defined in sys/msg.h; not needed; duplicates cjl #define */ 
#endif 

#define TRUE   1 
#define FALSE  0 
#define WAIT  -1 
#define NOWAIT 0 

/* #define NMESS  1800 */     /* Maximum length of a short message */ 
/* 060731 kad - Changed long message size to support dom phasecal messages */
// #define NMESS  2100        /* Maximum length of a short message */ 
// 070403 kad - to support TAPE_INFO with up to 16 num_su
//      (would like to support SU PHASECAL message of 12944 bytes )
#define NMESS  6000          /* Maximum length of a short message */ 
#define NNAME     4           /* Length of abbreviated program names */ 
                              /* (Actually this is 3 chars plus null) */ 
#define NCOMP    16           /* Maximum length of computer names */ 
#define LMC    -256           /* Long-message code */ 
#define LMR    -257           /* Long-message reply code = done with file */ 
#define NBLOCK 2048           /* Read long messages in blocks this size */ 
#define LINEL   256           /* Maximum length of a line */ 
#define NTOK     40           /* Maximum number of tokens */ 

#define KEYNUM 0x56000000     /* 'V' ... for Mark-IV VLBI */ 
#define NLM      32           /* Maximum number of active files for 
                               * long messages 
                               */ 

#define ALIAS "/correlator/prog/text/MesAlias" 
                              /* File of aliases for message addressees */ 
#define DIRT  "/correlator/tmp" 
                              /* Directory for temporary files */ 

/* The message buffer.  Except for to, from, and fromc, which are set 
 * by msg_snd(), these are set by the sending program. 
 */ 
struct Mess { 
  long mtype;        /* Message type, used as a priority:  1 is highest 
                      * priority, 7 is routine, 10 is low priority. 
                      * BEWARE:  We now signal SIGINT (interrupt) to 
                      * addressee(s) when mtype = 1.  Make sure your  
                      * addressee can catch SIGINT, else it dies. 
                      */ 
  char to[NNAME];    /* "To" program name abbreviated to 3 chars */ 
  char toc[NCOMP];   /* "To" computer name, maximum NCOMP-1 chars */ 
  char from[NNAME];  /* "From" program name abbreviated to 3 chars */ 
  char fromc[NCOMP]; /* "From" computer name, maximum NCOMP-1 chars */ 
  long msize;        /* Length of everything following, bytes.  Negative 
                      * msize is used for special message codes, in which 
                      * case abs(msize) is the length. 
                      */ 
  char mtext[NMESS]; /* Message text */ 
}; 
struct MsgP {        /* Status parameters for long messages */ 
  int filelm;        /* File descriptor for received long message */ 
  int sfiles[NLM];   /* Status of files[] = 1 plus number of addressees */ 
  int dfiles[NLM];   /* File descriptors of files[] */ 
  char * files[NLM]; /* Names of files containing long messages */ 
}; 
extern struct MsgP msgp; 
extern struct msqid_ds msgbuf; 
                     /* Message queue status buffer */ 
extern char * me;    /* My name from argv[0] */ 
extern char * Me;    /* My name abbreviated to 3 chars */ 

extern int prisnd;   /* Send message default priority */ 
extern int prirec;   /* Received message priority */ 

/* On most OSs, the following line is in unistd.h--but not HP-UX */ 
unsigned int sleep(unsigned int seconds); 

/* The following functions are all defined in mess.c */ 
int msg_mlong (struct Mess * messa, int cntrl); 
int msg_snd   (struct Mess * messa, int cntrl); 
int msg_rcv   (struct Mess * messa, int cntrl); 
int msg_done  (struct Mess * messa, int cntrl); 
int msg_cklong(struct Mess * messa, int cntrl); 
int send_mess (void * ptr, int length, char * dest); 
int get_mess  (int wait, void * * ptr, char * * source); 
int msg_chk   (char * who); 
char * whichc (); 
int messflip (void *, int);
extern void emsg (char *, int, int, ...);

#endif 

