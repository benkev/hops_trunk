/*
 * $Id$
 *
 * Something to load an alist and reduce it to the required Aobs data
 */

#include "dsolv3.h"

#include "adata.h"
#include "mk4_afio.h"
#include "mk4_util.h"

/*
 * Helper function to report ra/dec in sexy notation
 * into the buf (assumed large enough) which is returned.
 */
static char *radec(double ra, double dec, char *buf)
{
    int rahr, ramin, dcdg, dcmin;
    int sgn = (dec < 0) ? '-' : '+';

    rahr = floor(ra);
    ra -= (double)rahr;
    ra *= 60;
    ramin = floor(ra);
    ra -= (double)ramin;
    ra *= 60;

    if (dec < 0) dec = -dec;
    dcdg = floor(dec);
    dec -= (double)dcdg;
    dec *= 60;
    dcmin = floor(dec);
    dec -= (double)dcmin;
    dec *= 60;

    sprintf(buf, "%2dh%02dm%09.6lfs %c%02dd%02dm%09.6lfs",
        rahr, ramin, ra, sgn, dcdg, dcmin, dec);
    return(buf);
}

/*
 * Precess sky coords (assumed to be J2000) to epoch of observation
 * Time is seconds since 1/1/1980 which is UTC calendar seconds.
 * Unix clock was 946684800 at J2000
 * Unix clock was 315532800 at  1980
 *
 * From Allens Astrophysical Quantities (Sec 27.3) we compute from
 * time T in centuries from J2000 and ra(alpha) and dec(delta) are
 * in degrees.
 *
 * These are perhaps good to some number of sec of RA and arcsec of Decl
 * based on comparison to SCHED calculations.
 */
void precess_J2000_aaq(Aobs *ao)
{
    static char radec1[80], radec2[80];
    double T, M, N, alpha_m, delta_m;
    double alpha, delta, arad, drad, adeg, ddeg, rahr;

    alpha = ao->ra * 15.0;  /* hr to deg */
    delta = ao->dec;

    T = (double)ao->time1980 - (double)(946684800 - 315532800);
    T /= 86400.0;   /* to days */
    T /= 36525.0;   /* to centuries */

    M = T * (1.2812323 + T * (0.0003879 + T * 0.0000101));  /* deg */
    N = T * (0.5567530 - T * (0.0001185 + T * 0.0000116));  /* deg */

    arad = alpha/DEG_PER_RAD;
    drad = delta/DEG_PER_RAD;
    alpha_m = (alpha - 0.5 * (M + N * sin(arad) * tan(drad)))/DEG_PER_RAD;
    delta_m = (delta - 0.5 * N * cos(arad))/DEG_PER_RAD;
    
    adeg = alpha + M + N * sin(alpha_m) * tan(delta_m);
    rahr = adeg / 15.0;
    ddeg = delta + N * cos(alpha_m);

    if (opt.verb>2) fprintf(stdout,
        "## precess aaq  %s from %s)\n"
        "## precess aaq  %s to   %s (%g cen)\n",
        ao->source, radec(ao->ra, ao->dec, radec1),
        ao->source, radec(rahr, ddeg, radec2), T);

    ao->ra = rahr;
    ao->dec = ddeg;
}

/*
 * Precess sky coords (assumed to be J2000) to epoch of observation...
 * A variant coded
 * from http://www.cv.nrao.edu/~rfisher/Ephemerides/earth_rot.html
 *
 * Equations, accurate to one arcsecond, for computing precession
 * corrections to right ascension and declination for a given date
 * within about 20 years of the year 2000 are
 *
 *  RA = RA(2000) + (3.075 + 1.336 * sin(RA) * tan(Dec)) * y
 *  Dec = Dec(2000) + 20.04 * cos(RA) * y
 *
 * where y is the time from January 1, 2000 in fractional years, and
 * the offsets in RA and Dec are in seconds of time and arcseconds,
 * respectively. Very accurate telescope pointing calculations should
 * use the exact equations given on pages 104 and 105 of ref [1]. 
 * [1] Hohenkerk, C.Y., Yallop, B.D., Smith, C.A., Sinclair, A.T., 1992,
 * "Celestial Reference Systems", Chapter 3, p. 96, Explanatory Supplement
 * to the Astronomical Almanac, Seidelmann, P.K., Ed.,
 * U. S. Naval Observatory, University Science Books, Mill Valley, CA.
 */
void precess_J2000_nrao(Aobs *ao)
{
    static char radec1[80], radec2[80];
    double y, ddeg, rahr, RA, Dec;

    y = (double)ao->time1980 - (double)(946684800 - 315532800);
    y /= 86400.0;   /* to days */
    y /= 365.250;   /* to centuries */

    RA = 15.0 * ao->ra / DEG_PER_RAD;
    Dec = ao->dec / DEG_PER_RAD;

    rahr = ao->ra + ((3.075 + 1.336 * sin(RA) * tan(Dec)) * y) / 3600.0;
    ddeg = ao->dec + (20.04 * cos(RA) * y) / 3600.0;

    if (opt.verb>2) fprintf(stdout,
        "## precess nrao %s from %s)\n"
        "## precess nrao %s to   %s (%g cen)\n",
        ao->source, radec(ao->ra, ao->dec, radec1),
        ao->source, radec(rahr, ddeg, radec2), y/100.0);

    ao->ra = rahr;
    ao->dec = ddeg;
}

