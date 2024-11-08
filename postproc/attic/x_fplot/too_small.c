/*************************************************************/
/*                                                           */
/* Part of the Xwindow fringe plot popup library.            */
/* Simply puts an error message in the window when shrunk    */
/* smaller than reasonable dimensions.                       */
/*                                                           */
/* Initial version CJL 8 Oct 1991                            */
/*                                                           */
/*************************************************************/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>

int
too_small (width, height)
unsigned int width, height;
    {
    int swidth, x, y;
    char *string = "Window too small ... please resize";
    extern Display *display;			/* Declared in open_x_fplot.c */
    extern GC gc_instr;
    extern XFontStruct *font_instr;
    extern Window win;
    extern XSizeHints size_hints;

    XClearWindow (display, win);

    swidth = XTextWidth (font_instr, string, strlen(string));
    y = height/2;
    x = (width - swidth) / 2;

    XDrawString (display, win, gc_instr, x, y, string, strlen(string));

    return (0);
    }
