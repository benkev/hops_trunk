/***************************************************
*  Subroutine to generate data for the plot        *
*  structure . cmn 91.9.??                         *
*                                                  *
*  93.6.16 rjc generate rate spectrum for all lags *
*  99.8.16 rjc modify for Mk4                      *
*  00.10.20 cjl modify for fractional APs          *
***************************************************/

#include "msg.h"
#include "mk4_data.h"
#include "param_struct.h"
#include "pass_struct.h"
#include "ffmath.h"
#include <math.h>
#include <stdio.h>
#include "hops_complex.h"
#include "apply_funcs.h"
//#include "ff_misc_if.h"
#include <fftw3.h>

// MAXLAG == 8192 so 4*MAXLAG is 4x MBDMXPTS which is 3x more than needed for MBD.
// However, Y is re-used with the lagdata and 4*MAXLAG is then needed.
// +3 is temporarily for slop...
// assert(plot.num_mb_pts < 8192)

#ifndef GRID_PTS
#define GRID_PTS 2048
#endif /* GRID_PTS = MBDMXPTS / MBDMULT */

int make_plotdata(struct type_pass *pass)
    {
    extern struct type_param param;
    extern struct type_status status;
    extern struct type_plot plot;
    struct freq_corel *pdata;
    struct data_corel *datum;
    hops_complex Z, sum;
    static hops_complex X[2*MAXMAX], Y[4*MAXLAG+3], sbsp[MAXFREQ][2*MAXLAG], sum_ap[MAXAP];
    hops_complex sum_all, sum_freq;
    extern hops_complex vrot(int, double, double, int, int, struct type_pass*);
    hops_complex wght_phsr;
    int maxchan[MAXFREQ], i, j, np, fr, ap, lag, st, seg, lagbit;
    double ap_seg, max[MAXFREQ], maxv, peak, frac,
           sbdbox[MAXFREQ], rj, c, nap,
           offset, maxamp, sumwt, q[3],
           yy[3], eff_npol, sb_factor;
    static double ap_cnt[MAXAP], temp[MAXAP];

    int n, ij,
        maxi, npmax, nl;
                                        // function prototypes
    //int minvert (double [3][3], double [3][3]);
    void ion_covariance (struct type_pass *);
    extern void calc_rms (struct type_pass* );

                                        /* Make sure data will fit */
    if (param.nlags*param.num_ap > MAX_APXLAG)
        {
        msg ("Too many lags and/or aps for plot arrays", 2);
        return (-1);
        }

    fftw_plan fftplan;
    pdata = pass->pass_data;            /* For convenience */

                                        /* Calculate delay rate spectrum */
                                        /* (at max delay channel and averaged */
                                        /* over all frequencies */
    np = status.drsp_size;
    npmax = np;
    if (npmax < 256) npmax = 256;       /* Empirically determined from FRNGE plots */

                                        /* first apply rotator to single band */
                                        /* delay values (at max sb delay) and */
                                        /* total sum over all freqs */
    for (i = 0; i < MAXAP; i++)
        X[i] = 0.0;

    for (fr = 0; fr < pass->nfreq; fr++)
        for(ap = pass->ap_off; ap < pass->ap_off+pass->num_ap; ap++)
            {
            datum = pdata[fr].data + ap;
            Z = datum->sbdelay[status.max_delchan]
              * vrot (ap,status.dr_max_global, status.mbd_max_global, fr, datum->sband, pass);
                                        /* Weight by fractional AP */
            frac = 0.0;
            if (datum->usbfrac >= 0.0) frac = datum->usbfrac;
            if (datum->lsbfrac >= 0.0) frac += datum->lsbfrac;
                                        /* When both sidebands added together, */
                                        /* we use the mean fraction */
            if ((datum->usbfrac >= 0.0) && (datum->lsbfrac >= 0.0)) frac /= 2.0;
            Z = Z * frac;

            X[ap-pass->ap_off] = X[ap-pass->ap_off] + Z;
            }
                                        /* Then perform FFT over time to obtain */
                                        /* d-r spectrum avg'd over all frequencies */
    fftplan = fftw_plan_dft_1d (npmax, (fftw_complex*) X, (fftw_complex*) X, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute (fftplan);
    for (i = 0; i < npmax; i++)
        {
        rj = (double)i - (double)(npmax/2);
        offset = status.dr_max_global * pdata[0].frequency * param.acc_period
                        * (double)npmax;
        rj -= offset;
        if (rj < -0.5) rj += (double)npmax;
        if (rj > (npmax - 0.5)) rj -= (double)npmax;
        j = (int)(rj + 0.5);
        plot.d_rate[i] = abs_complex (X[j]) / (double)status.total_ap_frac;
        }
                                        /* adjust dr plot for effects of coherence time
                                         * if that feature has been invoked  rjc 2006.4.27 */
    if (pass->control.t_cohere > 0.0)
        {
        n = npmax * param.acc_period / pass->control.t_cohere;
        n = (n%2) ? n : n+1;            /* force n to be odd for symmetry (no bias) */
        msg ("convolving plot rate spectrum over %d res. elements", 0, n);
        msg ("delay rate spectrum:", 0);
                                        /* store rate spectrum temporarily & prescale */
        for (i = 0; i < npmax; i++)
            temp[i] = plot.d_rate[i] / n;

        for (i = 0; i < npmax; i++)
            {
            plot.d_rate[i] = 0.0;
                                        /* form boxcar sum about the ith point */
            for (j=-(n/2); j<=n/2; j++)
                {
                ij = i + j;             /* force sum of i+j to wrap (lie within 0..npmax-1) */
                ij = (ij < 0) ? ij + npmax : ij;
                ij = (ij >= npmax) ? ij - npmax : ij;

                plot.d_rate[i] += temp[ij];
                }
            msg ("%3d %12.6lf", 0, i, plot.d_rate[i]);
            }
        }


    plot.dr_size = np;
    plot.dr_size_max = npmax;
    plot.num_ap = pass->num_ap;
    plot.num_freq = pass->nfreq;

                                        /* Calculate multi band delay. */
                                        /* Apply rotator to single band delay */
                                        /* values and add up over time. */
                                        /* MBD FFT max size hardcoded to 8192 at present */
                                        // set floor of 256 points in mbd plot
    plot.num_mb_pts = (status.grid_points < 256) ? 256 : status.grid_points;
    for (i = 0; i < plot.num_mb_pts; i++)
        {
        X[i] = 0.0;
        Y[i] = 0.0;
        }
    for (fr = 0; fr < pass->nfreq; fr++)
        {
        for (ap = pass->ap_off; ap < pass->ap_off+pass->num_ap; ap++)
            {
            datum = pdata[fr].data + ap;
            Z = datum->sbdelay[status.max_delchan]
              * vrot (ap, status.dr_max_global, 0.0, fr, datum->sband, pass);
                                        /* Weight by fractional AP */
            frac = 0.0;
            if (datum->usbfrac >= 0.0)
                frac = datum->usbfrac;
            if (datum->lsbfrac >= 0.0)
                frac += datum->lsbfrac;
                                        /* When both sidebands added together, */
                                        /* we use the mean fraction */
            if ((datum->usbfrac >= 0.0) && (datum->lsbfrac >= 0.0))
                frac /= 2.0;
            Z = Z * frac;

            X[fr] = X[fr] + Z;
            }                           /* Space frequencies in array for FFT */
        // allow garbage to be ignored
        if (status.mb_index[fr] < GRID_PTS)
            Y[status.mb_index[fr]] = X[fr];
        }
                                        /* FFt across freq to mbdelay spectrum */
    fftplan = fftw_plan_dft_1d (plot.num_mb_pts, (fftw_complex*) Y, (fftw_complex*) Y, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute (fftplan);
    for (i = 0; i < plot.num_mb_pts; i++)
        {
        j = i - plot.num_mb_pts / 2;
        if (j < 0)
            j += plot.num_mb_pts;
        plot.mb_amp[i] = abs_complex(Y[j]) / status.total_ap_frac;
        }

                                        /* Calculate single band delay, */
                                        /* Xpower spectrum, & sbdbox */
    for (i = 0; i < 2*MAXLAG; i++)
        X[i] = 0.0;
    for (i = 0; i < 4*MAXLAG; i++)
        Y[i] = 0.0;
    for (i = 0; i < MAXFREQ; i++)
        {
        max[i] = 0.0;
        maxchan[i] = 0;
        }
    nl = param.nlags;
    for (lag = 0; lag < nl * 2; lag++)
        {
        for (fr = 0; fr < pass->nfreq; fr++)
            {
            sum = 0.0;
            for (ap = pass->ap_off; ap < pass->ap_off+pass->num_ap; ap++)
                {
                datum = pdata[fr].data + ap;
                Z = datum->sbdelay[lag]
                  * vrot (ap, status.dr_max_global, status.mbd_max_global, fr, 0, pass);
                                        // sb correction made by spectral below
                                        /* Weight by fractional AP */
                frac = 0.0;
                if (datum->usbfrac >= 0.0) frac  = datum->usbfrac;
                if (datum->lsbfrac >= 0.0) frac += datum->lsbfrac;
                                        /* When both sidebands added together, */
                                        /* we use the mean fraction */
                if ((datum->usbfrac >= 0.0) && (datum->lsbfrac >= 0.0)) frac /= 2.0;
                Z = Z * frac;

                sum = Z + sum;
                }
                                        /* sbsp is singleband spect. for each freq */
                                        /* X is singleb sp. summed over all freq's */
            sbsp[fr][lag] = sum;
            X[lag] = X[lag] + sum;
                                        /* Find the max lag for each frequency */
            if (abs_complex(sum) > max[fr])
                {
                max[fr] = abs_complex (sum);
                maxchan[fr] = lag;
                }
            }
        plot.sb_amp[lag] = abs_complex(X[lag]) / status.total_ap_frac;

        if (lag == status.max_delchan)
            status.coh_avg_phase = arg_complex (X[lag]);
        j = lag - nl;
        if (j < 0)
            j += 4 * nl;
        if (lag == 0)
            j = 2 * nl;             // pure real lsb/dc channel goes in middle
        Y[j] = X[lag];
        }
                                        /* FFT sband spectrum -> XPower spectrum */
    fftplan = fftw_plan_dft_1d (4 * nl, (fftw_complex*) Y, (fftw_complex*) Y, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute (fftplan);
                                        // scale crosspower spectra for correct amplitude
    for (i = 0; i < 2*nl; i++)
       {
       j = nl - i;
       if (j <= 0)
           sb_factor = status.total_usb_frac > 0 ?
               sqrt (0.5) / (M_PI * status.total_usb_frac) : 0.0;
       else
           sb_factor = status.total_lsb_frac > 0 ?
               sqrt (0.5) / (M_PI * status.total_lsb_frac) : 0.0;

       if (j < 0)
           j += 4*nl;

       plot.cp_spectrum[i] = Y[j] * sb_factor;
                                        /* Counter rotate to eliminate sband delay */
       Z = exp_complex(-cmplx_unit_I * (status.sbd_max * (i-nl) * M_PI / (status.sbd_sep * 2*nl)));
       plot.cp_spectrum[i] = Z * plot.cp_spectrum[i];
       }
                                        // if requested, fit video bandpass params
                                        // and print them out
    if (pass->control.vbp_fit == TRUE)
        fit_vbp (nl);
                                        /* For each freq, interpolate through */
                                        /* 3 sband delays to find sbdbox */
    for (fr = 0; fr < MAXFREQ; fr++)
        status.sbdbox[fr] = 0.0;
    for (fr = 0; fr < pass->nfreq; fr++)
       {
       for (i=0; i<3; i++)
           yy[i] = abs_complex (sbsp[fr][maxchan[fr]-1+i]);
       parabola (yy, -1.0, 1.0, &peak, &maxv, q);
       status.sbdbox[fr] = maxchan[fr] + peak + 1;
       }
    status.sbdbox[MAXFREQ] = nl + 1 + status.sbd_max / status.sbd_sep;
                                        // effective number of polarizations
    eff_npol = pass->npols > 2 ? 2 : pass->npols;
                                        /*  Signal to Noise Ratio */
    status.snr = status.delres_max * param.inv_sigma
            * sqrt((double)status.total_ap_frac * eff_npol)
                       / (1.0E4 * status.amp_corr_fact);
    status.snr = adjust_snr(pass, &status);

    msg ("SNR %8.2f", 1, status.snr);

                                        /* Probability of false detection */
    status.prob_false = 1.0 - (pow (1.0 - exp(-status.snr * status.snr / 2.0),
                                    (double)status.pts_searched));
    if (status.prob_false < 0.01) status.prob_false = status.pts_searched
                                   * exp(-status.snr * status.snr / 2.0);

                                        /*  Calculate fringe values */
    status.timerms_phase = 0.0;
    status.timerms_amp = 0.0;
    status.freqrms_phase = 0.0;
    status.freqrms_amp = 0.0;
    status.inc_avg_amp = 0.0;
    status.inc_avg_amp_freq = 0.0;

    for (i = 0; i < pass->num_ap; i++)
        {
        ap_cnt[i] = 0;
        sum_ap[i] = 0.0;
        }
    sum_all = 0.0;
                                        /* For each freq & ap, the fringe phasor */
                                        /* is the sband delay spectrum at the max */
                                        /* delay chan., rotated by vrot() */
    for (fr = 0; fr < pass->nfreq; fr++)
        {
        sum_freq = 0.0;
        sumwt = 0.0;
        for (ap = pass->ap_off; ap < pass->ap_off+pass->num_ap; ap++)
            {
            datum = pdata[fr].data + ap;
            plot.weights[fr][ap] = 0.0;
                                        /* Flag bits set indicate data present */
            if (datum->flag != 0)
                {
                ap_cnt[ap]++;
                if (datum->lsbfrac >= 0.0)
                    plot.weights[fr][ap] += datum->lsbfrac;
                if (datum->usbfrac >= 0.0)
                    plot.weights[fr][ap] += datum->usbfrac;
                                        /* True weights - factor of 2 inherent */
                                        /* in sbdelay values for double sideband */
                sumwt += plot.weights[fr][ap];
                                        /* Double-sideband relative weights, */
                                        /* take mean */
                if ((datum->usbfrac >= 0.0) && (datum->lsbfrac >= 0.0))
                    plot.weights[fr][ap] /= 2.0;

                Z = datum->sbdelay[status.max_delchan]
                  * vrot(ap,status.dr_max_global,status.mbd_max_global,fr,datum->sband,pass);
                plot.phasor[fr][ap] = Z;
                }
            else
                plot.phasor[fr][ap] = 0.0;
            wght_phsr = plot.phasor[fr][ap] * plot.weights[fr][ap];
            sum_all = sum_all + wght_phsr;
            sum_freq = sum_freq + wght_phsr;
            sum_ap[ap] = sum_ap[ap] + wght_phsr;
            }
                                        /* Changed to reflect fractional APs */
        c = (sumwt > 0.0) ? status.amp_corr_fact/sumwt : 0.0;

        status.fringe[fr] = sum_freq * c;
        msg ("status.fringe[%d] %f %f", 0, fr, status.fringe[fr]);
        status.inc_avg_amp_freq += abs_complex(sum_freq) * status.amp_corr_fact;
        }

    if (param.ion_pts > 1)
        ion_covariance (pass);          // do ionosphere covariance analysis

    for (ap = pass->ap_off; ap < pass->ap_off+pass->num_ap; ap++)
        {
        plot.phasor[pass->nfreq][ap] = sum_ap[ap];
        plot.weights[pass->nfreq][ap] = ap_cnt[ap];
        }
    status.coh_avg_phase = arg_complex (sum_all);
    status.inc_avg_amp_freq /= status.total_ap_frac;
                                        /* Noise bias correction ? */
    status.inc_avg_amp_freq /= ((1.0 + 1.0/(2.0 * status.snr
                                        * status.snr / pass->nfreq)));

    /* NB: this routine calc plot data by segment in addition to RMS */
    calc_rms (pass);

    fftw_destroy_plan (fftplan);
    return(0);
    }
