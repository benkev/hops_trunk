/*******************************************************************************
* message_structs.h -- define specific message formats for Mark 4 correlator   *
*                                                                              *
* original version                                             rjc  95.6.14    *
* new format; added messages from RJC's 95.6.23 conductor memo jab  95.12.27   *
* modify format somewhat; add more messages                    rjc  96.11.25   *
* update messages to reflect changes in system architecture    rjc  98.7.8     *
*                                               last updated:       99.8.16    *
*                                                              JAB  01.01.10   *
*******************************************************************************/

#include "mk4_sizes.h"
#include "mk4_typedefs.h"
#include "type_120.h"

/*******************************************************************************
*                                                                              *
*                 g e n e r a l i z e d  s t r u c t u r e s                   *
*                                                                              *
*******************************************************************************/

struct header
  {
  U32 msg_code;                      /* msg-specific code(see message_codes.h)*/
  U32 task_id;                       /* number specific to each task          */
  };

struct su_chan
  {
  char chan_name[8];                 /* 8 char. name identifying each channel */
  char station;                      /* source station for this channel       */
  U16 ch_status;                     /* channel status code                   */
  char pcal;                         /* Phasecal amp, thousandths, -1 no data */
  };

                                     /* channel status code definitions       */
enum ch_status_codes
  {
  CHANNEL_INACTIVE = 0,              /* channel is currently not active       */
  CHANNEL_NOT_SYNCED,                /* channel active, but not synced        */
  CHANNEL_SYNCED,                    /* channel sync status is AOK            */
  CHANNEL_UNDER_REPAIR,              /* TRM's are repairing this channel      */
  CHANNEL_MIXED_TRACK_STATUS,        /* contributing tracks have mixed stati  */
  CHANNEL_DELAY_BUFF_OVERRUN         /* DMM is currently in overrun (NYI)     */
  };

struct su_entry
  {
  U32 task_id;                       /* task_id for this SU's current task    */
  U16 su_status;                     /* various SU global status bits         */
  U16 suman_state;                   /* major state that suman FSM is in      */
  U16 su_init_flags;                 /* status flags upon initialization      */
  U8 su;                             /* su# [0..15] for this SU               */
  U8 stream;                         /* current stream for this SU            */
  U8 bank;                           /* if a mk5b dom, current bank 0|1 => A|B*/
  char dom_alias[4];                 /* if a dom, contains 3 char alias string*/
  char station;                      /* station code (master code if mult. stn*/
  char vsn[8];                       /* volume serial number of tape          */
  char checksum[4];                  /* checksum             "   "            */
  char pass[4];                      /* tape pass of curr. scan (null term.)  */
  char direction;                    /* tape dir. of curr. scan ('F' or 'R')  */
  short current_position;            /* current tape position (feet)   word 30*/ 
  U16 supply_reel;                   /* est. footage on supply (upper) reel 31*/
  U16 takeup_reel;                   /*  "      "     " takeup (lower)   "  32*/
  short head_pos_command;            /* commanded head pos. in kA      word 41*/
  short head_position;               /* current head pos. in kA        word 42*/
  U16 dpu_gsw;                       /* GSW of the tape drive (DPU)    word 73*/
  U16 dpu_error_flags;               /* error flag word for the DPU         74*/
  U16 current_speed;                 /* actual (as read) tape speed 0.01 ipsB5*/
  U16 playback_speed;                /* speed for playback of data (0.01 ips) */
  double speed_up;                   /* ratio ROT/COT {0.25,0.5,1,2,4,8,16}   */
  double recent_rot;                 /* ROT timestamp from most recent msg.   */
  double recent_tot;                 /* recently read TOT                     */
  double sample_rate;                /* formatter sample rate (Hz)            */
  double start_rot;                  /* scheduled starting ROT                */
  double stop_rot;                   /*    "      ending   ROT                */
  struct su_chan channels[16];
  };

                                     /* global su_status bit definitions      */
#define INITIALIZED   0x0001
#define CONFIGURED    0x0002
#define TAPE_MOUNTED  0x0004
#define VACUUM_OK     0x0008
#define VALID_VSN     0x0010
#define DOM           0x0020
#define REAL_TIME     0x0040

                                     /* suman_state definitions               */
enum suman_state_defs
  {
  INITIALIZING,
  READY,
  CONFIGURING,
  PRE_PEAK_POSING,
  PEAKING,
  POST_PEAK_POSING,
  REPOSITIONING,
  FINDING_TOT,
  FOUND_TOT,
  MISSING_TOT,
  SYNCING,
  SYNCED,
  LOCATING_SCAN,
  LOCATED_SCAN,
  MISSING_SCAN,
  REPORTED_TOT
  };
                                     /* su_init_flag bit definitions          */
