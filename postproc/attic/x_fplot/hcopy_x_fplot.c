/*************************************************************/
/*                                                           */
/* Part of the Xwindow fringe plot popup library.            */
/* Takes care of hardcopy.  The philosophy here is to dump   */
/* a copy of the fringe plot to a temporary file, and then   */
/* invoke a shell script to print it, facilitating           */
/* customization outside of the C code                       */
/*                                                           */
/* Initial version CJL 25 Oct 1991                           */
/*                                                           */
/*************************************************************/

#include <stdio.h>
#include <stdlib.h>

void 
hcopy_x_fplot (fplot)
char **fplot;
    {
    int i;
    char tempname[20];
    char command[100];
    FILE *sfp;

    sprintf (tempname, "/tmp/fplot.XXXXXX");
    if (mktemp (tempname) == NULL)
	{
	msg ("Could not make temporary file for fringe plot printout", 2);
	return;
	}
    if ((sfp = fopen (tempname, "w")) == NULL)
	{
	msg ("Could not open temporary file for fringe plot printout", 2);
	return;
	}

    for(i=0;fplot[i]!=NULL; i++)
	{
	if (strncmp (fplot[i], "**", 2) == 0) break;
	if ((fplot[i][0] == '*') && i != 0) fprintf (sfp, "\n");
	fprintf(sfp, "%s\n", fplot[i] + 1);
	}
    fclose (sfp);

    msg ("Printing hardcopy of fringe plot ...", 2);
    sprintf (command, "/usr/local/bin/fplot_print %s", tempname);
    system (command);

    } 	
