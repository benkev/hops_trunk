/*************************************************************/
/*                                                           */
/* Part of the Xwindow fringe plot popup library.            */
/* Opens up the font.  Currently, the 6x10 font is hardcoded */
/* but ultimately we want to be more intelligent and/or allow*/
/* user overrides                                            */
/*                                                           */
/* Initial version CJL 8 Oct 1991                            */
/* Modified to handle arbitrary fonts 25 Oct 1991 CJL        */
/*                                                           */
/*************************************************************/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>

#include "print_page.h"

int
getfont_fplot (finfo, font, height, width)
XFontStruct **finfo;
char *font;
int *height, *width;
    {					/* These are the only really important chars */
    char *fplot_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int direction, descent, ascent;
    XCharStruct stringinfo;
    extern Display *display;

    if ((*finfo = XLoadQueryFont (display, font)) == NULL)
	{
	msg ("Error, cannot open 6x10 font", 2);
	return (1);
	}
					/* Figure out spacing for fplot chars */
    XTextExtents (*finfo, fplot_chars, strlen(fplot_chars), &direction,
		&descent, &ascent, &stringinfo);

    *height = stringinfo.ascent + stringinfo.descent;
    *width = (*finfo)->max_bounds.rbearing - (*finfo)->min_bounds.lbearing;

    return (0);
    }
