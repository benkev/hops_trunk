/*
 * $Id: mk5_main.cpp 106 2011-01-20 17:39:56Z gbc $
 *
 * Wrapper Main for various DLS utilities for getting answers
 * to various streamstor questions in the absense of Mark5A.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "hops_config.h"
#include "dls_utils.h"

/*
 * Global options
 */
int	    verb = 0;
char	    *cmd = "help";

/*
 * Boilerplate
 */
static int cmdhelp = 0;
static int usage(char *name)
{
    printf("Usage: %s [options]\n", name);
    printf( "where the options are:\n"
	    "  -v	     verbose, may be repeated for more\n"
	    "  -c <cmd>	     utility command to execute (help)\n"
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
    while ((c = getopt(argc, argv, "vc:")) != -1) switch(c) {
    case 'v': verb++;						break;
    case 'c': cmd = optarg;					break;
    default:							return(3);
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

static int help(int argc, char **argv)
{
    printf("The available utilities are\n");
    printf("  status  GetBankStatus\n");
    printf("  dir     GetDirectoryInfo\n");
    printf("  dvinfo  GetDeviceInfo\n");
    printf("  drinfo  GetDriveInfo\n");
    printf("  drtemp  GetDriveTemp%s\n",
	HAVE_XLRGetDriveTemp ? "" : " (not available)");
    printf("  serial  GetSerial\n");
    printf("  smart   GetSMART\n");
    printf("and are selected with the -c option.\n\n");
    printf( "The \"dir\" command takes the bank name(s) A and B as\n"
	    "additional arguments.  You can also call them a and b\n"
	    "which will cause the named bank to be selected, however\n"
	    "the new directory may not be immediately available so\n"
	    "you may need to wait a moment and try again....\n"
    );
    return(0);
}

/*
 * Main Entry.
 */
int main(int argc, char **argv)
{
    /* basic command line parsing */
    if (cmdline(&argc, &argv))  return(!cmdhelp);
    if (!strcmp(cmd, "status")) return(GetBankStatus_main(argc, argv));
    if (!strcmp(cmd, "dir"))    return(GetDirectoryInfo_main(argc, argv));
    if (!strcmp(cmd, "dvinfo")) return(GetDeviceInfo_main(argc, argv));
    if (!strcmp(cmd, "drinfo")) return(GetDriveInfo_main(argc, argv));
    if (!strcmp(cmd, "drtemp")) return(GetDriveTemp_main(argc, argv));
    if (!strcmp(cmd, "serial")) return(GetSerial_main(argc, argv));
    if (!strcmp(cmd, "smart"))  return(GetSMART_main(argc, argv));
    return(help(argc, argv));
}

/*
 * eof
 */
