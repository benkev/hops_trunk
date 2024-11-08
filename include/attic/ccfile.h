/************************************************************************/
/*									*/
/* This is the header file containing all the definitions required for	*/
/* the cc file io package to operate.  It also defines the		*/
/* structure in which the contents of a cc file are stored, and		*/
/* is the reference from which one determines how to retrieve specific	*/
/* information from memory.						*/
/*									*/
/* Created  Aug/Sept 1990 by CJL and P. Shaughnessy			*/
/*									*/
/************************************************************************/

#ifndef CCFILE_H
#define  CCFILE_H

#define MAX_STATIONS 20
#define MAX_FREQS 16
#define MAX_GROUPS 20
#define MAX_SOURCES 100
#define MAX_PASS 61		/* Same as number supported at correlator */
#define MAX_ESPIN 10
#define MAX_CONFIG 200
#define MAX_TAPE 50
#define MAX_ADHOC 50
#define MAX_STATUS 10
#define MAX_SCHEDS 2000
#define MAX_COMMENTS 500
#define SOURCE_STRLEN 9

#define START    0
#define EXPER    1
#define STAT     2
#define CLOCK    3
#define FREQ     4
#define SUBGROUP 5
#define HEAD     6
#define SCHED    7
#define SOURCE   8
#define ESPIN    9
#define CONFIG   10
#define TAPE     11
#define ADHOC    12
#define STATUS   13

typedef struct
    {
    int nstations;
    int nclocks;
    int nfreqs;
    int nsubgroups;
    int nheads;
    int nscheds;
    int nsources;
    int nespins;
    int nconfigs;
    int ntapes;
    int nadhocs;
    int nstatus;
    int ncomments;
    } Count;

/***************************************************************************

Section Definitions:

INDEX section: */

#define INDEX_MASK 1<<0

extern int index_parse();

typedef struct
    {
    int exper;
    int stat;
    int clock;
    int freq;
    int subgroup;
    int head;
    int sched;
    int source;
    int espin;
    int config;
    int tape;
    int adhoc;
    int status;
    int nscheds;
    } Index;

/***************************************************************************

Section Definitions:

EXPER section: */

#define EXPER_MASK 1<<1

extern int exper_parse();

/***************************************************************************

STAT section:  */

#define STAT_MASK 1<<2

extern int stat_parse();

typedef struct
    {
    char station_id;
    char name[9];
    int id_number;
    double x;
    double y;
    double z;
    double zatm;
    char occucode[9];
    } Stat;

/***************************************************************************

CLOCK section: */

#define CLOCK_MASK 1<<3

extern int clock_parse();

typedef struct
    {
    char clock_id;
    double offset;
    double rate;
    int epoch_year;
    int epoch_day;
    int epoch_hour;
    int epoch_min;
    double instr_delay;
    } Clock;

/***************************************************************************

FREQ section: */

#define FREQ_MASK 1<<4

extern int freq_parse();

typedef struct
    {
    char station;
    double offset;
    } lofst;

typedef struct
    {
    char group[3];
    char subgroup[3];
    double sky;
    double phasecal;
    int tracks_pass1[2];
    int tracks_pass2[2];
    int nlofst;
    lofst lo_offset[MAX_STATIONS];
    } Freq;

/***************************************************************************

SUBGROUP section: */

#define SUBGROUP_MASK 1<<5

extern int subgroup_parse();

typedef struct
    {
    char station;
    char group[3];
    char sgroup[MAX_FREQS][3];
    } Subgroup;

/***************************************************************************

HEAD section: */

#define HEAD_MASK 1<<6

extern int head_parse();

typedef struct
    {
    char pass;
    int equiv_pass;
    int head_offset;
    } passinfo;

typedef struct
    {
    char station;
    char group[3];
    int npass;
    passinfo pass[MAX_PASS];
    } Head;

/***************************************************************************

SCHED section: */

#define SCHED_MASK 1<<7
#define STATION_STRLEN MAX_STATIONS*6

extern int sched_parse();

typedef struct
    {
    char station;
    char clock;
    char pass;
    char tapekey;
    char adhoc[3];
    } station_info;

