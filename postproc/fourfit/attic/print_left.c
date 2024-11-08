/*************************************************/
/*                                               */
/*      print_left is a procedure for printing   */
/* things left justified.  It takes the same     */
/* parameters as print_at, only the X in question*/
/* means the right side of the string, not the   */
/* left.  All the warnings about print_at apply  */
/* but I bet you aren't going to read them, are  */
/* you?  Just going to ignore common sense, and  */
/* work without looking at all the nice comments */
/* I wrote for you!  It's enough to make a       */
/* programmer repent his art!                    */
/*                                               */
/*      Author: Tony Lower                       */
/*              Created: June 25, 1991           */
/*              Last Edit: June 26, 1991         */
/*                                               */
/*************************************************/

#include <stdio.h>
#include <string.h>
#include "print_page.h"

#define BOUNDS_ERROR            TRUE    
#include <stdlib.h>

void 
print_left ( matrix, X, Y, str )
char **matrix ;
int X, Y ;
char *str ;
        
    {
    int lower_bound = 0;
    char *point;

    if ((point = strchr(str, '\n'))!=NULL)
        {
            point[0] = 0;
            print_left(matrix, X, Y, str);
            print_left(matrix, X, Y+1, point+1);
            return;
        } /* if */
        
        /* Scan the matrix to determine the lower bound. */

    for(;matrix[lower_bound+1]!=NULL;lower_bound++);

    if (Y > lower_bound)
        {
            if (BOUNDS_ERROR)
                fprintf(stderr,"Bounds error in procedure print_left.\n");
printf ("string = '%s'\n", str);
            return;
        } /* if */

    if ((X >= strlen(matrix[Y])+strlen(str)) || (X < 0))
        {
            if (BOUNDS_ERROR)
                fprintf(stderr,"Bounds error in procedure print_left.  All info lost.\n");
printf ("string = '%s'\n", str);
            return;
        } /* if */

    if (X < strlen(str))
        {
            if (BOUNDS_ERROR)
                fprintf(stderr,"Bounds error in print_left.  Some info lost.\n"); 
            strncpy(matrix[Y], str+(strlen(str)-X), X);
printf ("string = '%s'\n", str);
            return;
        } /* if */      

    if (X > strlen(matrix[Y]))
        {
            if (BOUNDS_ERROR)
                fprintf(stderr,"Bounds error in print_left.  Info lost.\n");
            strncpy(matrix[Y]+(X-strlen(str)), str, strlen(matrix[Y])-(X-strlen(str))) ;
printf ("string = '%s'\n", str);
            return;
        } /* if */

    strncpy(matrix[Y]+(X-strlen(str)), str, strlen(str));

    } /* print_left */
