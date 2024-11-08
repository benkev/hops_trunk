/*
 * $Id: hook_make_plotdata.c 1025 2015-01-21 19:31:14Z gbc $
 *
 * Hook for pulling numerical data out of make_plotdata
 */

#include <stdio.h>
#include <stdlib.h>
#include "type_comp.h"

static FILE *fx = 0, *fs = 0;
static int xpspec = 0, sdelay = 0;

static void report_cp_spectrum(void);
static void report_sb_spectrum(void);
static int load_plot_arrays(double frq[], double mag[], double phs[]);
static int load_sbd_array(double lag[], double amp[]);
static void connect_pointers(double **frqp, double **magp, double **phsp);
static void connect_sbd_pntr(double **lagp, double **ampp);
static char *spectral_legend(void);
static char *sbd_legend(void);

/*
 * one time setup calls to open file(s)
 */
static int hackage_setup_xps(void)
{
    char *ep;
    ep = getenv("HOPS_FEARFIT_XPSPEC");
    xpspec = ep ? atoi(ep) : 0;
    if (xpspec) {
        fx = fopen("fearfit.spec", "w");
        if (!fx) fx = fopen("/dev/null", "w");
        if (!fx) { perror("report_cp_spectrum:fopen"); exit(1); }
    }
    return(0);  /* called once, no matter what */
}
static int hackage_setup_sbd(void)
{
    char *lp;
    lp = getenv("HOPS_FEARFIT_SDELAY");
    sdelay = lp ? atoi(lp) : 0;
    if (sdelay) {
        fs = fopen("fearfit.sdly", "w");
        if (!fs) fs = fopen("/dev/null", "w");
        if (!fs) { perror("report_sb_delay:fopen"); exit(1); }
    }
    return(0);  /* called once, no matter what */
}

/* hook function to dump out numbers of cross-power spectrum */
static void report_cp_spectrum(void)
{
    static int once = 1;
    double *frq, *mag, *phs;
    int nn, ii;
    if (once) once = hackage_setup_xps();
    if (!xpspec || !fx) return;
    connect_pointers(&frq, &mag, &phs);
    nn = load_plot_arrays(frq, mag, phs);
    fprintf(fx, "## %s\n", spectral_legend());
    for (ii = 0; ii < nn; ii++)
	fprintf(fx, "%+e %+e %+e\n", frq[ii], mag[ii], phs[ii]);
    fprintf(fx, "\n\n");
}

/* hook function to dump out numbers of sbd cross correlation */
static void report_sb_delay(void)
{
    static int once = 1;
    double *lag, *amp;
    int nn, ii;
    if (once) once = hackage_setup_sbd();
    if (!sdelay || !fs) return;
    connect_sbd_pntr(&lag, &amp);
    nn = load_sbd_array(lag, amp);
    fprintf(fs, "## %s\n", sbd_legend());
    for (ii = 0; ii < nn; ii++)
        fprintf(fs, "%+e %+e\n", lag[ii], amp[ii]);
    fprintf(fs, "\n\n");
}

#define PLOTDATA_HOOK\
    do { report_cp_spectrum(); report_sb_delay(); } while(0)
#include "make_plotdata.c"

/*
 * param is defined in type_param.h but it's already included by
 * norm.c and it's not #ifdef-wrapped for re-inclusion
 * ditto status
 *
 * Code here is cribbed from make_plotdata.c, but we'll just output
 * both sidebands.
 */
static int load_plot_arrays(double frq[], double mag[], double phs[])
{
    extern struct type_plot plot;
    extern struct type_param param;
    extern struct type_status status;
    double bandwidth, xstart, xend;
    //double c_phase(), c_mag();
    int ii, ncp;

    bandwidth = 0.25 / status.sbd_sep;
    xstart = -bandwidth;
    xend = bandwidth;
    ncp = 2 * param.nlags;
    for (ii=0; ii<ncp; ii++) {
	frq[ii] = xstart + (xend - xstart) * ii / ncp;
	//mag[ii] = c_mag (plot.cp_spectrum[ii]);
	mag[ii] = cabs (plot.cp_spectrum[ii]);
	//phs[ii] = c_phase (plot.cp_spectrum[ii]) * 180.0 / M_PI;
	phs[ii] = carg (plot.cp_spectrum[ii]) * 180.0 / M_PI;
    }
    return(ncp);
}
static int load_sbd_array(double lag[], double amp[])
{
    extern struct type_plot plot;
    extern struct type_param param;
    extern struct type_status status;
    int ii, ncp;
    ncp = 2 * param.nlags;
    /* FIXME: convert lag */
    for (ii=0; ii<ncp; ii++) {
        lag[ii] = 1000.0 * status.sbd_sep *
            (double)(ii - param.nlags) / (double)ncp;
        amp[ii] = plot.sb_amp[ii];
    }
    return(ncp);
}

static void connect_pointers(double **frqp, double **magp, double **phsp)
{
    static double frq[2*MAXMAX];
    static double mag[2*MAXMAX];
    static double phs[2*MAXMAX];
    *frqp = frq;
    *magp = mag;
    *phsp = phs;
}
static void connect_sbd_pntr(double **lagp, double **ampp)
{
    static double lag[2*MAXMAX];
    static double amp[2*MAXMAX];
    *lagp = lag;
    *ampp = amp;
}

static char *spectral_legend(void)
{
    static char mine[80];
    static int index = 0;
    extern struct type_param param;
    snprintf(mine, sizeof(mine), " index %d baseline %c%c",
	index++, param.baseline[0], param.baseline[1]);
    return(mine);
}
static char *sbd_legend(void)
{
    static char mine[80];
    static int index = 0;
    extern struct type_param param;
    snprintf(mine, sizeof(mine), " index %d baseline %c%c",
        index++, param.baseline[0], param.baseline[1]);
    return(mine);
}

/*
 * eof
 */
