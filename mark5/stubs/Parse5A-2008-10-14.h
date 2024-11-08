#if MARK5DIR_ORIG

/* Parse5A.h 
 * Revised:  2008 October 14, JAB */ 
//#ifndef PARSE5A 
//#define PARSE5A 
/* *** Special-situation variables *** */ 
#define BCOLD /* Cold start permitted */ 
#define BCLEAR /* Clear during cold start */ 
#define DOMPLAY /* Add DOM instructions in Play() in Parse5A.c */ 
#define DOMINTERRUPT /* Add interrupt handler (in IOBoard.c) */ 
#define DOMESSAGES /* Receive and send messages */ 
// #define TEST TRUE /* TRUE for DOM internal clock, */ 
#define TEST FALSE /* FALSE for DOM external clock */ 
/* Note TEST is default only; it can be changed by test = ... */ 
// #define AMAZON /* Expect to have AMAZON board? */ 
#define SDK7 /* Have SDK 7.x or later from Conduant? (else SDK 6.x) */ 
#define DIRC "/usr/local/src/mark5a/C/" /* Where to find the *.c and *.h files */ 
/* *** */ 
#endif /* MARK5DIR_ORIG */
#define MAXSCANS  1024 /* Maximum number of scans in SDir */ 
#define MAXLENGTH   64 /* Maximum length of a scan's extended name +1 */ 
#define MAXNAME    128 /* Maximum length of a file name */ 
#if MARK5DIR_ORIG
#define BUFFL  249856 /* Buffer length for XLRRead(), long words */ 
  /* To cover 5 * 2520 * 9 / 8 * 64 = 907200 bytes, worst case. 
   * That's five track frames for 64 tracks VLBA with parity. 
   * 249856 long words is 999424 (16384 * 61) SS bytes. 
   * We need to keep this under 1000000 bytes for the present 
   * version of scan_check and for the Field System, each of 
    which backs up 1000000 bytes from the end of a scan. */ 
#define SOCBUF 131072 /* To send() to socket, bytes */ 
  /* (To match the socket buffer??) */ 
#define SSPAT 0x11223344 /* SS Test patterns */ 
#define SERIAL FALSE /* Use RS-232 serial port? */ 
#define FILL_PATTERN 0x11223344 
#define _GNU_SOURCE 1 /* GNU extensions (e.g., strcasestr()) */ 
#define DSKIP 2.0 /* Duration of net2out skip, seconds */ 
// #define DOT "." /* Dot */ 
#define UNL "_" /* Underline */ 
#define SYNC 0xabaddeed /* Mark5B:  A bad deed? */ 
#define CYCLE 32767L /* = 2^15 - 1 for TVG */ 
#define MAXWD 5010 /* Mark5B:  Maximum number of words to search for SYNC */
  /* (That's two full frames plus) */ 
#define ROTOFFSET 972000 /* Microseconds, used in getrot() */ 
#define PPSOFF 0.03 /* Precision aperture for itim.tvo, seconds */ 
  /* ?? Set a smaller number when we get NTP fixed ?? */ 
#define USERWORD 0xbead /* (Originally bead meant "to ask," as in prayer) */
/* For XLRBindInputChannel() and XLRBindOutputChannel(): */ 
#define CHANNEL_PCI          0 
#define CHANNEL_FPDP_TOP    30 /* Upper edge of Amazon daughter board */ 
#define CHANNEL_FPDP_FRONT  31 /* Bracket end of Amazon PCI card */ 

#include <stdio.h> 
#include <string.h> 
#include <ctype.h> /* For toupper(), isalpha(), etc. */ 
#include <stdlib.h> /* For atoi(), etc. */ 
#include <sys/types.h> /* For stat() and open() */ 
#include <sys/socket.h> /* For socket() and connect() */
#include <netinet/in.h> /* For socket() with PF_INET */
    /* And also for ntohl(), ntohs(), etc. */ 