#define TRM0_FAILURE        0x0001
#define TRM1_FAILURE        0x0002
#define TRM2_FAILURE        0x0004
#define TRM3_FAILURE        0x0008
#define TRM4_FAILURE        0x0010
#define TRM5_FAILURE        0x0020
#define TRM6_FAILURE        0x0040
#define TRM7_FAILURE        0x0080
#define CRM_FAILURE         0x0100
#define DCM_FAILURE         0x0200
#define DMM0_FAILURE        0x0400
#define DMM1_FAILURE        0x0800
#define PCM_FAILURE         0x1000
#define DPU_TIMEOUT         0x2000
#define SOCKET_OPEN_FAILURE 0x4000
#define NO_SU_RESPONSE      0x8000

                                     /* stream variable special value         */
#define NO_STREAM     0xff           /* no stream currently defined for SU    */

struct su_table
  {
  U8 num_su;                          /* number of SU's currently initialized */
  struct su_entry su_s[MAX_NUM_SU];   /* array index is actual SU number      */
  };

struct snake_head_table
  {
  int dummy;
  };

struct block_allocation_table
  {
  int dummy;
  };

typedef struct MountTape              /* info for a single tape mount         */ 
  {                                
  U8 stream;                          /* stream number for this tape          */ 
  char volser[8];                     /* VSN                                  */ 
  char slot[5];                       /* where the tape lives in the library  */ 
  char station;                       /* where the tape was recorded          */ 
                                      /* for the 3 fields above ?'s = unknown */ 
  U8 su;                              /* SU number where tape is              */ 
  } MountTape; 
#define TAPE_UNAVAILABLE 0xff         /* value of su when tape can't be found */
#define SU_UNKNOWN       0xfe         /* SU not yet known; displayed as su=?  */

/*******************************************************************************
*                                                                              *
*                 t o / f r o m    g e n a r o o t                             *
*                                                                              *
*******************************************************************************/



struct Generate_Root                  /* conductor --> genaroot               */
  {                                   /* generate a new root at specified time*/
  struct header bin_head;             /* generic binary message info.         */
  char ekey[MAX_LEN_KEY];             /* evex key = experiment number         */
  char scan_id[MAX_LEN_KEY];          /* ovex key = identifier of desired scan*/
  char init_key[MAX_LEN_KEY];         /* ivex key = init. key currently active*/
  char corr_mode[MAX_LEN_KEY];        /* cvex key = correlator mode (or null) */
  char stations[MAX_NUM_STN];         /* string containing 1 char station IDs */
  U16 ap_length;                      /* accumulation period (ms of ROT)      */
  U16 mirror_flags;                   /* controls for mirroring               */
  }; 

struct Root_Complete                  /* genaroot --> conductor               */
  {                                   /* root file has been generated         */
  struct header bin_head;             /* generic binary message info.         */
  char root_filename[MAX_LEN_FILNAM]; /* string w/ complete path to root file */
  }; 

struct Root_Failure                   /* genaroot --> conductor               */
  {                                   /* root file has not been generated     */
  struct header bin_head;             /* generic binary message info.         */
  U16 return_code;                    /* identifies exact nature of the error */
  }; 





/*******************************************************************************
*                                                                              *
*                 t o / f r o m    c o r r m a n                               *
*                                                                              *
*******************************************************************************/

struct Initialize_Correlator          /* conductor --> corrman                */
  {                                   /* controls writing of corr. out. data  */
  struct header bin_head;             /* generic binary message info.         */
  char ivex_key[MAX_LEN_KEY];         /* key into ivex file                   */
  U8 su_list[MAX_NUM_SU];             /* Boolean array of SU's to be sync'd   */
  };

struct Correlator_Initialized         /* corrman --> conductor                */
  {                                   /* reports back result of initialization*/
  struct header bin_head;             /* generic binary message info.         */
  U8 link_status[4][MAX_NUM_SU];      /* 1st index is SU ch. grp. (0..3)      */
                                      /* bits 4..0 are a(ctive), s(synced)    */
                                      /* r(eady), e(rror), d(data avail.)     */
  };

struct Configure_Correlator           /* conductor --> corrman                */
  {                                   /* prepare correlator to start task     */
  struct header bin_head;             /* generic binary message info.         */
  char root_filename[MAX_LEN_FILNAM]; /* name of correlator config. file      */
  char ref_station_order[MAX_NUM_STN];/* string of stns; 1st of pair is ref.  */
  double configure_cot;               /* COT at which to configure correlator
                                         -- not currently implemented         */
  U8 stream;                          /* stream that this task is in          */
  U8 slice_pool;                      /* bitmap constrains alloc. bit#n = sl#n*/
  struct su_table sut;                /* config. of SU output channels        */
  }; 

