/*
 * $Id: dumpswin.c 2020 2017-09-26 21:45:21Z gbc $
 *
 * Main routine for yet another dump some data program.
 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
// #include <sys/types.h>

#include "difxio.h"


#define DEDUCE 0x10000
typedef struct {
    FILE *fp; char *name; int size; int bl; int fq; char pref; char prem;
} BLF;

/*
 * Global options
 */
int	    verb = 0;
int	    nchan = DEDUCE;
char        *bfile = 0;
char        *bpol = "";
int	    bint = 0, bfirst = 0, bcount = 1, bthis = 0;
int	    bfreq = -1;
long	    bline = -1;
int	    tsys = 0;
int         twgh = 0;
double	    difx_corr[257][257];
double	    mark_corr[257][257];
double      *integvals;
int         integhits = 0;
int	    maxa = -1, maxb = -1;
BLF         *blf = 0;
int         nblf = 0;
long        max_vis = 0L;

int parse_swin(char *file, int nchan);

/*
 * Boilerplate
 */
static int cmdhelp = 0;
static int usage(char *name)
{
    printf("Usage: %s [options] [times]\n", name);
    printf("where the options are:\n"
	"  -v            verbose, may be repeated for more\n"
	"  -b <file>     deposit data by baseline\n"
	"  -f <int>      -b limited to this freq_index\n"
	"  -x <int>      -b limited to this baseline (hex)\n"
        "  -p <XX>       -b limited to pol choice\n"
	"  -i            -b integrate amplitudes.\n"
	"  -j <f,c>      -i first (0) and count (999)\n"
	"  -c <int>      number of visibility channels\n"
        "  -m <int>      max number of visibilities (0)\n"
	"  -t <int>      0 or 1..4 provide integration across channel\n"
        "                too in comments or 5..8 just the integration\n"
        "                1/5 is all, 2/6 is all by first, 3/7 is all\n"
        "                but last, and 4/8 is all but first and last\n"
	"                These options only work with -v set.\n"
	"  -w            if set, apply the visibility weight\n"
	"\n"
        "With one -v, the amplitude and phase are computed and output\n"
	"in addition to the real and imaginary part of the data.\n"
	"\n"
	"The -b option splits the output by baseline into files with\n"
        "the supplied prefix.  Use the -f option to limit output to a\n"
        "particular frequency index and -x a baseline, e.g. 00000102\n"
        "If -i is set, the selected -f/-x case will be integrated.\n"
        "These options are only available if -v has been set.  E.g.\n"
        "\n"
        "  dumpswin -v -t2 -b foo -f 8 -x 00000102 -i DiFX.out\n"
        "\n"
	"When the -c value is at least %d, the behavior is to\n"
	"deduce the number of channels per visibility, which should\n"
	"not be larger than this value.  So normally don't use -c.\n"
        "\n"
        "In any case, processing stops after the max number of\n"
        "visibilities has counted down to zero.\n"
	"\n",
        nchan
    );
    return(cmdhelp = 1);
}
static int version(char **argv)
{
    if (!argv[1]) return(0);
    if (!strcmp(argv[1], "--help"))    return(usage(argv[0]));
    if ( strcmp(argv[1], "--version")) return(0);
    printf(__FILE__ "\t[" __DATE__ " " __TIME__ "]\n");
    return(cmdhelp = 1);
}
static int options(int argc, char **argv)
{
    int	    c;
    if (version(argv)) return(1);
    while ((c = getopt(argc, argv, "wvb:p:f:x:ij:c:t:m:")) != -1) switch(c) {
    case 'w': twgh++;						break;
    case 'v': verb++;						break;
    case 'c': nchan = atoi(optarg);				break;
    case 'b': bfile = optarg;                                   break;
    case 'p': bpol = optarg;                                    break;
    case 'f': bfreq = atoi(optarg);				break;
    case 'x': bline = strtol(optarg, 0, 16);			break;
    case 'i': bint++;						break;
    case 'j': if (2==sscanf(optarg,"%d,%d",&bfirst,&bcount))    break;
              else                                              return(2);
    case 't': tsys = atoi(optarg);				break;
    case 'm': max_vis = atol(optarg);                           break;
    default:							return(1);
    }
    return(0);
}
static int
cmdline(int *argc, char ***argv)
{
    int	    x = options(*argc, *argv);
    *argc -= optind;
    *argv += optind;
    return(x);
}

