#define FALSE 0
#define TRUE  1

#include <stdio.h>
#include "adata.h"

#define MAX_BNO 190			/* 20 telescopes */
#define MAX_NSEGLEN 30			/* Reasonable maximum */

typedef struct
    {
    int		order;
    int		lastorder;
    int		keyval;
    fringesum	fdata;
    } avg_data;

struct cosummary
    {
    fringesum	*datum;			/* Data structure ptr for this scan */
    float	ampl[MAX_NSEGLEN];
    float	plateau;
    float	breakpoint;
    float	slope;
    float	snr[MAX_NSEGLEN];
    float	fitsnr[MAX_NSEGLEN];
    float	seglen[MAX_NSEGLEN];
    int		navg[MAX_NSEGLEN];
    int         ign_chisq;
    int         iterations;             /* on error */
    float       chisq;                  /* on error */
    float       orig_srch;              /* for plotting */
    int         snr_peak[3];            /* indicies thereof */
    };

extern void ampf(float x, float a[], float *y, float dyda[], int na);
extern int fit_ampl(struct cosummary *, int );
extern int normalize_snr(struct cosummary *, int );
extern int fit_snr(struct cosummary *, int );
extern void sorter(avg_data *, int);
extern int parse_cmdline(int, char **, FILE **, int *, int *);
extern int read_data(avg_data **, char *, int *);
extern int sort_data(avg_data *, int);
extern void fit_codata (struct cosummary *);
extern void plot_codata (struct cosummary *);
extern int write_codata (struct cosummary *, FILE *);
extern void clear_codata (struct cosummary *);

/*
 * eof
 */