struct Cu_Configuration               /* corrman  -->  conductor              */
  {                                   /* report successful CU configuration   */
  struct header bin_head;             /* generic binary message info.         */
  struct snake_head_table sht;
  struct block_allocation_table bat;
  }; 

struct Cu_Resource_Error              /* corrman  -->  conductor              */
  {                                   /* report unsuccessful CU configure req.*/
  struct header bin_head;             /* generic binary message info.         */
  U16 return_code;                    /* identifies the type of error         */
  }; 
                                      /* bit definitions for return_code:     */
#define MISSING_CVEX         0x0001
#define MISSING_IVEX         0x0002
#define MISSING_EVEX         0x0004
#define MISSING_OVEX         0x0008
#define SLICE_UNAVAILABLE    0x0010
#define BASELINE_OVERFLOW    0x0020
#define ROOTFILE_NAME_ERR    0x0040
#define DATAFILE_NAME_ERR    0x0080
#define DATAFILE_OPEN_ERR    0x0100
#define DATAFILE_CHMOD_ERR   0x0200
#define DATAFILE_FDOPEN_ERR  0x0400
#define DATAFILE_FWRITE_ERR  0x0800
#define DATAFILE_FWRITE_ERR2 0x1000
#define CU_SENDMESS_ERR      0x2000

struct Correlator_Abort               /* conductor --> corrman                */
  {                                   /* shutdown current correlation         */
  struct header bin_head;             /* generic binary message info.         */
  U16 abort_flags;                    /* details of CU abort request          */
  char station;                       /* iff single station to be removed     */
  };

                                      /* abort flag bit definitions           */
#define FLUSH_FILE     1              /* delete current output file(s)        */
#define ABORT_STATION  2              /* shutdown baselines involving station */
#define ABORT_TASK     4              /* shutdown all baselines in this task  */
#define ABORT_ALLTASKS 8              /* shutdown all baselines, period.      */

struct Corrtask_Complete              /* corrman  -->  conductor              */
  {                                   /* correlation complete for given task  */
  struct header bin_head;             /* generic binary message info.         */
  U16 slice_count;                    /* number of slices now available (0..3)*/
  U16 error_flags;                    /* identifies the type of error, if any */
  }; 

struct Realtime_Display               /* opera --> corrman                    */
  {                                   /* control realtime fringe display      */
  struct header bin_head;             /* generic binary message info.         */
  char baseline[2];                   /* baseline (e.g. EV)                   */
  char chan_name[8];                  /* channel name (e.g. X1L)              */
  U8 enable;                          /* enable display (0 = false, 1 = true) */
  };

struct Realtime_Data                  /* corrman --> soloist                  */
  {                                   /* contains data for 1 AP, 1 BL, 1 ch   */
  struct header bin_head;             /* generic binary message info.         */
  char chan_name[8];                  /* channel name (e.g. X1L)              */
  struct type_120 ch_data;            /* type 120 record with data for 1 chan */
  };

struct Channel_Stats                  /* Channel statistics from correlator   */ 
  {                        
                                      /* Reference:  T1_R130 in T1.h          */ 
  signed char      SUId;              /* Units are % errors                   */ 
  signed char     ChnId; 
  signed char     CFNum; 
  signed char    ChkSum;  
  signed char  InBdLink; 
  signed char  InBdSync; 
  }; 

struct Corr_Stats                     /* Correlation statistics               */ 
  {
  signed char  headTapPastEnd;        /* Units are % errors                   */ 
  signed char   headTap3Carry; 
  signed char  tailTapPastEnd; 
  signed char   tailTap3Carry; 
  }; 

struct Correlator_Stats               /* corrman --> opera                    */ 
  {
  struct header bin_head;             /* One of these msgs per baseline       */ 
  signed char  su[2];                 /* su[j] = SU number of ends of b-line  */ 
                                      /* with j = 0|1 for ref|rem             */ 
  signed char  numChannels; 
  signed char  ChnData[MAX_CHAN_RT]; /* Got this channel data, TRUE or FALSE */ 
  struct Channel_Stats chs[2][MAX_CHAN_RT];  /* That's chs[SUIndex][channel] */ 
  struct Corr_Stats cos[MAX_CHAN_RT];
  }; 

/*******************************************************************************
*                                                                              *
*                 t o / f r o m    s u m a n                                   *
*                                                                              *
*******************************************************************************/

