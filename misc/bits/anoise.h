/*
 * $Id: anoise.h 777 2012-08-30 20:50:21Z gbc $
 *
 * A deviant form of bnoise/vnoise which makes vdif formed data simulating
 * VDIF data in a variety of flavors.  The goal here is to simulate ALMA
 * data and correlate it against Mark5b-like or other flavors of data.
 */

#ifndef anoise_h
#define anoise_h

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <complex.h>
#include <fftw3.h>
#include <gsl/gsl_randist.h>
#include "hops_config.h"

#ifndef profile
# define profile static
#endif /* profile */

#define MAX_NUM_TONES           32
#define MAX_NUM_STATIONS        20
#define VDIF_HDR_OCTETS         (32/8)
#define MAX_OCTETS_PER_FRAME    1024
#define MAX_OCTETS_PER_PACKET   (VDIF_HDR_OCTETS + MAX_OCTETS_PER_FRAME)
#define HONESTY_LINE            5

/* original packet packing code required 32 here */
#define MAX_NUM_CHANNELS        256

/*
 * A representation for a generic VDIF header
 * Bit representations are considered non-portable,
 * but this is fine for our needs.
 *
 * Note this differs slightly from the vnoise.h version.
 */
typedef struct vdif_ext {
    /* word0 */
    uint32_t secs_inre:30;
    uint32_t legacy:1;
    uint32_t invalid:1;
    /* word1 */
    uint32_t df_num_insec:24;
    uint32_t ref_epoch:6;
    uint32_t UA:2;
    /* word2 */
    uint32_t df_len_octets:24;
    uint32_t num_chan_log2:5 ;
    uint32_t ver:3;
    /* word3 */
    uint32_t stationID:16 ;
    uint32_t threadID:10 ;
    uint32_t bpsm1:5 ;
    uint32_t dt:1 ;
    /* word4 */
    uint32_t magic:24;  /* formerly seql */
    uint32_t edv:8;     /* formerly edv5 */
    /* word5 */
    uint32_t status;    /* formerly seqh */
    /* word6 */
    uint32_t seql;      /* formerly psn */
    /* word7 */
    uint32_t seqh;      /* formerly eud8 */
} VdifExt;

/*
 * Just the bits from above that a human might actually want to supply.
 * Plus all the other information needed to do the deed.
 *
 * The fundamental numerology is per-us (MHz), but the global "slices"
 * variable can provide finer spectral resolution.
 */
typedef struct vdif_cnf {
    /* input information */
    char        *file;      /* name of file to hold outputdata */
    short       id;         /* station ID */
    int         epoch;      /* ref epoch */
    int         esecs;      /* secs in epoch */
    int         bytes;      /* data bytes per packet */
    int         chans;      /* data channels */
    int         srate;      /* samples per channel per us */
    double      choff;      /* channel offset (FFT units) */
    double      chgap;      /* channel gap (FFT units) */
    /* working data */
    int         smpus;      /* samples (all channels) per us */
    int         opp;        /* octets per packet */
    int         pps;        /* packets per second */
    FILE        *ofp;       /* output packet FILE */
    FILE        *osp;       /* output spectral FILE */
    char        *name;      /* output spectral file name */
    double      *work;      /* input time-series work array */
    fftw_complex*spec;      /* intermediate spectral work array */
    fftw_complex*chop;      /* intermediate spectral work array */
    fftw_plan   plan_ws;    /* FFTW3 plan work->spec */
    fftw_plan   plan_cc;    /* FFTW3 plan chop->chan */
    double      *chan;      /* output time-series work array */
    double      sigma;      /* station sigma */
    double      thresh;     /* station 2-bit threshold */
    uint64_t    *load;      /* load point into packet */
    int         shift;      /* working current shift */
    double      opus;       /* octets per us */
    int         over;       /* oversampling factor */
    double      chbw;       /* channel bandwidth in fft space */
    uint64_t    vhd[MAX_OCTETS_PER_PACKET]; /* packet */
    uint64_t    bs[MAX_NUM_CHANNELS][4];    /* bit states */
    uint64_t    bc[MAX_NUM_CHANNELS];       /* sample count */
    double      sq[MAX_NUM_CHANNELS];       /* signal^2 for RMS */
    double      ab[MAX_NUM_CHANNELS];       /* |signal| for |S| */
    double      tm[MAX_NUM_CHANNELS];       /* threshold multiplier */
} VdifConf;

/* anoise.c */
extern int  verb;

/* anoise_common.c */
extern gsl_rng *rng_inst;
extern double csigma;
extern int set_comm(char *);
extern double *fabricate_common(int);

/* anoise_maker.c */
extern int nstn, fftsps, honest, slices;
extern double dura;
extern uint64_t bsmx;
extern VdifConf vcnf[MAX_NUM_STATIONS];
extern void print_type_help(void);
extern void set_bsmx(int);
extern void set_dura(double);
extern void set_reps(double);
extern void set_thrs(double);
extern int new_station(char *name);
extern int fake_the_data(void);
extern void report_processing(int, int, char*, int);
extern void describe_station(VdifConf *);

/* anoise_extend.c */
extern void set_extn(double);
extern int extend_the_data(void);

/* anoise_station.c */
extern void fabricate_station(int, double *, int);
extern int initialize_station(int, int);
extern void destroy_station(int);

/* quant_hacker.c */
extern int  set_quant(char *);
extern double (*adcq)(double);

/* anoise_describe.c */
extern int describe_data(char *);
extern int analyze_data(char *);

#endif /* anoise_h */

/*
 * eof
 */
