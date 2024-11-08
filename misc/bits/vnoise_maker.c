/*
 * $Id: vnoise_maker.c 741 2012-06-29 15:22:56Z gbc $
 *
 * A deviant form of bnoise which makes vdif formed data simulating
 * N (2Gbps) channels of the the RDBE output in Astro 8Gbps mode.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <gsl/gsl_randist.h>

#include "hops_config.h"
#include "vnoise.h"

#define MHZ_TO_SAMPLE_TIME  (2*M_PI*1e6/1.024e9)
#define	FRAMES_PER_SECOND   31250
#define OCTETS_PER_FRAME    1024
#define VDIF_HDR_OCTETS	    (sizeof(VdifExt)/8)
#define OCTETS_PER_PACKET   (VDIF_HDR_OCTETS + OCTETS_PER_FRAME)
#define	MAX_NUM_STATIONS    20
#define	MAX_NUM_TONES	    32

static double	corr = 0.01;
static double	thrs = 1.00;	/* threshold in sigma units */
static int	idur = 2;	/* data duration in full seconds */
static int	durf = 0;	/* runt frames left over */

static int	nfps = FRAMES_PER_SECOND;
static int	nfrp = 3125;

static int	nstn = 0;
static FILE	*ofp[MAX_NUM_STATIONS];
static uint64_t	vhd[MAX_NUM_STATIONS][OCTETS_PER_PACKET];

static double	tfrq[MAX_NUM_TONES];
static double	tamp[MAX_NUM_TONES];
static int	ntones = 0;

static const	gsl_rng_type *T;
static gsl_rng	*r;
static double	csigma, sigma, gamp = 1.0;
static double	bias[MAX_NUM_STATIONS];
static long	lazy = 0, lready = 0, lpick = 0, lran = 0;
static uint64_t	*lzp = 0;
static uint64_t	bs[MAX_NUM_STATIONS][4], bsc = 0, bsm = 1024 * 1024;

void set_corr(double c)
{
    corr = c;
    if (verb>0) fprintf(stdout, "Correlation coefficient now %g\n", corr);
}
void set_dura(double d)
{
    if (verb>0) fprintf(stdout, "Duration %g secs, ", d);
    idur = (int)floor(d);
    d -= (double)idur;
    durf = (d > 0.0) ? nfps * d : 0;
    if (verb>0) fprintf(stdout, "%d secs + %d frames\n", idur, durf);
}
void set_reps(double r)
{
    if (verb>0) fprintf(stdout, "Reporting every %g secs, ", r);
    nfrp = nfps * r;
    if (verb>0) fprintf(stdout, "which is %d frames\n", nfrp);
}
void add_tone(char *frqamp)
{
    double  frq, amp;
    if (2 != sscanf(frqamp, "%lf,%lf", &frq, &amp)) return;
    if (verb>0) fprintf(stdout, "Added tone %g at %g MHz\n", amp, frq);
    tfrq[ntones] = frq * MHZ_TO_SAMPLE_TIME;
    tamp[ntones] = amp;
    ntones++;
}
void set_gamp(double a)
{
    gamp = a;
    if (verb>0) fprintf(stdout, "Gaussian multiplier now %g\n", gamp);
}
void set_lazy(long l)
{
    lazy = l;
    if (lazy < 0) { lran = 1; lazy = -lazy; }
    if (verb>0) fprintf(stdout, "Reusing octets after %ld %s\n",
	lazy, lran ? "in random order" : "in time order");
}
void set_bias(char *a)
{
    int s, n, ns = 0;
    double b;
    while (1 == sscanf(a, "%lf%n", &b, &n)) {
	if (verb>0) fprintf(stdout, "Bias of station %d is %lf\n", ns, b);
	bias[ns++] = b;
	a += n;
	if (*a == ',') a++;
    }
}

static void starting_header(VdifExt *vh)
{
    memset(vh, 0, sizeof(VdifExt));
    vh->ref_epoch = 20;	/* 2010 */
    vh->df_len_octets = 0x404;
    vh->stationID = nstn;
    vh->threadID = nstn;
    vh->bpsm1 = 2 - 1;
    vh->eud8 = VDIF_CHAN_A + nstn;
}

int new_station(char *name)
{
    if (nstn >= MAX_NUM_STATIONS) return(1);
    ofp[nstn] = fopen(name, "w");
    starting_header((VdifExt*)vhd[nstn]);
    if (verb>0) fprintf(stdout, "Prepped station %s (%d)\n", name, nstn);
    return(ofp[nstn++] ? 0 : 1);
}

static int finish_data(void)
{
    int	ii;
    if (verb>0) fprintf(stdout, "Flushing data...\n");
    for (ii = 0; ii < nstn; ii++) fclose(ofp[ii]);
    if (verb>0) fprintf(stdout, "... done.\n");
    if (verb>0) for (ii = 0; ii < nstn; ii++) {
	fprintf(stdout, "BS[%d] %.3f %.3f %.3f %.3f (%.1f%% %.3f Ms)\n",
	    ii,
	    bs[ii][0] / (double)bsc, bs[ii][1] / (double)bsc,
	    bs[ii][2] / (double)bsc, bs[ii][3] / (double)bsc,
	    ( bs[ii][1] + bs[ii][2] ) * 100 / (double)bsc,
	    (double)bsc / 1e6);
    }
    return(0);
}