typedef struct sched_line_tag
    {
    int year;			/* Time of observation */
    int day;
    int hour;
    int minute;
    int second;
    int nlengths;
    int length[MAX_STATIONS];	/* Length of scan, one for every station */
    char source[9];		/* Name of source */
    int nstat;			/* Number of stations this scan */
    station_info stn[MAX_STATIONS];	/* Station tape/pass/clock
					 * information */
    char mode[5];		/* Mode string */
    int period;			/* Accumulation period */
    int kmode;			/* Long integer containing kmode bits */
    } Sched;

/***************************************************************************

SOURCE section: */

#define SOURCE_MASK 1<<8

extern int source_parse();

typedef struct
    {
    char name[9];
    char alias[9];
    int rahour;
    int ramin;
    double rasec;
    int decdeg;
    int decmin;
    double decsec;
    double epoch;
    } Source;

/***************************************************************************

ESPIN section: */

#define ESPIN_MASK 1<<9

extern int espin_parse();

typedef struct
    {
    int year;
    int day;
    double ut1_utc;
    double xwob;
    double ywob;
    } Espin;

/***************************************************************************

CONFIG section: */

#define CONFIG_MASK 1<<10

extern int config_parse();

typedef struct
    {
    char line[128];		/* Hardware-specific, wait for Mk4 */
    } Config;

/***************************************************************************

TAPE section: */

#define TAPE_MASK 1<<11

extern int tape_parse();

typedef struct
    {
    char line[128];		/* Hardware-specific, wait for Mk4 */
    } Tape;

/***************************************************************************

ADHOC section: */

#define ADHOC_MASK 1<<12
#define ADHOC_MAX_INTS 20
#define ADHOC_MAX_DOUBLES 20

extern int adhoc_parse();

typedef struct
    {
    char key[3];
    int type;
    int ninteger;
    int ndouble;
    int integers[ADHOC_MAX_INTS];
    int doubles[ADHOC_MAX_DOUBLES];
    } Adhoc;

/***************************************************************************

STATUS section: */

#define STATUS_MASK 1<<13

extern int status_parse();

typedef struct
    {
    char line[100];
    } Status;

/***************************************************************************

Comments: */

typedef struct
    {
    char *text;
    short section;
    short position;
    short append;
    } Comment;

/***************************************************************************

   The following definitions should remain at the end of this file and allow
   the main loop to access things correctly; new additions should be entered
   here as well:

*/

#define NUMBER_SECTIONS 14

static char *section_names[] =
    {
    "INDEX",
    "EXPER",
    "STAT",
    "CLOCK",
    "FREQ",
    "SUBGROUP",
    "HEAD",
    "SCHED",
    "SOURCE",
    "ESPIN",
    "CONFIG",
    "TAPE",
    "ADHOC",
    "STATUS"
   };

static int (*parse_procedures[]) () =
    {
    index_parse,
    exper_parse,
    stat_parse,
    clock_parse,
    freq_parse,
    subgroup_parse,
    head_parse,
    sched_parse,
    source_parse,
    espin_parse,
    config_parse,
    tape_parse,
    adhoc_parse,
    status_parse
    };

/* Definition of global structure - this is returned by the main procedure;
   it should contain all of the data read and parsed from the cc file.
*/

typedef struct
    {
    Count counters;
    char exper_name[100];
    int exper_num;
    Index index;
    Stat stat[MAX_STATIONS];
    Clock clock[MAX_STATIONS + 20];
    Freq freq[MAX_FREQS * MAX_GROUPS];
    Subgroup subgroup[MAX_STATIONS];
    int global_head_offset;
    Head head[MAX_STATIONS * 10];
    Sched *sched;
    Source source[MAX_SOURCES];
    Espin espin[MAX_ESPIN];
    Config config[MAX_CONFIG];
    Tape tape[MAX_TAPE];
    Adhoc adhoc[MAX_ADHOC];
    Status status[MAX_STATUS];
    Comment com[MAX_COMMENTS];
    } ccontrol_file;

extern ccontrol_file *read_ccfile();
extern int read_line();

#ifndef NULL
   #define NULL 0
#endif
#define TRUE 1
#define FALSE 0

extern char * print_comment();

#endif
