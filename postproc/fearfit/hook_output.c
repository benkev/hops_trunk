/*
 * $Id: hook_output.c 1509 2017-01-09 21:18:52Z gbc $
 *
 * Routine to supply numbers when pc_mode is manual
 * regardless of whether a plot is being made.
 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mk4_data.h"
#include "param_struct.h"
#include "pass_struct.h"

extern struct mk4_fringe fringe;
extern struct type_param param;
extern struct type_status status;
extern struct type_plot plot;
//extern double c_phase();

static int vrb = 2;             /* 3 to shut off the commentary */
static int flip = 0;            /* 1 assigns phases/delays to rem */
static int mast = 0;            /* controls masthead information */
static int zphs = 0;            /* controls zero phase option */
static double zphs_val = 0.0;   /* value of the zero phase offset */

/* if this code ever gets accepted, these functions get replaced */
static int get_pcphase_verb(void)
{
    char *ep = getenv("HOPS_FEARFIT_PCPHASE_VERB");
    int verb = ep ? atoi(ep) : 2;
    return(verb);
}
static int get_pcphase_mast(void)
{
    char *ep = getenv("HOPS_FEARFIT_PCPHASE_NOMAST");
    int nomast = ep ? atoi(ep) : 0;
    return(nomast);
}
static int get_pcphase_swap(void)
{
    char *ep = getenv("HOPS_FEARFIT_PCPHASE_SWAP");
    int swap = ep ? atoi(ep) : 0;
    return(swap);
}
static int get_pcphase_zphs(void)
{
    char *ep = getenv("HOPS_FEARFIT_PCPHASE_ZPHS");
    zphs_val = ep ? atof(ep) : 0.0;
    return(ep ? 1 : 0);
}

/* used for a sort */
typedef struct ch_sbd { int chn; double sbd; } ChSbd;
/* compares them */
static int chsbd_cmp(const void *p1, const void *p2)
{
    ChSbd *cs1 = (ChSbd*)p1, *cs2 = (ChSbd*)p2;
    if (cs1->sbd < cs2->sbd)return (-1);
    if (cs1->sbd > cs2->sbd)return ( 1);
    return(0);
}

/* something for a library, later */
static char *pol_string(int pol)
{
    char *polstr;
    switch(pol) {
    case 0: polstr = "LL"; break;
    case 1: polstr = "RR"; break;
    case 2: polstr = "LR"; break;
    case 3: polstr = "RL"; break;
    default: polstr = "??"; break;
    }
    return(polstr);
}

/* return the proper lower case letter */
static int pol_letter(int pol, int rr)
{
    char *polstr = pol_string(pol);
    switch (polstr[rr]) {
    case 'L': return('l'); break;
    case 'R': return('r'); break;
    }
    return('?');
}

/*
 * Find the channels to use for the interpolation
 * If HOPS_FEARFIT_PCPHASE_NCHAN is set, we are talking about a
 * set of channels around the gap.  If there are less than 2 channels
 * in the sampler, a&b and A&B end up identified and we're headed
 * towards a bit noop.
 */
static void find_channels(int *ch_a, int *ch_b, int *ch_A, int *ch_B,
                          struct type_pass *pass, int first, int final,
                          double d[])
{
    int ch, ls0, nc;
    char *p_a, *p_b, *p_A, *p_B;
    char *ep = getenv("HOPS_FEARFIT_PCPHASE_NCHAN");

    ls0 = strlen(pass->control.psamplers[0]);
    nc = ep ? atoi(ep) : 2;
    if (nc > ls0) nc = ls0;
    
    p_a = pass->control.psamplers[0] + ls0 - nc;
    p_b = pass->control.psamplers[0] + ls0 - 1;
    p_A = pass->control.psamplers[1] -  1  + 1;
    p_B = pass->control.psamplers[1] -  1  + nc;
    for (ch = first; ch <= final; ch++)
        {
        if (pass->pass_data[ch].freq_code == *p_a) *ch_a = ch;
        if (pass->pass_data[ch].freq_code == *p_b) *ch_b = ch;
        if (pass->pass_data[ch].freq_code == *p_A) *ch_A = ch;
        if (pass->pass_data[ch].freq_code == *p_B) *ch_B = ch;
        /* back to frequency for clarity in debugging */
        // already done
        // d[ch] /= (360.0 * fringe.t208->resid_mbd);
        }
    if (vrb<1) fprintf(stdout, "\n* sampler phase shift using"
        " %c[%d],%c[%d] <-> %c[%d],%c[%d]",
        *p_a, *ch_a, *p_b, *ch_b, *p_A, *ch_A, *p_B, *ch_B);
}

