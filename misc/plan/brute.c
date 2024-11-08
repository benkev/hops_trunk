/*
 * $Id: brute.c 1785 2017-06-19 21:21:52Z gbc $
 *
 * Brute force solution of LS Fit problem, modified
 * from APP phase study application.  In this case
 * we cannot arbitrarily argue that any station error
 * should be zero....
 * 
 * The logic actually is well laid out in Num.Recipes
 *   y = sum(a X)
 * y is observed baseline error, a is position error, x is baseline
 * X is the baseline characteristic function (+1 or -1)
 *
 * Design matrix:
 *   A_ij = Xj(x_i) / s_i    i e 1..N baseline observations
 * Observations:
 *   b_i  = y_i / s_i        j e 1..M stations/antennas
 *
 * Minimization gives alpha = A^T . A, beta = A^T . b
 * and inverting alpha for solution  alpha^-1 . b = a (answer)
 */

#include "network.h"

/*
 * For debugging
 */
static void show_vector(char *lab, gsl_vector *v, int nb, Nopt *opt)
{
    int bb;
    fprintf(opt->fpx, "%s", lab);
    for (bb = 0; bb < nb; bb++)
        fprintf(opt->fpx, " %4d %+8.4g\n", bb, gsl_vector_get(v, bb));
}
static void show_matrix(char *lab, gsl_matrix *m, int na, Nopt *opt)
{
    int ii, jj;
    fprintf(opt->fpx, "%s", lab);
    for (ii = 0; ii < na; ii++) {
        fprintf(opt->fpx, " %2d ", ii);
        for (jj = 0; jj < na; jj++)
            fprintf(opt->fpx, " %+8.4g ", gsl_matrix_get(m, ii, jj));
        fprintf(opt->fpx, "\n");
    }
}

/*
 * Do the LSF of the A.x = B system
 */
int brute_solve(Inp *in, int nb, double *out, int na, Nopt *opt)
{
    double atmp, btmp;
    int bb, ii, jj, kk, err;
    gsl_matrix *amtx = gsl_matrix_alloc(na, na);
    gsl_vector *bvec = gsl_vector_alloc(na);
    gsl_permutation *perm = gsl_permutation_alloc(na);
    gsl_vector *answ = gsl_vector_alloc(na);
    gsl_matrix *covr = gsl_matrix_alloc(na, na);

    /* create A (aka alpha) */
    gsl_matrix_set_zero(amtx);
    for (bb = 0; bb < nb; bb++) {
        for (ii = 0; ii < na; ii++)
            for (jj = 0; jj < na; jj++) {
                atmp = gsl_matrix_get(amtx, ii, jj);
                atmp += in[bb].chr[ii] * in[bb].chr[jj]
                    / (in[bb].sig * in[bb].sig);
                gsl_matrix_set(amtx, ii, jj, atmp);
            }
    }
    if (opt->verb>2) show_matrix("# A matrix:\n", amtx, na, opt);

    /* create B (aka beta) */
    gsl_vector_set_zero(bvec);
    for (bb = 0; bb < nb; bb++) {
        for (jj = 0; jj < na; jj++) {
            btmp = gsl_vector_get(bvec, jj);
            btmp += in[bb].obs * in[bb].chr[jj] / (in[bb].sig * in[bb].sig);
            gsl_vector_set(bvec, jj, btmp);
        }
    }
    if (opt->verb>2) show_vector("# B vector:\n", bvec, na, opt);

    /* ok, compute the answer as A^-1 . B */
    bb = 0;
    err = gsl_linalg_LU_decomp(amtx, perm, &bb);
    if (err) return(1);
    err = gsl_linalg_LU_solve(amtx, perm, bvec, answ);
    if (err) return(2);
    err = gsl_linalg_LU_invert(amtx, perm, covr);
    if (err) return(3);

    /* copy out the answer */
    for (ii = 0; ii < na; ii++)
        out[ii] = gsl_vector_get(answ, ii);

    /* copy out the inverse */
    for (ii = 0; ii < na; ii++)
        for (jj = 0; jj < na; jj++)
            opt->covariance[ii][jj] = gsl_matrix_get(covr, ii, jj);

    /* free underlying allocations */
    gsl_matrix_free(amtx);
    gsl_vector_free(bvec);
    gsl_permutation_free(perm);
    gsl_vector_free(answ);

    return(0);
}

/*
 * eof
 */
