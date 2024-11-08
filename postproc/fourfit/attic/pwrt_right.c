/************************************************************************/
/*                                                                      */
/* Creates postscript commands for writing a right-justified string     */
/*                                                                      */
/*      Inputs:     x               device coord 0 to 1                 */
/*                  y               device coord 0 to 1                 */
/*                  string          text to show                        */
/*                                                                      */
/*      Output:     return value    postscript commands                 */
/*                                                                      */
/* Created October 15 1999  by CJL                                      */
/*                                                                      */
/************************************************************************/
#include <stdio.h>

char *
pwrt_left (
double x,
double y,
char *string)
    {
    static char buf[256];
    int xval, yval;
                                        /* Based on 7.8x10 plot surface */
    xval = x * 800;
    yval = y * 1000;
    sprintf (buf, "%d %d moveto (%s) showr\n", xval, yval, string);
    return (buf);
    }