struct Initialize_Sus                 /* conductor --> SU manager             */
  {                                   /* ask SU mgr. to init. requested SU's  */
  struct header bin_head;             /* generic binary message info.         */
  char ivex_key[MAX_LEN_KEY];         /* key into ivex file                   */
  U8 su_list[MAX_NUM_SU];             /* Boolean array of SU's to be init'd   */
  }; 

struct Sus_Initialized                /* SUM --> conductor                    */
  {                                   /* report the init. status of all SU's  */
  struct header bin_head;             /* generic binary message info.         */
  U16 su_init_flags[MAX_NUM_SU];      /* init. status bits for each SU        */
  }; 

struct Configure_Sus                  /* conductor --> SU manager             */
  {                                   /* ask SU mgr. to configure all drives  */
  struct header bin_head;             /* generic binary message info.         */
  char root_filename[MAX_LEN_FILNAM]; /* string w/ complete path to root file */
  U8 stream;                          /* stream that this task is in (0..3)   */
  U16 chan_mask;                      /* channel n enabled iff bit n is set   */
  U8 realtime_mode;                   /* 0:normal  1:realtime                 */
  }; 

struct Sus_Configured                 /* SU manager --> conductor             */
  {                                   /* confirm successful SU configuration  */
  struct header bin_head;             /* generic binary message info.         */
  struct su_table sut;                /* config. of SU output channels        */
  };


struct Su_Config_Error                /* SU manager  -->  conductor           */
  {                                   /* report unsuccessful SU configure req.*/
  struct header bin_head;             /* generic binary message info.         */
  U16 return_code;                    /* identifies the type of error         */
  }; 
                                      /* bit definitions for return_code:     */
enum su_cfg_errors
    {
    MISSING_HEAD_NUM = 0,
    MISSING_HEAD_POS,
    MISSING_START_POS,
    PRIOR_TASK_ACTIVE
    };

struct Cant_Mount_Tape                /* SU manager --> conductor             */
  {                                   /* single tape can't be found/mounted   */
  struct header bin_head;             /* generic binary message info.         */
  U16 return_code;                    /* specifies exact nature of failure    */
  char station;                       /* correlation station identifier       */
  }; 

struct All_Mounted                    /* SU manager --> conductor             */
  {                                   /* all tapes for this scan are mounted  */
  struct header bin_head;             /* generic binary message info.         */
  }; 

struct Pos_To_Tot                     /* conductor --> SU manager             */
  {                                   /* position all tapes to 1st good TOT   */
  struct header bin_head;             /* generic binary message info.         */
  }; 

struct Repos_To_Tot                   /* opera --> SU manager                 */
  {                                   /* try to reposition an SU to a TOT     */
  struct header bin_head;             /* generic binary message info.         */
  U8 su;                              /* the SU# (0..15) to be repositioned   */
  U8 location;                        /* location to start TOT search:        */
  };                                  /* (1=original; 2=current)              */

struct Valid_Time                     /* SUM --> conductor                    */
  {                                   /* reports good time found on 1 station */
  struct header bin_head;             /* generic binary message info.         */
  double first_useable_tot;           /* earliest TOT found in this task      */
  U8 su;                              /* SU# (0..15) on which this time found */
  }; 

struct Cant_Find_Time                 /* SUM --> conductor                    */
  {                                   /* SU has given up looking for good time*/
  struct header bin_head;             /* generic binary message info.         */
  double bad_tot;                     /* failing TOT as returned by SU        */
  U16  failure_code;                  /* Code defining reason for failure     */
  char station;                       /* correlation station identifier       */
  U8 su;                              /* number of failing SU (0..15)         */
  }; 

                                      /* failure_code bit definitions         */
#define TIME_OOR     0x0001           /* time out of range                    */
#define BAD_DIGITS   0x0002           /* invalid time digits returned by SU   */

struct Synchronize_Tapes              /* conductor --> SUM                    */
  {                                   /* initiate ROT-queued startup of tapes */
  struct header bin_head;             /* generic binary message info.         */
  double tape_start_rot[MAX_NUM_SU];  /* ROT at which to start each drive     */
  }; 

struct Send_Tape_Info                 /* opera --> SUM                        */
  {                                   /* initiate periodic status reports     */
  struct header bin_head;             /* generic binary message info.         */
  short interval;                     /* send stati every interval secs. if>0 */
  };                                  /* once if = 0; stop sending if < 0     */

struct Tape_Info                      /* SUM --> opera                        */
  {                                   /* periodic message of tape stati       */
  struct header bin_head;             /* generic binary message info.         */
  struct su_table sut;                /* current info. about all tapes & SU's */
  }; 

struct End_Task                       /* conductor --> SUM, etc.              */
  {                                   /* task has ended; do cleanup, etc.     */
  struct header bin_head;             /* generic binary message info.         */
  };

