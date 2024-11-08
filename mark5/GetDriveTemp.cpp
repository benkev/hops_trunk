/*
 * $Id: GetDriveTemp.cpp 106 2011-01-20 17:39:56Z gbc $

   This program retrieves the temperature reading from the StreamStor drives.

*/

#include <stdio.h>
#include <stdlib.h>
#include "xlrapi.h"
#include "dls_compat.h"

#define  BUF_SIZE    0x8000
static SSHANDLE   xlrDevice;

#if HAVE_XLRGetDriveTemp
int GetDriveTemp_main( int argc, char *argv[] )
{
   S_DEVINFO       devInfo;
   UINT            temperature;
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

   for ( bus = 0; bus < devInfo.NumBuses; bus++ )
      {
      for ( slave = 0; slave < devInfo.NumDrives/devInfo.NumBuses; slave++ )
         {

         printf( "Drive %d ", 2 * bus + slave );
   
         xlrReturn = XLRGetDriveTemp( xlrDevice, bus, slave, &temperature );
         if( xlrReturn == XLR_SUCCESS )
            printf( "temperature is %d degrees C\n", temperature );
	 }
      }
   return(0);
}
#else /* HAVE_XLRGetDriveTemp */
int GetDriveTemp_main( int argc, char *argv[] )
{
    printf( "Temperature not knowable on this system.\n" );
    return(0);
}
#endif /* HAVE_XLRGetDriveTemp */