#include <byteswap.h> /* For htonll() and ntohll() below */ 
#define htonll(x) bswap_64(x) /* Following htonl() in <netinet/in.h> */ 
#define ntohll(x) bswap_64(x) /* Following ntohl() in <netinet/in.h> */ 
// #include <linux/in.h> /* For IP_MTU in getsockopt() */ 
#define IP_MTU 14 /* From <linux/in.h> */
#include <netpacket/packet.h> /* For struct sockaddr_ll */ 
#include <netdb.h> /* For getservbyname() and gethostbyname() */
#include <unistd.h> /* For gethostname() and close() */ 
#include <errno.h> /* For perror(), etc. */ 
extern int errno; /* From open(), fstat(), etc. */ 
extern int h_errno; /* From gethostbyname() for herror() */ 
#include <sys/stat.h> /* For stat() and open() */ 
#include <fcntl.h> /* For open() */ 
#include <time.h> /* For nanosleep(), localtime(), etc. */ 
#include <sys/time.h> /* For gettimeofday() */ 
#include <sys/timeb.h> /* For ftime() */ 
#include <glib.h> /* For ABS(), TRUE, etc. */ 
#include <pthread.h> /* For pthread_create() */ 
#include <math.h> /* For fabs() */ 
double fabs(double x); /* Why isn't this in <math.h>? */ 
#include <sys/poll.h> /* For poll() in Net2out() */ 
#include <semaphore.h> /* For semaphores with multi-buffer FIFO */ 
#include <curses.h> /* For beep() */ 
/* For Conduant XLR functions: */ 
#include "xlrapi.h" /* Needs also xlrtypes.h */ 
/* N.B.:  Adding "-I ." on the gcc line should get xlrapi.h, xlrtypes.h 
 * because these have symbolic links in ~/C 
 * You'll need either "-L . -lssapi -lpthread" to include StreamStor or 
 * stubs5A.c otherwise */ 

/* * Messaging stuff follows * */ 
extern char * me; /* My name from argv[0] */ 
extern char * Me; /* My name abbreviated to 3 chars */ 
/* TRUE and FALSE are defined in mess.h */ 
#undef TRUE 
#undef FALSE
#include "mess.h" 
#include "message_structs.h" 
#include "message_codes.h" 
#include "polynomial.h" 
#include "cf_header.h" 
// #include "message_fields.h" /* Needed? */ 
/* Messages we might receive: */ 
extern struct Initialize_Mk5 initialize_mk5; 
extern struct Send_M5_Status send_m5_status; 
extern struct Lookup_Scan lookup_scan; 
extern struct Find_Next_Header find_next_header; 
extern struct Configure_Scan configure_scan; 
extern struct Model_Data model_data; 
extern struct Pcal_Request pcal_request; 
extern struct Scount_Request scount_request; 
extern struct Abort_Scan abort_scan; 
extern struct Transmit_Ffs transmit_ffs; 
extern struct Set_Rot set_rot; 
/* More messages needed here */ 
/* * My stuff follows * */ 
struct Dict { /* Dictionary entry */ 
  char * key; /* Keyword name */ 
  char * (* fun)(); /* Function to do it */ 
  }; 
#define SET 2
#define GET 3
struct outB { /* Output board parameters */
  char mode[16]; /* vlbi, st, tvr, or ? */ 
  double freq; /* Clock frequency, MHz */ 
  int active; /* TRUE or FALSE */
  int synced; /* TRUE or FALSE */ 
  int trka; /* Decoder channel A */
  int trkb; /* Decoder channel B */
  int tmode; /* VLBI track mode, 8, 16, 32, 64, or 0 if unknown */ 
  int numrs; /* Number of re-syncs */ 
  int throt; /* Throttled (recording suspend flag), TRUE of FALSE */ 
  char formt[16]; /* mark4, vlba, tvr, or null if unknown */ 
  int code; /* Output board CODE */ 
  /* Add more here later */ 
  }; /* End of struct outB */ 
