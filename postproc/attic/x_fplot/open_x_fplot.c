/*************************************************************/
/*                                                           */
/* Part of the Xwindow fringe plot popup library.            */
/* This routine opens the display and font, figures out how  */
/* big the screen is, decides whether we can display it all  */
/* on one page or not, and does all the other incredibly     */
/* tedious and undefaulted things X seems to find necessary  */
/*                                                           */
/* Initial version CJL 8 Oct 1991                            */
/*                                                           */
/*************************************************************/
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>

#include "print_page.h"
#include "icon_bitmap.h"        /* Needs to be prepared using X utilities */

#define FULLPAGE 0
#define HALFPAGE 1


Display *display;               /* Declare these global to avoid nasty */
int screen;                     /* long argument lists in routines whose */
Window win;                     /* function is conceptually simple */
XSizeHints size_hints;
XFontStruct *font_info, *font_instr;
GC gc, gc_instr;
int font_width, font_height;
int fplot_mode = FULLPAGE;
int split_location = 64;

int
open_x_fplot (argc, argv)
char **argv;
int argc;
    {
    unsigned int width, height, cwidth, cheight, dwidth, dheight, dummy1, dummy2;
    int x = 0, y = 0;                   /* User decides where window goes */
    int window_size = 0;
    unsigned int border_width = 4;
    char *window_name = "Fringe Plot Window";
    char *icon_name = "fplot";
    Pixmap icon_pixmap;
    char *display_name = NULL;          /* Tells X to use DISPLAY env. var. */
                                        /* Standard stuff here */
    if ((display = XOpenDisplay (display_name)) == NULL)
        {
        msg ("Cannot open Xwindow display", 2);
        return (1);
        }
    screen = DefaultScreen (display);

    dwidth = DisplayWidth (display, screen);
    dheight = DisplayHeight (display, screen);

                                        /* This is where we decide on a font to */
                                        /* fit the screen size */
    if (getfont_fplot(&font_info, "6x10", &font_height, &font_width) != 0)
        {
        msg ("Error getting font for fringe plot popup window", 2);
        return (1);
        }
/*  else font_height -= 1;              /* Squish 6x10 font for this application */
    else font_height -= 2;              /* modified by rjc 99.8.20 to fit ccc console */

    if (getfont_fplot(&font_instr, "9x15", &dummy1, &dummy2) != 0)
        {
        msg ("Error getting font for fringe plot popup window", 2);
        return (1);
        }
                                        /* Make window big enough to hold entire plot */
    width = font_width * WIDTH + 8;
    height = font_height * HEIGHT + 20;
    if (height > dheight+40)            /* Accomodate 1024 high screen */
        {
        height = font_height * HEIGHT / 2 + 20;
        fplot_mode = HALFPAGE;
        }

                                        /* Do the simplest possible window */
    win = XCreateSimpleWindow (display, RootWindow(display,screen), x, y, 
                width, height, border_width,  BlackPixel(display,screen), 
                WhitePixel(display,screen));

                                        /* Standard stuff */
    icon_pixmap = XCreateBitmapFromData (display, win, icon_bitmap_bits,
                icon_bitmap_width, icon_bitmap_height);

    size_hints.flags = PPosition | PSize | PMinSize;
    size_hints.x = x;
    size_hints.y = y;
    size_hints.width = width;
    size_hints.height = height;
    size_hints.min_width = 50 * font_width;
    size_hints.min_height = 50 * font_height;
                                        /* Pass nothing for command line ??? */
    XSetStandardProperties (display, win, window_name, icon_name, icon_pixmap,
                argv, argc, &size_hints);
                                        /* We want action on key or button press */
                                        /* and must redo contents on resize */
    XSelectInput (display, win, ExposureMask | KeyPressMask | ButtonPressMask |
                StructureNotifyMask | EnterWindowMask);

    get_gc (win, &gc, font_info);
    get_gc (win, &gc_instr, font_instr);

    return (0);                         /* All ready to go */
    }