/*
 * Broken out as a separate function for the debugging statement
 */
static double calc_delta(double lower, double upper)
{
    double delta = lower - upper;
    if (vrb<1) fprintf(stdout,
            "\n* sampler shift (%+7.3f - %+7.3f) = %+7.3f",
            lower, upper, delta);
    return(delta);
}

/*
 * Do a linear fit on the point pairs on each branch and jigger
 * them as needed to get the smallest slope as a way to deal with
 * the branch cut, this is ok on the assumption that the slope is
 * small, which it is supposed to be at this point.
 * 
 * This is pretty crude and could be replaced with much smarter code.
 * FIXME:  generalize to more than 2 samplers.
 */
static double estimate_gap2(int ch_a, int ch_b, int ch_A, int ch_B,
                            double dzero, double c[], double d[])
{
    double delta, lower, upper;

    if (fabs(c[ch_a] - c[ch_b] + 360.0)/fabs(d[ch_a] - d[ch_b]) <
        fabs(c[ch_a] - c[ch_b]        )/fabs(d[ch_a] - d[ch_b]))
            c[ch_a] += 360.0;
    if (fabs(c[ch_a] - c[ch_b] - 360.0)/fabs(d[ch_a] - d[ch_b]) <
        fabs(c[ch_a] - c[ch_b]        )/fabs(d[ch_a] - d[ch_b]))
            c[ch_a] -= 360.0;
    if (fabs(c[ch_A] - c[ch_B] - 360.0)/fabs(d[ch_A] - d[ch_B]) <
        fabs(c[ch_A] - c[ch_B]        )/fabs(d[ch_A] - d[ch_B]))
            c[ch_B] += 360.0;
    if (fabs(c[ch_A] - c[ch_B] + 360.0)/fabs(d[ch_A] - d[ch_B]) <
        fabs(c[ch_A] - c[ch_B]        )/fabs(d[ch_A] - d[ch_B]))
            c[ch_B] -= 360.0;

    lower  = c[ch_a] * (dzero - d[ch_b]) / (d[ch_a] - d[ch_b]);
    lower += c[ch_b] * (dzero - d[ch_a]) / (d[ch_b] - d[ch_a]);
    upper  = c[ch_A] * (dzero - d[ch_B]) / (d[ch_A] - d[ch_B]);
    upper += c[ch_B] * (dzero - d[ch_A]) / (d[ch_B] - d[ch_A]);

    return(calc_delta(lower, upper));
}

/*
 * Calculate the average of some number of channels.  We do this
 * as a running average to deal with a possible 2pi phase wrap.
 */
static double running_ave(int ch_a, int ch_b, double c[], char *lab)
{
    double ave = 0.0;
    int ch, del, nc;
    del = (ch_b > ch_a) ? 1 : -1;
    if (vrb<1) fprintf(stdout, "\n* average %s -> ", lab);
    for (ch = ch_a, nc = 0; ; ch += del, nc++)
        {
        ave *= (double)nc++;
        ave /= (double)nc;
        if (fabs(c[ch] - 360.0 - ave) < fabs(c[ch] - ave))
            c[ch] -= 360.0;
        if (fabs(c[ch] + 360.0 - ave) < fabs(c[ch] - ave))
            c[ch] += 360.0;
        ave += c[ch]/(double)nc;
        if (vrb<1) fprintf(stdout, " [%d]%+7.3f ", ch, ave);
        if (ch == ch_b) break;
        }
    return(ave);
}