extern volatile struct outB oub;
struct inB { /* Input board parameters */
  char mode[16]; /* Input mode, vlbi, mark4, vlba, st, tvg, or test */ 
      /* But see newm below */ 
  int ntracks; /* Number of tracks, 8, 16, 32, or 64 */ 
  int notclock; /* No clock, TRUE or FALSE, normal FALSE */ 
  int errb; /* Error bit(s) */ 
  /* Following added for Mark5B: */ 
  int tvg; /* TVG status, see notes */ 
  unsigned long bsm; /* BSM, bit-stream mask, BSM_WORD, L and H */ 
  int bsmb; /* Number of bits set in BSM, 1, 2, 4, 8, 16, or 32 */ 
  int userword; /* USER_WORD, USERWORD */ 
  int J; /* Decimation, J; dcm = 2^J */ 
  int K; /* Input clock frequency, K, fclock = 2^(K+1) */ 
  int selcgclk; /* For SELCGCLK, 0 = VSI clock, 1 = clock generator */ 
  int tvgsel; /* For TVGSEL, select input, 0 = external, 1 = internal */ 
  int seltvcnt; /* For SELTVCNT, internal pattern, 0 = TVG, 1 = ramp */  
  int selpp; /* For SELPP, select external PPS source */ 
      /* selpp = 0, none, 1 = AltA, 2 = AltB, 3 = VSI */ 
  int II; /* Set FPDP II? */ 
  int ii; /* Read back, 1 = FPDP II, 0 = FPDP I */ 
  int ss; /* START/STOP bit */ 
  int gocom; /* GOCOM TVR off or on */ 
  int dimcasc; /* For DIN_CASC, cascade on? */ 
  int envsiout; /* For EN_VSI_OUT, enable VSI output? */ 
  unsigned long tvr_mask; /* TVR_MASK, L and H, bits to check by TVR */ 
  unsigned long hw2; /* HDR_WORD_2, L and H */ 
  unsigned long hw3; /* HDR_WORD_3, L and H */ 
  int newm; /* New mode: * 
      * 0 Mark5A mode
      * 1 DIMino mode ext or unknown 
      * 2 DIMino mode tvg 
      * 3 DIMino mode ramp 
      * Others to be defined */
  /* (More later) */ 
  }; /* End of struct inB */ 
// extern struct inB inbb; 
extern volatile struct inB * ib; 
#endif /* MARK5DIR_ORIG */
struct SDir_B { /* Directory of recorded scans, etc. */ 
  int nscans; /* Number of scans herein */ 
  int n; /* Next scan to be accessed by "next_scan" */ 
  char scanName[MAXSCANS][MAXLENGTH]; /* Extended name */ 
  unsigned long long start[MAXSCANS]; /* Start byte position */ 
  unsigned long long length[MAXSCANS]; /* Length in bytes */ 
  unsigned long long recpnt; /* Record offset, bytes (not a pointer) */
  long long plapnt; /* Play offset, bytes */
      /* Beware:  Some folks call these indices "pointers" */ 
  double playRate; /* Playback clock rate, MHz */ 
  /* (sizeof(struct SDir) must be less than XLR_MAX_UDIR_LENGTH) */ 
  }; /* End of struct SDir */ 
#if MARK5DIR_ORIG
extern volatile struct SDir * sdir; 
#endif /* MARK5DIR_ORIG */
struct SDir4 { /* Bigger directory of recorded scans, etc. */
  int nscans; /* Number of scans herein */
  int n; /* Next scan to be accessed by "next_scan" */
  char scanName[4 * MAXSCANS][MAXLENGTH]; /* Extended name */
  unsigned long long start[4 * MAXSCANS]; /* Start byte position */
  unsigned long long length[4 * MAXSCANS]; /* Length in bytes */
  unsigned long long recpnt; /* Record offset, bytes (not a pointer) */
  long long plapnt; /* Play offset, bytes */
      /* Beware:  Some folks call these indices "pointers" */
  double playRate; /* Playback clock rate, MHz */
  /* (sizeof(struct SDir4) must be less than XLR_MAX_UDIR_LENGTH) */
  }; /* End of struct SDir4 */
