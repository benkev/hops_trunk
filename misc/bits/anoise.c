/*
 * $Id: anoise.c 777 2012-08-30 20:50:21Z gbc $
 *
 * A deviant form of bnoise/vnoise which makes vdif formed data simulating
 * VDIF data in a variety of flavors.  The goal here is to simulate ALMA
 * data and correlate it against Mark5b-like or other flavors of data.
 *
 * This file contains main() with basic usage and farms out the work.
 */

#include "anoise.h"

/*
 * Global options
 */
int         verb = 0;
static int  examine = 0;
static int  analyze = 0;

/*
 * Boilerplate
 */
static int cmdhelp = 0;
static int usage(char *name)
{
    printf("Usage: %s [options] stn1 [stn2 ...]\n\n", name);
    printf("where the options are:\n\n"
        "  -v            verbose, may be repeated for more\n"
        "  -b <int>      samples used for bit statistics (1024^2)\n"
        "  -d <float>    duration of observation (2.0)\n"
        "  -e <float>    extension of observation (0.0)\n"
        "  -n <string>   make adjustments to common noise;\n"
        "                use \"help\" for details\n"
        "  -r <float>    report processing interval (usecs)\n"
        "  -t <float>    2-bit threshold in sigma units (1.00)\n"
        "  -q <string>   quantize prior to 2-bit thresholding;\n"
        "                use \"help\" for various options.\n"
        "  -x            rather than generate, examine files and\n"
        "                provide output setting up correlation\n"
        "  -y            same as -x, just additional output\n"
        "\n"
        "If the observation is \"extended\" (-e) past the \"duration\" (-d),\n"
        "data from the initial portion is duplicated as needed, with the\n"
        "header timestamps incremented approprately.\n"
        "The -r flag produces a progress report for the \"duration\".\n"
        "The -t option is not implemented.\n"
        "\n"
        "Use \"help\" as a station name for station configuration options.\n"
        "\n"
        "A sample invocation giving a (very) short sample with 8 channels\n"
        "of ALMA data (station Aa) and 16 channels of typical VLBI data\n"
        "(station Vv) is:\n"
        "\n"
        "anoise -v -d 0.00064 Aa:Aa.vdif:alma62.5x8 Vv:Vv.vdif:vlbi32.0x16\n"
        "\n"
        "If -d and -e are both 0.0, the first packet is generated, but no\n"
        "data is created; which allows you to check what you will eventually\n"
        "get without waiting a long time for it....\n"
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
    int     c;
    if (version(argv)) return(1);
    while ((c = getopt(argc, argv, "vxyb:d:e:n:r:t:q:")) != -1) switch(c) {
    case 'v': verb++;                                           break;
    case 'x': examine++;                                        break;
    case 'y': analyze++;                                        break;
    case 'b': set_bsmx(atoi(optarg));                           break;
    case 'd': set_dura(atof(optarg));                           break;
    case 'e': set_extn(atof(optarg));                           break;
    case 'n': if ((c = set_comm(optarg)))    return(c<0?cmdhelp=1:c);
              else                                              break;
    case 'r': set_reps(atof(optarg));                           break;
    case 't': set_thrs(atof(optarg));                           break;
    case 'q': if ((c = set_quant(optarg)))    return(c<0?cmdhelp=1:c);
              else                                              break;
    default:                                                    return(1);
    }
    return(0);
}
static int cmdline(int *argc, char ***argv)
{
    int     x = options(*argc, *argv);
    *argc -= optind;
    *argv += optind;
    return(x);
}
static void compile_checks(void)
{
    assert(VDIF_HDR_OCTETS == sizeof(VdifExt)/8);
}

/*
 * Main Entry.
 */
int main(int argc, char **argv)
{
    int         errs = 0;

    /* compile checks */
    compile_checks();

    /* basic command line parsing */
    if (cmdline(&argc, &argv)) return(!cmdhelp);
    
    if (argc && !strcmp(*argv,"help")) {
        print_type_help();
        return(0);
    }

    if (examine) {
        while (argc-- && !errs) errs += describe_data(*argv++);
    } else if (analyze) {
        while (argc-- && !errs) errs += analyze_data(*argv++);
    } else {
        while (argc-- && !errs) errs += new_station(*argv++);
        if (!errs) errs += fake_the_data();
        if (!errs) errs += extend_the_data();
    }

    return(errs);
}

/*
 * eof
 */
