/****************************************************************/
/*                                                              */
/* Part of the Xwindow fringe plot popup library.               */
/* This routine is the main event-driven loop for the plot      */
/* window.  It also takes care of pagination in the event       */
/* that we are dealing with a half-page display                 */
/* A return value of 0 means that a 'q' was pressed, and the    */
/* window has been unmapped.                                    */
/* A return value of 1 means that a 'n' was pressed, and the    */
/* window is still visible                                      */
/* A return value of -1 means that a 'p' was pressed, and       */
/* the window is still visible                                  */
/*                                                              */
/* The "mode" argument alters the behaviour of the popup.  At   */
/* the present time, 0 and 1 are supported                      */
/* 0 means activate keys "n" and "p" for passing back return    */
/* values 1 and -1 respectively.                                */
/* 1 means do only the "q" key                                  */
/*                                                              */
/* Initial version CJL 8 Oct 1991                               */
/* Added mode argument to permit some application customization */
/* CJL 10 March 1992                                            */
/*                                                              */
/****************************************************************/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/keysym.h>
#include <stdio.h>

#include "print_page.h"

#define SMALL 1
#define OK 0
#define HALFPAGE 1

int
show_x_fplot (fplot, mode)
char **fplot;
int mode;
    {
    static unsigned int width, height;
    static int window_size = OK;
    static int half = 1;
    XEvent report;                              /* Contains event information */
    char buffer[10];
    int bufsize = 10;
    KeySym keysym;
    XComposeStatus compose;
    int count;

    extern Display *display;                    /* Declared in open_x_fplot.c */
    extern Window win;
    extern XSizeHints size_hints;
    extern int fplot_mode, split_location;

    XMapWindow (display, win);                  /* Make it visible and empty */
    XClearWindow (display, win);
                                                /* Figure out where to split 1/2 page */
    if (fplot_mode == HALFPAGE)
        if ((split_location = pgbreak_fplot (fplot)) < 0) return (1);
                                                /* Draw plot in empty window */
    print_x_fplot (fplot, width, height, half, mode);

    while (TRUE)
        {
        XNextEvent (display, &report);          /* Process various events */
        switch (report.type)
            {
            case Expose:                        /* Redraw window contents */
                while (XCheckTypedEvent (display, Expose, &report)) ;
                if (window_size == SMALL)
                    too_small (width, height);
                else
                    print_x_fplot (fplot, width, height, half, mode);
                break;

            case ConfigureNotify:               /* Resizing .. check if still big */
                width = report.xconfigure.width;                /* enough */
                height = report.xconfigure.height;
                if ((width < size_hints.min_width) || (height < size_hints.min_height))
                    window_size = SMALL;
                else window_size = OK;
                break;
                                                /* For now, get keyboard focus on any button press */
            case ButtonPress:
                XSetInputFocus (display, win, RevertToParent, CurrentTime);
                break;
                                                /* Process keypress events.  For spacebar, */
                                                /* toggle pages in halfpage mode */
                                                /* for 'q', don't exit, just hide the window */
                                                /* for possible later use and return */
            case KeyPress:                      /* for 'n' and 'p', leave window open and return */
                count = XLookupString ((XKeyEvent *) &report, buffer, bufsize, &keysym, &compose);
                if ((keysym == XK_space) &&  (fplot_mode == HALFPAGE))
                    {
                    if (half == 1) half = 2;
                    else if (half == 2) half = 1;
                    print_x_fplot (fplot, width, height, half, mode);
                    }
                else if ((keysym == XK_q) || (keysym == XK_Q))
                    {
                    XUnmapWindow (display, win);
                    XFlush (display);
                    return (0);
                    }
                else if ((keysym == XK_h) || (keysym == XK_H))
                    {
                    if (report.xkey.state & ControlMask) 
                        hcopy_x_fplot (fplot);
                    }
                else if ((keysym == XK_p) || (keysym == XK_P))
                    {
                    if (mode == 0) return (-1);
                    }
                else if ((keysym == XK_n) || (keysym == XK_N))
                    {
                    if (mode == 0) return (1);
                    }
                break;
                                                /* For now, get keyboard focus when pointer in window */
            case EnterNotify:
                XSetInputFocus (display, win, RevertToParent, CurrentTime);
                break;

            default:                            /* Discard all other events */
                break;
            }
        }
    }