struct Transmit_Ffs                   /* bos --> SUM, etc.                    */
  {                                   /* cause serial link to send 1s of FF's */
  struct header bin_head;             /* generic binary message info.         */
  };



/*******************************************************************************
*                                                                              *
*                 t o / f r o m    d o m i n o                                 *
*                                                                              *
*******************************************************************************/

struct Initialize_Mk5                 /* SUM --> Mk5b                         */
  {                                   /* (re)initialize a Mk5b unit           */
  struct header bin_head;             /* generic binary message info.         */
  U32 cf_length;                      /* length of corr. frame in sysclks     */
  U8 bocf_stretch;                    /* (1,2,4,8,16) for (240..3840) sysclks */
  U8 su_id;                           /* station unit ID (0..15)              */
  U8 send_mounts;                     /* 0|1 don't|do send all mount changes  */
  U8 realtime;                        /* 0|1 isn't|is remote realtime station */
  U16 packet_size;                    /* (realtime only; UDP pkt size (bytes) */
  };

struct Mount_Change                   /* Mk5b --> SUM                         */
  {                                   /* disk pack info sent on mnt detection */
  struct header bin_head;             /* generic binary message info.         */
  char vsn[2][8];                     /* volser for disk packs in banks A&B   */
  char serno[2][8][20];               /* ser#'s indexed by bank, drv, digit   */
  U8 su_id;                           /* station unit ID (0..15)              */
  };

struct Send_M5_Status                 /* SUM --> Mk5b                         */
  {                                   /* request current status of tasks      */
  struct header bin_head;             /* generic binary message info.         */
  short interval;                     /* send stati every interval secs. if>0 */
  };                                  /* =0 send once, <0 stop sending        */

struct M5_Status                      /* Mk5b --> SUM                         */
  {                                   /* current status of the Mk5b unit      */
  struct header bin_head;             /* generic binary message info.         */
  U32 status;                         /* GSW word (bits 15..0 DOM stat reg)   */
  U32 tot;                            /* most recently read TOT (BCD jjjsssss)*/
  U8 su_id;                           /* this station unit ID (0..15)         */
  };

struct Lookup_Scan                    /* SUM --> Mk5b                         */
  {                                   /* find requested scan in the directory */
  struct header bin_head;             /* generic binary message info.         */
  char scan_label[51];                /* standardized name formed from ovex   */
  U8 bank_select;                     /* 0|1 => select bank A|B               */
  };

struct Scan_Bounds                    /* Mk5b --> SUM                         */
  {                                   /* info from requested scan             */
  struct header bin_head;             /* generic binary message info.         */
  long long int start_byte;           /* loc. of scan start (-1 => no match)  */
  long long int end_byte;             /*   "   " scan end                     */
  U8 su_id;                           /* station unit ID (0..15)              */
  };

struct Find_Next_Header               /* SUM --> Mk5b                         */
  {                                   /* look for next disk frame header      */
  struct header bin_head;             /* generic binary message info.         */
  long long int byte_num;             /* location on disk to start looking    */
  };

struct Header_Contents                /* Mk5b --> SUM                         */
  {                                   /* return byte# found by prev. find     */
  struct header bin_head;             /* generic binary message info.         */
  long long int byte_num;             /* loc. of header (-1 => no hdr found)  */
  U32 df_header[4];                   /* contents of 1st header after byte#   */
  U8 su_id;                           /* station unit ID (0..15)              */
  };

struct Configure_Scan                 /* SUM --> Mk5b                         */
  {                                   /* (re)configure a Mk5b unit            */
  struct header bin_head;             /* generic binary message info.         */
  long long int byte_num;             /* absolute byte# of scan start - 1 sec */
  double start_rot;                   /* start ROT for scan(sysclks since BOY)*/
  double end_rot;                     /* ending    "   "    "    "   "    "   */
  U32 ss_invalid_code;                /* code for invalid used by StreamStor  */
  U32 dim_invalid_code;               /*  "    "     "      "   " DIM         */
  U8 crossbar_map[32];                /* source track (0..31) for each output */
  U8 sample_rate;                     /* DIM sample rate (2,4,8,16,32,64 MS/s)*/
  U8 output_rate;                     /* DOM output rate (2,4,8,16,32) MS/s)  */
  U8 num_bitstreams;                  /* # of m'plexed strs. (1,2,4,8,16,32)  */
  U8 bits_sample;                     /* # of bits per sample (1,2)           */
  };

