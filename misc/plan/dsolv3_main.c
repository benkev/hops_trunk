/*
 * $Id: dsolv3_main.c 3459 2021-11-22 16:55:08Z gbc $
 *
 * Re-implementation of dsolv2 in C to allow use of GSL
 * routines, and later, direct read of alists, &c.
 *
 * This version allows reading alist files directly,
 * and other options for processing
 */

#include "dsolv3.h"
#include "msg.h"

Dopt opt = { .verb = 0, .quiet = 0, .numb = 0, .alist = 0, .base = 0,
             .snr = 0.0, .ref_freq = 0.0, .polarization = 0,
             .precess = 0, .fixGMST = 0, .redCHI = 0, .data = 0,
             .solver = 0 };

static int cmdhelp = 0;
static int usage(char *name)
{
    set_progname("ignore");
    set_msglev(0);
    printf("Usage: %s [options]\n\n", name);
    printf( "where the options are:\n"
            "  -v            verbose, may be repeated for more\n"
            "  -q            quiet, turns off vector/matrix output\n"
            "  -P            toggle precession of sources ([off],aag,nrao)\n"
            "  -G            toggle GMST calculation (on or [off])\n"
            "  -R            report reduced chi-square (normally not)\n"
            "  -S            equiv to -R -G -P2\n"
            "  -f <alist>    uses a version 6 Alist as input\n"
            "  -b <BL>       baseline of interest\n"
            "  -s <SNR>      (lower) SNR limit\n"
            "  -r <RF>       reference frequency\n"
            "  -p <PN>       polarization\n"
            "\n"
            "The primary input is an alist (-f) which is solved for position\n"
            "corrections.  The other arguments control how it is done, and\n"
            "what you see.  For example:\n"
            "\n"
            " position_dsolv3 -q -b AK -s 8 -f 3768.alist\n"
            "\n"
            "will solve for AK baseline corrections, clock offset and\n"
            "and clock rate corrections.  A lternate invocations\n"
            "\n"
            "  position_dsolv3 <input>\n"
            "  position_dsolv3 -B <input>\n"
            "\n"
            "provide solutions using older forms of the code which was\n"
            "derived (incrementally) from Brian Corey's original F77 code.\n"
/*
 * see dsolv3.c:main.c() which contains this invocation.
 *
            "which does what position_dsolv2 does with the input, but\n"
            "using GSL routines for the fit.  Or, any of the following\n"
            "options may be used to change some of the fitting details:\n"

            "\n"
            "In the first mode, %s is used merely to compare the numerics\n"
            "on the input file. The other options allow an Alist to be used\n"
            "for input which avoids the Aedit and awk reduction steps.\n"
 */
            "\n",
            name);
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
    int c;
    if (version(argv)) return(1);
    while ((c = getopt(argc, argv, "vqPGRSB:f:b:s:r:p:")) != -1) switch(c) {
    case 'v': opt.verb ++; break;
    case 'q': opt.quiet ++; break;
    case 'P': opt.precess = ((opt.precess+1) % 3); break;
    case 'G': opt.fixGMST = !opt.fixGMST; break;
    case 'R': opt.redCHI = !opt.redCHI; break;
    case 'S': opt.precess = 2; opt.fixGMST = 1; opt.redCHI = 1; break;
    case 'B': opt.input = optarg; break;
    case 'f': opt.alist = optarg; break;
    case 'b': opt.base = optarg; break;
    case 's': opt.snr = atof(optarg); break;
    case 'r': opt.ref_freq = atof(optarg); break;
    case 'p': opt.polarization = optarg; break;
    default: return(2);
    }
    return(0);
}
static int
cmdline(int *argc, char ***argv)
{
    int     x = options(*argc, *argv);
    *argc -= optind;
    *argv += optind;
    return(x);
}

/*
 * This is mostly for testing to provide a separate path to dsolv2().
 */
int dsolv2proxy(void)
{
    return(dsolv2(opt.input));
}

/*
 * Trivially modified dsolv2().
 * It gets input from load_alist().
 */
int dsolv3(void)
{
    int num = opt.numb;                                             // dif-3
    double csq = 0.0;
    gsl_matrix *design; 
    gsl_vector *delays, *params, *resids;
    Aobs *observations = opt.data;                                  // dif-3
    if (num < 6) return(fprintf(stderr,
        "Only have %d lines of input from %s\n", num, opt.alist));  // dif-3
    make_working_data_approx(observations, num);
    design = make_design_matrix(observations, num);
    if (!design) return(fprintf(stderr, "No design matrix!\n"));
    report_design_matrix(observations, design, num);
    delays = make_delay_vector(observations, num);
    if (!delays) return(fprintf(stderr, "No delay vector!\n"));
    params = calc_parameters(num, design, delays, &csq);
    if (!params) return(fprintf(stderr, "No param vector!\n"));
    resids = calc_residuals(num, design, delays, params);
    if (!resids) return(fprintf(stderr, "No residual vector!\n"));
    report_results(num, observations, resids, params, csq);
    /* cleanup */
    free(observations);
    gsl_matrix_free(design);
    gsl_vector_free(delays);
    gsl_vector_free(params);
    gsl_vector_free(resids);
    return(0);
}

int dsolv3_main(int argc, char **argv)
{
    /* basic command line parsing */
    if (cmdline(&argc, &argv)) return(!cmdhelp);

    if (!opt.solver && !opt.alist) {    /* compatibility mode */
        opt.solver = &dsolv2proxy;
    } else {                            /* enhanced mode */
        if (opt.alist && load_alist(opt.alist))
            return(1);
        opt.solver = dsolv3;
    }
    return((*opt.solver)());
}

/*
 * eof
 */