/*
 * A simpler method that just averages some number of channels on
 * each side.  An average of 2 on each side isn't supported.  Too bad.
 */
static double estimate_gapN(int ch_a, int ch_b, int ch_A, int ch_B,
                            double dzero, double c[])
{
    double lower, upper;
    int ch, dch, nc;

    lower = running_ave(ch_a, ch_b, c, "lower");
    upper = running_ave(ch_B, ch_A, c, "upper");
    return(calc_delta(lower, upper));
}

/*
 * A common diagnostic on the freq channels actually used.
 */
static void phase_freq_rep(int ch_a, int ch_b, int ch_A, int ch_B,
                           double dzero, double delta, double c[], double d[])
{
    fprintf(stdout,
        "\n* sampler phases (deg) %+8.3f %+8.3f %+8.3f %+8.3f %+8.3f",
        c[ch_a], c[ch_b], delta, c[ch_A], c[ch_B]);
    fprintf(stdout,
        "\n* sampler freq (MHz)   %+8.3f %+8.3f %+8.3f %+8.3f %+8.3f",
        d[ch_a], d[ch_b], dzero, d[ch_A], d[ch_B]);
}

/*
 * Calculate a phase shift between the samplers with a simple linear model
 * At the moment this code only works for 2 samplers.
 * FIXME:  generalize to more than 2 samplers.
 */
static void sdelta_phases(struct type_pass *pass, int first, int final, int rm,
                          double c[], double d[], double e[], double df)
{
    int ch, ch_a, ch_b, ch_A, ch_B, ss, pol;
    int policy = 0;
    char *ep = getenv("HOPS_FEARFIT_PCPHASE_POLICY");
    char *er = getenv("HOPS_FEARFIT_PCPHASE_DERATE");
    double dzero, delta;
    static char *policy_label[] = { "add shift to upper band",
                                    "sub shift from lower band",
                                    "shift both bands to center",
                                    "do nothing whatsoever" };

    policy = atoi(ep ? ep : "2");
    if (policy < 0 || policy > 3) policy = 3;

    find_channels(&ch_a, &ch_b, &ch_A, &ch_B, pass, first, final, d);

    dzero = (d[ch_b] + d[ch_A]) / 2.0;  /* midpoint */

    if ((ch_a == ch_b) || (ch_A == ch_B))
        policy = 3;
    else if ((fabs(ch_a - ch_b) == 1) || (fabs(ch_A - ch_B) == 1))
        delta = estimate_gap2(ch_a, ch_b, ch_A, ch_B, dzero, c, d);
    else
        delta = estimate_gapN(ch_a, ch_b, ch_A, ch_B, dzero, c);
    if (vrb<1) phase_freq_rep(ch_a, ch_b, ch_A, ch_B, dzero, delta, c, d);

    if (vrb<1) fprintf(stdout, "\n* sampler shift policy is: %s",
        policy_label[policy]);

    delta *= (er) ? atof(er) : 1.0;
    delta += df;

    if (vrb<3)
        {
        pol = pol_letter(pass->pol, flip);
        fprintf(stdout, "\n  pc_phases_%s ", pol);
        for (ch = first; ch <= final; ch++)
            fprintf(stdout, "%c", pass->pass_data[ch].freq_code);
        fprintf(stdout, " * applying sampler phase delta");
        for (ch = first, ss = 0; ch <= final; ch++, ss++)
            {
            switch (policy)
                {
                case 0: if (ch >= ch_A) e[ch] += delta;     break;
                case 1: if (ch <= ch_b) e[ch] -= delta;     break;
                case 2: if (ch >= ch_A) e[ch] += delta/2.0;
                        if (ch <= ch_b) e[ch] -= delta/2.0; break;
                default:                                    break;
                }
            if (e[ch] < -180.0) e[ch] += 360.0;
            if (e[ch] >  180.0) e[ch] -= 360.0;
            if (vrb<3) fprintf(stdout,
                "%s%+7.2f", (ss%rm)?" ":"\n    ", e[ch]);
            }
        fprintf(stdout, "\n*");
        }
}

