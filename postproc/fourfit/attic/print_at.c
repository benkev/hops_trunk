/********************************************************/
/*                                                      */
/* print_at is a function to print into a square        */
/* character matrix.  It takes as arguments a pointer   */
/* to the matrix, the X and Y coordinates, and the      */
/* string to be printed.                                */
/*                                                      */
/*      WARNING:  If the string exceeds the bounds of   */
/*              the character matrix (to be determined  */
/*              by the NULL terminated strings, and     */
/*              a NULL pointer at the bottom of the     */
/*              array) then the excess information      */
/*              will be LOST.  There will be NO error   */
/*              messages by default, although setting   */
/*              the BOUNDS_ERROR constant to TRUE       */
/*              at compile time will allow errors to    */
/*              be routed to STDERR.                    */
/*                                                      */
/*      Author: Tony Lower                              */
/*              Created: June 25, 1991,                 */
/*              Last Edited: June 25, 1991              */
/*              Modified October 2 1991 to conform to   */
/*              style conventions, CJL.                 */
/*                                                      */
/********************************************************/

#include <stdio.h>
#include <string.h>
#include "print_page.h"

#define BOUNDS_ERROR            TRUE    
#include <stdlib.h>

void 
print_at (char** matrix, int X, int Y, char* str )
    {
    int lower_bound = 0;
    char *point;
    char temp[128];

    strncpy (temp, str, 127);
    temp[127] = '\0';
                                        /* Recursively process multiple lines */
    if ((point = strchr(temp, '\n')) != NULL)
        {
        point[0] = 0;
        print_at(matrix, X, Y, temp);
        print_at(matrix, X, Y+1, point+1);
        return;
        } /* if */

                                        /*  Scan the matrix to determine the lower bound. */

    for( ;matrix[lower_bound+1] != NULL;lower_bound++);

    if (Y > lower_bound)
        {
        if (BOUNDS_ERROR)
            fprintf(stderr, "Bounds error in procedure print_at.\n");
        return ;
        } 

    if (X >= strlen(matrix[Y]) )
        {
        if (BOUNDS_ERROR)
            fprintf(stderr,"Bounds error in procedure print_at.  All info lost.\n");
        return;
        }       
        
    if (X+strlen(temp) > strlen(matrix[Y]))
        {
        if (BOUNDS_ERROR)
            fprintf(stderr,"Bounds error in procedure print_at.  Info lost.\n");
        strncpy(matrix[Y]+X, temp, strlen(matrix[Y])-X-1);
printf ("string = '%s'\n",temp);
        return ;
        }

    strncpy(matrix[Y]+X, temp, strlen(temp));

    } /* print_at */
