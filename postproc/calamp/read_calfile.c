/************************************************************************/
/*                                                                      */
/* Reads a file in the format written by John Ball's RCal program.  The */
/* file contains preprocessed calibration information for a number of   */
/* stations.  Most of it is automatically generated from machine        */
/* readable station logs, but hand editing will typically have been     */
/* performed.  The job of this routine is to parse the information into */
/* the extern structure array "dal", where it can be conveniently       */
/* accessed by the main calibration routine "calfact()".                */
/*                                                                      */
/*      Inputs:         finam           Name of RCal output file        */
/*                                                                      */
/*      Output:         dal             All filled in                   */
/*                                                                      */
/* Created October 1996 by JAB                                          */
/* Converted to CJL's preferred style October 24 1996 by CJL, no        */
/* criticism intended.                                                  */
/* Fixed infinite loop in code Nov. 21, 1997 by DAB                     */
/*                                                                      */
/************************************************************************/
#include "calamp.h" 
#include "mk4_util.h" 
 
int 
read_calfile (char *finam) 
    { 
    char * tok[20];                     /* Point to tokens on a line */ 
    FILE * fil; 
    char line[1024];                    /* To hold a line from the file */ 
    char * sepr = " \t\n\r";            /* Separators between tokens */ 
    int i, len;  
    int ns = 0;                         /* Station number = index into dal... */ 
    int nn = 0;                         /* Cal line number for each station */ 
    int y = 1996;                       /* UT year default */ 
    int d, da = 0, h, m, s;             /* UT day and time */ 
    double parms[4];
    extern char * sta[];
    extern int sta_size, force_tsys;
    extern char stl[];
    extern struct Dal dal;

                                        /* Open input file; errors? */ 
    if ((fil = fopen(finam, "r")) == NULL) 
        { 
        msg ("ERROR: \007 Can't fopen file ", 2); 
        perror(finam); 
        return(-1); 
        } 
                                        /* Loop forever */ 
    while (TRUE) 
        { 
                                        /* Read a line; errors? */ 
        if (fgets(line, sizeof(line), fil) == NULL) 
            { 
                                        /* Have we read anything significant? */ 
            if (ns == 0 || nn == 0) 
                {
                msg ("End of file on ", 2); 
                perror(finam); 
                msg ("\007 This may be an ERROR", 2); 
                }
            break;
            } 
                                        /* Strip newline in case need to print it */
        len = strlen (line);
        if (len > 0) line[len-1] = '\0';
                                        /* Not blank is comment or header line */ 
        if (line[0] != ' ') 
            { 
            msg ("reading \"%s\"", 0, line);
                                        /* START header? */ 
            if (strncmp(line, "START", 5) == 0) 
                { 
                ns = 0; 
                                        /* Find station name in sta[] */ 
                for (i = 1; i < sta_size; i++) 
                    if (strncmp(line+6, sta[i], 2) == 0) 
                        { 
                        ns = i;
                                        /* Initialize line number for this source */ 
                        nn = 1; 
                        break; 
                        } 
                                        /* Did we find this station? */ 
                if (ns == 0) 
                    { 
                    msg ("ERROR: \007 Unknown station after START", 2); 
                    msg ("%s", 2, line); 
                    return(-2);
                    } 
                } 
                                        /* PARAMeters? */ 
            else if (strncmp(line, "PARAM", 5) == 0) 
                { 
                                        /* read them in */ 
                tok[0] = strtok(line, sepr); 
                parms[0] = atof(strtok(NULL, sepr)); /* S2 */ 
                parms[1] = atof(strtok(NULL, sepr)); /* C2 */ 
                parms[2] = atof(strtok(NULL, sepr)); /* E0 */ 
                parms[3] = atof(strtok(NULL, sepr)); /* G */ 
                }
                                        /* UT year? */ 
            else if (strncmp(line, "YEAR", 4) == 0) 
                y = atoi(line+5); 
                                        /* Substitute UT day? */ 
            else if (strncmp(line, "DAY", 3) == 0) 
                da = atoi(line+4);
                                        /* We ignore all else as comments */
            continue; 
            }                           /* End of if not blank */ 

                                        /* Check for comment character and */
                                        /* remove */
        i = 0;
        while (line[i] != '\0')
            {
            if (line[i] == '!')
                {
                line[i] = '\0';
                break;
                }
            ++i;
            }
                                        /* Here we have a line[] with cal data */ 
                                        /* Get all the tokens */ 
        for (i = 0; i < 20; i++) 
            {
                                        /* Get first token */ 
            if (i == 0) tok[0] = strtok(line, sepr); 
                                        /* Subsequent tokens */ 
            else if ((tok[i] = strtok(NULL, sepr)) == NULL) break; 
            }
                                        /* Check for the right number of tokens */ 
        if (i != 3 && i != 4 && i != 6) 
            { 
            msg ("ERROR: \007 Wrong number of tokens %d", 2, i); 
                                        /* Can't print line[] after using strtok() */ 
            msg ("tok[] = %s %s %s %s ...", 2, tok[0], tok[1], tok[2], tok[3]); 
            return(-3);
            } 
                                        /* Use substitute UT day? */ 
        if ((d = atoi(tok[0])) <= 0 && da > 0) d = da;
                                        /* UT time */ 
        (void) sscanf(tok[1], "%d:%d:%d", &h, &m, &s);
        dal.ctime[ns][nn] = time_to_int (y, d, h, m, s);
                                        /* Copy in current parm values */
        dal.parms[ns][nn][0] = parms[0];
        dal.parms[ns][nn][1] = parms[1];
        dal.parms[ns][nn][2] = parms[2];
        dal.parms[ns][nn][3] = parms[3];
                                        /* Tsys only, -ve means 3-token */ 
        if (i == 3) dal.ctigs[ns][nn] = -atof(tok[2]);
                                        /* Tsys and source */
        else if (i == 4)
            {
            dal.ctigs[ns][nn] = -atof(tok[2]);
            (void) strncpy (dal.sources[ns][nn], tok[3], MAXNAME);
            }
                                        /* Tigs  S*Tsys/Tant */ 
        else  
            {
            dal.ctigs[ns][nn] = atof(tok[5])*atof(tok[2])/atof(tok[3]); 
                                        /* User requested Tsys override */
            if (force_tsys) dal.ctigs[ns][nn] = -atof(tok[2]);
            (void) strncpy (dal.sources[ns][nn], tok[4], MAXNAME);
            }
                                        /* Increment nn for next time; overflow? */ 
        if (++nn >= MAXRUNS) 
            { 
            msg ("%s ERROR:  MAXRUNS exceeded; array overflow \n", 2); 
            return(-5); /* So go increase MAXRUNS */ 
            } 
        }                               /* End of while loop forever */ 

                                        /* Probably normal end */ 
    msg ("read_calfile(), normal end", -1);
    return(0); 
    }