/*
 * Fix the GMST calculation based on the day number
 * If a nonzero value is supplied, it is used instead of
 * gst0 = (inp->doy - 264) * 24.0 / 365.25;  # with year mod
 * in dsolv3.c:make_working_data_approx().
 *
 * GMST = 17 days at 6107.1936775 days since JD 2451544.0
 * which is on day 264 of 2016 (20160920_040000)
 */
void fixGMST(Aobs *ao)
{
    double a = 24110.54841;
    double b = 8640184.812866;
    double c = 0.093104;
    double d = -6.2e-06;
    double du, T, GMST, ha, hi, y;

    y = (double)ao->time1980 - (double)(946684800 - 315532800); /* secs */
    du = y / 86400.0 + 0.5;                 /* days since JD 2451544.0 */

    T = du/36525.0;
    GMST = a + T * (b + T * (c + T * d));   /* seconds */
    GMST /= 3600.0;                         /* hours */

    ha = 24.0 * modf(GMST / 24.0, &hi);
    if (opt.verb>2) fprintf(stdout,
        "%lf days: gmst %lg hrs = %d days %lf hrs\n",
        du, GMST, (int)rint(hi), ha);

    /* subtract a day to coincide with Brian's quick formula */
    ao->gst0 = ha - 24.0;
}

/*
 * This allows us to cull what data we keep
 * Want doy, hhmm, ra(hr) dec(deg) rrate rdelay baseline
 *
 * If not usable, return !0.
 */
static int not_usable(Aobs *ao, fringesum *fr)
{
    static int cnt = 0;
    static char date[80];
    int meantime, syear, sday, shour, smin, ssec;

    memset(ao, 0, sizeof(Aobs));

    meantime = fr->time_tag + fr->duration/2;
    int_to_time(meantime, &syear,&sday,&shour,&smin,&ssec);
    ao->year = syear;
    ao->doy = sday;
    ao->hhmm = shour*100 + smin;

    ao->ra = fr->ra_hrs;
    ao->dec = fr->dec_deg;
    ao->rate = fr->delay_rate;      // resid delay rate psec/sec
    ao->delay = fr->resid_delay;    // resid multiband delay usec
                                    // resid_delay N*AMB + MBD to match SBD
    ao->bl[0] = fr->baseline[0];
    ao->bl[1] = fr->baseline[1];
    ao->bl[2] = 0;
    ao->snr = fr->snr;
    ao->ref_freq = fr->ref_freq;
    ao->polarization[0] = fr->polarization[0];
    ao->polarization[1] = fr->polarization[1];
    ao->polarization[2] = 0;
    strncpy(ao->source, fr->source, sizeof(ao->source));
    ao->time1980 = meantime;

    cnt ++;

    if (opt.base && strncmp(ao->bl, opt.base, 2)) return(1);
    if (opt.snr > 0.0 && fr->snr < opt.snr) return(2);
    if (opt.ref_freq > 0.0 &&
        fabs(fr->ref_freq - opt.ref_freq) > 0.1) return(3);
    if (opt.polarization &&
        strncmp(ao->polarization, opt.polarization, 2)) return(4);

    if (opt.precess == 1) precess_J2000_aaq(ao);
    if (opt.precess == 2) precess_J2000_nrao(ao);
    if (opt.fixGMST) fixGMST(ao);

    if (opt.verb>0) fprintf(stdout,
        "%03d %04d %7.4lf %8.4lf %9.3lf %11.5lf  %c%c %2s %7.2f %.3f %4d %3d\n",
        ao->doy, ao->hhmm, ao->ra, ao->dec, ao->rate, ao->delay,
        ao->bl[0], ao->bl[1], ao->polarization,
        ao->snr, ao->ref_freq, ao->year, cnt);

    return(0);
}

extern int load_alist(char *alist)
{
    static char line[1024];
    FILE *fp = fopen(alist, "r");
    int av, at, nalc = 100;
    int lcnt = 0, pcnt = 0, n2ok = 0, ninp = 0;
    Aobs *ao = malloc(100 * sizeof(Aobs));
    fringesum fringe;

    if (!fp) { perror("load_alist:fopen"); return(1); }
    if (!ao) { perror("load_alist:malloc"); return(2); }
    if (opt.verb>0) fprintf(stdout,
        "#dy hhmm rahm     decdm       rrate     rdelay   bl pz"
        "   snr   r.frq      year inp#\n");

    while (fgets(line, sizeof(line), fp)) {
        lcnt ++;
        if (afile_comment(line)) continue;
        aline_id(line, &av, &at);
        if (av < 6) continue;  /* only version 6 or greater supported */
        if (at != 2) continue; /* only looking for fringe files */
        pcnt ++;
        if (parse_fsumm(line, &fringe)) continue;
        n2ok ++;
        if (not_usable(ao+ninp, &fringe)) continue;
        if (++ninp == nalc) ao = realloc(ao, (nalc += 100) * sizeof(Aobs));
        if (!ao) { perror("load_alist:realloc"); return(3); }
    }

    fclose(fp);
    if (opt.verb>0) fprintf(stdout,
        "# Input fringe data from %s\n"
        "# %d lines %d parsed %d valid %d retained\n",
        alist, lcnt, pcnt, n2ok, ninp);
    opt.data = ao;
    opt.numb = ninp;
    return(0);
}

/*
 * eof
 */