#if MARK5DIR_ORIG
extern volatile struct SDir4 * sdir4;
extern volatile char * udir; 
extern volatile double lldir[]; /* Set to *sdir and *udir */ 
struct LDir { /* Directory of last scans and file names, etc. */ 
  char filenam[MAXNAME]; /* Default set by Set_scan() */ 
  char d2ffile[MAXNAME]; /* Last file name written by Disc_2file() */ 
  char f2dfile[MAXNAME]; /* Last file name read by File2disc() */ 
  int scann; /* Last scan in Set_scan() */ 
  int reclstn; /* Last scan created by Record() */ 
  int f2dlstn; /* Last scan created by File2disc() */ 
  int n2dlstn; /* Last scan created by Net2disc() */ 
  int i2ndoing; /* What In2net() is doing */ 
  int d2ndoing; /* What Disc2net() is doing */ 
  int n2ddoing; /* What Net2disc() is doing */ 
  int n2odoing; /* What Net2out() is doing */ 
  int d2fdoing; /* What Disc2file() is doing */ 
  int f2ddoing; /* What File2disc() is doing */ 
  int iob; /* 0 = none or Mark-5P I/O boards 
            * 1 = Mark-5A board 
            * 2 = Mark-5B board 
            * 3 = Mark-5B AMAZON board */ 
  int dxm; /* 0 = not Mark-5B nor Mark-5A+ 
            * 1 = Mark-5B DIM 
            * 2 = Mark-5B DOM 
            * 3 = Mark-5A+ playback of Mark-5B recordings (AP0) 
            * 4 = Mark-5A+ playback with AP1 set 
            * 5 = Mark-5A+ playback with AP2 set */ 
  /* Boolean status info: */ 
  int mounted; /* Are disks mounted? */ 
  int bmode; /* Bank mode? */ 
  int abmode; /* Auto-switching-banks mode? */ 
  int dfifo; /* Disk-FIFO mode? */ 
  int delayed; /* Delayed completion pending? */  
  int toXActv; /* Thread toX() active? */ 
  int XtoActv; /* Thread Xto() active? */ 
  int recording; /* Should we be recording? */ 
  int playing; /* Should we be playing? */
  int scanplaying; /* Should be be scan_playing? */ 
  int doOff; /* Do offPlay() on halted? */ 
  /* Other status information */ 
  int dsmask; /* Disk-state mask for DMS updates */ 
  /* Some Mark-5B status info: */ 
  int test; /* TEST mode, TRUE or FALSE */ 
  volatile int intr; /* Up to 5 interrupts from DIM or DOM */ 
  int inter; 
      /* DIM PPS interrupt:  0 none, 1 DOT, 2 external, or 
       * DOM copy of DOM Interrupt Mask Register */ 
      /* (Don't confuse these two) */ 
  int fps; /* DOM correlator frames (CFs) per second */ 
  int dfps; /* DOM d (??) frames per second */ 
  unsigned long cflen; 
      /* DOM correlator frame length (including header), samples */ 
  unsigned long su_id; /* Station-unit number */ 
  int bocflen; /* DOM BOCF length, samples */ 
  int bocfclen; /* DOM BOCF code length, samples */ 
  int cfpl; /* DOM CF payload length, samples */ 
  int suprsc; /* DOM SU prescale (interpolation factor) */ 
  int sendmounts; /* Send all mount changes? */ 
  unsigned long statstt; /* Status from Status() */ 
  int interval; /* Send statstt every interval seconds */ 
  }; /* End of struct LDir */ 
// extern struct LDir ldir; 
extern volatile struct LDir * ld; 
typedef struct dates { /* Variant from mk4_typedefs.h */ 
  short  year; 
  short  day; /* Of the year */ 
  short  hour; 
  short  minute; 
  double second; /* (Formerly float) */ 
  } dates_struct; 
struct ITim { 
      /* Times for Mark-5B DIM or DOM (UT from gettimeofday()) */ 
  unsigned long tvs; /* DIM time or ROT time (DOM) at latest PPS, seconds */ 
      /* (Or next PPS in case operator specified restart, below) */ 
  struct timeval tvi; /* UT at latest PPS or ROT interrupt */ 
  struct timeval tvp; /* UT at previous PPS or ROT interrupt */ 
  double tvo; /* Offset from UT to TOT (DOT) or ROT, seconds */ 
      /* (DIM or DOM time (DOT) is OSUT plus tvo at any UT
       * or tvo is DOT time minus OSUT) */ 
  double rot; /* ROT at last previous ROT PPS */ 
  int restart; /* TRUE on operator input tvs (recalculate tvo), else FALSE */ 
  int beep; /* Should we beep on each PPS? */ 
  }; /* End of struct ITim */ 
// extern volatile struct ITim itim; 
extern volatile struct ITim * it; 
struct DTim { /* Information for data_time */ 
  struct dates timet; /* Data time from header */ 
  unsigned long long offset; /* Byte offset to frame header */ 
  double timep; /* Track frame period in seconds */ 
  unsigned long long period; /* Track frame period in bytes */ 
  double rate; /* Track data rate, Mbaud for Mark-5A, 
                * Agregate data rate, Mbaud for Mark-5B */ 
  }; /* End of struct DTim */ 
extern volatile struct DTim dattime; 
struct Vsn { /* VSN plus serial numbers, etc. */ 
  volatile char vsn[64]; /* VSN */  
  S_DRIVEINFO dinfo[16]; /* Model, Serial, etc. */ 
  char vsnb[64]; /* VSN of B module in case non-bank mode */ 
  }; /* End of struct Vsn */ 
