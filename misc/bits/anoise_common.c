/*
 * $Id: anoise_common.c 741 2012-06-29 15:22:56Z gbc $
 *
 * A deviant form of bnoise/vnoise which makes vdif formed data simulating
 * VDIF data in a variety of flavors.  The goal here is to simulate ALMA
 * data and correlate it against Mark5b-like or other flavors of data.
 *
 * For the moment, we assume sample clocks let us work in data slices
 * of exactly one microsecond (us), as defined by ONE_SEC_IN_USEC, but
 * if necessary, redefining that symbolic lets us handle other cases.
 *
 * This portion generates the common signal data, one slice at a time.
 */

#include "anoise.h"

/* common random number generator data */
gsl_rng         *rng_inst;
double          csigma;

/* user controlled */
static double   corr = 0.01;    /* common component correlation amplitude */
static double   gamp = 1.0;     /* allows derating the gaussian */
static int      undr = 0;       /* undersample factor */
int fftsps = 0;                 /* override samples/us heuristic */
int slices = 1;                 /* number of 1us slices per fft */
int honest = 1;                 /* honest gaussian receiver */

/* private working data */
static double	tphs[MAX_NUM_TONES];
static double	tfrq[MAX_NUM_TONES];
static double	tamp[MAX_NUM_TONES];
static int	ntones = 0;

static void set_corr(double c)
{
    corr = c;
    if (verb>0) fprintf(stdout, "Correlation coefficient now %g\n", corr);
}

/* could add phase */
static void add_tone(char *frqamp)
{
    double  frq, amp, phs = 0;
    if ((3 != sscanf(frqamp, "%lf,%lf,%lf", &frq, &amp, &phs)) &&
        (2 != sscanf(frqamp, "%lf,%lf", &frq, &amp))) return;
    if (verb>0) fprintf(stdout,
        "Added tone %g at %g MHz (%g deg)\n", amp, frq, phs);
    tphs[ntones] = phs * M_PI / 180.0;
    tfrq[ntones] = frq;
    tamp[ntones] = amp;
    ntones++;
}
static void set_gamp(double a)
{
    if (a < 0.0) a = 0.0;
    gamp = a;
    if (verb>0) fprintf(stdout, "Gaussian multiplier now %g\n", gamp);
}
static void set_under(int a)
{
    if (a < 0) a = 0;
    undr = a;
    if (verb>0) fprintf(stdout, "Undersampling by %d\n", undr);
}
static void set_fftn(int a)
{
    if (a < 0) a = 0;
    fftsps = a;
    if (verb>0) fprintf(stdout, "Requesting FFT %d points/us\n", fftsps);
}
static void set_slices(int a)
{
    if (a < 1) a = 1;
    slices = a;
    if (verb>0) fprintf(stdout, "Requesting FFT every %d us\n", slices);
}

int set_comm(char *c)
{
    if (!strcmp(c, "help")) {
        printf( "  corr:<float>     amplitude of common signal relative\n"
                "                   to the station receiver noise (0.01)\n"
                "  amp:<float>      adjusts the strength of the gaussian\n"
                "                   component of the common signal (1.0)\n"
                "                   (relative to any tones).\n"
                "  tone:<freq,amp>  adds a tone at the <float> freq,amp\n"
                "  tone:<freq,amp,phs>  specifies phase as well\n"
                "  under:<int>      undersamples the common signal (0)\n"
                "  fftn:<int>       force underlying fft to have this many\n"
                "                   samples/us (0 -- means work it out)\n"
                "  slices:<int>     number of 1-us sample groups per fft (1)\n"
                "  honest:<int>     controls the degree of honesty in the\n"
                "                   generation of the gaussians; for more\n"
                "                   information use \"honest:help\".\n"
        );
        return(-1);
    } else if (!strncmp(c, "honest:help", 11)) {
        printf( "The lowest values select different generators:\n"
                "  0 -- very fast, but non-gaussian\n"
                "  1 -- faster, but low in quality\n"
                "  2 -- slow, and decent\n"
                "  3 -- slow, and decent\n"
                "  4 -- slowest and best quality\n"
                "  5 -- uses GSL_RNG_TYPE\n"
                "to specify the generator.  The default is 1.\n"
                "For larger values (6..10) the receiver noise is\n"
                "not oversampled, should you wish to try that.\n"
                "In all cases, you can get a different sequence by\n"
                "setting GSL_RNG_SEED in the environment.\n"
        );
        return(-1);
    } else if (!strncmp(c, "corr:", 5)) {
        set_corr(atof(c+5));
    } else if (!strncmp(c, "amp:", 4)) {
        set_gamp(atof(c+4));
    } else if (!strncmp(c, "tone:", 5)) {
        add_tone(c+5);
    } else if (!strncmp(c, "under:", 6)) {
        set_under(atoi(c+6));
    } else if (!strncmp(c, "fftn:", 5)) {
        set_fftn(atoi(c+5));
    } else if (!strncmp(c, "slices:", 7)) {
        set_slices(atoi(c+7));
    } else if (!strncmp(c, "honest:", 7)) {
        honest = atoi(c+7);
        if (verb>0) fprintf(stdout, "Gaussian honesty %d selected\n", honest);
    } else {
        fprintf(stderr, "What is %s ?\n", c);
        return(1);
    }
    return(0);
}

