/*************************************************************/
/*                                                           */
/* Trivial routine to send messages to stderr, under control */
/* of "importance level" argument.  This allows for user     */
/* selectable verbosity levels for programs.                 */
/*                                                           */
/* Initial version CJL 10 July 1991                          */
/* Modified by CJL, 23 Oct 1991 to allow printf()-style      */
/*      format string and argument list, using varargs       */
/* Expanded to support logfiles and Mk4 messaging, and	     */
/* updated to use the ANSI stdarg.h macros.		     */
/*					CJL, 9 December 1998 */
/*                                                           */
/*************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "mess.h"
#include "message_structs.h"
#include "message_codes.h"
					/* Some necessary externs to be */
					/* overwritten by user program */
char progname[128] = "newbie HOPS programmer's program";
int msglev = 2;
char msg_logfile[128] = "";
					/* Used for Mk4 online system */
int opera_level = 4;

void					/* To understand this, check man pages for */
msg (/* string, level, ...) */		/* stdarg, or K+R 2nd ed., p 174 */
char *string,
int level,
...)
    {
    extern int msglev;
    extern char progname[];
    char outstring[1024], *ptr;
    int len;
    FILE *outfp;
    time_t now;
    struct Error_Message err_msg;
    static int first = TRUE, log = FALSE;
    va_list args;
					/* Some things to do on 1st execution */
    if (first)
	{
					/* Initialize messaging */
	err_msg.bin_head.msg_code = ERROR_MESSAGE;
	err_msg.bin_head.task_id = 0;
					/* Figure out what we do with msgs */
	if (strlen (msg_logfile) > 0)
	    {
	    if ((outfp = fopen (msg_logfile, "w")) == NULL)
		{
		fprintf (stderr, "Could not open logfile '%s'\n", msg_logfile);
		fprintf (stderr, "Logging messages to stderr\n");
		fflush (stderr);
		outfp = stderr;
		}
	    else log = TRUE;
	    }
	else outfp = stderr;
	first = FALSE;
	}
					/* prepend program name */
    ptr = outstring;
    sprintf (ptr, "%s: ", progname);
    len = strlen (ptr);
    ptr += len;
					/* Process variable arg list, hoping */
					/* not to overflow 1024-char buffer */
					/* (vsnprintf is non-standard GNU) */
    va_start (args, level);
    vsprintf (ptr, string, args);
					/* If logging, prepend time stamp */
    if (level >= msglev)
	{
	if (log)
	    {
	    now = time (NULL);
	    fprintf (outfp, "%s: ", ctime (&now));
	    }
					/* Write message with newline */
	fprintf (outfp, "%s\n", outstring);
	fflush (outfp);
	}
					/* Send string by messaging system */
					/* according to opera_level */
					/* Used only for Mk4 online system */
    if (level >= opera_level)
	{
					/* Truncate just in case */
	outstring[LEN_ERR - 1] = '\0';
	strcpy (err_msg.text, outstring);
					/* Send it to opera program */
	send_mess (&err_msg, sizeof (struct Error_Message), "OPR@");
	}
					/* Clean up properly */
    va_end (args);
    }