static void dump_integration(void)
{
    int ii;
    fprintf(stderr, "Dumping Integration on hits %d (%d < %d %d)\n",
        integhits, bfirst, bfirst+bcount, bthis);
    fprintf(stdout, "# Integration hits %d\n", integhits);
    for (ii = 0; ii < bint; ii++)
        fprintf(stdout, "%d %e\n", ii, integvals[ii]);
}

/*
 * Main Entry.
 */
int main(int argc, char **argv)
{
    int		errs = 0;

    /* basic command line parsing */
    if (cmdline(&argc, &argv)) return(!cmdhelp);

    while (argc-- && !errs) errs += parse_swin(*argv++, nchan);
    if (integvals) dump_integration();

    return(errs);
}

/*
 * Support for per-baseline output;
 * a list of BLFs gets built for each baseline.
 */
static void close_blfiles(void)
{
    while ((nblf > 0) && blf[--nblf].fp) {
        fclose(blf[nblf].fp);
        if (verb>2) fprintf(stderr, "Closed %s.\n", blf[nblf].name);
        free(blf[nblf].name);
    }
}
static int blfok(DifxVisRecord *vr)
{
    /* return nonzero to allow */
    if (bline < 0 && bfreq < 0 && !bpol[0]) return(1);
    if (bline < 0 && bfreq == vr->freqindex && !bpol[0]) return(1);
    if (bline == vr->baseline && bfreq < 0 && !bpol[0]) return(1);
    if (bline == vr->baseline && bfreq == vr->freqindex && !bpol[0]) return(1);
    if (bline == vr->baseline && bfreq == vr->freqindex &&
        bpol[0] == vr->polpair[0] && bpol[1] == vr->polpair[1]) return(1);
    return(0);
}
static FILE *bl2fp(DifxVisRecord *vr)
{
    static BLF nfp = { 0,0,0,0,0 };   /* for /dev/null output */
    int bl;

    if (!bfile) return(stdout);
    if (!nfp.fp) nfp.fp = fopen("/dev/null", "w");

    for (bl = 0; bl < nblf; bl++)   /* return previously opened file */
        if (blf[bl].bl == vr->baseline &&
            blf[bl].fq == vr->freqindex &&
            blf[bl].pref == vr->polpair[0] &&
            blf[bl].prem == vr->polpair[1])
                return(blfok(vr) ? blf[bl].fp : nfp.fp);
    if (!blfok(vr)) return(nfp.fp);

    if (verb>2) fprintf(stderr,
        "New bl[%d] %08x-%d-%c%d\n", bl, vr->baseline, vr->freqindex,
        vr->polpair[0], vr->polpair[1]);
    blf = realloc(blf, ++nblf * sizeof(BLF));
    if (!blf) return(perror("realloc"),nfp.fp);

    blf[bl].bl = vr->baseline;
    blf[bl].fq = vr->freqindex;
    blf[bl].size = strlen(bfile) + 20;
    blf[bl].name = malloc(blf[bl].size);
    blf[bl].pref = vr->polpair[0];
    blf[bl].prem = vr->polpair[1];
    if (!blf[bl].name) return(perror("malloc"),nfp.fp);
    snprintf(blf[bl].name, blf[bl].size, "%s.%08X.%02d.%c%c",
        bfile, blf[bl].bl, blf[bl].fq, blf[bl].pref, blf[bl].prem);
    blf[bl].fp = fopen(blf[bl].name, "w");
    if (!blf[bl].fp) return(perror("fopen"),nfp.fp);
    if (verb>2) fprintf(stderr, "Opened %s for output\n", blf[bl].name);

    nblf++;
    return(blf[bl].fp);
}

/*
 * This doesn't seem to have data in it anymore
 */
static void print_vis_params(FILE *fp, DifxParameters *vp)
{
    int rn;
    DifxRow *row = vp->rows;
    fprintf(fp, "## params %d\n", vp->num_rows);
    for (rn = 0; rn < vp->num_rows; rn++) {
        fprintf(fp, "##line: %s", row[rn].line);
        fprintf(fp, "##      %s = %s\n", row[rn].key, row[rn].value);
    }
}

/*
 * Visibility printer
 * 0.17*-3.336*-3.336 + 0.33*-1*-1 + 0.33*1*1 + 0.17*3.336*3.336 == 4.44382464
 * or x10^4 for whitneys
 */
