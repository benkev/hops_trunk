/*
 * $Id: anoise_station.c 777 2012-08-30 20:50:21Z gbc $
 * 
 * A deviant form of bnoise/vnoise which makes vdif formed data simulating
 * VDIF data in a variety of flavors.  The goal here is to simulate ALMA
 * data and correlate it against Mark5b-like or other flavors of data.
 *
 * This file handles the channelization and packetization of the data.
 */

#include "anoise.h"

/*
 * If the channelized data departs significantly from gaussian,
 * the channel thresholds will need adjustment to achieve full
 * population of the 4 bit states.
 *
 * Here we compute and capture a running average rms value of the
 * sample data seen so far.  This value is used later to rescale
 * the samples so that the rms of the frequency channels is the
 * same as the rms of the original data.
 *
 * It should be possible to predict what the scaling factor is,
 * but it's simpler to just compute it.
 */
profile void update_thresh_mult(int ch, VdifConf *vc)
{
    double rms = sqrt(vc->sq[ch] / (double)(vc->bc[ch]));
    vc->tm[ch] = rms;
}

/*
 * As packets are finished, output the data, and bump the header.
 */
profile void output_station(int sn)
{
    VdifExt *hdr = (VdifExt*)vcnf[sn].vhd;
    /* write the packet */
    if (verb>2) fprintf(stdout, "Writing Station-%d packet...", sn);
    fwrite(hdr, sizeof(uint64_t), vcnf[sn].opp, vcnf[sn].ofp);
    if (verb>2) fprintf(stdout,
        " sec %d fr %05d", hdr->secs_inre, hdr->df_num_insec);
    /* boost the counters */
    hdr->seql ++;
    if (hdr->seql == 0) hdr->seqh ++;
    /* boost the timestamp */
    hdr->df_num_insec++;
    if (hdr->df_num_insec == vcnf[sn].pps) {
        hdr->df_num_insec = 0;
        hdr->secs_inre++;
    }
    if (verb>2) fprintf(stdout,
        " ( %d fr %05d)\n", hdr->secs_inre, hdr->df_num_insec);
}

/*
 * As a debugging aid, dump out the samples and spectra to the file
 * if it has been opened.  Once we've cycled through all the channels
 * on the first slice, it will be closed.
 */
static void spec_out(int wh, int ch,
    double *dd, fftw_complex *cd, int nn, FILE *sp)
{
    static char *ats[4] = {
        "input data", "spectral data", "channel spec", "output data" };
    static int index = 0;
    int ii;
    fprintf(sp, "# index %d\n", index++);
    fprintf(sp, "# %s, channel %d with %d per us\n", ats[wh], ch, nn);
    fprintf(sp, "# %d rows of %s\n", (wh == 0 || wh == 3) ? nn : nn/2,
        (wh == 0 || wh == 3)
            ? "index(time) data[index]"
            : "index(freq) cabs[index] carg[index]"
    );
    fprintf(sp, "#\n");
    if (wh == 0 || wh == 3) for (ii = 0; ii < nn; ii++)
        fprintf(sp, "%8d %e\n", ii, dd[ii]);
    else for (ii = 0; ii < nn/2; ii++)
        fprintf(sp, "%8d %e %e\n", ii, cabs(cd[ii]), carg(cd[ii])*180.0/M_PI);
    fprintf(sp, "\n\n");
}

/*
 * Take the time series data in work, fft it to frequency space,
 * top-hat filter for the approprate channel, fft it back, and
 * put the result into the chan array.  The spectral array is
 * supposed to have sps/2 + 1 elements (with the missing elements
 * available as complex conjugates of existing ones since the input
 * is real:  i.e. spec[fq - sps] = conj(spec[fq]) for fq = 0..sps/2 ).
 * (I.e. we grab one slice, but we're implicitly grabbing the conjugate.)
 *
 * Because of the oversampling, our channels are limited to some
 * very low frequency components.  Note these are NZ 0 channels.
 *
 * We need to downshift prior to going back to the time domain
 * (equivalent to placing a LO at the lower edge of the band).
 *
 * We divide by sps because with fftw's normalization FORWARD+BACKWARD
 * is equivalent to multiplying by the number of samples (sps).
 */
