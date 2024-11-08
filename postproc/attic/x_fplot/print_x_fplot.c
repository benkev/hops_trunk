/*************************************************************/
/*                                                           */
/* Part of the Xwindow fringe plot popup library.            */
/* This routine actually draws the text onto the screen.     */
/* In half-page mode, it is smart enough to only clear the   */
/* screen if the contents of the window are not already what */
/* is to be written there (I still can't believe that X      */
/* requires the client to worry about this ... there must be */
/* a model of a really dumb server built in)                 */
/*                                                           */
/* Initial version CJL 8 Oct 1991                            */
/*                                                           */
/*************************************************************/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>

#include "print_page.h"

#define FULLPAGE 0
#define HALFPAGE 1

int
print_x_fplot (fplot, width, height, half, mode)
char **fplot;
unsigned int width, height;
int half;               /* 1 or 2 */
int mode;               /* 0 or 1 for now */
    {
    int nchar, nlines, i, x, swidth;
    static int oldhalf;
    char messageh[100], messagef[100];
    extern Display *display;                    /* Declared in open_x_fplot.c */
    extern GC gc, gc_instr;
    extern XFontStruct *font_info, *font_instr;
    extern Window win;
    extern int font_width, font_height;
    extern int fplot_mode, split_location;

    if (mode == 0)
        {
        sprintf (messageh,
            "*** SPACEBAR FOR PAGE 2 (q-quit, n-next, p-previous, ^h-hardcopy) ***");
        sprintf (messagef,"*** q-quit, n-next, p-previous, ^h-hardcopy ***");
        }
    else if (mode == 1)
        {
        sprintf (messageh, "*** SPACEBAR FOR PAGE 2 (q-quit, ^h-hardcopy) ***");
        sprintf (messagef, "*** q-quit, ^h-hardcopy ***");
        }
    else
        {
        msg ("Invalid mode %d in print_x_fplot()", 2, mode);
        return (-1);
        }

    nchar = (width-2) / font_width;
    if (nchar > WIDTH) nchar = WIDTH;
    nlines = (height-10) / font_height;
    if (nlines > HEIGHT) nlines = HEIGHT;

    if ((half != 1) && (half != 2))
        {
        msg ("half variable in print_x_fplot out of range", 2);
        return (1);
        }

                                        /* Main print loop ... leave margins */
                                        /* at top and left edge */
                                        /* Note that fringe plots start in column 1, not 0 */
    if (fplot_mode == FULLPAGE)
        {
        for (i=0; i<nlines; i++)
            {
            if (strncmp (fplot[i], "**", 2) == 0) break;
            XDrawString (display, win, gc, 2,  i*font_height + 10, 
                fplot[i]+1, nchar);
            }
        swidth = XTextWidth (font_instr, messagef, strlen(messagef));
        x = (width - swidth) / 2;
        XDrawString (display, win, gc_instr, x, height-5, messagef, strlen (messagef));
        }
    else if (fplot_mode == HALFPAGE)
        {
        if (half != oldhalf)
            {
            XClearWindow (display, win);
            oldhalf = half;
            }
        if (half == 1)
            {
            for (i=0; i<split_location; i++)
                XDrawString (display, win, gc, 2, i*font_height + 10,
                        fplot[i]+1, nchar);
            messageh[22] = '2';
            swidth = XTextWidth (font_instr, messageh, strlen(messageh));
            x = (width - swidth) / 2;
            XDrawString (display, win, gc_instr, x, height-5, messageh, strlen (messageh));
            }
        else
            {
            for (i=0; (i+split_location)<HEIGHT; i++)
                {
                if (strncmp (fplot[i+split_location], "**", 2) == 0) break;
                XDrawString (display, win, gc, 2, i*font_height + 10,
                        fplot[i+split_location]+1, nchar);
                }
            messageh[22] = '1';
            swidth = XTextWidth (font_instr, messageh, strlen(messageh));
            x = (width - swidth) / 2;
            XDrawString (display, win, gc_instr, x, height-5, messageh, strlen (messageh));
            }
        }
    else
        {
        msg ("Value of fplot_mode out of range in print_x_fplot", 2);
        return (1);
        }

    return (0);
    }
