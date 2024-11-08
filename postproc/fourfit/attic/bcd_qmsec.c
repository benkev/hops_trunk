/*******************************************************************************/
/*                                                                             */
/* Converts a 28-bit bcd-format time stamp stored right-justified in the input */
/* as found in type_corel data records, into an integer number of 1/4 msecs    */
/* No range checking is done.                                                  */
/*                                                                             */
/* Original version  CJL August 5 1991                                         */
/* Rewritten to do integer arithmetic by CJL, 24 March 1993		       */
/* Add filter for invalid time digits    RJC  28 April 1993                    */
/* Throw out digits 6,7,8,9 in 10's digit of mins or secs, CJL, 1 April 1994   */
/*                                                                             */
/*******************************************************************************/

int
bcd_qmsec (input)
int input;
    {
    int minsec[2], mins, secs, result, rem,i,shifted_time;
    void bcd_to_2int();

    shifted_time = input;
    for (i=0; i<7; i++)
	{
	if ((shifted_time & 0xF) > 9)
	    return (-1);                /* Return error if invalid time digit */
        shifted_time >>= 4;             /* shift in next nibble */
	}

    bcd_to_2int (input, 7, 3, minsec);
    mins = minsec[0] / 100;
    secs = minsec[0] % 100;
    if ((mins > 59) || (secs > 59)) return (-1);
    result = 240000*mins + 4000*secs + 4*minsec[1];
					/* Take care of fractional msecs */
					/* Assumes multiples of 1.25 msec */
    rem = minsec[1] % 10;
    switch (rem)
	{
	case 1:
	case 6:
	    result += 1; break;
	case 2:
	case 7:
	    result += 2; break;
	case 3:
	case 8:
	    result += 3; break;
	case 0:
	case 5:
	    break;
	default:                        /* Don't want bad point accidentally used */
	    result = -1;
	}
    return (result);
    }