profile void channelize(int ch, VdifConf *vc, int sps)
{
    int fq, fq_min, fq_max, fx;
    double fq_min_dbl;

    /* fft work array into spectral array */
    if (vc->osp) spec_out(0, ch, vc->work, 0, sps, vc->osp);
    fftw_execute(vc->plan_ws);
    if (vc->osp) spec_out(1, ch, 0, vc->spec, sps, vc->osp);

    /* slice this channel to chop array */
    /* we need a copy because fft is destructive */
    memset(vc->chop, 0, sps * sizeof(double));
    fq_min_dbl = (double)ch * (vc->chbw + vc->chgap) + vc->choff;
    fq_min = rint(fq_min_dbl) + 1;
    fq_max = rint(fq_min_dbl + vc->chbw);
    if (fq_min < 1) fq_min = 1;                 /* always punt DC */
    if (fq_max >= sps/2) fq_max = sps/2 - 1;    /* stay inbounds */
    for (fx = 1, fq = fq_min; fq <= fq_max; fq++, fx++)
        vc->chop[fx] = vc->spec[fq] / (double)sps;

    /* fft chopped array back to time array */
    if (vc->osp) spec_out(2, ch, 0, vc->chop, sps, vc->osp);
    fftw_execute(vc->plan_cc);
    if (vc->osp) spec_out(3, ch, vc->chan, 0, sps, vc->osp);
}

/*
 * Do the bit banging to put the bits from the chan array into
 * the relevant bits in the packet.  The packet can be loaded
 * with garbage--we just preserve the bits we're not overwriting.
 *
 * If there are at most 32 channels, the 3 statements marked if/while
 * can be coded with "if".  With >32 channels, we extend into the
 * next octet (64 bits) and so we may need shift optr more than once.
 * (The original code had "if" and MAX_NUM_CHANNELS equal to 32.)
 *
 * Each slice has sps samples, but since we are oversampled, we
 * step by vc->over to collect only vc->smpus of them.
 */
profile void load_chan(int ch, VdifConf *vc, int sps)
{
    int tick, shift = vc->shift + 2 * ch;
    double sample, thresh = vc->thresh * vc->tm[ch];
    uint64_t bits, mask, *optr = vc->load;

    while (shift > 63) { shift -= 64; optr++; }   /* if/while */
    for (tick = 0; tick < sps; tick += vc->over) {
        sample = vc->chan[tick];
        /* quantize it to 2 bits */
        if (adcq) sample = adcq(sample);
        if      (sample >  thresh) bits = 0x3;
        else if (sample < -thresh) bits = 0x0;
        else if (sample >     0.0) bits = 0x2;
        else if (sample <     0.0) bits = 0x1;
        else  /* sample == 0.0 */  bits = 0x1;    /* minor bias */
        /* update statistics up to limit */
        if (vc->bc[ch] < bsmx) {
            (vc->bs[ch][bits])++;
            (vc->bc[ch])++;
            vc->sq[ch] += sample*sample;
            vc->ab[ch] += fabs(sample);
        }
        /* install the octet bits at proper location */
        bits <<= shift;
        mask = 0x3;
        mask <<= shift;
        (*optr) &= ~mask;
        (*optr) |= bits;
        /* move to the next bit location for this channel */
        shift += 2 * vc->chans;
        while (shift > 63) { shift -= 64; optr++; } /* if/while */
    }

    if (vc->bc[ch] <= bsmx) update_thresh_mult(ch, vc);

    if (ch == (vc->chans-1)) {
        /* undo previous advancement, then forward 2 bits */
        shift -= 2 * (vc->chans - 1);
        while (shift < 0) { shift += 64; optr--; }  /* if/while */
        vc->shift = shift;          /* starting ch 0 shift */
        vc->load = optr;            /* starting ch 0 load */
    }
}

#define USE_POINTER_CHECK 1
#if USE_POINTER_CHECK
/*
 * Debugging paranoia
 */
profile void pointer_check(int where, int sn, int ch, VdifConf *vc)
{
    static uint64_t *p;
    static int os;
    int bits;
    if (where == 0) { p = vc->load; os = vc->shift; bits = 0; }
    if (where == 1) { bits = (vc->load - p) * 64 + (vc->shift - os); }
    if (verb>4) {
        fprintf(stdout,
            "%d-Station-%d[%d] shift %2d load  %p <= %p <= %p %d\n",
            where, sn, ch, vc->shift,
            &vc->vhd[VDIF_HDR_OCTETS], vc->load, &vc->vhd[vc->opp], bits);
    }
    assert(vc->load >= &vc->vhd[VDIF_HDR_OCTETS]);
    assert(vc->load <= &vc->vhd[vc->opp]);
    assert(vc->shift >= 0);
    assert(vc->shift < 64);
}
#else /* USE_POINTER_CHECK */
#define pointer_check(W,S,C,P)  0
#endif /* USE_POINTER_CHECK */

