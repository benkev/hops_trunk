/*
 * $Id: dsolv3.c 1790 2017-06-22 21:11:26Z gbc $
 *
 * Re-implementation of dsolv2 in C to allow use of GSL
 * routines, and later, direct read of alists, &c.
 *
 * Many thanks to Brian Corey for the first steps....
 */

#include "dsolv3.h"

/*
 * Scan the lines of the file expecting 6 quantities per line
 */
Aobs *scan_file(char *data, int *num)
{
    static char line[256];
    int nall = 100, ninp = 0, ns, warn = 0, pos;
    Aobs *inp = malloc(nall * sizeof(Aobs));
    FILE *fp = fopen(data, "r");
    if (!inp) { perror("scan_file:malloc:1"); *num = 0; return(0); }
    if (!fp) { perror("scan_file:fopen"); *num = 0; return(0); }
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#') continue;   /* skip comments */
        memset(&inp[ninp], 0, sizeof(Aobs));
        pos = 0;
        ns = sscanf(line, "%d %d %lf %lf %lf %lf%n",
            &inp[ninp].doy, &inp[ninp].hhmm, &inp[ninp].ra,
            &inp[ninp].dec, &inp[ninp].rate, &inp[ninp].delay, &pos);
        if (ns < 6) {
            fprintf(stderr, "Insufficient input at line %d of %s\n",
                ninp, data);
            return(0);
        }
        fprintf(stdout, "### %s", line+pos);
        if (pos > 0) {
            while (isspace(line[pos])) pos++;
            inp[ninp].bl[0] = line[pos+0];
            inp[ninp].bl[1] = line[pos+1];
            inp[ninp].bl[2] = 0;
        }
        if (! isalnum(inp[ninp].bl[0])) inp[ninp].bl[0] = '?';
        if (! isalnum(inp[ninp].bl[1])) inp[ninp].bl[1] = '?';
        inp[ninp].bl[2] = 0;

        if (inp[0].bl[0] != inp[ninp].bl[0] ||
            inp[0].bl[1] != inp[ninp].bl[1]) warn++;
        if (++ninp >= nall) {
            inp = realloc(inp, nall += 100);
            if (!inp) { perror("scan_file:malloc:2"); *num = 0; return(0); }
        }
    }
    if (warn > 0) fprintf(stderr, "Warning, mixed baselines (%d)\n", warn);
    fclose(fp);
    *num = ninp;
    return(inp);
}

/*
 * Convert hhmm into decimal hours (ra) or degrees (dec)
 */
double dangg(int hhmm)
{
    double dang, hh, h1, sgn = 1.0;
    hh = fabs((double)hhmm);
    if (hhmm < 0) sgn = -1.0;
    h1 = (int)(hh / 100.0);
    dang = sgn * (h1 + (hh - h1 * 100.0) / 60.0);
    return dang;
}

/*
 * Simple reductions of data, results stored in work elements of Aobs.
 * This version makes the same approximations as dsolv2.
 *
 * GMST is 17 days (exactly) at 4am on day 264 of 2016,
 * and each day gets us a 24hr/365.25 shift in the sky.
 */
void make_working_data_approx(Aobs *inp, int num)
{
    int nn;
    double utcref, gst0, ghahr, diy;
    for (nn = 0; nn < num; nn++, inp++) {
        /* for output */
        inp->hour = inp->hhmm / 100;
        inp->minute = inp->hhmm - 100*inp->hour;
        /* for design matrix */
        inp->dutcus = dangg(inp->hhmm);
        if (inp->gst0 == 0.0) {     /* fixGMST not used */
            gst0 = (inp->doy - 264) * 24.0 / 365.25;
            if (inp->year > 101) {  /* usual 1900 issue */
                diy = (((inp->year-1) % 4) == 0) ? 366.0 : 365.0;
                gst0 += (inp->year - 116) * 24.0 * diy / 365.25;
            }
            ghahr = gst0 + inp->dutcus - inp->ra;
        } else {
            ghahr = inp->gst0 + inp->dutcus - inp->ra;
        }
        if (nn == 0) utcref = inp->dutcus;
        inp->dutcus -= utcref;
        inp->dutcus *= 3600.0 * 1.e-6;
        inp->harad = ghahr * 15.0 / DEG_PER_RAD;
        inp->decrad = inp->dec / DEG_PER_RAD;
    }
}

