/*
 * $Id: anoise_describe.c 777 2012-08-30 20:50:21Z gbc $
 *
 * A deviant form of bnoise/vnoise which makes vdif formed data simulating
 * VDIF data in a variety of flavors.  The goal here is to simulate ALMA
 * data and correlate it against Mark5b-like or other flavors of data.
 *
 * This module opens existing files, and reports on contents based on
 * the header data.  It is intended as an aid to preparing for DiFX use.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "anoise.h"

static void work_harder(int fd, VdifExt *p0, VdifExt *pn, size_t nn, size_t np);

static int two_to_the(int power)
{
    int val = 1;
    while (power-- > 0) val <<= 1;
    return(val);
}

static double to_mjd(int re, int sc, int df)
{
    static int  mjdo[64] = {
        51544.0, 51726.0, 51910.0, 52091.0, 52275.0, 52456.0, 52640.0, 52821.0,
        53005.0, 53187.0, 53371.0, 53552.0, 53736.0, 53917.0, 54101.0, 54282.0,
        54466.0, 54648.0, 54832.0, 55013.0, 55197.0, 55378.0, 55562.0, 55743.0,
        55927.0, 56109.0, 56293.0, 56474.0, 56658.0, 56839.0, 57023.0, 57204.0,
        57388.0, 57570.0, 57754.0, 57935.0, 58119.0, 58300.0, 58484.0, 58665.0,
        58849.0, 59031.0, 59215.0, 59396.0, 59580.0, 59761.0, 59945.0, 60126.0,
        60310.0, 60492.0, 60676.0, 60857.0, 61041.0, 61222.0, 61406.0, 61587.0,
        61771.0, 61953.0, 62137.0, 62318.0, 62502.0, 62683.0, 62867.0, 63048.0
    };
    double mjd;
    if (re >= 0 && re < 64) mjd = mjdo[re]; else mjd = 0.0;
    if (df>0) sc++;
    mjd += (double)sc / 86400.0;
    return(mjd);
}

/*
 * #   2-letter antenna name
 * #   data file host name
 * #   data file path name
 * #   file format (Mark5b or VDIF/8224/2 or INTERLACEDVDIF/ch[:...]/8224/2)
 * #   das name (rdbe)
 */
static void report_packet(VdifExt *pkt, char *label, char *path, int size)
{
    int chans = two_to_the(pkt->num_chan_log2), bps;
    char id[3];
    double mjd;
    bps = pkt->bpsm1 + 1;
    id[0] = (pkt->stationID & 0x00FF);
    id[1] = (pkt->stationID & 0xFF00) >> 8;
    id[2] = 0;
    mjd = to_mjd(pkt->ref_epoch, pkt->secs_inre, pkt->df_num_insec);
    if (verb) printf("# %s packet(%s): %d@%d fr %5d w/ %d ch %d bps\n",
        label, id, pkt->ref_epoch, pkt->secs_inre, pkt->df_num_insec,
        chans, bps);
    printf("%smjd=%.10f\npath=%s\nid=%s\nformat=\"VDIF/%d/%d\"\nchans=%d\n",
        label, mjd, path, id, size, bps, chans);
}

/*
 * Seek and read suport function
 */
int seek_and_read(int fd, VdifExt *pkt, off_t where, char *label, size_t nb)
{
    int rv;
    if (lseek(fd, where, SEEK_SET) < 0)
        return(perror("lseek-3"),3);
    if ((rv = read(fd, pkt, sizeof(VdifExt))) != sizeof(VdifExt))
        return(fprintf(stderr, "Read %d at %ld",rv,where),perror("read-4"),4);
    if (label) report_packet(pkt, label, "same", nb);
    return(0);
}

/*
 * Grab the first and last packets and report stuff
 */
int study_data(char *file, int more)
{
    int fd;
    off_t lastpkt;
    size_t pktsize;
    struct stat sb;
    VdifExt first, final;
    if (stat(file, &sb))
        return(perror("stat-6"),6);
    if (!(fd = open(file, O_RDONLY)))
        return(perror("open-1"),1);
    if (read(fd, &first, sizeof(first)) != sizeof(VdifExt))
        return(perror("read-2"),2);
    pktsize = first.df_len_octets * 8;
    lastpkt = sb.st_size - pktsize;
    report_packet(&first, "first", file, pktsize);
    if (seek_and_read(fd, &final, lastpkt, 0, pktsize)) return(5);
    report_packet(&final, "final", file, pktsize);
    if (more) work_harder(fd, &first, &final, pktsize, sb.st_size/pktsize);
    if (close(fd))
        return(perror("close-5"),5);
    return(0);
}

/*
 * Entries from main
 */
int describe_data(char *file)
{
    return(study_data(file, 0));
}
int analyze_data(char *file)
{
    return(study_data(file, 1));
}

/*
 * Report what we found
 */
static void work_hard_report(long fps, long secs, size_t nb, VdifExt *p0)
{
    int chans = two_to_the(p0->num_chan_log2);
    int bps = p0->bpsm1 + 1;
    int sps = fps * 8 * (nb - 32) / chans / bps;
    if (verb>0) fprintf(stdout,
        "# %ld packets/second %ld seconds"
        "  %ld bytes/packet %d chans %d bits/sample\n",
        fps, secs, nb, chans, bps);
    fprintf(stdout, "sps=%d\nbw_mhz=%g\n", sps, (double)sps/2e6);
}

static long binary_search(int fd, VdifExt *b, VdifExt *e,
    size_t nb, size_t pb, size_t pe)
{
    VdifExt middle, *m = &middle;
    size_t pktnum, pm;
    char trial[5];
    int tn = 0;
    long fps;
    while ((b->secs_inre < e->secs_inre) && (pe > pb+1)) {
        sprintf(trial, "t%04d", tn++ % 10000);
        pm = (pb + pe)/2;
        if (seek_and_read(fd, m, (off_t)nb*pm, (verb>0)?trial:0, nb)) break;
        if (b->secs_inre < m->secs_inre) {  /* toss later packet */
            *e = *m;
            pe = pm;
        } else {                            /* toss early packet */
            *b = *m;
            pb = pm;
        }
        fps = b->df_num_insec + 1;
    }
    return(fps);
}

/*
 * Work out the bit rate using a binary search around a second transition
 * if we have at least one second of data.  If we have only one second,
 * all we can determine is a lower bound.
 */
static void work_harder(int fd, VdifExt *p0, VdifExt *pn, size_t nb, size_t np)
{
    long secs = 0; 
    long fps = 0;
    off_t try;
    VdifExt inner;
    if (verb>0) fprintf(stdout, "# bit rate with %ld pkts....\n", np);
    secs = pn->secs_inre - p0->secs_inre;
    if (secs <= 0) {
        /* this is just a lower bound */
        fps = pn->df_num_insec;
    } else {
        try = nb * ( (secs-1)*(np/secs) );
        if (seek_and_read(fd, &inner, try, (verb>0)?"hard_":0, nb)) return;
        fps = binary_search(fd, &inner, pn, nb, try/nb, np-1);
    }
    work_hard_report(fps, secs, nb, p0);
}

/*
 * eof
 */
