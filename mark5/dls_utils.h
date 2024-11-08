/*
 * $Id: dls_utils.h 61 2010-11-03 21:43:55Z gbc $
 *
 * Wrapper Main for various DLS utilities for getting answers
 * to various streamstor questions in the absense of Mark5A.
 */

extern int GetBankStatus_main( int argc, char *argv[] );
extern int GetDeviceInfo_main( int argc, char *argv[] );
extern int GetDirectoryInfo_main( int argc, char *argv[] );
extern int GetDriveInfo_main( int argc, char *argv[] );
extern int GetDriveTemp_main( int argc, char *argv[] );
extern int GetSerial_main( int argc, char *argv[] );
extern int GetSMART_main( int argc, char *argv[] );

/*
 * eof
 */