static void print_vis_verbx(FILE *fp, DifxVisRecord *vr, int nc, int vn)
{
    double  ave = 0, xx, yy, zz, zero, last, norm;
    int	ii, aa, bb;
    if (tsys<5) {
        fprintf(fp, "## Visibility %d\n", vn);
        if (verb>2) print_vis_params(fp, vr->params);
        fprintf(fp, "# sync %08x version %08x baseline %08x mjd %d\n", 
		vr->sync, vr->headerversion, vr->baseline, vr->mjd);
        fprintf(fp, "# iat %lf config_index %d source_index %d freq_index %d\n",
		vr->seconds, vr->configindex, vr->sourceindex, vr->freqindex);
        fprintf(fp, "# pols %c%c pulsar_bin %d weight %le\n", 
		vr->polpair[0], vr->polpair[1],
		vr->pulsarbin, vr->dataweight);
        fprintf(fp, "# uvw %lf %lf %lf\n# visibilities (%d = %d):\n",
		vr->uvw[0], vr->uvw[1], vr->uvw[2], vr->nchan, nc);
        fprintf(fp, "# chan real imag  mag phase\n");
    }
    for (ii=0; ii<nc; ii++) {
        xx = creal(vr->visdata[ii]);
        yy = cimag(vr->visdata[ii]);
        zz = sqrt(xx*xx+yy*yy);
        if (tsys<5) fprintf(fp, "%4d  %14.9f %14.9f  %14.9f %17.9f %.10lf\n",
            ii, xx, yy, zz, atan2(yy,xx)*180.0/M_PI,
            vr->seconds);
        if (integvals &&
            (vr->baseline == bline) && (vr->freqindex == bfreq)) {
            if (bthis >= bfirst && bthis < bfirst + bcount) {
                integvals[ii] += zz;
                integhits++;
            }
            if (ii == nc-1) bthis++;
        }
	ave += xx * xx;
	ave += yy * yy;
	if (ii ==    0) zero = xx*xx + yy*yy;
	if (ii == nc-1) last = xx*xx + yy*yy;
    }
    // norm = (tsys == 0) ? 4.44382464 : 1.000;
    // Checking normalization of the band
    if (twgh) norm = (0.0 <= vr->dataweight && vr->dataweight <= 1.0)
         ? 10000.0 / vr->dataweight : 10000.0;
    else      norm = 10000.0;
    switch (tsys) {
    default:
    case 0:
      break;
    case 1:
          fprintf(fp, "# sqrt%d(<R*R+I*I>)[%04X](%.1lf) = %9.7lf %.10lf %s\n",
            nc, vr->baseline, vr->seconds,
            yy = sqrt((ave)/(double)(nc)) * norm, vr->seconds,
            "# average amp over all channels");
      break;
    case 5:
          fprintf(fp, "%.10lf %9.7lf\n",
            vr->seconds, yy = sqrt((ave)/(double)(nc)) * norm);
      break;
    case 2:
          fprintf(fp, "# sqr%d-(<R*R+I*I>)[%04X](%.1lf) = %9.7lf %.10lf %s\n",
            nc-1, vr->baseline, vr->seconds,
            yy = sqrt((ave-zero)/(double)(nc-1)) * norm, vr->seconds, 
            "# average amp over all but first chan");
      break;
    case 6:
          fprintf(fp, "%.10lf %9.7lf\n",
            vr->seconds, yy = sqrt((ave-zero)/(double)(nc-1)) * norm);
      break;
    case 3:
          fprintf(fp, "# sqr%d+(<R*R+I*I>)[%04X](%.1lf) = %9.7lf %.10lf %s\n",
            nc-1, vr->baseline, vr->seconds,
            yy = sqrt((ave-last)/(double)(nc-1)) * norm, vr->seconds, 
            "# average amp over all but final chan");
      break;
    case 7:
          fprintf(fp, "%.10lf %9.7lf\n",
            vr->seconds, yy = sqrt((ave-last)/(double)(nc-1)) * norm);
      break;
    case 4:
          fprintf(fp, "# sqrt%d(<R*R+I*I>)[%04X](%.1lf) = %9.7lf %.10lf %s\n",
            nc-2, vr->baseline, vr->seconds,
            yy = sqrt((ave-zero-last)/(double)(nc-2)) * norm, vr->seconds, 
            "# average amp skipping first and last chans");
      break;
    case 8:
          fprintf(fp, "%.10lf %9.7lf\n",
            vr->seconds, yy = sqrt((ave-zero-last)/(double)(nc-2)) * norm);
      break;
    }
    if (tsys<5) fputs("\n\n", fp);	    /* for gnuplot */
    aa = (vr->baseline>>8)&0xFF;
    bb = (vr->baseline)&0xFF;
    difx_corr[aa][bb] = yy/10000.0;
    mark_corr[aa][bb] = yy;
    if (aa > maxa) maxa = aa;
    if (bb > maxb) maxb = bb;
}
static void print_vis_verb0(FILE *fp, DifxVisRecord *vr, int nc, int vn)
{
    double  ave = 0, xx, yy, norm;
    double  zero, last;
    int	ii, aa, bb;
    fprintf(fp, "## Visibility %d\n", vn);
    fprintf(fp, "# baseline %08x mjd %d secs %lf (ch r i)\n",
	vr->baseline, vr->mjd, vr->seconds);
    for (ii=0; ii<nc; ii++) {
	fprintf(fp, "%4d %14.9f %14.9f\n", ii,
	    xx = creal(vr->visdata[ii]),
	    yy = cimag(vr->visdata[ii]));
	ave += xx * xx;
	ave += yy * yy;
	if (ii ==    0) zero = xx*xx + yy*yy;
	if (ii == nc-1) last = xx*xx + yy*yy;
    }
    norm = (tsys == 0) ? 4.44382464 : 1.000;
    if (tsys < 2) {
      fprintf(fp, "# sqrt%d(<R*R+I*I>)[%04X](%.1lf) = %9.7lf %9.3lf\n",
	nc-2, vr->baseline, vr->seconds,
	xx = sqrt((ave-zero-last)/(double)(nc-2)) / norm,
	yy = sqrt((ave-zero-last)/(double)(nc-2)) * 10000.0);
    }
    fputs("\n\n", fp);	    /* for gnuplot */
    aa = (vr->baseline>>8)&0xFF;
    bb = (vr->baseline)&0xFF;
    difx_corr[aa][bb] = xx;
    mark_corr[aa][bb] = yy;
    if (aa > maxa) maxa = aa;
    if (bb > maxb) maxb = bb;
}

