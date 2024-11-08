/*
 * $Id: GetDirInfo.cpp 61 2010-11-03 21:43:55Z gbc $

This program retrieves bank status information from the StreamStor.
Revised by Dan Smythe at Haystack Observatory on 2010 May 14.

*/

#include <stdio.h>
#include <stdlib.h>
#include "xlrapi.h"

#define  BUF_SIZE    0x8000
static SSHANDLE   xlrDevice;

int GetBankStatus_main( int argc, char *argv[] )
{
   S_BANKSTATUS    AbankStatus;
   S_BANKSTATUS    BbankStatus;
   XLR_RETURN_CODE xlrReturn;
   float           Capacity;
   float           Length;

   xlrReturn = XLROpen( 1, &xlrDevice );
   if( xlrReturn != XLR_SUCCESS )
      {
      printf( "XLROpen() failed.\n" );
      return(1);
      }

   xlrReturn = XLRSetBankMode ( xlrDevice, SS_BANKMODE_NORMAL );
   if( xlrReturn != XLR_SUCCESS )
      return(1);

   xlrReturn = XLRGetBankStatus ( xlrDevice, BANK_A, &AbankStatus );
   if( xlrReturn != XLR_SUCCESS )
      return(1);
   printf ( "Bank A:\n");
   if ( AbankStatus.PowerEnabled )
      {
      printf ( "%s\n", AbankStatus.Label);
      printf ( "Power is On, ");
      }
   else
      printf ( "Power is Off.\n");
   if ( AbankStatus.State == STATE_READY )
      {
      printf ( "Ready, ");
      if ( AbankStatus.Selected )
         printf ( "Selected.\n");
      else
         printf ( "Not selected.\n");
      Length = 1e-9 * AbankStatus.Length;
      printf ( "%f GB recorded.\n", Length);
      Capacity = 4096e-9 * AbankStatus.TotalCapacity;
      printf ( "Total Capacity is %f GB.\n", Capacity);
      }

   xlrReturn = XLRGetBankStatus ( xlrDevice, BANK_B, &BbankStatus );
   if( xlrReturn != XLR_SUCCESS )
      return(1);
   printf ( "Bank B:\n");
   if ( BbankStatus.PowerEnabled )
      {
      printf ( "%s\n", BbankStatus.Label);
      printf ( "Power is On, ");
      }
   else
      {
      printf ( "Power is Off.\n");
      return(0);
      }
   if ( BbankStatus.State == STATE_READY )
      {
         printf ( "Ready, ");
      if ( BbankStatus.Selected )
         printf ( "Selected.\n");
      else
         printf ( "Not selected.\n");
      Length = 1e-9 * BbankStatus.Length;
      printf ( "%f GB recorded.\n", Length);
      Capacity = 4096e-9 * BbankStatus.TotalCapacity;
      printf ( "Total Capacity is %f GB.\n", Capacity);
      }
   return(0);
}
