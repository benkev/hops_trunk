/*
 * $Id: vnoise.c 453 2011-09-12 16:09:14Z gbc $
 *
 * A deviant form of bnoise which makes vdif formed data simulating
 * N (2Gbps) channels of the the RDBE output in Astro 8Gbps mode.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "vnoise.h"

/*
 * Global options
 */
int	    verb = 0;

/*
 * Boilerplate
 */
static int cmdhelp = 0;
static int usage(char *name)
{
    printf("Usage: %s [options] stn1 [stn2 ...]\n", name);
    printf("where the options are:\n"
	"  -v            verbose, may be repeated for more\n"
	"  -c <float>    correlation coefficient (0.01)\n"
	"  -d <float>    duration of observation (2.0)\n"
	"  -r <float>    report processing interval (secs)\n"
	"  -t <flt,flt>  add (another) tone (MHz,Amp)\n"
	"  -a <float>    gaussian amplitude (1.0)\n"
	"  -b <flt,...>  add a DC-biases to each station (0,...)\n"
	"  -l <long>     reuse data after this many octets (0)\n"
	"                (randomly, if argument is negative\n"
	"  -q <string>   quantize prior to 2-bit thresholding:\n"
	"                use \"help\" for various options.\n"
	"\n"
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
    while ((c = getopt(argc, argv, "vc:d:r:t:a:b:l:q:")) != -1) switch(c) {
    case 'v': verb++;						break;
    case 'c': set_corr(atof(optarg));				break;
    case 'd': set_dura(atof(optarg));				break;
    case 'r': set_reps(atof(optarg));				break;
    case 't': add_tone(optarg);					break;
    case 'a': set_gamp(atof(optarg));				break;
    case 'b': set_bias(optarg);					break;
    case 'l': set_lazy(atol(optarg));				break;
    case 'q': if(set_quant(optarg))                             return(2);
              else                                              break;
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

/*
 * Main Entry.
 */
int main(int argc, char **argv)
{
    int		errs = 0;

    /* basic command line parsing */
    if (cmdline(&argc, &argv)) return(!cmdhelp);

    while (argc-- && !errs) errs += new_station(*argv++);
    if (!errs) errs += fake_the_data();

    return(errs);
}

/*
 * eof
 */