static int sync_diff_to_nchan(int nf)
{
    int hb = sizeof(int) * 1    /* baseline */
           + sizeof(int) * 1    /* mjd */
           + sizeof(double) * 1 /* seconds */
           + sizeof(int) * 1    /* configindex */
           + sizeof(int) * 1    /* sourceindex */
           + sizeof(int) * 1    /* freqindex */
           + 1 * 2              /* polpair */
           + sizeof(int) * 1    /* pulsarbin */
           + sizeof(double) * 1 /* dataweight */
           + sizeof(double) * 3 /* uvw */
           + sizeof(int) * 1;   /* headerversion */
    int nc = nf;
    nc -= hb;
    nc /= sizeof(cplx32f);
    if (verb>2) fprintf(stderr,
        "# Hdr %d B cplx32f %d B, (%d - %d) / %d = %d\n",
        hb, sizeof(cplx32f), nf, hb, sizeof(cplx32f), nc);
    if (nc > 0 && bint && !integvals) {
        integvals = (double*)calloc(nc, sizeof(double));
        bint = (integvals) ? nc : 0;
        if (verb>2) fprintf(stderr,
            "# Integrating %d chans %d..%d\n", bint, bfirst, bfirst + bcount);
    }
    return(nc);
}

/*
 * Channel deducer, return negative if problems
 */
static int deduce_nchan(char *file, FILE *fp)
{
    int	    nc = -1, nf = 0, use, isn, ish;
    unsigned int sync, next = 0, half;
    long    current = ftell(fp);

    if (verb>2) fprintf(stderr,
        "# Deducing channels of %s at %ld\n", file, current);
    if (current < 0) return(perror("ftell"),-3);

    fread(&sync, 4, 1, fp);
    if (verb>2) fprintf(stderr, "# sync [%06d] %08X\n", nf, sync);
    do {
	nf+=4;

        half  = ((next & 0xFFFF0000)>>16);
	if (1 != fread(&next, 4, 1, fp)) return(-3);
        half |= ((next & 0x0000FFFF)<<16);

	if (verb>4) fprintf(stderr, "# next [%06d] half [%06d] %08X %08X\n",
            nf, nf-2, next, half);
    } while (sync != next && sync != half);

    if (sync == next) { use = nf;     isn = '*'; ish = ' '; }
    if (sync == half) { use = nf - 2; isn = ' '; ish = '*'; }
    if (verb>2) fprintf(stderr, "# next [%06d] %08X %c\n", nf, next, isn);
    if (verb>2) fprintf(stderr, "# half [%06d] %08X %c\n", nf-2, half, ish);

    if (0 > fseek(fp, current, SEEK_SET)) return(perror("fseek"),-4);
    return(nc = sync_diff_to_nchan(use));
}

