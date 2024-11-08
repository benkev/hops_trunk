/*
 * $Id: anoise_maker.c 773 2012-08-29 13:37:47Z gbc $
 *
 * A deviant form of bnoise which makes vdif formed data simulating
 * N (2Gbps) channels of the the RDBE output in Astro 8Gbps mode.
 *
 * For the moment, we assume sample clocks let us work in data slices
 * of exactly one microsecond (us), as defined by ONE_SEC_IN_USEC, but
 * if necessary, redefining that symbolic lets us handle other cases.
 *
 * This is the top-level driver, with common noise and station specific
 * routines in other files.
 */

#include "anoise.h"

#define ONE_SEC_IN_USEC	1000000

/* user-controlled */
double   dura = 2.00;           /* target duration of data generation */
uint64_t bsmx = 1024 * 1024;    /* number of states to examine */

int             nstn = 0;       /* total number of stations */
VdifConf        vcnf[MAX_NUM_STATIONS];

static int      nusr = 1000;    /* number of us between reports */
static double   thrs = 1.00;    /* threshold in sigma units */

/*
 * Support functions for option setting from main()
 */
void set_bsmx(int b)
{
    bsmx = b;
    if (verb>0) fprintf(stdout, "Bits states with %d counts\n", bsmx);
}
void set_dura(double d)
{
    dura = d;
    if (verb>0) fprintf(stdout, "Running for %g secs\n", dura);
}
void set_reps(double r)
{
    nusr = (int)r;
    r = (double)nusr / 1e6;
    if (verb>0) fprintf(stdout, "Reporting every %g secs\n", r);
}
void set_thrs(double t)
{
    thrs = t;
    if (verb>0) fprintf(stdout, "2-bit threshold %g sigma\n", thrs);
}

/*
 * The station definition should have set
 *  chans   -- the number of channels (power of 2)
 *  bytes   -- the number of bytes per logical packet
 *  srate   -- the sample rate for a single channel
 *
 * The packet rate is 31250 logical packets per second, but we need
 * to impose the ethernet limit; we divide by 2 until it is ok.
 *
 * Then we need to define the following
 *  smpus   -- total sample rate (leads to bits/second)
 *  opus    -- total octet rate (octets per second)
 *  opp     -- octets per packet
 *  load    -- load point in the packet
 *  shift   -- shift of bits in octet
 *  choff   -- fft-space channel offset (provided by user)
 *
 * SlicesFixme: unfortunately, this also has to come out
 * even on a per-packet basis, which is not guaranteed.
 */
static char *compute_derived(char *type, VdifConf *vdcp)
{
    int nb;
    vdcp->pps = 31250;
    while (vdcp->bytes > 8192) { vdcp->bytes /= 2; vdcp->pps *= 2; }
    /* derived quantities */
    vdcp->smpus = vdcp->srate * vdcp->chans;
    vdcp->opus = (vdcp->smpus * 2.0) / 64.0;    /* bits/us / bits/octet */
    vdcp->opp = VDIF_HDR_OCTETS + vdcp->bytes/8;
    vdcp->load = &vdcp->vhd[VDIF_HDR_OCTETS];
    vdcp->shift = 0;
    if (*type == '-' || *type == '+') {
        if (0 < sscanf(type, "%lf%n", &vdcp->choff, &nb)) {
            type += nb;
        } else {
            fprintf(stderr, "Illegal channel shift %s\n", type);
            return(0);
        }
    }
    if (*type == ',') {
        if (0 < sscanf(type, ",%lf%n", &vdcp->chgap, &nb)) {
            type += nb;
        } else {
            fprintf(stderr, "Illegal channel gap %s\n", type);
        }
    }
    return(type);
}

/*
 * Help for the choices below
 */