extern struct Vsn * pvsn; 
struct Header { /* Mark5B:  Decoded frame headers */
  unsigned short userspec; /* User-specified 16 bits */
  int t; /* T bit, presumably TRUE or FALSE */
  unsigned short frameno; /* Frame# within second (starting at 0) */
  char tcword1[10]; /* VLBA BCD Time Code Word 1 ('JJJSSSSS') */
  char tcword2[6]; /* VLBA BCD Time Code Word 2 (.'SSSS') */
  unsigned short crcc; /* 16-bit CRCC */
  }; /* End of struct Header */ 
extern struct Header header, header0, header64; /* For Mark5B */ 
extern unsigned long long foff0, foff64; /* For Mark5B */ 
extern int mei; /* Index into namep[] and endp[] */ 
extern char * namep[]; /* Program names */ 
extern char * endp[]; /* Names of scripts to end programs in namep[] */ 
#ifdef SDK7 
extern S_DBINFO dbinfo; /* For XLRGetDBInfo() (AMAZON) */
#endif /* SDK7 */ 
struct Mk5Aparm { /* See memo "Mark 5A Output FPGA Parameters", WHA */ 
  int word; /* 16-bit words relative to start of parameter buffer */ 
  int endBit; 
  int startBit; /* In backwards order */ 
  char name[32]; /* Will's name for these bits */ 
  }; /* End of struct DIMparm */ 
// extern struct Mk5Aparm mk5aparm[]; /* Mark5A and Mark5A+ */ 
struct DIMparm { 
    /* From memo "DIM parameters and procedures", WHA & ARW */ 
  int word; /* 16-bit words relative to start of DIM registers */ 
  int endBit; 
  int startBit; /* In backwards order as in the cited memo */ 
  char name[32]; /* Will's name for these bits */ 
  }; /* End of struct DIMparm */ 
// extern struct DIMparm dimparm[]; /* Mark5B */ 
struct DOMparm { /* From Brian's "MARK5B DOM SOFTWARE REGISTER MAP" */ 
  int word; /* 16-bit words relative to start of DOM registers */ 
  int endBit; 
  int startBit; /* In backwards order */ 
  char name[64]; /* Brian's name for these bits */ 
  }; /* End of struct DOMparm */ 
// extern struct DOMparm domparm[]; /* Mark5B */ 
struct FTVG { /* Mark-5B TVG info found and used by FFH() */ 
  unsigned long tvgw; /* Most recent TVG word */ 
  unsigned long bsm; /* Bit-stream mask, BSM */ 
  int jm; /* BSM index into msktab[] (bsm=msktab[jm]) (TVG only) */ 
  int bsmb; /* Number of bits set in BSM:  1, 2, 4, 8, 16, or 32 */ 
  int dcm; /* Decimation, DCM:  1, 2, 4, 8, or 16 */ 
  int j; /* DCM index into bittab[] (dcm = bittab[j]) */ 
  }; /* End of struct FTVG */ 
extern struct FTVG ftvg; /* Mark-5B TVG info */ 
extern struct poly_array polyray; /* Mark-5B Model_Data to header_gen() */ 
struct Othr { /* Other stuff to include in User Directory */ 
  volatile XLR_ERROR_CODE error; /* Return from error request */
  S_DIR dir; /* Directory */ 
  S_DRIVEINFO dinfo[16]; /* Drive information */ 
  S_DRIVESTATS dstats[XLR_MAXBINS]; /* Drive stats */ 
  S_DEVINFO devinfo; /* Device information */ 
  S_XLRSWREV swrev; /* Software and firmware versions, etc. */
  S_BANKSTATUS bkstat; /* Bank status */
  S_DEVSTATUS devstatus; /* Device status, devStatus */ 
  volatile UINT bank; /* Which bank */ 
  int datah; /* Status of ports[0], data socket, if any */ 
  volatile unsigned long long startbyte; /* Used by disc2net, etc. */ 
  volatile unsigned long long nowbyte; 
  volatile unsigned long long endbyte; 
  int msglev; /* Debug message level */ 
  /* (The following two must be a special case) */ 
  unsigned short idr; /* Input design revision, I/O board */ 
  unsigned short odr; /* Output design revision, I/O board */ 
  }; /* End of struct Othr */ 
