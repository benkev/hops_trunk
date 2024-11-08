/*
 * Do some math on bits for N antennas
 *
 * $Id: b4antsum.c 763 2012-08-10 18:22:43Z gbc $
 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern int verb;

extern double eta4(double enn, double vs, double *pphi);

/*
 * Compute the probabilities with recursion from the N-1 case
 */
static void update_probs(double prb[4], double *probs, double *prnm1, int nn)
{
    int pp;
    if (nn == 1) {
        for (pp = 0; pp < 4; pp++) probs[pp] = prb[pp];
    } else {
        for (pp = 0; pp <= 3*nn; pp++) {
            probs[pp] = prb[0] * prnm1[pp];
            if (pp < 1) continue;
            probs[pp] += prb[1] * prnm1[pp-1];
            if (pp < 2) continue;
            probs[pp] += prb[2] * prnm1[pp-2];
            if (pp < 3) continue;
            probs[pp] += prb[3] * prnm1[pp-3];
        }
    }
}

/*
 * Convert from index space to -3,-1,1,3 space
 */
static double xaxis(int pp, int nn)
{
    double dn = (double)nn;
    double dp = (double)pp;
    return( 2 * (dp - 1.5 * dn) / sqrt(dn) );
}

/*
 * Compute lookup table contents for this entry
 */
static void lookuptable(int pp, int nn, int state, int *index, int *hi, int *lo)
{
    int vals[] = { 0x2, 0x3, 0x0, 0x1 };    /* -3, -1, 1, 3 */
    *hi = (vals[state] & 0x2) ? 1 : 0;
    *lo = (vals[state] & 0x1) ? 1 : 0;
    *index = (2 * pp - 3 * nn);
    *index &= 0x1FF;
    *index >>= 1;
}

/*
 * Print out a one-liner
 */
static void display_probs(double *probs, int nn, double prb[4], int final)
{
    int pp, thr[3], tt, labi = 0, trigger = 0, cnts[4];
    double intp = 0.0, last = 0.0, iprb[3], error = 0.0;
    char labs[4][5] = { "--", " -", " +", "++" };
    int index, hibit, lobit;
    memset(cnts, 0, sizeof(cnts));
    iprb[0] = prb[0];
    iprb[1] = prb[0] + prb[1];
    iprb[2] = prb[0] + prb[1] + prb[2];
    if (final) fprintf(stdout,
        "#%8s %9s(%02d)%10s %10s %3s %2s %12s%3s %3s\n",
        "x(v)", "prob*sqrt", nn,
        "I(prob<x)", "I(prob<=x)", "-/+",
        "v", "prob", "in", "out");
    for (pp = 0; pp <= 3*nn; pp++) {
        last = intp;
        intp += probs[pp];
        for (tt = 0; tt<3; tt++)
            if (last < iprb[tt] && iprb[tt] <= intp) {
                thr[tt] = pp;
                trigger = tt+1;
                /* if closer to upper threshold, do it now */
                if (iprb[tt] - last <= intp - iprb[tt]) labi = trigger;
            }
        cnts[labi] ++;
        if (labi == 0) error = intp;
        lookuptable(pp, nn, labi, &index, &hibit, &lobit);
        if (final && (probs[pp] > 1e-11 || pp == 0 || pp == 3*nn))
            fprintf(stdout,
                " %8.4f %.10f %.8f %.8f %s %3d %.10f %2X %1d%1d\n",
                xaxis(pp, nn),
                probs[pp]*sqrt((double)nn), last, intp, labs[labi],
                pp, probs[pp],
                index, hibit, lobit);
        labi = trigger;
    }
    if (verb>0) fprintf(stdout, "##%s-%03d %3d %3d %3.0f %3d %3d %.1f%%\n",
        2*(nn/2) == nn ? "even" : "-odd", nn,
        cnts[0], cnts[1], floor(sqrt(nn+1.0)),
        cnts[2], cnts[3], 100*fabs(error - prb[0]));
    if (final) fprintf(stdout, "\n\n");
}

/*
 * Driver for these calcuations
 * The arguments are:
 *   representation value for the +/- hi bit (n)
 *   lo/hi threshold in sigma units
 */
int b4antsum(int argc, char **argv)
{
    double enn  = atof(argv[1]), phi, prb[4];
    double vs   = atof(argv[2]), eta, *probs, *prnm1;
    int nant = atoi(argv[3]), nn;
    if (verb>0) printf("# Evaluating for n=%f, vo/sigma=%f with %d antennas\n",
        enn, vs, nant);
    eta = eta4(enn, vs, &phi);
    if (verb>0) printf("# eta4(%.8f,%.8f) = %.8f phi(%.8f/sqrt(2)) = %.8f\n",
            enn, vs, eta, vs, phi);
    prb[1] = prb[2] = phi / 2.0;
    prb[3] = prb[0] = 0.5 - prb[1];
    if (verb>0) printf("# One antenna probabilities: %f %f %f %f\n",
        prb[0], prb[1], prb[2], prb[3]);
    probs = malloc(3 * nant * sizeof(double));
    prnm1 = malloc(3 * nant * sizeof(double));
    if (!probs || !prnm1) return(perror("malloc"),1);

    for (nn = 1; nn <= nant; nn++) {
        update_probs(prb, probs, prnm1, nn);
        display_probs(probs, nn, prb, (nn == nant) || (verb>2));
        memcpy(prnm1, probs, 3 * nant * sizeof(double));
    }

    return(0);
}

/*
 * eof
 */
