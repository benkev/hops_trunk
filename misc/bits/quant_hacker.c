/*
 * $Id: quant_hacker.c 737 2012-06-27 21:05:32Z gbc $
 *
 * Reduce the infinite precision sample into a quantized one.
 *
 * This code allows the assignment of a quantizer handler, so that
 * different types could be used.  The default (adcq == 0) ignores this.
 */

#include <math.h>
#include <stdio.h>
#include <string.h>

extern int verb;

/* indicates what quantizer is active */
double (*adcq)(double) = 0;

/*
 * a simple linear quantizer
 */
static double qs_min, qs_max, qs_num, qs_mul;
static double qs_doit(double xx)
{
    double numb = (xx - qs_min) * qs_mul;
    double valu = qs_min + rint(numb) / qs_mul;
    if (valu < qs_min) valu = qs_min;
    if (valu > qs_max) valu = qs_max;
    return( valu );
}
static void qs_help(void)
{
    printf( "  s:mn,mx,nm   where the input value will be converted\n"
	    "               to nm (int) values between mn and mx\n"
	    "               (float) inclusive.\n"
	    "  s:xx         for testing: evaluate the defined quantization\n"
            "               model using xx as input.\n"
    );
}
static int qs_setup(char *args)
{
    double xx;
    int ns = sscanf(args, "s:%lf,%lf,%lf", &xx, &qs_max, &qs_num);
    if (1 == ns) {
	printf("qs(%lf) = %lf\n", xx, qs_doit(xx));
	return(0);
    }
    if (3 != ns)
	return(fputs("insufficient arguments\n", stderr));
    qs_min = xx;
    if (qs_min >= qs_max || qs_num <= 0)
	return(fputs("illegal arguments\n", stderr));
    qs_num = rint(qs_num);
    qs_mul = (qs_num) / (qs_max - qs_min);
    adcq = qs_doit;
    if (verb>0) printf("Quantizing from %g .. %g in %d steps (mult: %g)\n",
	qs_min, qs_max, (int)qs_num, qs_mul);
    return(0);
}

int set_quant(char *args)
{
    if (!strcmp(args,"help")) {
	printf("The ADC quantizer models are specified as follows:\n");
	qs_help();
	return(-1);
    } else if (*args == 's') {
	return(qs_setup(args));
    } /* and others... */
    fprintf(stderr, "Undefined ADC quantizer model '%s'\n", args);
    return(1);
}


/*
 * eof
 */
