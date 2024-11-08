/*
 * $Id: netsolv.c 2174 2017-12-19 20:58:52Z gbc $
 *
 * Read in the data, solve the problem and report the result
 */

#include "network.h"

static int load_inp(char *name, int nval, int ref, int rem, int pf, int pm,
    int pn, double redcsq, double pval, char datum[], char dunit[], Nopt *opt)
{
    opt->inp = (Inp*)realloc(opt->inp, (opt->ninp + 2) * sizeof(Inp));
    if (!opt->inp) return(1);

    if (opt->verb>2) fprintf(stdout,
        "# %s param%d: (%d) %c%c %c%c %f %f %s-%s\n",
        name, pn, nval, ref, rem, pf, pm, redcsq, pval, datum, dunit);
    snprintf(opt->inp[opt->ninp].base,   3, "%c%c", ref, rem);
    snprintf(opt->inp[opt->ninp].pozn,   3, "%c%c", pf,  pm);
    opt->inp[opt->ninp].nval = nval;
    opt->inp[opt->ninp].redcsq = redcsq;
    snprintf(opt->inp[opt->ninp].label, 40, "%s-%s", datum, dunit);
    opt->inp[opt->ninp].obs = pval;

    opt->ninp += 1;
    return(0);
}

static int scan_file(FILE *fpi, char *name, Nopt *opt)
{
    static char line[256], datum[20], dunit[20];
    int nn, skip = 0, nval, ref, rem, pf, pm, pn;
    double redcsq, pval;

    if (opt->verb>1) fprintf(stdout, "# scanning %s\n", name);
    while (fgets(line, sizeof(line), fpi)) {
        if (opt->verb>3) fputs(line, stdout);
        if (skip == 0) {    /* look for Model line */
            nn = sscanf(line,
                "# Model fit %d %*s redChiSq %lf %*s %*s (%c%c) pol %c%c",
                &nval, &redcsq, &ref, &rem, &pf, &pm);
            if (nn == 6) {
                /* check restrictions */
                if (opt->maxRedChiSq > 0.0 && redcsq > opt->maxRedChiSq) {
                    skip = 0;
                    if (opt->verb>1) fprintf(stdout,
                        "# %9lf < %9lf, skip %c%c %c%c with RedChiSq %d\n",
                        redcsq, opt->maxRedChiSq, ref, rem, pf, pm, nval);
                } else if (opt->minNumVals > 0 && nval < opt->minNumVals) {
                    skip = 0;
                    if (opt->verb>1) fprintf(stdout,
                        "# %d < %d, skip %c%c %c%c with only %d values\n",
                        nval, opt->minNumVals, ref, rem, pf, pm, nval);
                } else if (skip_baseline(opt, ref, rem)) {
                    skip = 0;
                    if (opt->verb>1) fprintf(stdout,
                        "# Skipping %c%c\n", ref, rem);
                } else {
                    /* look for param line */
                    skip = opt->pnum;
                    if (opt->verb>1) fprintf(stdout,
                        "# %9lf > %9lf, keep %c%c %c%c with %d\n",
                        redcsq, opt->maxRedChiSq, ref, rem, pf, pm, nval);
                }
            } else {
                if (opt->verb>2) fprintf(stdout, "#  only got %d\n", nn);
            }
        } else if (--skip == 0) {   /* found param line */
            nn = sscanf(line,
                "# param%c = %lf %s %s", &pn, &pval, datum, dunit);
            if (nn == 4) {
                /* load Param line */
                if (load_inp(name, nval, ref, rem, pf, pm, pn - '0',
                    redcsq, pval, datum, dunit, opt)) break;
            }
            skip = 0;
        }
    }
    return(0);
}

int get_data(int argc, char **argv, Nopt *opt)
{
    FILE *fpi = 0;

    opt->nfiles = 0;
    opt->ninp = 0;
    opt->inp = (Inp*)malloc((opt->ninp + 1) * sizeof(Inp));
    while (argc > 0 && *(argv)) {
        if (fpi && (fpi != stdin)) fclose(fpi);
        if (!strcmp(*argv, "-")) fpi = stdin;
        else fpi = fopen(*argv, "r");
        if (scan_file(fpi, *argv, opt)) break;
        opt->nfiles ++;
        argc--;
        argv++;
    }
    if (fpi && (fpi != stdout)) fclose(fpi);
    if (opt->verb>1) fprintf(stdout, "# parser %d input files\n", opt->nfiles);
    return(0);
}