/*
 * Populate the design matrix
 */
gsl_matrix *make_design_matrix(Aobs *inp, int num)
{
    gsl_matrix *X = gsl_matrix_alloc(num, 5);
    double tmp[5];
    int nn;
    if (!X) return(X);
    if (opt.verb>1) fprintf(stdout, "#      design matrix row elements\n");
    for (nn = 0; nn < num; nn++, inp++) {
        gsl_matrix_set(X, nn, 0, (tmp[0] = 
            - cos(inp->decrad) * cos(inp->harad) / LIGHT_SPEED) );
        gsl_matrix_set(X, nn, 1, (tmp[1] = 
              cos(inp->decrad) * sin(inp->harad) / LIGHT_SPEED) );
        gsl_matrix_set(X, nn, 2, (tmp[2] = 
            - sin(inp->decrad) / LIGHT_SPEED) );
        gsl_matrix_set(X, nn, 3, (tmp[3] = 1.0) );
        gsl_matrix_set(X, nn, 4, (tmp[4] = inp->dutcus) );
        if (opt.verb>1) fprintf(stdout,
            "%3d%6s %10.5f %10.5f %10.5f %10.5f %10.5f %+.2f %+.2f\n",
            nn+1, " ", tmp[0], tmp[1], tmp[2], tmp[3], tmp[4],
                inp->decrad, inp->harad);
    }
    return(X);
}

/*
 * Output design matrix in same format as dsolv2
 */
void report_design_matrix(Aobs *inp, gsl_matrix *X, int num)
{
    int nn, kk;
    if (opt.verb == 0) return;
    fprintf(stdout, "#n HH MM design matrix row elements\n");
    for (nn = 0; nn < num; nn++) {
        fprintf(stdout, "%3d%3d%3d",
            nn+1, inp[nn].hour, inp[nn].minute);
        for (kk = 0; kk < 5; kk++)
            fprintf(stdout, "%10.5f", gsl_matrix_get(X,nn,kk));
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "#Read a total of %5d points\n", num);
}

/*
 * Populate the vector of delays
 */
gsl_vector *make_delay_vector(Aobs *inp, int num)
{
    gsl_vector *y = gsl_vector_alloc(num);
    int nn;
    if (!y) return(y);
    for (nn = 0; nn < num; nn++, inp++)
        gsl_vector_set(y, nn, inp->delay);
    return(y);
}

/*
 * This does a simple non-weighted multiparameter least squares fit
 */
gsl_vector *calc_parameters(int num, gsl_matrix *X, gsl_vector *y, double *csq)
{
    gsl_vector *c = gsl_vector_alloc(5);
    gsl_matrix *cov = gsl_matrix_alloc(5,5);
    gsl_vector *r = gsl_vector_alloc(num);
    gsl_multifit_linear_workspace *work = gsl_multifit_linear_alloc(num,5);

    gsl_multifit_linear(X, y, c, cov, csq, work);
    gsl_multifit_linear_free(work);

    gsl_matrix_free(cov);
    return(c);
}

/*
 * And this calculates residuals
 */
gsl_vector *calc_residuals(int num, gsl_matrix *X, gsl_vector *y, gsl_vector *c)
{
    gsl_vector *r = gsl_vector_alloc(num);
    gsl_multifit_linear_residuals(X, y, c, r);
    return(r);
}

/*
 * Something to report results
 *
 * 7.29e-5 = 2 * 3.1415926 / (86400 * (365.25/366.25))
 *
 * which is the Earth's siderial rotation rate (radians/sec).
 */
