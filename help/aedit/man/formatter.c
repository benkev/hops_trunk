#include <stdio.h>

#define NOSECTION   0
#define COMMAND     1
#define TYPE        2
#define SYNTAX      3
#define DESCRIPTION 4
#define TRUE        1
#define FALSE       0

main (argc, argv)
int argc;
char *argv[];
    {
    FILE *fp;
    int i, first_section, section, len, desc, newitem, inquote, verb;
    char *string, line[250], buf[250], dummy[250];

    fp = fopen (argv[1], "r");
    first_section = TRUE; inquote = FALSE; verb = FALSE;
    while (fgets (line, 250, fp) != NULL)
	{
	len = strlen (line) - 1;
	if (line[len] == '\n') line[len] = '\0';
	if (strstr (line, "begin{verbatim}") != NULL)
	    {
	    printf ("%s\n", line);
	    verb = TRUE;
	    continue;
	    }
	if (strstr (line, "end{verbatim}") != NULL)
	    {
	    printf ("%s\n", line);
	    verb = FALSE;
	    continue;
	    }
	if (verb)
	    {
	    printf ("%s\n", line);
	    continue;
	    }
	fixquotes (line, buf);

	section = NOSECTION;
	if (strncmp (buf, "COMMAND NAME:", 13) == 0) section = COMMAND;
	else if (strncmp (buf, "TYPE:", 5) == 0) section = TYPE;
	else if (strncmp (buf, "SYNTAX:", 7) == 0) section = SYNTAX;
	else if (strncmp (buf, "DESCRIPTION:", 12) == 0) section = DESCRIPTION;

	if (section != NOSECTION)
	    {
	    desc = FALSE;
	    if (! first_section) printf ("\\end{tabbing}\n\n");
	    first_section = FALSE;

	    switch (section)
		{
		case COMMAND:
		case TYPE:
		case SYNTAX:
		    printf ("\\begin{tabbing}\n");
		    printf ("Titlexxxxxxxxxxxxxxx \\= \\kill\n");
		    break;
		case DESCRIPTION:
		default:
		    ;
		}
	    switch (section)
		{
		case COMMAND:
		    printf ("\\underline{COMMAND} \\> {\\bf %s} \\\\\n", buf+13);
		    break;
		case TYPE:
		    printf ("\\underline{TYPE} \\> {\\bf %s} \\\\\n", buf+5);
		    break;
		case SYNTAX:
		    printf ("\\underline{SYNTAX} \\> {\\bf %s} \\\\\n", buf+7);
		    break;
		case DESCRIPTION:
		    printf ("\\underline{DESCRIPTION}\n\\begin{list}{}");
		    printf ("{\\setlength{\\leftmargin}{0.5in}\n");
		    printf ("     \\setlength{\\rightmargin}{0in}}\n");
		    printf ("\\item\n");
		    newitem = TRUE;
		    desc = TRUE;
		    break;
		default:
		    ;
		}
	    }

	else 
	    {
	    if (desc)
		{
		if (sscanf (buf, "%s", dummy) == 1) 
		    {
		    printf ("%s\n", buf);
		    newitem = FALSE;
		    }
		else 
		    {
		    if (! newitem) printf ("\\item\n");
		    newitem = TRUE;
		    }
		}
	    else if (sscanf (buf, "%s", dummy) == 1)
	       printf ("\\> {\\bf %s} \\\\\n", buf);
	    }
	}
    if (desc) printf ("\\end{list}\n\\vspace{.2in}\n\n");
    }
