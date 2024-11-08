/*
 * $Id: network.c 2174 2017-12-19 20:58:52Z gbc $
 *
 * for LS Fit, adapted from APP phase studies and Num Recipes
 */

#include "network.h"

Nopt opt = { .verb = 0, .pnum = 1, .sfix = 0, .wght = 0,
    .maxRedChiSq = 0.0, .minNumVals = 10, .nbase = 0 };

static int cmdhelp = 0;
static int usage(char *name)
{
    printf("Usage: %s [options]\n\n", name);
    printf("  -v            verbose; repeat for more\n");
    printf("  -p <int>      param to fit (1..5)\n");
    printf("  -s <char>     station to hold fixed\n");
    printf("  -r <float>    maximum redChiSq to keep\n");
    printf("  -m <int>      min number vals to keep\n");
    printf("  -b <cslist>   baselines to use\n");
    printf("\n");
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
    while ((c = getopt(argc, argv, "vwp:s:r:m:b:")) != -1) switch(c) {
    case 'v': opt.verb ++;                      break;
    case 'w': opt.wght ++;                      break;
    case 'p': opt.pnum = atoi(optarg);          break;
    case 's': opt.sfix = *optarg;               break;
    case 'r': opt.maxRedChiSq = atof(optarg);   break;
    case 'm': opt.minNumVals = atoi(optarg);    break;
    case 'b': load_baselines(&opt, optarg);     break;
    default: return(2);
    }
    return(0);
}
static int
cmdline(int *argc, char ***argv)
{
    int x = options(*argc, *argv);
    *argc -= optind;
    *argv += optind;
    return(x);
}

int main(int argc, char **argv)
{
    /* basic command line parsing */
    opt.fpx = stdout;
    if (cmdline(&argc, &argv)) return(!cmdhelp);
    if (opt.nbase == 0) fprintf(stdout, "# Retaining all Baselines.\n");
    if (get_data(argc, argv, &opt)) return(1);
    if (grok_data(&opt)) return(2);
    return(solve_data(&opt));
}

/*
 * eof
 */
