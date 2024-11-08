/*****************************************
* Calc input data structure -cmn 8/19/93 *
* added freq. quantities     rjc 94.5.20 *
*****************************************/
#include "mk4_sizes.h"

#define MAX_OBS 1000                           /* max # of total observations */
#define MAX_STAT 20                                      /* max # of stations */
#define MAX_SRC  100                                     /* max # of sources  */
#define MAX_LENGTH 30                          /* max # of points per station */
#define MAX_UT1 10                             /* max # of ut1/wobbble points */

struct calc_data_struct 
    {
    double      tai_utc;                                        /* TAI - UTC  */
    double      a1_tai;                                         /* A1 - TAI   */
    double      ut1;                            /* ut1 reference epoch (J.D.) */
    double      n_ut1;                                /* number of UT1 points */
    double      intrvl;                    /* Separation of UT1/wobble points */
    double      ut1_pts[MAX_UT1];                         /* UT1 points (sec) */
    double      wob_xyt[MAX_UT1][2];             /* wobble points array (mas) */
    double      ref_freq;                   /* ref. freq. for phase delay MHz */
    int         eph_epoch;
    int         tidal_ut1;              /* -1 if fortnightly ut1 isn't needed */
    int         axis_type[MAX_STAT];                 /* offset axis type code */

        
    double      site_recs[MAX_STAT][3];            /* site co-ordinates array */
    double      axis_offs[MAX_STAT];                         /* axis offsets  */
        
    struct clock_struct 
        {
        double  offset;
        double  rate;
        int     year;
        int     day;
        int     hour;
        int     min;
        } clock_offs[MAX_STAT];                             /* clock offsets  */
        
    struct freq_struct 
        {
        double total_lo;
        double bandwidth;                         /* channel bandwidth in MHz */
        char chan_id[32];
        char sideband;                                              /* U or L */
        } channels[MAX_STAT][MAX_CHAN_RT];
    int num_chan[MAX_STAT];                     /* actual # of chans. present */

    double site_zens[MAX_STAT];                  /* troposperic zenith delays */
    double starpos[2];                          /* star coord. array (RA/Dec) */
    double str_part[2][2];              /* partials wrt source;filled by CALC */
    double sit_oc_amp;                             /* ocean loading amplitude */
    double sit_oc_phs;                                /* ocean loading phases */
    double sit_h_oc_amp;                      /* horizontal ocean loading amp */
    double sit_h_oc_phs;                      /* horizontal ocean loading phs */
        
    int n_sites;                                                /* # of sites */
    int n_stars;                                              /* # of sources */
    int n_obs;                                           /* # of observations */
    int num_pts;                                 /* # of points for all sites */
    int spline_start;                   /* Start time of splines for all stns */
    char mk4_site_id[MAX_STAT];                 /* antenna codes for each sta */
    char site_intl_id[MAX_STAT][2];       /* International codes for each sta */
    char site_names[MAX_STAT][9];                               /* site names */
    char star_name[9];                                          /* star name  */
    int error_code;                          /* error code returned from CALC */
    }   calc_data;

struct observation
    {
    int         seqno;                       /* Sequential observation number */
    double      sec_tag;                   /* Seconds portion of the time tag */
    int         utc_tag[5];                    /* UTC time tag of observation */
    char        baseline[2][9];                              /* Station names */
    char        src[9];                                        /* Source name */
    double      delay;                     /* Delay & rate from Shapiro model */
    double      rate;
    double      delay_spline[MAX_CHAN_RT][6];     /* quintic spline coefficients */
    double      phase_spline[MAX_CHAN_RT][6];     /* for delay and phase models  */
    };
   
struct observation obs_data[MAX_OBS];

int rec_num;    

