/*
 * $Id: dsolv3.h 1778 2017-06-15 21:13:13Z gbc $
 *
 * Re-implementation of dsolv2 in C to allow use of GSL
 * routines, and later, direct read of alists, &c.
 */

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gsl/gsl_multifit.h>

#define LIGHT_SPEED (299.279458)        /* Mm/s */
#define DEG_PER_RAD (180.0 / M_PI)      /* degrees per radian */

/* observables from Alist format, with some working vars for design matrix */
typedef struct aobs {
    int year;                       /* input year */
    int doy, hhmm;                  /* input day an hhmm */
    double ra, dec;                 /* input source ra,dec (hr,deg) */
    double rate, delay;             /* input baseline rate,delay (us,us) */
    char bl[4];                     /* (optional) baseline */
    double snr;                     /* (optional) snr */
    double ref_freq;                /* (optional) ref freq */
    char polarization[4];           /* (optional) polarization */
    char source[32];                /* (optional) source name */
    int time1980;                   /* (optional) obs start secs since 1980 */
    double gst0;                    /* (optional) more GMST origin */
    int hour, minute;               /* working hour and minute */
    double dutcus;                  /* utc hrs relative to ref (us) */
    double harad, decrad;           /* working source ha,dec (radians) */
} Aobs;

typedef int (Dsolver)(void);

typedef struct dopts {
    /* command-line items */
    int verb;
    int quiet;
    char *input;
    char *alist;
    char *base;
    double snr;
    double ref_freq;
    char *polarization;
    int precess;
    int fixGMST;
    int redCHI;
    /* working data */
    int numb;
    Aobs *data;
    Dsolver *solver;
} Dopt;

/* most parameters are managed here: */
extern Dopt opt;

/* things in the minimal dsolv2-like gsl implementation */
extern Aobs *scan_file(char *data, int *num);
extern double dangg(int hhmm);
extern void make_working_data_approx(Aobs *inp, int num);
extern gsl_matrix *make_design_matrix(Aobs *inp, int num);
extern void report_design_matrix(Aobs *inp, gsl_matrix *X, int num);
extern gsl_vector *make_delay_vector(Aobs *inp, int num);
extern gsl_vector *calc_parameters(
    int num, gsl_matrix *X, gsl_vector *y, double *csq);
extern gsl_vector *calc_residuals(
    int num, gsl_matrix *X, gsl_vector *y, gsl_vector *c);
extern void report_results(int num, Aobs *inp,
    gsl_vector *resids, gsl_vector *params, double chisq);
extern int dsolv2(char *data);

/* main routine for extended functionality */
extern int dsolv3_main(int argc, char **argv);

/* load an alist and reduce the data to Aobs form */
extern int load_alist(char *alist);

/*
 * eof
 */
