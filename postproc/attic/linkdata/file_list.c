/************************************************************************/
/*									*/
/* Adds the string argument to a list of directory names.  This is to	*/
/* permit their clean removal in an abort situation.			*/
/*									*/
/*	Inputs:		file_name	File just created		*/
/*									*/
/*	Output:		return value	0=OK, 1=bad			*/
/*									*/
/* Created 21 October 1993 by CJL					*/
/*									*/
/************************************************************************/

int
file_list (file_name)
char *file_name;
    {
    extern int nfil;
    extern char **flist;

    if (strlen (file_name) > 31)
	{
	msg ("File name '%s' too long!", 3, file_name);
	return (1);
	}
/*    strcpy (flist[nfil], file_name);  */
    nfil++;

    return(0);
    }
