/************************************************************************/
/*                                                                      */
/* Figures out the name of the parent root file, given the source       */
/* name and the corel or fringe extent filename                         */
/*                                                                      */
/*      Inputs:         filename        of corel/fringe extent file     */
/*                      source          to construct root filename      */
/*                                                                      */
/*      Output:         return value    Root filename                   */
/*                                                                      */
/* Created 20 October 1993 by CJL                                       */
/*                                                                      */
/************************************************************************/

char *
parent_root (filename, source)
char *filename, *source;
    {
    int i, j, n;
    char c, lsource[32], rootcode[8], scandir[50];
    static char rootname[256];

                                        /* Strip out scan directory */
    i = j = 0;
    while ((c = filename[i++]) != '/') scandir[j++] = c;
    scandir[j++] = c;
    while ((c = filename[i++]) != '/') scandir[j++] = c;
    scandir[j] = '\0';
                                        /* Extract root id code */
    n = strlen (filename);
    strcpy (rootcode, filename + n - 6);
                                        /* Convert dots in source name */
    i = j = 0;
    while ((c = source[i++]) != '\0')
        if (c == '.') lsource[j++] = '_';
        else lsource[j++] = c;
    lsource[j] = '\0';
                                        /* Construct the root name */
    sprintf (rootname, "%s/%s.%s", scandir, lsource, rootcode);

    return (rootname);
    }
