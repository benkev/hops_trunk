/*
 * $Id: GetDeviceInfo.cpp 58 2010-11-03 13:01:26Z gbc $

This is a program to retrieve information about the StreamStor system.
Revised by Dan Smythe at Haystack Observatory 2010 May 14.

*/

#include <stdio.h>
#include <stdlib.h>
#include "xlrapi.h"

#define  BUF_SIZE    0x8000
static SSHANDLE   xlrDevice;

int GetDeviceInfo_main( int argc, char *argv[] )
{
   S_DEVINFO         devInfo;
   XLR_RETURN_CODE   xlrReturn;

   xlrReturn = XLROpen( 1, &xlrDevice );
   if( xlrReturn != XLR_SUCCESS )
      {
      printf( "XLROpen() failed.\n" );
      return(1);
      }

   xlrReturn = XLRGetDeviceInfo( xlrDevice, &devInfo );
   if( xlrReturn != XLR_SUCCESS )
      {
      printf("XLRGetDeviceInfo failed.\n");
      char temp[XLR_ERROR_LENGTH];
      XLRGetErrorMessage( temp, XLRGetLastError() );
      printf( "ERROR: %s\n", temp );
      return(1);
      }

   float Capacity;
// printf("StreamStor board type is %s\n", devInfo.BoardType);
   printf("%s serial number is %d\n", devInfo.BoardType, devInfo.SerialNum);
   printf("Number of drives is %d\n", devInfo.NumDrives);
   printf("Number of ATA buses is %d\n", devInfo.NumBuses);
// printf("Total Capacity is %d *4096 bytes\n", devInfo.TotalCapacity);
   Capacity = (4096/1e9) * devInfo.TotalCapacity ;
   printf("Total Capacity is %f GB\n", Capacity);
   
   return(0);
}
