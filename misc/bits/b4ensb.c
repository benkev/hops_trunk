/*
 * $Id: b4ensb.c 757 2012-07-18 21:14:57Z gbc $
 *
 * Consider the effective number of significant bits
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

extern int verb;

double *hist;

void load_hist(int values, int used, double gain, double *hist)
{
    int	nn, mm, kk;
    double dused = (double)used, du2 = dused/2.0, minv, maxv;

    if (used == values) {
	for (nn = 0; nn < values; nn++)
	    hist[nn] = 0.5 + erf(gain*((double)nn - du2)/dused);
	minv = hist[0];
	maxv = hist[nn-1];
    } else if (used < values) {	/* used in the middle */
	nn = kk = (values - used)/2 + 1;
	for (mm = 0; mm < used; mm++, nn++)
	    hist[nn] = 0.5 + erf(gain*((double)mm - du2)/dused);
	minv = hist[kk];
	maxv = hist[nn-1];
	for ( ; nn < values; nn++)
	    hist[nn] = maxv;
	for (nn = 0; nn < kk; nn++)
	    hist[nn] = minv;
    } else {	/* used > values, lose the wings */
	for (nn = 0, mm = (used - values)/2; nn < values; nn++, mm++)
	    hist[nn] = 0.5 + erf(gain*((double)mm - du2)/dused);
	minv = hist[0];
	maxv = hist[nn-1];
    }

    /* Normalize */
    for (nn = 0; nn < values; nn++) hist[nn] -= minv;
    for (nn = 0; nn < values; nn++) if (hist[nn] < 0) hist[nn] = 0.0;
    maxv -= minv;
    for (nn = 0; nn < values; nn++) hist[nn] /= maxv;
}

void find_edge(double middle, int values, double *hist, int edge[3])
{
    int	nn;
    double cut;
    cut = (1.0 - middle) / 2.0;
    for (nn = 0; nn < values; nn++) 
	if (hist[nn] >= cut) break;
    edge[0] = nn;
    cut = 0.5;
    for (nn = 0; nn < values; nn++) 
	if (hist[nn] >= cut) break;
    edge[1] = nn;
    cut = 0.5 + (1.0 - middle) / 2.0;
    for (nn = 0; nn < values; nn++) 
	if (hist[nn] >= cut) break;
    edge[2] = nn;
}

void report_edge(double middle, int values, double *hist, int edge[3])
{
    int	nn;
    for (nn = 0; nn < 3; nn++)
	printf("# Edge-%d Int(hist[%04d]) = %.4f\n"
	       "# Edge+%d Int(hist[%04d]) = %.4f dh = %.4f\n",
	       nn, edge[nn]-1, hist[edge[nn]-1],
	       nn, edge[nn]-0, hist[edge[nn]-0],
	       hist[edge[nn]-0] - hist[edge[nn]-1]);
    if (verb>1) for (nn = 0; nn < values; nn++)
	printf("%4d %+4d %.4f\n", nn, nn - values/2, hist[nn]);
}

int b4ensb(int argc, char **argv)
{
    int	values, used, edge[3];
    double middle, *hist, gain;
    values = atoi(argv[1]);
    used   = atoi(argv[2]);
    middle = atof(argv[3]);
    gain   = atof(argv[4]);
    hist = malloc(values * sizeof(double));
    if (!hist) { perror("malloc"); return(1); }
    load_hist(values, used, gain, hist);
    find_edge(middle, values, hist, edge);
    report_edge(middle, values, hist, edge);
    free(hist);
    return(edge[0] < edge[1] && edge[1] < edge[2] ? 0 : 1);
}


/*
 * eof
 */