struct Model_Data                     /* SUM --> Mk5b                         */
  {                                   /* packet containing spline model info  */
  struct header bin_head;             /* generic binary message info.         */
  double rot_interval[2];             /* ROT time of start & end of interval  */
  double delay[6];                    /* quintic spline delay polynomial      */
  double phase[16][6];                /*   "  " phase poly. per channel       */
  U16 sidebands;                      /* bit n for chan n; 0|1 for USB|LSB    */
  U8 nchan;                           /* number of channels present (1..16)   */
  U8 oversampling;                    /* oversampling factor (1,2,4)          */
  U8 k_div;                           /* specifies fringe phase update period */
  };                                  /*     kdiv = 0..4 => 2^k_div sysclks   */

struct Pcal_Request                   /* SUM --> Mk5b                         */
  {                                   /* setup and send periodic pcal data    */
  struct header bin_head;             /* generic binary message info.         */
  double tones[16];                   /* tone frequencies (Hz)                */
  U8 ntones;                          /* number of tones (1..16)              */
  U8 ap;                              /* product accumulation period (s)      */
  };

struct Pcal_Data                      /* Mk5b --> SUM                         */
  {                                   /* pcal values for an accum period      */
  struct header bin_head;             /* generic binary message info.         */
  double timestamp;                   /* ROT timestamp of start of ap         */
  U32 accums[16][16][2];              /* [channel_index][tone_index][c or s]  */
  U8 su_id;                           /* station unit ID (0..15)              */
  };

struct Scount_Request                 /* SUM --> Mk5b                         */
  {                                   /* request periodic state count data    */
  struct header bin_head;             /* generic binary message info.         */
  U8 ap;                              /* state count accumulation period (s)  */
  };

struct Scount_Data                    /* Mk5b --> SUM                         */
  {                                   /* returning state count values         */
  struct header bin_head;             /* generic binary message info.         */
  double timestamp;                   /* ROT timestamp of start of ap         */
  U32 accums[16][4];                  /* [channel_index][state_index]         */
  U8 su_id;                           /* station unit ID (0..15)              */
  };

struct Abort_Scan                     /* SUM --> Mk5b                         */
  {                                   /* stop current scan ASAP               */
  struct header bin_head;             /* generic binary message info.         */
  };

struct dir_entry                      // struct defining single directory entry
  {
  char scan_name[80];                 // scan name
  unsigned long long start;           // starting byte #
  unsigned long long length;          // length of scan in bytes
  };

struct Send_Directory                 // anyone --> domino
  {                                   // request dir. info for block of 50 scans
  struct header bin_head;             // generic binary message info.
  int bank;                           // 0|1 for A|B
  int nscan;                          // starting scan #
  };

struct Directory_Chunk                // domino --> anyone
  {
  struct header bin_head;             // generic binary message info.
  int bank;                           // 0|1 for A|B
  int nscan;                          // starting scan # for block of 50 results
  int nreturned;                      // number of scans ret'd. (0..50)
  int ntotal;                         // total # of scans in directory
  struct dir_entry dir_scan[50];      // scan-by-scan info
  };

struct Playback_Control               // anyone --> domino
  {
  struct header bin_head;             // generic binary message info.
  int play;                           // 0|1 for stop|play
  unsigned long long start;           // starting loc (byte#) at which to play
  U8 sample_rate;                     // DIM sample rate (2,4,8,16,32,64 MS/s)
  U8 output_rate;                     // DOM output rate (2,4,8,16,32) MS/s)
  U8 num_bitstreams;                  // # of m'plexed strs. (1,2,4,8,16,32)
  };

/*******************************************************************************
*                                                                              *
*                 t o / f r o m    r o t m a n                                 *
*                                                                              *
*******************************************************************************/

struct Set_Cot_Clock                  /* conductor --> rotman                 */
  {                                   /* sets/transmits master COT clock      */
  struct header bin_head;             /* generic binary message info.         */
  double initial_cot;                 /* epoch to which COT is set            */
  int set;                            /* 0|1|2 = don't set COT | set COT to   */
                                      /* initial_cot | set COT to system clock*/
  int broadcast;                      /* 0|1 = don't|do broadcast COT next tic*/
  };

struct Start_Rot_Clock                /* conductor --> rotman                 */
  {                                   /* begin ROT clock with specified time  */
  struct header bin_head;             /* generic binary message info.         */
  double starting_cot;                /* epoch at which to start clock (COT)  */
  double initial_rot;                 /* ROT with which to start the clock    */
  double ending_rot;                  /* epoch at which to stop clock (ROT)   */
  double rot_rate;                    /* secs/ROT tick (0.25,0.5,1,2,4,8,16)  */
  U16 initial_rot_year;               /* year of starting ROT (e.g. 1993)     */
  U8 stream;                          /* stream for this clock (0..3)         */
  };  

