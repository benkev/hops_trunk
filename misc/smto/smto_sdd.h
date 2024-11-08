/*
 * $Id$
 *
 * Program to take apart SMTO sdd files.
 * Based on code written by AEER;
 * later hacked into oblivion by GBC.
 */

/* bootstrap file information */
typedef struct {
    int numrec;		    /* in index, including this record */
    int numdata;	    /* records following index */
    int bytes_per_rec;
    int bytes_per_index;
    int	num_used;	    /* index entries */
    int counter;	    /* flagging updates */
    int sddtyp;		    /* 0 if data, 1 if individual records */
    int sddver;		    /* better be 1 */
    char pad[512 - 8*sizeof(int)];
} SddBoots;

/* some sort of scan record */
typedef struct {
    int scanstart;	    /* starting record number for scan */
    int scanstop;	    /* final record number for scan */
    float hor;		    /* coordinate, in degrees */
    float vert;		    /* coordinate, in degrees */
    char scan[16];	    /* source name */
    float scannum;
    float fres;		    /* spectral scans: freq resol (MHz)
			       continuum scans: slew rate (deg/s) */
    double restfreq;	    /* spectral scans: freq of obs (MHz)
			       continuum scans: int time / point */
    float lst;		    /* local time in hours */
    float utdate;	    /* YYYY.MMDD format */
    short mode;		    /* observing mode, see below */
    short recnum;	    /* record_num * 64 + phase_num */
    short poscode;	    /* position code */
    char pad[512 - 2*sizeof(int) - 6*sizeof(float) - sizeof(double)
		 - 3*sizeof(short) - 16];
} SddIndex;

/* SDD Observational Mode Code */
/* Table defined in smto_sdt.c: */
/*  Continuum    = type_code + 256 */
/*  Spectralline = type_code + 512 */
typedef struct sdd_type {
    int code;
    char name[8];
    char desc[50];
} SddTypeDesc;

/* preamble for each scan */
typedef struct preamble {
     short numclass;
     short classstart[15];
} SddPreamble;

/* Basic Information */
typedef struct class1 {
    char pad;
} SddClass1;

/* Pointing Parameters */
typedef struct class2 {
    char pad;
} SddClass2;

/* Observing Parameters */
typedef struct class3 {
    char pad;
} SddClass3;

/* Positions */
typedef struct class4 {
    char pad;
} SddClass4;

/* Environment */
typedef struct class5 {
    char pad;
} SddClass5;

/* Map Parameters */
typedef struct class6 {
    char pad;
} SddClass6;

/* Data Parameters */
typedef struct class7 {
    char pad;
} SddClass7;

/* Engineering Parameters */
typedef struct class8 {
    char pad;
} SddClass8;

/* Telescope Dependent Parameters, Green Bank */
typedef struct class9gb {
    char pad;
} SddClass9GB;

/* Telescope Dependent Parameters, Tucson 12m */
typedef struct class9tt {
    char pad;
} SddClass9TT;

/* Open Parameters */
typedef struct class10 {
    char pad;
} SddClass10;

/* Phase Block CL11Type = "ORIG" */
typedef struct class11orig {
    char pad;
} SddClass11Orig;

/* Phase Block CL11Type = "PROTO" */
typedef struct class11proto {
    char pad;
} SddClass11Proto;

/* Descriptor Block for each Receiver Channel */
typedef struct class12 {
    char pad;
} SddClass12;

/* Reduction Parameters */
typedef struct class13 {
    char pad;
} SddClass13;

/*
 * prototypes
 */
extern int parse_file(char *file, char *plot);

extern int noplot, verb, min_record, max_record;

/*
 * eof
 */