// extern struct Othr othr; 
extern struct Othr * ot; 
#ifndef DIRHEAD 
#define DIRHEAD 
/* Following two structs added for enhanced Mark-5 module directory *  
 * Reference:  "Proposed specifications for enhanced Mark 5 module directory," 
 * a memo by Alan Whitney, Roger Cappallo, Kevin Dudevoir, Mike Titus, 
 * "16 August 2006" */ 
struct DirHed { /* ** Directory Header ** */ 
  long dirvers; /* Directory version, start at 1 */ 
  long status;  /* Binary:  3 bits for Disk Module Status, 
                 * 1 bit for bank_mode */ 
  char vsn[32]; /* Module VSN */ 
  char cmp[32]; /* VSN of continued-from module (bank mode), or 
                 * VSN of companion module (non-bank mode) */ 
  char con[32]; /* VSN of continued-to module (bank mode) */ 
  long spare[6]; 
  }; /* End of struct DirHed */ 
// extern struct DirHed dirhed; 
extern struct DirHed * dh; 
struct DirEnt { /* ** Directory scan entry ** */ 
  long dattyp;  /* Data type 
                 *  0 empty directory entry (end of directory) 
                 *  1 unknown 
                 *  2 SS test-pattern (no frame headers) 
                 *  3 Mark 5A:  VLBA-format data (no parity) 
                 *  4 Mark 5A:  Mark-4-format data (no parity) 
                 *  5 Mark 5A:  Straight-through VLBA-format data 
                 *  6 Mark 5A:  Straight-through Mark4-format data 
                 *  7 Mark 5A:  TVG data (no frame headers) 
                 *  8 Mark 5B:  Normal data 
                 *  9 Mark 5B:  TVG data 
                 * 10 Mark 5B:  numerical-ramp data */ 
  long scnum;   /* Scan serial number on Mark-5 module (starts at 1) 
                 * (save upper 2 bits for future use) */ 
  char scnam[32]; /* Scan name */ 
  char exnam[8];  /* Experiment name */ 
  char stacd[8]; /* Station code */ 
  unsigned long long startbyte; 
  unsigned long long endbyte; 
  unsigned char firstt[8]; /* First time tag (8 BCDs) */ 
  long firhnm; /* First header number */ 
  long bytoff; /* Byte offset to beginning of first frame header */ 
  long scnlen; /* Scan length in (complete) data frames */ 
  long datrat; /* Total data rate, Mbps */ 
  long bsmt;   /* Mark 5A:  number of tracks 
                * Mark 5B:  bit-stream mask */ 
  unsigned char spare[28]; 
  }; /* End of struct DirEnt */ 
// extern struct DirEnt dirent; 
extern struct DirEnt * de; 
#endif /* DIRHEAD */ 
// extern volatile XLR_ERROR_CODE error; /* Return from error request */
// extern S_DIR dir; /* Directory */ 
// extern S_DRIVEINFO dinfo[]; /* Drive information */ 
// extern S_DEVSTATUS devstatus; /* Device status, devStatus */ 
// extern S_DRIVESTATS dstats[]; /* Drive stats */ 
// extern S_DEVINFO devinfo; /* Device information */ 
// extern S_XLRSWREV swrev; /* Software and firmware versions, etc. */
// extern S_BANKSTATUS bkstat; /* Bank status */
// extern S_DEVSTATUS devstatus; /* Device status, devStatus */ 
// extern UINT bank; /* Which bank */ 
// extern volatile int datah; /* Status of ports[0], data socket, if any */ 
// extern unsigned long long startbyte; /* Used by disc2net, etc. */ 
// extern unsigned long long endbyte; 
// extern int msglev; /* Debug message level */ 
extern S_READDESC readdesc; /* Read description (readDesc) */ 
extern char messg[XLR_ERROR_LENGTH]; /* Error message as a string */
extern SSHANDLE dev; /* xlrDevice */ 
extern UINT indx; 
extern int drive; /* SS drive number, 0 ... 15 */ 
extern unsigned long rbuff[4 * BUFFL]; /* Read buffer */ 
extern unsigned long long * rbuf; /* From calloc() for FIFO buffers */ 
    /* (Do not confuse these two:  rbuff[] and rbuf[]) */ 
