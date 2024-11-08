/*
 * $Id: b4thresh.c 757 2012-07-18 21:14:57Z gbc $
 *
 * Stupid program to check threshold for bit states
 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern int b4ensb(int argc, char **argv);
extern int b4antsum(int argc, char **argv);

/* The value DiFX uses for decoding */
#define OPTIMAL_2BIT_HIGH       3.3359
/* which presumably maximizes these */
#define OPTIMAL_THRESHOLD	0.9816
#define OPTIMAL_PERCENTAGE	0.6737

int verb = 1;

/*
 * From Thompson, Moran and Swenson, 8.51 or 8.52
 *   enn is the high state value, n
 *   vs  is the threshold vo divided by sigma, vo/sigma
 *   phi is the error function integral over [-vo,vo]
 */
double eta4(double enn, double vs, double *pphi)
{
    double eee, phi, tmp, num, den;

    eee = exp(-vs * vs * 0.5);
    phi = erf(vs * M_SQRT1_2);
    tmp = (enn - 1.0) * eee + 1;
    num = 2.0 * tmp * tmp;
    den = M_PI * (phi + enn * enn * (1.0 - phi));

    *pphi = phi;
    return(num / den);
}

int usage(char *name)
{
    printf( "%s n vo/sigma\n", name);
    printf( "  for threshold/vanvleck corections\n"
            "    where n is the representation of the high bit and\n"
            "    vo is the low-high threshold in sigma units\n"
    );
    printf( "%s n vo/sigma nantennas\n", name);
    printf( "  same as above, but do the math for a sum of antennas\n");
    printf( "%s number used middle gain\n", name);
    printf( "  for fooling around with quantization\n"
            "    where the histogram has some 'number' of states\n"
            "    of which 'used' are actually used (i.e. the middle)\n"
            "    and the center of the distibution is at 'middle'\n"
            "    and a gain=1.0 is for quantizing with gaussian noise\n"
    );
    printf( "In any case, testverb=X for more verbosity\n");
    return(1);
}

int main(int argc, char **argv)
{
    double  enn, vs, phi, eta, pc;
    char *tv = getenv("testverb");

    if (getenv("srcdir")) verb = 0;
    if (tv) verb = atoi(tv);

    if (argc == 3) {
	enn = atof(argv[1]);
	vs  = atof(argv[2]);
	pc  = 0.0;
    } else if (argc == 1) {
	enn = OPTIMAL_2BIT_HIGH;
	vs  = OPTIMAL_THRESHOLD;
	pc  = OPTIMAL_PERCENTAGE;
    } else if (argc == 4) {
        return(b4antsum(argc, argv));
    } else if (argc == 5) {
	return(b4ensb(argc, argv));
    } else {
        return(usage(*argv));
    }

    eta = eta4(enn, vs, &phi);
    if (pc) pc -= phi;
    if (verb>0) printf("eta4(%.8f,%.8f) = %.8f phi(%.8f/sqrt(2)) = %.8f\n",
	enn, vs, eta, vs, phi);
    if (fabs(pc) > 1e-5) printf("Error from optimal is %g\n", pc);

    return(fabs(pc) > 1e-5 ? 1 : 0);
}

/*
 * eof
 */
