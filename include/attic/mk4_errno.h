/*******************************************************************************
*                                                                              *
* Mark 4 error number file  --  for use with Error_Message messages,           *
*                               and the emsg() routine.                        *
*                                                                              *
*******************************************************************************/

                                        /* bits 16-31 are reserved to denote
                                           target programs. emsg() will strip
                                           this field off before inserting into
                                           Error_Message's errnum (which is U16) */
#define OPERA     0x010000
#define CONDUCTOR 0x020000
#define SUMAN     0x040000
#define CORRMAN   0x080000
#define GENAROOT  0x100000
#define BOS       0x200000


                                        /* bits 00-15 are reserved for error #'s,
                                           allocated in blocks to each program */

                                        /* decimal 0 - 999 reserved for global msg's */
#define BUILDING_ON_FIRE          001   /* example - replace when real code available */

                                        /* decimal 1000 - 1999 reserved for opera's use */

                                        /* decimal 2000 - 2999 reserved for conductor's use */
#define ERROR_CONFIG_CORR        2000
#define ERROR_CONFIG_SU          2001

                                        /* decimal 3000 - 3999 reserved for suman's use */
#define ERROR_SU_INIT            3000   /* failure to initialize SU -- no response */

                                        /* decimal 4000 - 4999 reserved for corrman's use */

                                        /* decimal 5000 - 5999 reserved for genaroot's use */

                                        /* decimal 6000 - 9999 reserved for miscellaneous
                                         *                     small programs */
#define ERROR_SUTRX_SOCKET       6000

#define ERROR_MCB_BAD_IVEX       7000
#define ERROR_MCB_INITIALIZATION 7001
