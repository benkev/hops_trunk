/*************************************************************/
/*                                                           */
/* Part of the Xwindow fringe plot popup library.            */
/* Trivially figures out what line separates the two pages   */
/* I think the correct way is to look for "*" in column 0,   */
/* but what the hell ... dashes work.                        */
/*                                                           */
/* Initial version CJL 8 Oct 1991                            */
/*                                                           */
/*************************************************************/

#include <stdio.h>

int
pgbreak_fplot (fplot)
char **fplot;
    {
    char *dash = "---";
    int i;

    for (i=43; i<64; i++)
	if (strstr (fplot[i], dash) != NULL) break;

    if (i < 64) return (i);
    else return (-1);
    }
