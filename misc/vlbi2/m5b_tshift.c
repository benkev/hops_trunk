/*
 * $Id$
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int usage(char *name)
{
    char *me = strrchr(name, '/');
    if (me) me++;
    else me = name;
    printf( "Usage:  %s 0xJJJSSSS input.m5b output.m5b [frameshift]\n"
	    "\n"
	    "where the input file is shifted to the new VLBA timestamp\n"
	    "and written to the output file.  If a small frameshift is\n"
	    "provided, that will be applied as well\n"
	    "\n"
	    "This version is a hack that assumes a small sample within\n"
	    "the same second.\n"
	    "\n",
	    me
    );
    return(1);
}

int
main(int argc, char **argv)
{
    unsigned long new;
    FILE *fin, *fou;
    int	dfr, frames;
    static unsigned int buf[10016];

    if (argc < 4 || argc > 5) return(usage(argv[0]));

    new = strtoul(argv[1] ? argv[1] : "0", NULL, 16);
    new &= 0xFFFFFFFF;

    fin = fopen(argv[2] ? argv[2] : "/dev/null", "r");
    if (!fin) return(perror("in"),1);

    fou = fopen(argv[3] ? argv[3] : "/dev/null", "w");
    if (!fou) return(perror("ou"),1);

    dfr = atoi(argv[4] ? argv[4] : "0");

    while (10016 == fread(buf, 1, 10016, fin)) {
	buf[2]  = new;
	frames = buf[1] & 0x7FFF;
	frames += dfr;
	if (frames >= 0) {
	    buf[1] &= 0xFFFF8000;
	    buf[1] |= frames;
	    fwrite(buf, 1, 10016, fou);
	}
    }

    return(0);
}

/*
 * eof
 */
