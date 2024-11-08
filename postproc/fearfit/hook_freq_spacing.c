/*
 * $Id: hook_freq_spacing.c 1377 2016-07-27 13:15:13Z gbc $
 *
 * Hook which allows variants on the mbd setup made in freq_spacing.c
 * search.c:#define MBD_GRID_MAX 8192 hooked into oblivion.
 */
#include <math.h>
#include <stdlib.h>

extern void msg (char *, int, ...);

#define MBD_GRID_MAX 2*8192

/* values used in the original freq_spacing() */
#define fs_tol      0.001
#define fs_mgpb4m1  2047
#define fs_divmax   256
#define fs_power2   8

/* revised values */
//#define my_tol      0.01
#define my_mgpb4m1  (MBD_GRID_MAX/4 - 1)
#define my_divmax   10000
#define my_power2   12

static void freq_space_hook(
    double min_freq, double min_space, double *spacingp, int *grid_ptsp);

#define FREQ_SPACING_HOOK do {\
    freq_space_hook(min_freq,min_space,&spacing,&grid_pts);\
    msg ("spacing %g grid_pts %d (dammit)", 0, spacing, grid_pts); } while(0)
#define freq_spacing orig_freq_spacing
#include "freq_spacing.c"
#undef  freq_spacing

static struct type_pass *mypass;
static double my_tol = fs_tol;

/*
 * A modified version for the do {} while loop
 */
static void retry_find_grid(
    double min_freq, double min_space, double *spacingp, int *grid_ptsp)
{
    //extern struct type_param param;
    extern struct type_status status;
    struct type_pass *pass = mypass;
    int ii, fr, div, grid_pts, index, mxndx;
    double spacing, exact, mx_tol, tol;

    div = 1;
    do {
        spacing = min_space / div++;
        mx_tol = 0;
        for (fr = 0, mxndx = 0; fr < pass->nfreq; fr++) {
            exact = (pass->pass_data[fr].frequency - min_freq) / spacing;
            index = (int)(exact + 0.5);
            if (index > mxndx) mxndx = index;
            tol = fabs(exact - index);
            if (tol > mx_tol) mx_tol = tol;
            if (tol > my_tol) break;    /* to try another divisor */
            status.mb_index[fr] = (double)index;
        }
    } while (fr < pass->nfreq && div < my_divmax);

    for (ii = 1, grid_pts = 2; ii < my_power2; ii++)
        if (grid_pts - 1 < mxndx) grid_pts *= 2;
    msg("mxndx %d grid_pts %d div %d mx_tol %g", 2,
        mxndx, grid_pts, div, mx_tol);
    if ((grid_pts == 2) || grid_pts > (my_mgpb4m1)) {
        status.space_err = 1;
        grid_pts = my_mgpb4m1 + 1;
        for (fr = 0; fr < pass->nfreq; fr++)
            if (status.mb_index[fr] > grid_pts) {
                msg("grid_pts %d < freq[%d] mb_index %d, nuked", 2,
                    grid_pts, fr, status.mb_index[fr]);
                status.mb_index[fr] = 0;
            }
    }
    *spacingp = spacing;
    *grid_ptsp = grid_pts;
}

/*
 * Revised inner loop for working out the spacing
 */
static void freq_space_hook(
    double min_freq, double min_space,
    double *spacingp, int *grid_ptsp)
{
    char *ep = getenv("HOPS_FEARFIT_MBDTOL");
    my_tol = ep ? atof(ep) : 0.0;
    if (my_tol <= 0.0) return;
    msg("Modified mbd freq spacing in use, tolerance = %g (cf %g)", 2,
        my_tol, fs_tol);
    msg("min_freq %lf min_space %lf (hook entry)", 1, min_freq, min_space);
    msg("spacing %lf grid_pts %d (<=%d)", 2, *spacingp,*grid_ptsp,my_mgpb4m1);
    retry_find_grid(min_freq, min_space, spacingp, grid_ptsp);
    msg("spacing %lf grid_pts %d (updated)", 2, *spacingp, *grid_ptsp);
}

/*
 * call the original routine and expect the preceding hook to try harder.
 */
void freq_spacing(struct type_pass *pass)
{
    mypass = pass;
    orig_freq_spacing(pass);
}

/*
 * eof
 */