static void
dump_corr_table(void)
{
    int ii, jj;
    if (verb>0) fprintf(stderr, "Dumping corr_table\n");
    fputs("##corr:\n", stdout);
    fputs("##corr:     ", stdout);
    for (ii = 1; ii <= maxa; ii++) {
	fprintf(stdout, " %9d", ii);
    }
    fputs("\n", stdout);
    for (ii = 1; ii <= maxa; ii++) {
	if (difx_corr[ii][ii] == 0) break;
	fprintf(stdout, "##corr: %3d ", ii);
	for (jj = 1; jj < ii; jj++) {
	    fputs(" .........", stdout);
	}
	for ( ; jj <= maxb; jj++) {
	    fprintf(stdout, " %+9.6f", difx_corr[ii][jj]);
	}
	fputs("\n", stdout);
    }
    fputs("##corr:\n", stdout);
}

/*
 * Parse the swin output file
 * DifxVisRecord *newDifxVisRecord(const char *filename, int nchan);
 * figure out the number of channels from the VISRECORD_SYNC_WORD_DIFX2
 */
int parse_swin_deduce(char *file, int nc)
{
    DifxVisRecord   *vr;
    int		    xx;

    if (verb>2) fprintf(stderr, "# nchan is %d\n", nc);
    vr = newDifxVisRecord(file, DEDUCE);

    if (verb>2) fprintf(stderr, "Deducing.\n");
    while (nc >= DEDUCE) {
        if (!vr) return(fprintf(stderr, "Cannot file %s\n", file));
        xx = deduce_nchan(file, vr->infile);
        if (feof(vr->infile)) return(0);
        if (xx > DEDUCE) return(fprintf(stderr,"Deduced %d > %d\n",xx,DEDUCE));
        if (xx < 0) return(fprintf(stderr, "No channels for %s\n", file));
        fflush(stderr);
        vr->nchan = xx;
        xx = DifxVisRecordgetnext(vr);
        if (xx > 0) {
            if (verb>0) print_vis_verbx(bl2fp(vr), vr, vr->nchan, xx);
            else        print_vis_verb0(bl2fp(vr), vr, vr->nchan, xx);
        } else {
            fprintf(stderr, "No more visibilities\n");
            break;
        }
        fflush(bl2fp(vr));
        if (--max_vis == 0) break;
    }

    if (verb>0) fprintf(stderr, "Cleaning up\n");
    deleteDifxVisRecord(vr);
    dump_corr_table();
    close_blfiles();
    return(0);
}

static int parse_swin_given(char *file, int nc)
{
    DifxVisRecord   *vr;
    int		    xx;

    if (verb>1) fprintf(stderr, "# nchan is %d\n", nc);
    vr = newDifxVisRecord(file, DEDUCE);

    if (verb>0) fprintf(stderr, "Running.\n");
    vr->nchan = nc;
    while (nc < DEDUCE) {
        if (!vr) return(fprintf(stderr, "Cannot file %s\n", file));
        xx = DifxVisRecordgetnext(vr);
        if (xx > 0) {
            if (verb>0) print_vis_verbx(bl2fp(vr), vr, nc, xx);
            else        print_vis_verb0(bl2fp(vr), vr, nc, xx);
        } else {
            fprintf(stderr, "No more visibilities\n");
            break;
        }
        if (--max_vis == 0) break;
    }

    if (verb>0) fprintf(stderr, "Cleaning up\n");
    deleteDifxVisRecord(vr);
    dump_corr_table();
    close_blfiles();
    return(0);
}

int parse_swin(char *file, int nc)
{
    if (nc >= DEDUCE) return( parse_swin_deduce(file, nc) );
    else              return( parse_swin_given(file, nc) );
    return(-1);
}

/*
 * eof
 */
