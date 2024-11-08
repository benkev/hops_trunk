/************************************************************************/
/*                                                                      */
/* Figures out the name of the type 3 (station) file associated with    */
/* this fringe file                                                     */
/*                                                                      */
/*      Inputs:         filename        of fringe file                  */
/*                      refrem          reference or remote station     */
/*                                                                      */
/*      Output:         return value    type 3 filename                 */
/*                                                                      */
/* Created 19 March 2002 by CJL                                         */
/*                                                                      */
/************************************************************************/

char *
type_3 (filename, refrem)
char *filename, *refrem;
    {
    int i, j, n;
    char c, lsource[32], rootcode[8], scandir[50], station;
    static char type3name[256];

                                        /* Strip out scan directory */
    i = j = 0;
    while ((c = filename[i++]) != '/') scandir[j++] = c;
    scandir[j++] = c;
    while ((c = filename[i++]) != '/') scandir[j++] = c;
    scandir[j] = '\0';
                                        /* Extract root id code */
    n = strlen (filename);
    strcpy (rootcode, filename + n - 6);
                                        /* Reference or remote */
    if (strcmp (refrem, "ref") == 0) station = filename[i];
    else if (strcmp (refrem, "rem") == 0) station = filename[i+1];
                                        /* Construct the type 3 name */
    sprintf (type3name, "%s/%c..%s", scandir, station, rootcode);

    return (type3name);
    }
