/*************************************************************/
/*                                                           */
/* Part of the Xwindow fringe plot popup library.            */
/* Tidies up everything ... nothing difficult here.          */
/*                                                           */
/* Initial version CJL 8 Oct 1991                            */
/*                                                           */
/*************************************************************/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>

int
close_x_fplot ()
    {
    extern Display *display;			/* Declared in open_x_fplot.c */
    extern Window win;
    extern GC gc, gc_instr;
    extern XFontStruct *font_info, *font_instr;

    XUnmapWindow (display, win);
    XUnloadFont (display, font_info->fid);
    XUnloadFont (display, font_instr->fid);
    XFreeGC (display, gc);
    XFreeGC (display, gc_instr);
/*    XCloseDisplay (display);    BUG IN HPUX 9.0 */

    return (0);
    }
