/*
 * $Id: GetDirectoryInfo.cpp 62 2010-11-04 16:43:18Z gbc $

This program retrieves directory information, modeled after
GetBankStatus (DLS utilities) and mark5dir.cpp (from DiFX world).

*/

#include <stdio.h>
#include <stdlib.h>
#include "xlrapi.h"

/* for pruning the originals to get (only) what we need */
#define MARK5DIR_ORIG 0

#include "stubs/mark5dir.h"		/* 2010y244d from difx/m5d */
#include "stubs/Parse5A-2007-05-22.h"	/* 2007y142d from linux1-pc */
#include "stubs/Parse5A-2008-10-14.h"	/* 2008y288d from muc02 */
#include "stubs/Parse5A-2009-03-17.h"	/* 2009y076d from muc01 */

extern int verb;

#define  BUF_SIZE    0x8000
static SSHANDLE   xlrDevice;

static int hex_dump_dir(unsigned char *d)
{
    int	o, c, x = 0, rows = 10*verb;
    printf ( "%04d:", x++ );
    for (o = 0; o < rows; o++) {
	for (c = 0; c < 20; c++, x++) printf (" %02x", *d++);
	printf( "\n%04d:", x );
    }
    printf( "\n" );
}

static int dumpO(struct SDirO *d)
{
    int	s;
    if (verb>0) printf( " with %d scans\n", d->nscans );
    for (s = 0; s < d->nscans && s < MAXSCANS && d->scanName[s][0]; s++)
	printf("%04d: %-30s %16lld %16lld\n",
	    s, d->scanName[s], d->start[s], d->length[s]);
    return((s == d->nscans) ? 0 : 1);
}

static int dump4(struct SDir4 *d)
{
    int	s;
    if (verb>0) printf( " with %d scans\n", d->nscans );
    for (s = 0; s < d->nscans && s < 4*MAXSCANS && d->scanName[s][0]; s++)
	printf("%05d: %-30s %16lld %16lld\n",
	    s, d->scanName[s], d->start[s], d->length[s]);
    return((s == d->nscans) ? 0 : 1);
}

static int dumpL(struct SDirL *d)
{
    int	s;
    if (verb>0) printf( " with %d scans\n", d->nscans );
    for (s = 0; s < d->nscans && s < LRGMAXSCANS && d->scanName[s][0]; s++)
	printf("%06d: %-30s %16lld %16lld\n",
	    s, d->scanName[s], d->start[s], d->length[s]);
    return((s == d->nscans) ? 0 : 1);
}

static void dump_WB_numbers(int len)
{
    int	oldLen1, oldLen2, oldLen3;
    if (verb<2) return;
    oldLen1 = (int)sizeof(struct Mark5Directory);
    oldLen2 = oldLen1 + 64 + 8*88;  /* 88 = sizeof(S_DRIVEINFO) */
    oldLen3 = oldLen1 + 64 + 16*88;
    printf ( "  WB: len%%128 = %d\n", len % 128 );
    printf ( "  WB: oldLen1 = %d\n", oldLen1 );
    printf ( "  WB: oldLen2 = %d\n", oldLen2 );
    printf ( "  WB: oldLen3 = %d\n", oldLen3 );
    printf ( "  WB: sizeof(M5Dir)  = %d\n", sizeof(struct Mark5Directory) );
}

static void dump_JB_numbers(int len, int mins[3], int maxx[3])
{
    struct Vsn *pvsn = 0;
    int len1 = sizeof(struct SDirO); /* Old value */
    int len2 = sizeof(struct SDirO) + sizeof(struct Vsn);
    int len3 = len2 - 8 * sizeof(S_DRIVEINFO); /* Old version of newer value */
    int len4 = len2 - sizeof(pvsn->vsnb); /* len2 minus char vsnb[64] */
    int len5 = len3 - sizeof(pvsn->vsnb); /* len3 minus char vsnb[64] */
    int l4n1 = sizeof(struct SDir4); /* Old value */
    int l4n2 = sizeof(struct SDir4) + sizeof(struct Vsn);
    int l4n3 = l4n2 - 8 * sizeof(S_DRIVEINFO); /* Old version of newer value */
    int l4n4 = l4n2 - sizeof(pvsn->vsnb); /* len2 minus char vsnb[64] */
    int l4n5 = l4n3 - sizeof(pvsn->vsnb); /* len3 minus char vsnb[64] */
    int len6 = sizeof(struct SDirL) + sizeof(struct Vsn);
    mins[0] = len1; maxx[0] = len2;
    mins[1] = l4n1; maxx[1] = l4n2;
    mins[2] = len6; maxx[2] = len6;
    if (verb<1) return;
    printf ( "  type O sizes:%8d..%-7d\n", mins[0], maxx[0]);
    printf ( "  type 4 sizes:%8d..%-7d\n", mins[1], maxx[1]);
    printf ( "  type L sizes:%8d..%7d\n",  mins[2], maxx[2]);
    if (verb<2) return;
    printf ( "  JB: sizeof(SDir_A) = %d\n", sizeof(struct SDir_A) );
    printf ( "  JB: sizeof(SDir_B) = %d\n", sizeof(struct SDir_B) );
    printf ( "  JB: sizeof(SDirL)  = %d\n", sizeof(struct SDirL) );
    printf ( "  JB: sizeof(SDir4)  = %d\n", sizeof(struct SDir4) );
    printf ( "  JB: sizeof(SDirO)  = %d\n", sizeof(struct SDirO) );
    printf ( "  JB: sizeof(Vsn_O)  = %d\n", sizeof(struct Vsn_O) );
    printf ( "  JB: sizeof(Vsn)    = %d\n", sizeof(struct Vsn) );
    printf ( "  JB: len1, l4n1     = %d, %d\n", len1, l4n1);
    printf ( "  JB: len2, l4n2     = %d, %d\n", len2, l4n2);
    printf ( "  JB: len3, l4n3     = %d, %d\n", len3, l4n3);
    printf ( "  JB: len4, l4n4     = %d, %d\n", len4, l4n4);
    printf ( "  JB: len5, l4n5     = %d, %d\n", len5, l4n5);
    printf ( "  JB: len6           = %d\n", len6);
}