/*
 * Setup for the GSL random number generator
 * Lots of flexibility here that we don't need:
 *  the default generator is good enough for our needs
 *  GSL_RNG_TYPE can be set to select the type
 *  GSL_RNG_SEED can be set to select the seed
 *
 * Quickest appears to be rand (crappy quality) or coveyou
 * The default appears to be mt19937
 * a comparable choice is taus2
 * The best (slowest) is ranlux
 */
static void common_gaussian_setup(void)
{
    gsl_rng_env_setup();
    switch (honest) {
    case 6: case 1:  rng_inst = gsl_rng_alloc(gsl_rng_rand);    break;
    case 7: case 2:  rng_inst = gsl_rng_alloc(gsl_rng_taus2);   break;
    case 8: case 3:  rng_inst = gsl_rng_alloc(gsl_rng_mt19937); break;
    case 9: case 4:  rng_inst = gsl_rng_alloc(gsl_rng_ranlux);  break;
    default:         rng_inst = gsl_rng_alloc(gsl_rng_default); break;
    }
    if (verb>0) {
        printf("GSL type: %s (honest=%d), ", gsl_rng_name (rng_inst), honest);
        printf("seed=%u: ", gsl_rng_default_seed);
        printf("%u, ", gsl_rng_get (rng_inst));
        printf("%u ...\n", gsl_rng_get (rng_inst));
        if (verb>3) {
            static const gsl_rng_type **t, **t0;
            printf("Available generators:\n");
            t0 = gsl_rng_types_setup ();
            for (t = t0; *t != 0; t++) printf("%s\n", (*t)->name);
        }
    }
    csigma = sqrt (corr / (1.0 - corr));
}

/*
 * Initializations for the common signal.
 */
static double *setup_common(int sps)
{
    int tt;
    double MHz_mult;
    double *dp = (double*)malloc(sps * sizeof(double));
    if (!dp) perror("setup_common");
    common_gaussian_setup();
    MHz_mult = 2 * M_PI / sps;
    for (tt = 0; tt < ntones; tt++) tfrq[tt] *= MHz_mult;
    return(dp);
}

/*
 * Loads the common slice with its gaussian component and
 * as many tones as are specified.
 *
 * If undersampling is enabled, the quality value (*dp) just
 * computed is copied to the  undr  subsequent values.
 */
profile void update_common(double *dp, int sps)
{
    int ss, tt, uu;
    double *du;
    for (ss = 0; ss < sps; ss++, dp++) {
        *dp = gamp * gsl_ran_gaussian_ziggurat(rng_inst, csigma);
        for (tt = 0; tt < ntones; tt++)
            *dp += tamp[tt] * sin(tphs[tt] + tfrq[tt]*ss);
        for (uu = 0, du = dp; uu < undr && ss < sps; uu++) {
            *++dp = *du;
            ++ss;
        }
    }
}

/*
 * Fabricate the next slice of common data and return pointer to
 * the data array.  The argument is the number of samples per slice
 * (total duration is 1us * slices).
 *
 * This is the simple version that just has one buffer.  A later
 * version will have two adjacent buffers to allow a nonzero delay
 * between the data at multiple stations.  Only one buffer is updated
 * per call.
 */
double *fabricate_common(int sps)
{
    static double *dp = 0;
    if (!dp) dp = setup_common(sps);
    else     update_common(dp, sps);
    return(dp);
}

/*
 * eof
 */
