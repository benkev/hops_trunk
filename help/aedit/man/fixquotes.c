#define TRUE 1
#define FALSE 0

void
fixquotes (input, output)
char *input, *output;
    {
    char c;
    int i, j;
    static int insquote = FALSE;
    static int indquote = FALSE;

    i = 0; j = 0;
    while ((c = input[i++]) != '\0')
	{
	if (c == '\'') 
	    {
	    if (insquote) 
		{
		output[j++] = c;
		insquote = FALSE;
		}
	    else
		{
		output[j++] = '`';
		insquote = TRUE;
		}
	    }
	else if (c == '"') 
	    {
	    if (indquote) 
		{
		output[j++] = c;
		indquote = FALSE;
		}
	    else
		{
		output[j++] = '`'; output[j++] = '`';
		indquote = TRUE;
		}
	    }
	else if ((c == '>') || (c == '<'))
	    {
	    output[j++] = '$';
	    output[j++] = c;
	    output[j++] = '$';
	    }
	else if (c == '%')
	    {
	    output[j++] = '\\';
	    output[j++] = c;
	    }
	else output[j++] = c;
	}
    output[j] = '\0';
    }