void print_type_help(void)
{
    printf(
        "The station specification is a string of the form:\n"
        "\n"
        "  ID:file[:type[choff][:time]]\n"
        "\n"
        "where the type specifies the VDIF packet format for 2-letter\n"
        "station ID, written to file with one of these types:\n"
        "\n"
        "  vlbi512      one single-channel of 512 MHz\n"
        "  alma500      one single-channel of 500 MHz\n"
        "  trad8.0xN    N (256) channels of 8.0 MHz\n"
        "  vlbi32.0xN   N (64) channels of 32.0 MHz\n"
        "  vlbi64.0xN   N (32) channels of 64.0 MHz\n"
        "  alma62.5xN   N (32) channels of 62.5 MHz\n"
        "\n"
        "where in the multi-channel cases the number of channels\n"
        "needed for ~ 2 GHz sampling is indicated in parentheses.\n"
        "The number of channels is required to be a power of 2 and\n"
        "at most %d; and in all cases, 2-bit sampling is assumed.\n",
        MAX_NUM_CHANNELS
    );
    printf(
        "\n"
        "A channel specification may follow.  It can be either or both of\n"
        "    +C or -C    to shift all channels in FFT space\n"
        "    ,G          to insert a gap of G spectral points\n"
        "where the units of C and G are MHz (most likely)\n"
	"In the -C case, the width of the zeroth channel will be reduced\n"
	"so that only one side band is used.  If both are given, the shift\n"
	"must come first.  Both are floating point quantities, but rounding\n"
        "in FFT space may not give you exactly what you want.\n"
        "\n"
        "Finally, the (starting) time specification is epoch@seconds.\n"
        "\n"
    );
}

/*
 * Returns log base 2 of the input
 */
static int mylog2(int nc)
{
    int rv = 0;
    while (nc > 1) { rv++; nc>>=1; }
    return(rv);
}

/*
 * Parse the command-line specification of each station data type.
 * Returns a pointer to the next part (time).
 */
static char *update_type(char *type, VdifConf *vdcp)
{
    if (!type) {
        fprintf(stderr, "Station type is required\n");
        return((char*)0);
    } else if (!strncmp(type,"vlbi512",7)) {
        type += 7;
        vdcp->chans = 1;
        vdcp->bytes = 8192;
        vdcp->srate = 1024;
    } else if (!strncmp(type,"alma500",7)) {
        type += 7;
        vdcp->chans = 1;
        vdcp->bytes = 8000;
        vdcp->srate = 1000;
    } else if (!strncmp(type,"trad8.0x",8)) {
        type += 8;
        vdcp->chans = atoi(type++);
        if (vdcp->chans > 9) type++;
        if (vdcp->chans > 99) type++;
        vdcp->bytes = 128 * vdcp->chans;
        vdcp->srate = 16;
    } else if (!strncmp(type,"vlbi32.0x",9)) {
        type += 9;
        vdcp->chans = atoi(type++);
        if (vdcp->chans > 9) type++;
        vdcp->bytes = 512 * vdcp->chans;
        vdcp->srate = 64;
    } else if (!strncmp(type,"vlbi64.0x",9)) {
        type += 9;
        vdcp->chans = atoi(type++);
        if (vdcp->chans > 9) type++;
        vdcp->bytes = 1024 * vdcp->chans;
        vdcp->srate = 128;
    } else if (!strncmp(type,"alma62.5x",9)) {
        type += 9;
        vdcp->chans = atoi(type++);
        if (vdcp->chans > 9) type++;
        vdcp->bytes = 1000 * vdcp->chans;
        vdcp->srate = 125;
    } else {
        fprintf(stderr, "Undefined station type %s\n", type);
        return((char*)0);
    }
    if (vdcp->chans != (1 << mylog2(vdcp->chans))) {
        fprintf(stderr, "%d is not a power of two\n", vdcp->chans);
        return((char*)0);
    }
    if (vdcp->chans > MAX_NUM_CHANNELS) {
        fprintf(stderr, "%d is too many channels\n", vdcp->chans);
        return((char*)0);
    }
    return(type = compute_derived(type, vdcp));
}

/*
 * Epoch of 2012 if the user can't be bothered to supply start time.
 */