struct Send_Times                     /* anyone --> rotman                    */
  {                                   /* request current times (periodically) */
  struct header bin_head;             /* generic binary message info.         */
  int interval;                       /* if >0, send times every interval COT */
  };                                  /* secs, =0 send once, <0 stop sending  */

struct Current_Times                  /* rotman --> anyone                    */
  {                                   /* current clock readings               */
  struct header bin_head;             /* generic binary message info.         */
  double cot;                         /* correlator time (in short ROT format)*/
  double rot[4];                      /* ROT for each of 4 streams            */
  };  

/*******************************************************************************
*                                                                              *
*                 t o / f r o m    o p e r a                                   *
*                                                                              *
*******************************************************************************/

struct Initialize_Hardware            /* opera  -->  conductor                */
  {                                   /* single task correlation request      */
  struct header bin_head;             /* generic binary message info.         */
  char ivex_key[MAX_LEN_KEY];         /* key into ivex file                   */
  U8 correlator;                      /* 0|1 = don't|do initialize correlator */
  U8 su_list[MAX_NUM_SU];             /* which SU's to initialize (0|1 = n|y) */
  }; 

struct Remove_Station                 /* opera  -->  conductor                */
  {                                   /* or conductor --> SUM                 */
  struct header bin_head;             /* generic binary message info.         */
  char station;                       /* correlation station identifier       */
  }; 

struct Send_Cond_Status               /* opera  -->  conductor                */
  {                                   /* request current status of tasks      */
  struct header bin_head;             /* generic binary message info.         */
  short interval;                     /* send stati every interval secs. if>0 */
  };                                  /* =0 send once, <0 stop sending        */

struct cs_tag
  {
  U32 task_id;                        /* task ID for this stream              */
  U16 status;                         /* status:  0=off 1=green 2=yel 3=red   */
  U16 fsm_state;                      /* finite-state machine state           */
  U16 tsf_lineno;                     /* current tsf line# (1 is first line)  */
  char tsf_name[MAX_LEN_FILNAM];      /* absolute ts file pathname            */
  char scan_suffix[16];               /* src & suffix of curr scan (or null)  */
  }; 
 
struct Cond_Status                    /* conductor --> opera                  */
  {                                   /* response to conductor status query   */
  struct header bin_head;             /* generic binary message info.         */
  struct cs_tag stream[4];            /* array of status structures by stream */
  }; 

struct Mount_Request                  /* SUM  --> opera                       */
  {                                   /* request for operator to mount tapes  */
  struct header bin_head;             /* generic binary message info.         */
  U8 ntapes;                          /* number of request structures in msg. */
  MountTape tape[MAX_NUM_TAPES];      /* one entry for each requested tape    */
  };                                  /* note that msg. length is variable!   */

struct Mount_Status                   /* opera --> SUM                        */ 
  {                                   /* information on a single mounted tape */ 
  struct header bin_head;             /* generic binary message info          */ 
  MountTape tape;                     /* info for tape whose status changed   */
  }; 

struct Dismount_Tape                  /* opera --> SUM                        */ 
  {                                   /* break log. connection of tape w/ SU  */
  struct header bin_head;             /* generic binary message info          */ 
  U8 su;                              /* SU number                            */
  }; 

struct Stream_Control                 /* opera --> conductor                  */
  {                                   /* control execution of a task stream   */
  struct header bin_head;             /* generic binary message info.         */
  U8 stream_num;                      /* which stream? (0..3)                 */
  U8 request;                         /* specifies type of control msg.       */
  U16 tsf_lineno;                     /* tsf line# at which to resume exec.   */
  char stream_filename[MAX_LEN_FILNAM];/* absolute tsf pathname               */
  }; 

enum request_specifiers
  {
  STREAM_START,
  STREAM_HOLD,
  STREAM_BREAK,
  STREAM_RESUME,
  STREAM_END 
  };

struct Reorder_Streams                /* opera --> conductor                  */
  {                                   /* change the priority order of streams */
  struct header bin_head;             /* generic binary message info.         */
  U8 stream_order[4];                 /* precedence order, high to low (0..3) */
  }; 



/*******************************************************************************
*                                                                              *
*                 t o / f r o m    m c b                                       *
*                                                                              *
*******************************************************************************/


struct Send_Tape_Status               /* anyone  -->  mcb                     */
  {                                   /* request periodic status of tape drvs.*/
  struct header bin_head;             /* generic binary message info.         */
  short interval;                     /* send stati every interval secs. if>0 */
                                      /* =0 send once, <0 stop sending        */
  U8 watch_su[MAX_NUM_SU];            /* TRUE iff corresponding SU is watched */
  U8 num_addr;                        /* number of addr. in watch list (<= 16)*/
  U8 watch_list[16];                  /* mcb addresses to be watched          */
  };