static int dump_dir(unsigned char *dd, int len)
{
    int	er, mins[3], maxx[3];

    printf ( "Directory length is %d at %p\n", len, dd);
    dump_WB_numbers(len);
    dump_JB_numbers(len, mins, maxx);

    if (verb>3) hex_dump_dir(dd);

    if        (len >= mins[2] && len <= maxx[2]) {
	er = len - sizeof(struct SDirL);
	if (verb>0) printf ( "Treating as Large Directory (diff %d)", er );
	er = dumpL((struct SDirL*)dd);
    } else if (len >= mins[1] && len <= maxx[1]) {
	er = len - sizeof(struct SDir4);
	if (verb>0) printf ( "Treating as Medium Directory (diff %d)", er );
	er = dump4((struct SDir4*)dd);
    } else if (len >= mins[0] && len <= maxx[0]) {
	er = len - sizeof(struct SDirO);
	if (verb>0) printf ( "Treating as Small Directory (diff %d)", er );
	er = dumpO((struct SDirO*)dd);
    } else {
	er = -1;
	printf ( "Unclassified Directory size\n" );
    }

    if (verb>1 || er) printf ("dump dir error is %d\n", er);
    return(er);
}

static int get_directory_info(void)
{
    int		    len, errs = 0;
    XLR_RETURN_CODE xlrReturn;
    unsigned char   *dirData;

    len = XLRGetUserDirLength(xlrDevice);
    if (len <= 0) return(printf("No Directory (%d)\n", len));

    /* WB has:(unsigned char *)calloc(len, sizeof(int)); */
    dirData = (unsigned char *)calloc(len, sizeof(char));
    xlrReturn = XLRGetUserDir(xlrDevice, len, 0, dirData);
    if(xlrReturn != XLR_SUCCESS) return(printf("Unreadable directory\n", len));

    errs = dump_dir(dirData, len);
    free(dirData);
    return(errs);
}

static int DoBank(E_BANK bank, int force)
{
    S_BANKSTATUS    BankStatus;
    XLR_RETURN_CODE xlrReturn;
    float           Capacity;
    float           Length;

    xlrReturn = XLROpen( 1, &xlrDevice );
    if( xlrReturn != XLR_SUCCESS )
	return(printf( "XLROpen() failed.\n" ));

    xlrReturn = XLRSetBankMode ( xlrDevice, SS_BANKMODE_NORMAL );
    if( xlrReturn != XLR_SUCCESS )
	return(printf( "Bank Normal failed.\n" ));

    xlrReturn = XLRGetBankStatus ( xlrDevice, bank, &BankStatus );
    if( xlrReturn != XLR_SUCCESS )
	return(printf( "XLRGetBankStatus() failed.\n" ));

    if (force && !BankStatus.Selected) {
	xlrReturn = XLRSelectBank ( xlrDevice, bank );
	if( xlrReturn != XLR_SUCCESS )
	    return(printf( "XLRSelectBank() failed.\n" ));
	xlrReturn = XLRGetBankStatus ( xlrDevice, bank, &BankStatus );
	if( xlrReturn != XLR_SUCCESS )
	    return(printf( "XLRGetBankStatus() failed.\n" ));
	printf ( "Selection of new bank %s\n",
	    BankStatus.Selected ? "succeeded" : "failed" );
    }

    if      (bank == BANK_A) printf ( "Bank A:\n");
    else if (bank == BANK_B) printf ( "Bank B:\n");
    else                     printf ( "Bank ?:\n");

    if ( BankStatus.PowerEnabled ) printf ( "%s on\n", BankStatus.Label);
    else                           printf ( "Power is Off.\n");

    switch (BankStatus.State) {
    case STATE_NOT_READY:   printf ( "Not Ready\n");    return(0);
    case STATE_TRANSITION:  printf ( "Transition\n");	return(0);
    case STATE_READY:	    printf ( "Ready, ");	break;
    default:		    printf ( "WHAT?\n");	return(1);
    }

    if ( BankStatus.Selected ) printf ( "Is Selected.\n");
    else                       printf ( "Not selected.\n");

    Length = 1e-9 * BankStatus.Length;
    printf ( "%f GB recorded.\n", Length);
    Capacity = 4096e-9 * BankStatus.TotalCapacity;
    printf ( "Total Capacity is %f GB.\n", Capacity);

    if ( !BankStatus.Selected ) return(0);
    return(get_directory_info());
}

int GetDirectoryInfo_main( int argc, char *argv[] )
{
    int	errs = 0;
    while (argc-- > 0) {
	if (**argv == 'A') errs += DoBank(BANK_A, 0);
	if (**argv == 'B') errs += DoBank(BANK_B, 0);
	if (**argv == 'a') errs += DoBank(BANK_A, 1);
	if (**argv == 'b') errs += DoBank(BANK_B, 1);
	argv++;
	XLRClose(xlrDevice);
    }
    return(errs);
}

/*
 * eof
 */
