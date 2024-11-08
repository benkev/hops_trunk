/*
 * $Id: dumpinput.c 1337 2016-05-24 20:59:37Z gbc $
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
    printf("Usage: %s [options] [path-to-difxinput-file[s...]]\n", name);
    printf("where the options are:\n"
	"  -v            verbose, may be repeated for more\n"
	"\n"
        "With one -v, the amplitude and phase are computed and output\n"
	"in addition to the real and imaginary part of the data.\n"
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
    while ((c = getopt(argc, argv, "v")) != -1) switch(c) {
    case 'v': verb++;						break;
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

static int dump_input(char *file)
{
    FILE *fp = stdout;
    DifxInput *D = loadDifxInput(file);
    fprintDifxInput(fp, D);
}

/*
 * Main Entry.
 */
int main(int argc, char **argv)
{
    int		errs = 0;

    /* basic command line parsing */
    if (cmdline(&argc, &argv)) return(!cmdhelp);

    while (argc-- && !errs) errs += dump_input(*argv++);

    return(errs);
}

/*
 * eof
 */