void report_results(int num, Aobs *inp,
    gsl_vector *resids, gsl_vector *params, double chisq)
{
    int nn;
    double ratemod, avedsq = 0.0, altdsq = 0.0, p0, p1, p4, del;
    char parameters[] = "parameters";
    if (opt.quiet == 0) fprintf(stdout,
        "#DOY HH MM    RAhhmm   decddmm   delay"
        "     dlymod    dlyres    GHA(hr) ratemod\n");
    p0 = gsl_vector_get(params,0);
    p1 = gsl_vector_get(params,1);
    p4 = gsl_vector_get(params,4);
    for (nn = 0; nn < num; nn++) {
        ratemod = cos(inp[nn].decrad) * (
            p0 * sin(inp[nn].harad) + p1 * cos(inp[nn].harad) );
        ratemod *= 7.29e-5 * 1.e6 / LIGHT_SPEED;
        ratemod += p4;
        if (opt.quiet == 0) fprintf(stdout,
            " %03d%3d%3d%10.4lf%10.4lf%10.5lf%10.5lf%10.5lf%10.3lf%8.3lf\n",
            inp[nn].doy, inp[nn].hour, inp[nn].minute,
            inp[nn].ra, inp[nn].dec, inp[nn].delay,
            (del = (inp[nn].delay - gsl_vector_get(resids,nn))),
            gsl_vector_get(resids,nn),
            inp[nn].harad * DEG_PER_RAD / 15.0, ratemod);
        else
            del = (inp[nn].delay - gsl_vector_get(resids,nn));
        avedsq += inp[nn].delay * inp[nn].delay;
        altdsq += del * del;
    }
    avedsq /= (double)num;
    altdsq /= (double)num;

    if (opt.redCHI) sprintf(parameters, "%d vals", num);
    fprintf(stdout,
        "#\n"
        "# Model fit %s: %s %g for baseline (%s) %s%s\n"
        "# param1 = %12.6lf delta Dx(m)\n"
        "# param2 = %12.6lf delta Dy(m)\n"
        "# param3 = %12.6lf delta Dz(m)\n"
        "# param4 = %12.6lf clock offset(us)\n"
        "# param5 = %12.6lf clock rate(ps/s)\n"
        "# (subtract from %c position or add these to %c position)\n"
        "#\n",
        parameters,
        opt.redCHI ? "redChiSq" : "chisq",
        opt.redCHI ?  chisq / altdsq / (double)num : chisq / avedsq,
        inp[0].bl,
        opt.redCHI ? "pol " : "",
        opt.redCHI ? inp[0].polarization : "",
        gsl_vector_get(params, 0),
        gsl_vector_get(params, 1), gsl_vector_get(params, 2),
        gsl_vector_get(params, 3), gsl_vector_get(params, 4),
        inp[0].bl[0], inp[0].bl[1]);
}

/*
 * This re-implments the dsolv2 code using gsl routines.
 * It gets input from scan_file().
 */
int dsolv2(char *data)
{
    int num = 0;                                                // dif-2
    double csq = 0.0;
    gsl_matrix *design;
    gsl_vector *delays, *params, *resids;
    Aobs *observations = scan_file(data, &num);                 // dif-2
    if (num < 6) return(fprintf(stderr,
        "Only have %d lines of input from %s\n", num, data));   // dif-2
    make_working_data_approx(observations, num);
    design = make_design_matrix(observations, num);
    if (!design) return(fprintf(stderr, "No design matrix!\n"));
    report_design_matrix(observations, design, num);
    delays = make_delay_vector(observations, num);
    if (!delays) return(fprintf(stderr, "No delay vector!\n"));
    params = calc_parameters(num, design, delays, &csq);
    if (!params) return(fprintf(stderr, "No param vector!\n"));
    resids = calc_residuals(num, design, delays, params);
    if (!resids) return(fprintf(stderr, "No residual vector!\n"));
    report_results(num, observations, resids, params, csq);
    /* cleanup */
    free(observations);
    gsl_matrix_free(design);
    gsl_vector_free(delays);
    gsl_vector_free(params);
    gsl_vector_free(resids);
    return(0);
}

/*
 * Main entry.  A single argument provides backwards compatible
 * calculation with dsolv.f and dsolv2.f.  Otherwise, we parse...
 */
int main(int argc, char **argv)
{
    if (argc == 2 && argv[1][0] != '-') return(dsolv2(argv[1]));

    /* a variety of different ways to do this */
    return(dsolv3_main(argc, argv));
}

/*
 * eof
 */
