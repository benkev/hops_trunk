/*
 * $Id: network.h 2174 2017-12-19 20:58:52Z gbc $
 *
 * for LS Fit, adapted from APP phase studies and Num Recipes
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <gsl/gsl_randist.h>
#include <gsl/gsl_linalg.h>

#define MAXANT 10

/*
 * Data to be fit
 */
typedef struct input {
    double  obs;                /* observed error */
    double  sig;                /* measurement sigma */
    double  chr[MAXANT];        /* +1 / -1 on baseline */
    /* from input */
    char    base[4];
    char    pozn[4];
    int     nval;
    double  redcsq;
    char    label[40];
} Inp;

/*
 * Work and options
 */
typedef struct optwork {
    /* processing options */
    int verb;
    int pnum;
    int sfix;
    int wght;
    double maxRedChiSq;
    int minNumVals;
    /* working variables */
    int nfiles;
    FILE *fpx;
    Inp *inp;
    int ninp;
    int nant;
    int nbase;
    char stations[MAXANT];
    char baselines[MAXANT*MAXANT][3];
    double answers[MAXANT];
    double covariance[MAXANT][MAXANT];
} Nopt;

/* netsolv.c */
extern int get_data(int argc, char **argv, Nopt *opt);
extern int grok_data(Nopt *opt);
extern int solve_data(Nopt *opt);
extern void load_baselines(Nopt *opt, char *blarg);
extern int skip_baseline(Nopt *opt, int ref, int rem);

/* brute.c */
extern int brute_solve(Inp *in, int nb, double *out, int na, Nopt *opt);

/*
 * eof
 */