/*
 * This is modified from make_postplot().  For each channel in the plot
 * just "do the math".  After developing this, it was arbitrarily resolved
 * that input phases are of the usb sense, hence sbmult is always 1.
 * We'll leave the variable here as a reminder in case things change.
 * FIXME:  generalize to more than 2 samplers.
 *
 * code is actually in generate_text.c now.
 */
static void phases_feedback(struct type_pass *pass,
                            int first, int final, int rm)
{
    int ch, ss, pol;
    double est_phase, sbmult, delta_delay;
    char *ep = getenv("HOPS_FEARFIT_PCPHASE_DELTA");
    char *sk = ep ? "* " : "";
    char *sp = ep ? "\n*   " : "\n    ";
    static double eph[MAXFREQ], e[MAXFREQ];
    static double f[MAXFREQ], m[MAXFREQ], c[MAXFREQ], d[MAXFREQ];
    int stnpol[2][4] = {0, 1, 0, 1, 0, 1, 1, 0}; // [stn][pol] = 0:L, 1:R

    pol = pol_letter(pass->pol, flip);
    if (vrb<3)
        {
        fprintf(stdout, "\n%s  pc_phases_%c ", sk, pol);
        for (ch = first; ch <= final; ch++)
            fprintf(stdout, "%c", pass->pass_data[ch].freq_code);
        }

    for (ch = first, ss = 0; ch <= final; ch++, ss++)
        {
        f[ch] =   (double)fringe.t207->ref_pcoffset[ch].lsb;
        m[ch] = - (double)fringe.t207->rem_pcoffset[ch].lsb;
        c[ch] = carg (status.fringe[ch]) * 180.0 / M_PI;
        d[ch] = 360.0 * fringe.t208->resid_mbd *
                (pass->pass_data[ch].frequency - fringe.t205->ref_freq);

        sbmult = (status.total_usb_frac > 0) ? 1 : 0;
        est_phase = status.pc_phase[ch][0][stnpol[0][pass->pol]]
                  - status.pc_phase[ch][1][stnpol[1][pass->pol]];
        est_phase *= 180.0 / M_PI;  // pc_phase in radians
        delta_delay = (param.mbd_anchor == MODEL) ?
            fringe.t208->resid_mbd :
            fringe.t208->resid_mbd - fringe.t208->resid_sbd;
        est_phase += sbmult * (carg (status.fringe[ch]) * 180.0 / M_PI
                  + 360.0 * delta_delay *
                    (pass->pass_data[ch].frequency - fringe.t205->ref_freq));

        //if (zphs) est_phase -= (fringe.t208->resphase + zphs_val);
        if (zphs) est_phase -= zphs_val;
        if (flip) est_phase = - est_phase;
        est_phase = ( fmod(est_phase, 360.0) );
        if (est_phase < -180.0) est_phase += 360.0;
        if (est_phase >  180.0) est_phase -= 360.0;
        if (vrb<3) fprintf(stdout, "%s%+7.2f", (ss%rm)?" ":sp, est_phase);
        e[ch] = f[ch] - m[ch];              /* flip is implicit */
        // if (flip) e[ch] = - e[ch];
        }

    if (vrb<1)
        {
        fprintf(stdout, "\n*");
        for (ch = first, ss = 0; ch <= final; ch++, ss++)
            fprintf(stdout, "%s%+7.2f", (ss%rm)?" ":"\n*rf ", f[ch]);
        for (ch = first, ss = 0; ch <= final; ch++, ss++)
            fprintf(stdout, "%s%+7.2f", (ss%rm)?" ":"\n*rm ", m[ch]);
        for (ch = first, ss = 0; ch <= final; ch++, ss++)
            fprintf(stdout, "%s%+7.2f", (ss%rm)?" ":"\n*cp ", c[ch]);
        for (ch = first, ss = 0; ch <= final; ch++, ss++)
            fprintf(stdout, "%s%+7.2f", (ss%rm)?" ":"\n*md ", d[ch]);
        fprintf(stdout, "\n*");
        }

    for (ch = first; ch <= final; ch++)
        d[ch] = (pass->pass_data[ch].frequency - fringe.t205->ref_freq);

    if (ep && (pass->control.nsamplers == 2))
        sdelta_phases(pass, first, final, rm, c, d, e, atof(ep));
}

