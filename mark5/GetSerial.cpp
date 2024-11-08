/*
 * $Id: GetSerial.cpp 77 2010-12-02 14:50:41Z gbc $

   This program retrieves model and serial numbers of the StreamStor drives.

*/

#include <stdio.h>
#include <stdlib.h>
#include "xlrapi.h"
#include "dls_compat.h"

#define  BUF_SIZE    0x8000
static SSHANDLE   xlrDevice;

int GetSerial_main( int argc, char *argv[] )
{
   S_DEVINFO       devInfo;
   S_DRIVEINFO     drvInfo;
   UINT            bus;
   UINT            slave;
   char            label[XLR_LABEL_LENGTH];
   XLR_RETURN_CODE xlrReturn;

   xlrReturn = XLROpen( 1, &xlrDevice );
   if( xlrReturn != XLR_SUCCESS )
      {
      printf( "XLROpen() failed.\n" );
      return(1);
      }

   xlrReturn = XLRGetLabel( xlrDevice, label );
   if( xlrReturn != XLR_SUCCESS )
      {
      printf( "XLRGetLabel() failed.\n" );
      return(1);
      }
   printf ( "%s\n", label );

   xlrReturn = XLRGetDeviceInfo( xlrDevice, &devInfo );
   if( xlrReturn != XLR_SUCCESS )
      {
      printf( "XLRGetDeviceInfo() failed.\n" );
      return(1);
      }

   for ( bus = 0; bus <= devInfo.NumBuses; bus++ )
      {
      for ( slave = 0; slave < devInfo.NumDrives/devInfo.NumBuses; slave++ )
         {
         xlrReturn = XLRGetDriveInfo( xlrDevice, bus, slave, &drvInfo );
         if( xlrReturn != XLR_SUCCESS )
            return(1);

         printf( "Drive %d ", 2 * bus + slave );
         printf( "%s ", drvInfo.Model );
         printf( "%s\n", drvInfo.Serial );
   
	 }
      }
   return(0);
}
