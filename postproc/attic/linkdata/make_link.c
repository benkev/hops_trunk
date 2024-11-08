/************************************************************************/
/*									*/
/* Creates directories as needed in the destination directory, and	*/
/* makes symbolic links to the specified data file.			*/
/* Called by the make_*links() routines.				*/
/*									*/
/*	Inputs:		filename	Name of file to link		*/
/*			datadir, destdir   By extern			*/
/*									*/
/*	Output:		return		0 = OK, >0 bad			*/
/*									*/
/* Created 20 October 1993 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

int
make_link (filename)
char *filename;
    {
    int j, k, mode;
    char c, expdir[10], scandir[20], dest_expdir[256], dest_scandir[256];
    char linkname[256], realname[256];
    char cmd[256];
    struct stat status;
    extern char datadir[], destdir[];
/*  extern int errno, sys_nerr; */
/*  extern char *sys_errlist[]; */

					/* These are the source and target pathnames */
    sprintf (linkname, "%s/%s", destdir, filename);
    sprintf (realname, "%s/%s", datadir, filename);
					/* Does link file already exist? */
    if (lstat (linkname, &status) == 0)
	if (S_ISLNK (status.st_mode)) return (0);
	else 
	    {
	    msg ("File '%s' exists already and is not a link!", 3, filename);
	    return (1);
	    }
					/* Check that the original file exists */
    if (stat (realname, &status) != 0)
	{
	if (errno == ENOENT)
	    msg ("Target file '%s' does not exist!", 3, realname);
	else
	    {
	    msg ("Could not stat '%s'", 3, realname);
/* 	    if (errno <= sys_nerr) msg ("%s", 3, sys_errlist[errno]); */
	    }
	return (1);
	}
					/* Split out experiment and scan dirs */
    j = k = 0;
    while ((c = filename[j++]) != '/')
	    expdir[k++] = c;
    expdir[k] = '\0';
    k = 0;
    while ((c = filename[j++]) != '/')
	    scandir[k++] = c;
    scandir[k] = '\0';
					/* Form full directory pathnames */
    sprintf (dest_expdir, "%s/%s", destdir, expdir);
    sprintf (dest_scandir, "%s/%s", dest_expdir, scandir);

					/* Create exp. directory */
					/* if does not already exist */
    mode = S_IRUSR | S_IWUSR | S_IXUSR |
                S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
    if (stat (dest_expdir, &status) != 0)
	if (mkdir (dest_expdir, mode) == -1)
    	    {
    	    msg ("Failed to create directory for experiment %s", 3, expdir);
/* 	    if (errno <= sys_nerr) msg ("%s", 3, sys_errlist[errno]); */
	    return (1);
    	    }				/* Add to list of new directories */
	else if (dir_list (expdir) != 0)
	    {
	    msg ("Error maintaining file/directory lists", 2);
	    return (1);
	    }
					/* Create scan directory */
					/* if does not already exist */
    if (stat (dest_scandir, &status) != 0)
	if (mkdir (dest_scandir, mode) == -1)
	    {
	    msg ("Failed to create directory for experiment %s, scan %s",
					 3, expdir, scandir);
/*	    if (errno <= sys_nerr) msg ("%s", 3, sys_errlist[errno]); */
	    return (1);
	    }
	else if (dir_list (scandir) != 0)
	    {
	    msg ("Error maintaining file/directory lists", 2);
	    return (1);
	    }

					/* Now make the symbolic link */
					/* No error if it's already there */
    if (symlink (realname, linkname) != 0)
	if (errno != EEXIST)
	    {
	    msg ("Unable to make symbolic link for file '%s'", 3, filename);
/* 	    if (errno <= sys_nerr) msg ("%s", 3, sys_errlist[errno]); */
	    return (1);
	    }				/* Add to list of new files */
    else if (file_list (filename) != 0)
	{
	msg ("Error maintaining file/directory lists", 2);
	return (1);
	}

    return (0);
    }