/*
 * For the nn-th sampler group, sort the sbd sub-array for the channels of
 * that group and replace the outliers by the median of the middle patch.
 */
static void edit_delays(int nn, double medfrac, struct type_pass *pass,
                        int first, int final, double sbd[])
{
    static ChSbd chsbd[MAXFREQ];
    int cnt, clo, cup, med, ch, fc;
    if (medfrac <= 0.0 || medfrac >= 1.0)
        {
        fprintf(stdout,
            "\n*** illegal HOPS_FEARFIT_PCPHASE_MEDFR value %g\n", medfrac);
        return;
        }
    for (cnt = 0, ch = first; ch <= final; ch++)
        {
        fc = pass->pass_data[ch].freq_code;
        if (!strchr(pass->control.psamplers[nn], fc)) continue;
        chsbd[cnt].chn = ch;
        chsbd[cnt].sbd = sbd[ch];
        cnt++;
        }
    if (!cnt) return;
    clo = floor(((double)cnt * (1.0 - medfrac)) / 2.0);
    cup = ceil(((double)cnt * medfrac)) + clo;
    med = (clo + cup)/2;
    if ((0 > clo) || (clo > med) || (med > cup) || (cup > cnt)) return;
    qsort(chsbd, cnt, sizeof(ChSbd), chsbd_cmp);
    if (vrb<2) fprintf(stdout,
        "\n* outliers of sampler-%d %d..%d & %d..%d set to median sbd %+7.3f",
        nn, 0, clo, cup, cnt, chsbd[med].sbd);
    /* lower and upper fraction get the median value */
    for (ch = 0;  ch <= clo; ch++) chsbd[ch].sbd = chsbd[med].sbd;
    for (ch = cup; ch < cnt; ch++) chsbd[ch].sbd = chsbd[med].sbd;
    for (ch = 0; ch < cnt; ch++) sbd[chsbd[ch].chn] = chsbd[ch].sbd;
}

/*
 * If samplers was used, average the sbd values by the sampler pools.
 * If HOPS_FEARFIT_PCPHASE_MEDFR is set, edit the average to only
 * include that middle fraction of delays in the average.
 */
static void delays_averaged(struct type_pass *pass, int first, int final,
                            double sbd[], int rm)
{
    static double ave[MAXFREQ];
    static int cnt[MAXFREQ], chs[MAXFREQ];
    char *ep = getenv("HOPS_FEARFIT_PCPHASE_MEDFR");
    double medfrac = ep ? atof(ep) : 0.0;
    int ch, nn, fc, ss, pol;

    if (vrb>=3) return;

    if (medfrac) for (nn = 0; nn < pass->control.nsamplers; nn++)
        edit_delays(nn, medfrac, pass, first, final, sbd);

    pol = pol_letter(pass->pol, flip);
    fprintf(stdout, "\n  delay_offs_%c ", pol);

    for (ch = first; ch <= final; ch++)
        {
        fprintf(stdout, "%c", fc = pass->pass_data[ch].freq_code);
        chs[ch] = -1;
        for (nn = 0; nn < pass->control.nsamplers; nn++)
            if (strchr(pass->control.psamplers[nn], fc))
                {
                ave[nn] += sbd[ch];
                cnt[nn] ++;
                chs[ch] = nn;
                break;
                }
        }
    for (nn = 0; nn < pass->control.nsamplers; nn++)
        ave[nn] /= (cnt[nn]) ? cnt[nn] : 1;

    for (ch = first, ss = 0; ch <= final; ch++, ss++)
        fprintf(stdout, "%s%+7.3f",
            (ss%rm)?" ":"\n    ", (chs[ch] >= 0) ? ave[chs[ch]] : 0.0);
}