/*
 * Channelize the data and populate the packet
 * Returns 0 if packet is incomplete.
 */
profile int packetize_station(int sn, int sps)
{
    VdifConf *vc = vcnf + sn;
    int ch;

    for (ch = 0; ch < vc->chans; ch++) {
        pointer_check(0,sn,ch,vc);
        channelize(ch, vc, sps);
        load_chan(ch, vc, sps);
        pointer_check(1,sn,ch,vc);
    }

    if (vc->load == &vc->vhd[vc->opp]) {        /* packet is full */
        vc->load = &vc->vhd[VDIF_HDR_OCTETS];   /* start of data */
        vc->shift = 0;
        return(1);
    }
    return(0);
}

/*
 * Receive the common signal with a noisy receiver
 * We save a boatload of time by not generating receiver noise
 * a frequencies higher than the eventual sampling rate.
 *
 * But that would be dishonest, and not correct, either.
 */
profile void receive_common(double *common, int sps, double *wp, int over)
{
    int ss, os;
    double gv;
    for (ss = 0; ss < sps; ) {
        gv = gsl_ran_gaussian_ziggurat(rng_inst, 1.0);
        wp[ss++] = common[ss] + gv;
        if (honest > HONESTY_LINE)
            for (os = 1; os < over && ss < sps; os++)
                wp[ss++] = common[ss] + gv;
    }
}

/*
 * Fabricate the station data using the common slice samples.
 * The gaussian here simulates the local station receiver noise.
 */
void fabricate_station(int ns, double *common, int sps)
{
    receive_common(common, sps, vcnf[ns].work, vcnf[ns].over);
    if (packetize_station(ns, sps)) output_station(ns);
    /* spectral diagnostic used one slice only */
    if (vcnf[ns].spec) {
        if (vcnf[ns].osp) fclose(vcnf[ns].osp);
        if (vcnf[ns].name) free(vcnf[ns].name);
        vcnf[ns].osp = 0;
        vcnf[ns].name = 0;
    }
}
 
/*
 * Create work arrays for the station.  The complex allocations here
 * are twice as large as necessary for conservative interface safety.
 * fftw_malloc is identical to malloc (we are told) except for possible
 * special alignment restrictions.
 *
 * We could probably reuse some of these arrays, but the current
 * plan allows for eventual thread parallelism, perhaps.
 *
 * Returns null on malloc errors.
 */
int initialize_station(int ns, int sps)
{
    int ch, st;
    VdifConf *vdcp = &vcnf[ns];
    vdcp->sigma = sqrt (1.0 + csigma * csigma);
    vdcp->thresh *= vdcp->sigma;
    vdcp->work = (double *)fftw_malloc(sizeof(double) * sps);
    if (!vdcp->work) return(perror("malloc"),1);
    vdcp->spec = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * sps);
    if (!vdcp->spec) return(perror("malloc"),1);
    vdcp->chop = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * sps);
    if (!vdcp->chop) return(perror("malloc"),1);
    vdcp->chan = (double *)fftw_malloc(sizeof(double) * sps);
    if (!vdcp->chan) return(perror("malloc"),1);
    /* zero statistics counters */
    for (ch = 0; ch < vdcp->chans; ch++) {
        vdcp->tm[ch] = 1.0;
        vdcp->sq[ch] = 0.0;
        vdcp->ab[ch] = 0.0;
        for (st = 0; st < 4; st++) vdcp->bs[ch][st] = 0;
        vdcp->bc[ch] = 0;
    }
    if (verb>0) describe_station(vdcp);
    report_processing(0, 0, "fft0", verb>1);
    vdcp->plan_ws = fftw_plan_dft_r2c_1d(sps,
        vdcp->work, vdcp->spec, FFTW_ESTIMATE);     /* always FORWARD */
    vdcp->plan_cc = fftw_plan_dft_c2r_1d(sps,
        vdcp->chop, vdcp->chan, FFTW_ESTIMATE);     /* always BACKWARD */
    report_processing(0, 0, "fft1", verb>1);
    if (vdcp->name) vdcp->osp = fopen(vdcp->name, "w");
    return(0);
}

/*
 * End of life tear-down: close the file, free willie.
 */
void destroy_station(int ns)
{
    VdifConf *vdcp = &vcnf[ns];
    fclose(vdcp->ofp);
    fftw_free(vdcp->work);
    fftw_free(vdcp->spec);
    fftw_free(vdcp->chop);
    fftw_free(vdcp->chan);
    fftw_destroy_plan(vdcp->plan_ws);
    fftw_destroy_plan(vdcp->plan_cc);
}

/*
 * eof
 */
