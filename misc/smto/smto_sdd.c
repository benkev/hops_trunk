/*
 * $Id$
 *
 * Program to take apart SMTO sdd files;
 * Based on code from AEER, hacked up/rearranged by GBC.
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern int parse_file(char *file, char *plot);

/* public data, controlling options */
int noplot = 0;
int verb = 0;
int min_record = 0, max_record = 99999;

/* private data */
static char *plotfile = "sdd-plot-%d.ps";

/* decode the rec option */
int recmm(char *arg)
{
    int mn,mx;
    if (2 == sscanf(arg, "%d,%d", &mn, &mx)) {
	min_record = mn;
	max_record = mx;
	return(0);
    }
    return(1);
}

/*
 * Command line parsing and other boilerplate...
 */
int cmdline(int *argc, char ***argv)
{
    int	c, errs = 0;
    while ((c = getopt(*argc, *argv, "vr:d")) > 0) switch(c) {
    case 'v': verb++;			    break;
    case 'd': noplot++;			    break;
    case 'r': if (recmm(optarg))	    return(2);
	      else			    break;
    default:				    return(1);
    }
    *argc -= optind;
    *argv += optind;
    return(*argc > 0 ? 0 : 1);
}

int usage(char *name)
{
    printf("Usage: %s [options] sddfile[s]\n\n"
	"where the options are:\n"
	"  -v          make it more verbose\n"
	"  -r min,max  records to process\n"
	"  -d          dump data to file, not a plot\n"
	"\n"
	"\n"
	, name);
    return(1);
}

int main(int argc, char **argv)
{
    int errs = 0, pc = 0;
    char pf[1024], *n = *argv;
    if (cmdline(&argc, &argv)) return(usage(n));
    while (argc-- && !errs) {
	snprintf(pf, 1024, plotfile, pc++);
	errs += parse_file((n = *argv++), pf);
	printf("### Parsed [%d] %s -> %s with %d errs.\n",
	    pc, n, pf, errs);
    }
    return(errs);
}

/*
 * eof
 */