static int sbd_cmp(const void *a, const void *b)
{
    double da = *(double*)a, db = *(double*)b;
    if (da<db) return(-1);
    if (da>db) return( 1);
    return(0);
}

static double get_ave_delay(double *sbd, int first, int final)
{
    int i, mid = (first + final)/2;
    qsort(sbd + first, final - first + 1, sizeof(double), &sbd_cmp);
    if (vrb<2) fprintf(stdout, "\n* delays: ");
    for (i = first; i < mid; i++) if (vrb<2) fprintf(stdout, " %.3f", sbd[i]);
    if (vrb<2) fprintf(stdout, "\n* middle [%d..%d]:  %.3f\n* delays: ",
        sbd[i++], first, final);
    for (; i <= final; i++) if (vrb<2) fprintf(stdout, " %.3f", sbd[i]);
    return(sbd[(first + final)/2]);
}

/*
 * The sbd box is in lag space...  nl = param.nlags.
 */
static void delays_feedback(struct type_pass *pass,
                            int first, int final, int rm)
{
    static double sbd[MAXFREQ];
    int ch, ss, fr, pol;
    double est_delay, ref_delay, rem_delay, ave_delay;
    char *sk = pass->control.nsamplers ? "* " : "";
    char *sp = pass->control.nsamplers ? "\n*   " : "\n    ";
    char *spac = "\n*   ";
    int stnpol[2][4] = {0, 1, 0, 1, 0, 1, 1, 0};

    pol = pol_letter(pass->pol, flip);
    if (vrb<2)
        {
        fprintf(stdout, "\n* delay_offs_%c ", pol);
        for (ch = first; ch <= final; ch++)
            fprintf(stdout, "%c", pass->pass_data[ch].freq_code);
        fprintf(stdout, " * based on sbd alone ");
        }
    for (ch = first, ss = 0; ch <= final; ch++, ss++)
        {
        /* Cf. status.sbdbox[MAXFREQ] <=> status.sbd_max */
        sbd[ch] = (status.sbdbox[ch] - param.nlags - 1) * status.sbd_sep;
        /* change sign for correction, x1000 to report as ns */
        sbd[ch] *= - 1000.0;
        if (1-flip) sbd[ch] = - sbd[ch];
        if (vrb<2) fprintf(stdout, "%s%+7.3f", (ss%rm)?" ":"\n*   ", sbd[ch]);
        }

    ref_delay = pass->control.delay_offs[fr].ref
              + pass->control.delay_offs_pol[fr][stnpol[0][pass->pol]].ref;
    rem_delay = pass->control.delay_offs[fr].rem
              + pass->control.delay_offs_pol[fr][stnpol[1][pass->pol]].rem;
    if (vrb<2) fprintf(stdout, "\n* ref delay is %+7.3f", ref_delay);
    if (vrb<2) fprintf(stdout, "\trem delay is %+7.3f", rem_delay);
    for (ch = first, ss = 0; ch <= final; ch++, ss++)
        {
        fr = fcode(pass->pass_data[ch].freq_code);
        /* sbd sign was flipped above */
        if (flip) sbd[ch] += rem_delay;
        else      sbd[ch] += ref_delay;
        //ave += sbd[ch];
        }
    ave_delay = get_ave_delay(sbd, first, final);

    if (vrb<3)
        {
        fprintf(stdout, "\n%s  delay_offs_%c ", sk, pol);
        for (ch = first; ch <= final; ch++)
            fprintf(stdout, "%c", pass->pass_data[ch].freq_code);
        // fprintf(stdout, " * sbd + ref - rem");
        }
    for (ch = first, ss = 0; ch <= final; ch++, ss++)
        if (vrb<3) fprintf(stdout,
            "%s%+7.3f", (ss%rm)?" ":"\n    ", ave_delay);

    // probably busted
    //if (pass->control.nsamplers) delays_averaged(pass, first, final, sbd, rm);
}

