/************************************************************************/
/*                                                                      */
/* Adds the string argument to a list of directory names.  This is to   */
/* permit their clean removal in an abort situation.                    */
/*                                                                      */
/*      Inputs:         dir_name        Directory just created          */
/*                                                                      */
/*      Output:         return value    0=OK, 1=bad                     */
/*                                                                      */
/* Created 21 October 1993 by CJL                                       */
/*                                                                      */
/************************************************************************/
#include <stdio.h>

int
dir_list (dir_name)
char *dir_name;
    {
    extern int ndir;
    extern char **dlist;

    if (strlen (dir_name) > 31)
        {
        msg ("Directory name '%s' too long!", 3, dir_name);
        return (1);
        }
/*    strcpy (dlist[ndir], dir_name);  */
    ndir++;

    return(0);
    }
