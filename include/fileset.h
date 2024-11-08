#ifndef FSTRUCT_H
#define FSTRUCT_H

#include "fstruct.h"

#define MAXFSET 10000

struct fileset
    {
    char        scandir[256];   /* Absolute pathname */
    short       expno;          /* Part of directory name */
    short       maxfile;        /* Biggest file number enountered */
    char        scanname[32];   /* Part of directory name */
    char        rootname[40];   /* Stripped of directory information */
    short       mxfiles[4];     /* maxfile,  but by mod4 */
    fstruct     file[MAXFSET];  /* Files belonging to fileset */
    };

/* code in sub/util */
extern int get_fileset(char *rootname, struct fileset *fset);

#endif