/*
 * It is better to put out what we are working with so there are no surprises.
 */
static void standard_report(struct type_pass *pass,
                            int first, int final, int rm)
{
    int ch, ss, pol;
    if (vrb>=3) return;
    pol = pol_letter(pass->pol, 1-flip);
    if (mast<1)
        {
        fprintf(stdout, "\nif station %c", fringe.t202->baseline[1-flip]);
        fprintf(stdout, "\n  pc_phases_%c ", pol);
        for (ch = first; ch <= final; ch++)
            fprintf(stdout, "%c", pass->pass_data[ch].freq_code);
        for (ch = first, ss = 0; ch <= final; ch++, ss++)
            fprintf(stdout, "%s%+7.3f", (ss%rm)?" ":"\n    ", 0.0);
        fprintf(stdout, "\n  delay_offs_%c ", pol);
        for (ch = first; ch <= final; ch++)
            fprintf(stdout, "%c", pass->pass_data[ch].freq_code);
        for (ch = first, ss = 0; ch <= final; ch++, ss++)
            fprintf(stdout, "%s%+7.3f", (ss%rm)?" ":"\n    ", 0.0);
        }
    fprintf(stdout, "\nif station %c", fringe.t202->baseline[flip]);
    /* all that follows applies to reference station [flip=0] */
}

/*
 * Entry into more information
 *
 * Note all the client functions leave off the trailing \n on output.
 */
static void manual_feedback(char *rootfile, struct type_pass *pass)
{
    int first_ch, final_ch, rm;
    extern char control_filename[];
                                        /* bail if we are not MANUAL */
    if (param.pc_mode[0] != MANUAL || param.pc_mode[1] != MANUAL) return;

    vrb = get_pcphase_verb();
    flip = get_pcphase_swap();
    mast = get_pcphase_mast();
    zphs = get_pcphase_zphs();
    rm = (pass->control.nsamplers) ? strlen(pass->control.psamplers[0]) : 8;

    first_ch = (param.first_plot == 0) ? 0 : param.first_plot;
    final_ch = (param.nplot_chans == 0) ? pass->nfreq : param.nplot_chans;
    final_ch += first_ch - 1;

    if (vrb<3 && mast<2) fprintf(stdout,
        "*\n"
        "* root: %s\n"
        "* cf:   %s\n"
        "* PCal est for %.8s - %.8s [%c%c], "
        "fgroup %c, pol %s channels %c..%c:\n",
        rootfile, control_filename,
        fringe.t202->ref_name, fringe.t202->rem_name,
        fringe.t202->baseline[0], fringe.t202->baseline[1],
        pass->pass_data[0].fgroup, pol_string((int)pass->pol),
        pass->pass_data[first_ch].freq_code,
        pass->pass_data[final_ch].freq_code);
    if (vrb<3 && mast<2) fprintf(stdout,
        "* snr %.3f amp %.6f phs %.6f (%s %.6f)\n"
        "* sbd %.6f mbd %.6f frr %.6f\n*",
        status.snr, status.delres_max, fringe.t208->resphase,
        zphs ? "zeroing phase" : "original phase", zphs_val,
        status.sbd_max, status.mbd_max_global,
        status.dr_max_global * param.ref_freq);

    standard_report(pass, first_ch, final_ch, rm);
    phases_feedback(pass, first_ch, final_ch, rm);
    delays_feedback(pass, first_ch, final_ch, rm);
    if (vrb<3) fprintf(stdout, "\n");
}

#define OUTPUT_HOOK\
    manual_feedback(root->ovex->filename, pass)
#include "output.c"

/*
 * eof
 */
