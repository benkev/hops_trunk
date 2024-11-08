/*
 * $Id: xlr_stubs.cpp 62 2010-11-04 16:43:18Z gbc $
 *
 * For compilation only, triggered by -DSTREAMSTOR_STUBS=1 in pc file.
 */

#ifndef STREAMSTOR_STUBS
#define STREAMSTOR_STUBS 0
#endif /* STREAMSTOR_STUBS */

#if STREAMSTOR_STUBS

#include <stdio.h>
#include "xlrapi.h"

static int stub_error(char *routine)
{
    return(fprintf(stderr, "Stub routine %s called\n", routine));
}

XLREXPORT void XLRCALLTYPE
XLRClose ( IN SSHANDLE xlrHandle )
{
    (void)(stub_error("XLRClose"));
}

XLREXPORT XLR_RETURN_CODE XLRCALLTYPE
XLRGetBankStatus( IN SSHANDLE device, IN UINT bankID, 
      OUT PS_BANKSTATUS bankStatus )
{
    return(stub_error("XLRGetBankStatus"));
}

XLREXPORT XLR_RETURN_CODE XLRCALLTYPE
XLRGetDeviceInfo( IN SSHANDLE xlrHandle, OUT PS_DEVINFO devInfo )
{
    return(stub_error("XLRGetDeviceInfo"));
}

XLR_RETURN_CODE XLRCALLTYPE
XLRGetDirectory (IN SSHANDLE xlrHandle, OUT PS_DIR Dir )
{
    return(stub_error("XLRGetDirectory"));
}

XLREXPORT XLR_RETURN_CODE XLRCALLTYPE
XLRGetDriveInfo( IN SSHANDLE device, IN UINT Bus, IN UINT MasterSlave,
                 OUT PS_DRIVEINFO driveInfo )
{
    return(stub_error("XLRGetDriveInfo"));
}

XLREXPORT XLR_RETURN_CODE XLRCALLTYPE
XLRGetDriveTemp( IN SSHANDLE device, IN UCHAR bus,
		 IN UCHAR dev, OUT PUINT temp )
{
    return(stub_error("XLRGetDriveTemp"));
}

XLREXPORT XLR_RETURN_CODE XLRCALLTYPE
XLRGetErrorMessage( char *errstr, XLR_ERROR_CODE error )
{
    return(stub_error("XLRGetErrorMessage"));
}

XLREXPORT XLR_RETURN_CODE XLRCALLTYPE
XLRGetLabel( IN SSHANDLE device, OUT char *label )
{
    return(stub_error("XLRGetLabel"));
}

XLREXPORT XLR_ERROR_CODE XLRCALLTYPE
XLRGetLastError( void )
{
    return(stub_error("XLRGetLastError"));
}

XLREXPORT XLR_RETURN_CODE XLRCALLTYPE
XLRGetUserDir( IN SSHANDLE xlrDevice, IN UINT XferLength, 
   IN UINT Offset, OUT PVOID udirPtr )
{
    return(stub_error("XLRGetUserDir"));
}

XLREXPORT UINT XLRCALLTYPE 
XLRGetUserDirLength( IN SSHANDLE xlrDevice )
{
    return(stub_error("XLRGetUserDirLength"));
}

XLREXPORT XLR_RETURN_CODE XLRCALLTYPE
XLROpen( IN UINT devIndex, OUT PSSHANDLE newHandle )
{
    return(stub_error("XLROpen"));
}

XLREXPORT XLR_RETURN_CODE XLRCALLTYPE
XLRSelectBank( IN SSHANDLE device, IN UINT bankID )
{
    return(stub_error("XLRSelectBank"));
}

XLREXPORT XLR_RETURN_CODE XLRCALLTYPE 
XLRSetBankMode( IN SSHANDLE device, IN S_BANKMODE mode )
{
    return(stub_error("XLRSetBankMode"));
}

#endif /* STREAMSTOR_STUBS */

/*
 * eof
 */