static void update_start(char *start, VdifConf *vdcp)
{
    int ns = sscanf(start, ":%d@%d", &vdcp->epoch, &vdcp->esecs);
    time_t now = time(0);
    if (ns < 2) {
        /* 20120101_000000 == 24@0.0000 */
        vdcp->epoch = 24;
        vdcp->esecs  = now - 1325376000;
    }
}

/*
 * Pick apart the station specification, returning a pointer to the
 * completed header, or else a null pointer if there is a problem.
 */
static VdifConf *parse_station(char *station)
{
    char *nc;
    VdifConf *vdcp = &vcnf[nstn];
    memset(vdcp, 0, sizeof(VdifConf));
    vdcp->id = (station[0]) | (station[1]<<8);
    /* station[2] is a : */
    if (station[2] != ':') return(
        fprintf(stderr, "Illegal station specification %s\n", station),
        (VdifConf*)0);
    vdcp->file  = station+3;
    nc = strchr(vdcp->file, ':');
    if (nc) *nc++ = 0;
    nc = update_type(nc, vdcp);
    if (nc) update_start(nc, vdcp);
    else return((VdifConf*)0);
    vdcp->thresh = thrs;        /* allows a per-station threshold */
    /* for fft monitoring */
    if (verb>0) {
        vdcp->name = malloc(strlen(vdcp->file)+20);
        if (vdcp->name) {
            sprintf(vdcp->name, "%s.spec", vdcp->file);
            /* if the file has vdif in the name, replace "vdif" with spec */
            nc = strstr(vdcp->name, "vdif");
            if (nc) strncpy(nc, "spec\0", 5);
        } else {
            perror("malloc");
        }
    }
    return(vdcp);
}

/*
 * Load the header with initial values.  After this, it's just bumping
 * the counters as each packet is output.
 */
static void starting_header(VdifExt *vh, VdifConf *vdcp)
{
    memset(vh, 0, sizeof(VdifExt));
    /* word0 */
    vh->legacy = vh->invalid = 0;
    vh->secs_inre = vdcp->esecs;
    /* word1 */
    vh->ref_epoch = vdcp->epoch;
    vh->df_num_insec = 0;
    vh->UA = 0;
    /* word2 */
    vh->df_len_octets = vdcp->opp;
    vh->num_chan_log2 = mylog2(vdcp->chans);
    vh->ver = 0;
    /* word3 */
    vh->stationID = vdcp->id;
    vh->threadID = nstn;
    vh->bpsm1 = 2 - 1;
    vh->dt = 0;
    /* extended header */
    vh->seql = vh->seqh = vh->edv = vh->status = 0;
    vh->magic = 0xbade1f;
}

void describe_station(VdifConf *vdcp)
{
    int ns = (vdcp - vcnf);
    assert(vdcp == &vcnf[ns]);
    fprintf(stdout,
        "Opened station %c%c[%d] to file %s (%s)\n"
        "  with %d sam/us = %d x %d sam/us, %d databytes/packet,\n"
        "  %d pkts/s and %g octets/us %d bytes/packet\n",
            (vdcp->id & 0x00FF), (vdcp->id & 0xFF00) >> 8,
            ns, vdcp->file, vdcp->ofp ? "ok" : "fail",
            vdcp->smpus, vdcp->chans, vdcp->srate, vdcp->bytes,
            vdcp->pps, vdcp->opus, vdcp->bytes + 32);
    fprintf(stdout,
        "  sigma %g, threshold %g, oversampling %d,\n"
        "  channel bw %.3f (%.2f MHz), shift %+.3f, gap %.3f\n",
        vdcp->sigma, vdcp->thresh, vdcp->over,
	vdcp->chbw, 1.0, vdcp->choff, vdcp->chgap    /* SlicesFixme */
    );
    if (verb>1) fprintf(stdout,
        "  Header bytes: %08X %08X %08X %08X (...)\n",
            (vdcp->vhd[0] & 0x00000000FFFFFFFF),
            (vdcp->vhd[0] & 0xFFFFFFFF00000000)>>32,
            (vdcp->vhd[1] & 0x00000000FFFFFFFF),
            (vdcp->vhd[1] & 0xFFFFFFFF00000000)>>32);
    if (verb>1) fprintf(stdout,
        "  Work pointers %p %p %p %p\n",
        vdcp->work, vdcp->spec, vdcp->chop, vdcp->chan);
}

