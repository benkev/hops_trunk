#include "data.h"			/* Definitions of data structures */
#include <stdio.h>
char progname[8] = "fourfit";		/* extern variables for messages */
int msglev = 1;
struct data_fringe fringe;

main(argc, argv)
int argc;
char **argv;
    {
    char name1[256], name2[256];
    char *fnames[50];
    int ret, i, filenum;


    if (argc < 2) 
	{
	printf ("You need to specify at least 1 input file\n");
	exit(0);
	}

    j = open_x_fplot (argc, argv);
    for (i=1; i<argc; i++)
	{
	j = read_fringe (argv[i]);
	if (j != 0)
	    {
	    printf("Failed to open %s, continuing\n", argv[i]);
	    continue;
	    }
	k = show_x_fplot (fringe.fplot);
	if (k == -1)
	    if (i >= 2) i -= 2;
	else if (k == 0) break;
	}
    close_x_fplot ();
    }
