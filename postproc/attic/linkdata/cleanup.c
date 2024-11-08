/************************************************************************/
/*									*/
/* Return destination directory to original state if so instructed	*/
/* by the user.								*/
/*									*/
/*	Inputs:								*/
/*									*/
/*	Output:								*/
/*									*/
/* Created 21 October 1993 by CJL					*/
/*									*/
/************************************************************************/

void
cleanup()
    {
    extern int nfil, ndir;
    extern char **dlist, **flist;

    if (confirm ("Abort: want to clean up the debris?"))
	{
	msg ("Sorry, cleanup not yet implemented, have to do it yerself", 2);
	}
    }