extern unsigned int socbuf; /* Size of buffer to send to socket, bytes */ 
extern int nbuf; /* Number of socbuf-size blocks in rbuf[] FIFO */ 
extern int wind; /* Write block index into rbuf[] */ 
extern int rind; /* Read block index into rbuf[] */ 
extern unsigned long task_id; /* Which task now? */ 
extern int ports[]; /* Sockets */
extern FILE * fports[]; /* Each of ports[] open also as a stream */ 
extern unsigned int sndbufs; /* Socket send buffer size, bytes */ 
extern unsigned int rcvbufs; /* Socket receive buffer size, bytes */ 
// extern unsigned long long nowbyte; 
extern unsigned long long orpntr; /* Old value of record pointer or nowbyte */ 
extern int rcount; /* Record-poll counter */ 
extern int icount; /* In2net overflow-poll counter */ 
extern int pcount; /* Play-poll counter */ 
extern int forml; /* Formal parsing in Parse5A()? */ 
extern struct timeval tvROT; /* Time at which ROT is valid */ 
extern double rot; /* ROT at time tvROT */ 
extern double rot_rate; /* To be 32.0e6 * speed_up */ 
extern volatile unsigned short * idr; /* Input design revision, I/O board */ 
extern  volatile unsigned short * odr; /* Output design revision, I/O board */ 
extern char * smod[]; /* For tcp or udp */ 
extern sem_t semtoX; /* Semaphore for toX() */ 
extern pthread_t threadtoX; /* For toX() */
extern sem_t semXto; /* Semaphore for Xto() */
extern pthread_t threadXto; /* For Xto() */ 
// extern unsigned long inbuf[MAXWD + 6]; /* Mark5B:  Input buffer */ 
extern unsigned long * inbuf; /* Mark5B:  Input buffer */ 
extern unsigned long * pdata; /* Mark5B:  Input data */ 
extern unsigned long long foff; /* Offset into disk file, longs */ 
extern unsigned long msktab[6]; /* Data mask */ 
extern unsigned long bittab[32]; /* One-bit-per-word bit mask */ 
/* In IOBoard.c */ 
extern unsigned long testo_par[][5]; 
extern int tsto_i; /* Index into testo_par[tsto_i][] */ 
/* * From IOBoard.c * */ 
int outBoard(int SoG); /* Set or get output board parameters */ 
int inBoard(int SoG); /* Set or get input board parameters */ 
int freq5B(double freq, int parm); /* Set Mark-5B clock frequency, MHz */ 
int DOMSUInit(); /* DOM Station Unit Operation---Initialization */ 
int DOMSUConfig(); /* DOM Station Unit Operation---Configuration */ 
int DOMSUBegin(); /* DOM Station Unit Operation---Begin */ 
int DOMSUStop(); /* Stop DOM playing */ 
int DOMSUResume(); /* Resume playing after a DOMSUStop() */ 
int DOMGRsetup(); /* DOM VSI mode, GLOBAL REGISTERS:  SETUP */ 
int DFEsetup(); /* DOM VSI mode FRONT END (DFE):  SETUP */ 
int set5A(char * name, int parm); /* Set a parameter in Mark-5A I/O */ 
int get5A(char * name); /* Get parameter from Mark-5A I/O */ 
int clearDIM(); /* Clear all the DIM writable parameters */ 
int setDIM(char * name, int parm); /* Set a parameter in Mark-5B DIM */ 
int getDIM(char * name); /* Get parameter from Mark-5B DIM */ 
int setDOM(char * name, int parm); /* Set parameter in Mark-5B DOM */ 
int setDOMs(char * name, int parm, int num); 
    /* Set parameter parm offset by num 16-bit words in Mark-5B DOM */ 
int getDOM(char * name); /* Get parameter from Mark-5B DOM */ 
/* * From Mark5A.c and Parse5A.c * */ 
extern char * pt; /* From Parse5A() */ 
extern char * ptr[]; 
char * Parse5A(char * instring); /* Parse input string, return answer */ 
/* Following four are in Parse5A.c but callable also from outside */ 
char * Status(); 
char * Play(); 
char * Data_check(); /* For data_check, track_check, and scan_check */ 
char * Set_scan(); /* For scan_set */ 
char * Bank_select(); /* For bank_select and bank_set */
int checkDisks(); /* Check disks and get disk info */ 
int FFHeader(PS_READDESC prdesc, struct DTim * pdtime, int st, int ntracks, 
  int trk); /* Decode some information from the track buffer */ 
int FFH(); /* Decode Mark-5B and TVG */ 
int cktvg(int nframe, unsigned long * buf, int dcm, unsigned long bsm); 
  /* Check TVG, called from FFH() */ 