/*
 * Add a new station from the command-line specification
 * as described in print_type_help() above.
 */
int new_station(char *station)
{
    VdifConf *vdcp = parse_station(station);
    if (!vdcp) return(1);
    assert(vdcp == vcnf + nstn);
    if (nstn >= MAX_NUM_STATIONS) return(2);
    vdcp->ofp = fopen(vdcp->file, "w");
    starting_header((VdifExt*)vdcp->vhd, vdcp);
    nstn++;
    return(vdcp->ofp ? 0 : 1);
}

/*
 * Close all the file descriptors and report on the bit states.
 */
static int close_and_report(void)
{
    int ii, ch;
    uint64_t *bsp;
    double bsch, pc, div = 1e6, rms, mag;
    char *lab = "Ms";
    if (verb>0) for (ii = 0; ii < nstn; ii++)
        for (ch = 0; ch < vcnf[ii].chans; ch++) {
            bsch = (double)(vcnf[ii].bc[ch]);
            lab = (bsch>1.e4) ? "Ms" : "ks";
            div = (bsch>1.e4) ? 1.e6 : 1.e3;
            bsp = vcnf[ii].bs[ch];
            pc = ( bsp[1] + bsp[2] ) * 100.0 / bsch;
            rms = sqrt(vcnf[ii].sq[ch] / bsch);
            mag = vcnf[ii].ab[ch] / bsch;
            fprintf(stdout,
                "BS[%d][%d] %.3f %.3f %.3f %.3f (%.1f%% %.3f %s) %.6f %.6f\n",
                ii, ch, bsp[0] / bsch, bsp[1] / bsch,
                        bsp[2] / bsch, bsp[3] / bsch, pc, bsch / div,
                lab, rms, mag);
        }
    if (verb>0) fprintf(stdout, "Flushing data...");
    for (ii = 0; ii < nstn; ii++) destroy_station(ii);
    if (verb>0) fprintf(stdout, " done.\n");
    return(0);
}

#if HAVE_LIBRT
void report_processing(int sc, int nus, char *where, int force)
{
    static struct timespec tl = { 0, 0 };
    static double total = 0;
    struct timespec ts;
    double dt, togo;

    if (nus < 0) return;
    if (nus%nusr != 0) return;

    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
    if (nus || force) {
        dt = ts.tv_sec - tl.tv_sec;
        dt += 1e-9*(ts.tv_nsec - tl.tv_nsec);
        total += dt;
        togo = dura - (double)sc - 1e-6 * (double)nus;
        fprintf(stdout, "At %s %d.%06d took %.3f s (tot %.3f) %f to go\n",
            where, sc, nus, dt, total, togo);
    }
    tl = ts;
}
#else /* HAVE_LIBRT */
void report_processing(int sc, int nus, char *where, int force)
{
    fprintf(stdout, ".");
    if (nus < 0) fprintf(stdout, "\n");
}
#endif /* HAVE_LIBRT */

/*
 * This is smart enough, perhaps.  The samples/us of each
 * station must evenly divide the value we return.
 *
 * In addition to working out the total samples per us slice,
 * we also set the oversampling and channel bw which are used often.
 */