int get_stations(Nopt *opt)
{
    int nn, ss, rr, s;
    memset(opt->stations, 0, sizeof(opt->stations));
    opt->nant = 0;
    for (nn = 0; nn < opt->ninp; nn++) {
        for (rr = 0; rr < 2; rr++) {
            s = opt->inp[nn].base[rr];
            for (ss = 0; ss < MAXANT; ss++) {
                if (0 == opt->stations[ss]) opt->stations[ss] = s;
                if (s == opt->stations[ss]) break;
            }
            if (ss == MAXANT) return(1);
            if (ss > opt->nant) opt->nant = ss;
        }
    }
    opt->nant ++;
    if (opt->verb>0) {
        fprintf(stdout, "# Input %d stations are: ", opt->nant);
        for (ss = 0; opt->stations[ss]; ss++)
            fprintf(stdout, " %c", opt->stations[ss]);
        fprintf(stdout, "\n");
    }
}

static void reorder_stations(Nopt *opt)
{
    int ss;
    for (ss = 0; ss < opt->nant; ss++) {
        if (opt->stations[ss] == opt->sfix) {
            opt->stations[ss] = opt->stations[opt->nant - 1];
            opt->stations[opt->nant - 1] = opt->sfix;
            break;
        }
    }
    if (opt->verb>0) {
        fprintf(stdout, "# Input %d stations now: ", opt->nant);
        for (ss = 0; opt->stations[ss]; ss++)
            fprintf(stdout, " %c", opt->stations[ss]);
        fprintf(stdout, "\n");
    }
}

static void set_charfunc(Nopt *opt)
{
    int nn, ss;
    for (nn = 0; nn < opt->ninp; nn++) {
        /* the smaller the redcsq, the higher the weight */
        opt->inp[nn].sig = (opt->wght && opt->inp[nn].redcsq > 0.0)
            ? sqrt(1.0 / opt->inp[nn].redcsq) : 1.0;
        /* load characteristic function values */
        for (ss = 0; ss < opt->nant; ss++) {
            if (opt->inp[nn].base[0] == opt->stations[ss])
                opt->inp[nn].chr[ss] =  1.0;
            else if (opt->inp[nn].base[1] == opt->stations[ss])
                opt->inp[nn].chr[ss] = -1.0;
            else
                opt->inp[nn].chr[ss] =  0.0;
        }
    }
    if (opt->verb>0)
        for (nn = 0; nn < opt->ninp; nn++) {
            fprintf(stdout, "# %s %+14f ",
                opt->inp[nn].base, opt->inp[nn].obs);
            for (ss = 0; ss < opt->nant; ss++)
                fprintf(stdout, " %+.1f", opt->inp[nn].chr[ss]);
            fprintf(stdout, " [%s]\n", opt->inp[nn].label);
        }
}

int grok_data(Nopt *opt)
{
    if (get_stations(opt)) return(1);
    if (opt->sfix) reorder_stations(opt);
    set_charfunc(opt);
    return(0);
}

static void report_answer(Nopt *opt)
{
    int ss, jj;
    fprintf(stdout, "# Fix / covariance ");
    for (ss = 0; ss < opt->nant; ss++)
        fprintf(stdout, "%c%6c", (ss==0)?'[':' ', opt->stations[ss]);
    fprintf(stdout, "]\n");
    for (ss = 0; ss < opt->nant; ss++) {
        fprintf(stdout, "#P%d-%c += %+10f ",
            opt->pnum, opt->stations[ss], - opt->answers[ss]);
        for (jj = 0; jj < opt->nant; jj++)
            fprintf(stdout, "%c%+.3f",(jj==0)?'[':' ',opt->covariance[ss][jj]);
        fprintf(stdout, "]\n");
    }
}

int solve_data(Nopt *opt)
{
    if (brute_solve(opt->inp, opt->ninp, opt->answers, opt->nant - 1, opt))
        return(1);
    opt->answers[opt->nant-1] = 0.0;
    report_answer(opt);
    return(0);
}

void load_baselines(Nopt *opt, char *blarg)
{
    int nn;
    char *pb = strtok(blarg, ",");
    while (pb) {
        if (strlen(pb) == 2) {      /* skip junk */
            strncpy(opt->baselines[opt->nbase], pb, 2);
            opt->baselines[opt->nbase++][2] = 0;
        }
        pb = strtok(NULL, ",");
    }
    fprintf(stdout, "# Ignoring %d baselines: ", opt->nbase);
    for (nn = 0; nn < opt->nbase; nn++)
        fprintf(stdout, " %s", opt->baselines[nn]);
    fprintf(stdout, "\n");
}

int skip_baseline(Nopt *opt, int ref, int rem)
{
    int nn;
    char bl[3];
    bl[0] = ref;
    bl[1] = rem;
    bl[2] = 0;
    for (nn = 0; nn < opt->nbase; nn++)
        if (!strcmp(opt->baselines[nn], bl)) return(1);
    return(0);
}

/*
 * eof
 */