int PatScan(PS_READDESC prdesc); /* Look for other patterns */ 
char * date2string(struct dates ind); /* Convert date to string */ 
dates_struct * string2dates(char * instring); /* Parse VSI date string */ 
double AddTime(char * instring); /* Time string to add seconds */ 
int ScanInfo(); /* Get information (from start of a scan) */ 
int crcc(unsigned char * idata, int len, int mask, int cycl); /* CRCC */ 
unsigned long tvg(int init); /* Test-vector generator */ 
int errors(); /* For XLR and other numbered errors */ 
int mksock(char * service, char * machine, int datahh); /* Make sockets */ 
int CheckBanks(); /* Check bank status, correct if necessary */
int setrot(); /* Set up to read ROT broadcasts */ 
void * Xto(void * null); /* Input data transfers */ 
void * toX(void * null); /* Output data transfers */ 
int ReadDir(); /* Read directory from SS disks */ 
int writedir(); /* Write directory to SS disks */ 
int offPlay(); /* Housekeeping after play is halted */ 
int getLabel(unsigned int tbank); /* Get VSN and DMS for bank tbank */ 
void * readit(void * null); 
    /* Receive and send messages for Mark-5B DOM (in Mark5mess.c) */ 
extern "C" { /* From FORTRAN */ 
void daze_(int * year, int * day,  int * mo, int * mda, int * nwd, int * juld); 
void dazl_(int * juld,  int * nyr, int * mo, int * nda, int * nyrd, int * nwd); 
  } /* End of from FORTRAN */ 
extern struct cf_header packt[]; /* One for each channel */
extern unsigned short pout[]; 
    /* array for the output of transpose(), the transposed version of 
     * the transpose() input, packets.  Each word is a time sample of
     * all 16 channels */ 
struct Header_Gen { /* All the parameters to call header_gen() */ 
  /* (Definitions copied from rjc_code.c) *  
   * Inputs: */ 
  double rot; /* ROT for the start of a correlator frame */ 
  long unsigned cf_len; /* correlator frame length (samples) */ 
  double fs; /* sample frequency (samples/s) */ 
  short unsigned nchan; /* number of channels to generate headers for */ 
  short unsigned k_div; 
      /* divisor specifying fringe phase update period: 
       * {0,1,2,3,4} => {1,2,4,8,16} divisor */ 
  struct poly_array * polya; /* pointer to an array of polynomials */ 
  short unsigned su_id; /* ID# for this station unit */ 
  short unsigned sys_per_stn; 
      /* ratio of sysclk frequency to stnclk frequency, 
       * i.e. the number of sysclk's per stnclk. {1,2,4,8,16} */ 
  short unsigned model_parameters; 
      /* parameter containing fields for model corrections 
       * bits 0-2: over-sampling factor {1, 2, 4} */ 
  short unsigned sidebands; 
    /* The sideband bit 3 of bugfix defined on a perchannel basis  
     * bit n : sideband   0 = USB   1 = LSB  for channel n */ 
  /* Outputs: */ 
  struct cf_header * packets; 
      /* pointer to output array where packets will be written */ 
  long * d_int; 
      /* ptr to delay value in integer samples, for use in determining 
       * by how much to delay the data streams. This whole delay must 
       * be added to the fractional delay, in order to calculate the 
       * correct amount by which to delay each bit stream. */ 
  long * d_fract; 
      /* pointer to the fractional part of the delay starting value and 
       * the delay generator increment, modified to be directly useable 
       * in the DOM delay generator registers. */ 
  }; /* End of struct Header_Gen */ 
extern struct Header_Gen head_gen; 
/* * header_gen() is in rjc_code.c * * 
 * and is used to feed and burp a Mark-5B DOM */ 
int header_gen (double rot,
                long unsigned cf_len,
                double fs,
                short unsigned nchan,
                short unsigned k_div,
                struct poly_array * polya,
                short unsigned su_id,
                short unsigned sys_per_stn,
                short unsigned model_parameters,
                short unsigned sidebands,
                struct cf_header * packets,
                long * d_int,
                long d_fract[2]); /* End of header_gen() */ 
void transpose (struct cf_header * packets, unsigned short int * pout); 
/* * transpose() inverts the *packets * */ 

//#endif /* PARSE_5A */

#endif /* MARK5DIR_ORIG */
