/*
 * $Id: GetSMART.cpp 77 2010-12-02 14:50:41Z gbc $

   This program retrieves info from the StreamStor drives about their SMART
State.

*/

#include <stdio.h>
#include <stdlib.h>
#include "xlrapi.h"
#include "dls_compat.h"

#define  BUF_SIZE    0x8000
static SSHANDLE   xlrDevice;

int GetSMART_main( int argc, char *argv[] )
{
   S_DEVINFO       devInfo;
   S_DRIVEINFO     drvInfo;
   UINT            bus;
   UINT            slave;
   XLR_RETURN_CODE xlrReturn;

   xlrReturn = XLROpen( 1, &xlrDevice );
   if( xlrReturn != XLR_SUCCESS )
      {
      printf( "XLROpen() failed.\n" );
      return(1);
      }

   char            label[XLR_LABEL_LENGTH];
   xlrReturn = XLRGetLabel( xlrDevice, label );
   if( xlrReturn != XLR_SUCCESS )
      {
      printf( "XLRGetLabel() failed.\n" );
      return(1);
      }
   printf ( "%s\n", label );

   xlrReturn = XLRGetDeviceInfo( xlrDevice, &devInfo );
   if( xlrReturn != XLR_SUCCESS )
      return(1);

   for ( bus = 0; bus <= devInfo.NumBuses; bus++ )
      {
      for ( slave = 0; slave < devInfo.NumDrives/devInfo.NumBuses; slave++ )
         {
         xlrReturn = XLRGetDriveInfo( xlrDevice, bus, slave, &drvInfo );
         if( xlrReturn != XLR_SUCCESS )
            return(1);

         printf( "Drive %d ", 2 * bus + slave );
         if ( drvInfo.SMARTCapable )
	    {
            if ( drvInfo.SMARTState )
               printf( "SMART State is OK.\n" );
            else
               printf( "SMART State failed.\n" );
	    }
	 }
      }
   return(0);
}