/* following main() of bnoise.c */
static void fake_data_prep(void)
{
    static const gsl_rng_type **t, **t0;
    csigma = sqrt (corr / (1.0 - corr));
    sigma = sqrt (1.0 + csigma * csigma);
    thrs = 1.0 * sigma;
    /* GSL_RNG_TYPE can be set to select the type */
    /* GSL_RNG_SEED can be set to select the seed */
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc (T);
    if (verb>0) {
	printf("generator type: %s\n", gsl_rng_name (r));
	printf("seed = %u\n", gsl_rng_default_seed);
	printf("first value = %u\n", gsl_rng_get (r));
	if (verb>1) {
	    printf("Available generators:\n");
	    t0 = gsl_rng_types_setup ();
	    for (t = t0; *t != 0; t++) printf("%s\n", (*t)->name);
	}
    }
    if (lazy>0) {
	lready = lpick = 0;
	lzp = malloc(nstn * lazy * sizeof(uint64_t));
	if (!lzp) { lazy = 0; perror("malloc"); }
    }
}

#define HH_BIT	(((uint64_t)0x3)<<62)
#define H__BIT	(((uint64_t)0x2)<<62)
#define L__BIT	(((uint64_t)0x1)<<62)
#define LL_BIT	(((uint64_t)0x0)<<62)

static void fabricate(int ot)
{
    static int	    alt = 0;
    static double   tick = 0;
    double	c, xx;
    uint64_t	*wd[MAX_NUM_STATIONS], ss;
    int		ii, ns, nt;

    /* set up pointers into packets */
    for (ns = 0; ns < nstn; ns++) {
	wd[ns] = &vhd[ns][ot];
	*wd[ns] = 0;
    }

    /* use the cache if it's ready */
    if ((lazy > 0) && lready) {
	if (lran) lpick = (long)gsl_ran_flat(r, 0.0, (double)lazy);
	for (ns = 0; ns < nstn; ns++) *wd[ns] = lzp[ns * lazy + lpick];
	if (++lpick >= lazy) lpick = 0;
	return;
    }

    /* build the octets */
    for (ii = 0; ii < 32; ii++) {
	/* common noise component */
	if (gamp) c = gamp * gsl_ran_gaussian (r, csigma);
	else      c = 0;
	/* common tones */
	for (nt = 0; nt < ntones; nt++)
	    c += tamp[nt] * sin(tfrq[nt]*tick);
	tick += 1.0;
	/* assemble the station signals */
	for (ns = 0; ns < nstn; ns++) {
	    xx = c + bias[ns];
	    /* per-station noise component */
	    if (gamp) xx += gamp * gsl_ran_gaussian (r, 1.0);
	    if (adcq) xx = adcq(xx);
	    if      (xx >  thrs) ss = HH_BIT;
	    else if (xx < -thrs) ss = LL_BIT;
	    else if (xx >   0.0) ss = H__BIT;
	    else if (xx <   0.0) ss = L__BIT;
	    else ss = (alt ^= 0x1)  ? L__BIT
				    : H__BIT;
	    *wd[ns] |= ss;
	    if (bsc < bsm) bs[ns][ss>>62] ++;
	    if (ii < 31) *wd[ns] >>= 2;
	}
	if (bsc < bsm) bsc++;
    }

    /* maybe cache these octets */
    if ((lazy > 0) && !lready) {
	for (ns = 0; ns < nstn; ns++) lzp[ns * lazy + lpick] = *wd[ns];
	if (++lpick >= lazy) {
	    lpick = 0;
	    lready = 1;
	}
    }
}

#if HAVE_LIBRT
static void report_processing(int sc, int nf)
{
    static struct timespec tl = { 0, 0 };
    static double total = 0;
    struct timespec ts;
    double dt;

    if (nf%nfrp != 0) return;

    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
    if (nf) {
	dt = ts.tv_sec - tl.tv_sec;
	dt += 1e-9*(ts.tv_nsec - tl.tv_nsec);
	total += dt;
	fprintf(stderr, "At second %d.%06d (fr %05d) took %.3f s (tot %.3f)\n",
	    sc, 1000000*nf/31250, nf, dt, total);
    } else {
	// fprintf(stderr, ".");
    }
    tl = ts;
}
#else /* HAVE_LIBRT */
static void report_processing(int sc, int nf)
{
    fprintf(stderr, ".");
}
#endif /* HAVE_LIBRT */

int fake_the_data(void)
{
    int	isec, ifr, ns, ot;
    uint64_t	*wd;
    VdifExt *hdr;

    if (nstn == 0) return(0);	/* no work */

    fake_data_prep();
    if (verb>1) report_processing(0, 0);
    for (isec = 0; isec <= idur; isec++) {
	if (isec == idur) nfps = durf;	/* runt */
	for (ifr = 0; ifr < nfps; ifr++) {
	    if (verb>1) report_processing(isec, ifr);

	    for (ot = 0; ot < OCTETS_PER_FRAME; ot++)
		fabricate(VDIF_HDR_OCTETS + ot);
    
	    for (ns = 0; ns < nstn; ns++) {
		fwrite(hdr = (VdifExt*)vhd[ns],
		    sizeof(uint64_t), OCTETS_PER_PACKET, ofp[ns]);
		hdr->df_num_insec ++;
		hdr->psn ++;
		hdr->seql ++;
		if (hdr->seql == 0) hdr->seqh ++;
	    }
	}
	/* once per second, fr->0, sec++ */
	for (ns = 0; ns < nstn; ns++) {
	    hdr = (VdifExt*)vhd[ns];
	    hdr->df_num_insec = 0;
	    hdr->secs_inre ++;
	}
	if (verb>1) report_processing(isec, ifr);
    }
    return(finish_data());
}

/*
 * eof
 */