static int work_out_samples_per_us(void)
{
    int tries[] = { 8, 16, 32, 64, 125, 128, 256, 500, 512, 1000, 1024,
                    2000, 2048, 4000, 4096, 8000, 8192, 16000, 16384,
                    32000, 32768, 64000, 65536, 128000, 131072 };
    int nt, rv, ns;
    VdifConf *vdcp;
    if (verb>1) fprintf(stdout, "Working out samples per (us) slice\n");
    for (nt = 0; nt < sizeof(tries)/sizeof(int); nt++) {
        rv = tries[nt];                             /* SlicesFixme */
        for (ns = 0, vdcp = vcnf; ns < nstn; ns++, vdcp++)
            if (rv != (vdcp->srate * (rv / vdcp->srate)) ||
                rv < 2*vdcp->smpus) break;          /* fail */
            else if (verb>1) fprintf(stdout, "  St[%d] %d/%d = %d, pass\n",
                ns, rv, vdcp->srate, rv/vdcp->srate);
        if (ns == nstn) break;
        else if (verb>1) fprintf(stdout, "  St[%d] %d/%d = %d, fail\n",
                ns, rv, vdcp->srate, rv/vdcp->srate);
    }
    rv *= slices;
    if (verb>0) fprintf(stdout, "Picked %d samples per us\n", rv);
    for (ns = 0, vdcp = vcnf; ns < nstn; ns++, vdcp++) {
        vdcp->over = rv / vdcp->srate / slices;     /* SlicesFixme */
        vdcp->chbw = slices * vdcp->srate / 2.0;    /* SlicesFixme */
    }
    if (fftsps > rv) {
        rv = fftsps;
        if (verb>0) fprintf(stdout,
            "Using %d samples per %d-us per input\n", rv, slices);
    } else if (fftsps > 0) {
        fprintf(stderr, "Asserting sampling %d < %d, good luck!\n", fftsps,rv);
        rv = fftsps;
    }
    return(rv);
}

/*
 * Prepare to fake all the data:
 *   Initialize the common data source
 *   Initialize each station (and channels)
 */
int prep_the_data(void)
{
    int ss, sps;
    double *dp;
    sps = work_out_samples_per_us();
    dp = fabricate_common(sps);
    if (!dp) return(0);
    for (ss = 0; ss < nstn; ss++)
        if (initialize_station(ss, sps)) return(0);
    return(sps);
}

/*
 * Based on number of slices, set the loop limits
 */
static int loop_numerology(int *idurp, int *fdusp, int *onep)
{
    double runt;
    /* loop numerology SlicesFixme */
    *onep = ONE_SEC_IN_USEC / slices;
    nusr /= slices;
    *idurp = (int)floor(dura);
    runt = (double)(dura - (*idurp));
    *fdusp = (int)rint(runt*(double)(*onep));
    if (slices * (*onep) != ONE_SEC_IN_USEC) {
        fprintf(stderr,
            "Processing slices must evenly divide a second\n"
            "You have %d * %d != %d\n", slices, *onep, ONE_SEC_IN_USEC);
        return(2);
    }
    if (verb>0) fprintf(stdout, verb>1
            ?  "Running %d.%06d secs, reporting every %d %d-us slices\n"
            :  "Running %d.%06d secs\n", *idurp,(*fdusp)*slices,nusr,slices);
    return(0);
}

/*
 * Loop over the basic processing unit (1 us) until done.
 */
int fake_the_data(void)
{
    int isec, cus, ns, sps, idur, fdus, one_sec;
    double *data;

    if (nstn == 0) return(0);   /* no work */
    sps = prep_the_data();
    if (sps == 0) return(1);    /* setup problem */

    if (loop_numerology(&idur, &fdus, &one_sec)) return(2);
    if (verb>0) report_processing(0, 0, "prep", verb>0);
    for (isec = 0; isec <= idur; isec++) {
        for (cus = 0; cus < one_sec; cus++) {           /* SlicesFixme */
            if ((isec == idur) && (cus > fdus)) break;
            if (verb>1) report_processing(isec, cus, "sec", 0);
            data = fabricate_common(sps);
            for (ns = 0; ns < nstn; ns++) fabricate_station(ns, data, sps);
        }
    }
    if (verb>0) report_processing(0, 0, "done", verb>0);
    /* report bit states and exit */
    return(close_and_report());
}

/*
 * eof
 */