struct Tape_Status                    /* mcb   -->  any subscriber            */
  {                                   /* periodic status of tape drives       */
  struct header bin_head;             /* generic binary message info.         */
  U8 watch_su[MAX_NUM_SU];            /* TRUE iff corresponding SU is watched */
  U8 num_addr;                        /* number of addresses in watch list    */
  U8 watch_list[16];                  /* mcb addresses to be watched          */
  U16 state[MAX_NUM_SU][16];          /* 16 bit contents of watched locations */
  };

struct Dpu_Command                    /* suman, etc. <--> mcb                 */
  {                                   /* transport command or monitor response*/
  struct header bin_head;             /* generic binary message info.         */
  char rw;                            /* r = read or w = write                */
  U8 su;                              /* station unit number (0..15)          */
  U8 address;                         /* command address (0..0xFF)            */
  U16 value;                          /* command or monitor value             */
  }; 



/*******************************************************************************
*                                                                              *
*                 m i s c e l l a n y                                          *
*                                                                              *
*******************************************************************************/

struct Binary_Message
  {
  struct header bin_head;             /* generic binary message info.         */
  }; 

struct Ascii_Message                  /* anyone --> anyone                    */
  {
  struct header bin_head;             /* generic binary message info.         */
  char text[1024];                    /* arbitrary ASCII data w/ null termnatr*/
  }; 

struct Terminate_Self                 /* anyone --> anyone                    */
  {                                   /* request program to self-terminate    */
  struct header bin_head;             /* generic binary message info.         */
  U16 flags;                          /* control details of shutdown          */
  }; 

struct Program_Control                /* anyone --> anyone                    */
  {                                   /* request program to change behaviour  */
  struct header bin_head;             /* generic binary message info.         */
  int msg_level;                      /* verbosity control (-3..3)            */
  U32 prog_flags;                     /* program-specific control flags       */
  }; 

struct Error_Message                  /* anyone --> anyone                    */
  {                                   /* response to status query             */
  struct header bin_head;             /* generic binary message info.         */
  U16 errnum;                         /* error message number                 */
  char text[MAX_LEN_ERR];             /* ASCII string of error message text   */
  }; 


/*******************************************************************************
*                                                                              *
*                 EVN-style (EVN doc #42) message to set ROT's                 *
*                                                                              *
* Note 1: does *not* use the J-K (Ball-Dudevoir) messaging system              *
* Note 2: due to EVN msg alignment, some doubles are on 4 byte boundaries,     *
*         which we work around by using a U32 array of two elements instead    *
*******************************************************************************/

struct Set_Rot
    {
    U32 msg_type;                       /* action request = 0x10              */
    U32 msg_id;                         /* task_id for this ROT clock         */
    U32 ref_id;                         /*   --unused--                       */
    U32 msg_src1;                       /*   --unused--                       */
    U32 msg_src2;                       /*   --unused--                       */
    U32 msg_dest1;                      /*   --unused--                       */
    U32 msg_dest2;                      /*   --unused--                       */
/*  double time_stamp_sysclks;             COT on next systick - sanity check */
    U32 time_stamp_sysclks[2];          /* COT on next systick - sanity check */
    U32 time_stamp_date;                /*   --unused--                       */
/*  double full_rot_sysclks;                 --unused--                       */
    U32 full_rot_sysclks[2];            /*   --unused--                       */
    U32 full_rot_date;                  /*   --unused--                       */
    U32 msg_size;                       /* length = 0x7C                      */
    U32 action_code;                    /* SET_ROT = 0x10001                  */
    U32 queueing_flags;                 /*   set to 0                         */
/*  double obey_rot;                         --unused--                       */
    U32 obey_rot[2];                    /*   --unused--                       */
/*  double end_rot;                          --unused--                       */
    U32 end_rot[2];                     /*   --unused--                       */
/*  double repeat_interval;                  --unused--                       */
    U32 repeat_interval[2];             /*   --unused--                       */
    U32 num_ent;                        /*   --unused--                       */
    U32 offset;                         /*   --unused--                       */
    U32 su_array;                       /* task_id => set corresponding ROT   */
                                        /*                    -1 => set COT   */
    U32 rot[2];                         /* actually a double                  */
                                        /* set ROT to this at next systick;   */
    U32 rot_year;                       /* of current observation             */
    U32 rot_rate[2];                    /* ROT inc. per systick (in sysclks)  */
                                        /* actually a double                  */
    U32 dummy;                          /*   --unused--                       */
    };
