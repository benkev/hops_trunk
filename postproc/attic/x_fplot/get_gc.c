/*************************************************************/
/*                                                           */
/* Part of the Xwindow fringe plot popup library.            */
/* Get graphics context.  Note setting of foreground, to     */
/* avoid white-on-white display                              */
/*                                                           */
/* Initial version CJL 8 Oct 1991                            */
/*                                                           */
/*************************************************************/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>

void
get_gc (win, gc, font_info)
Window win;
GC *gc;
XFontStruct *font_info;
    {
    unsigned int valuemask = 0;
    XGCValues values;
    unsigned int line_width = 1;
    extern Display *display;			/* Declared in open_x_fplot.c */
    extern int screen;
    
    *gc = XCreateGC (display, win, valuemask, &values);

    XSetFont (display, *gc, font_info->fid);

    XSetForeground (display, *gc, BlackPixel (display, screen));
    }
