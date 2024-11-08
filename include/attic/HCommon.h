/* @(#)$Id$ */
#ifndef HCommon_H
#define HCommon_H


/*============================================================================*/
#include <HConfig.h>

/* ANSI C Headers */
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
/* #include <snprintf.h> */

/* POSIX Headers */
#include <unistd.h>

#ifdef THREADED_LIBRARY
#include <pthread.h> 
#endif


/*============================================================================*/
/** @defgroup HCommon  Common Interface 
 *   This module contains miscellaneous public declarations and
 *   definitions that are common to the Haystack General Purpose Library.
 */
/** @{ */


/*============================================================================*/

/*----------------------------------------------------------------------------*/
/** @name Defs for Native Primitive Types */
/*@{*/

typedef signed char             H_SChar;
typedef signed short            H_Short;
typedef signed int              H_Int;
typedef signed long             H_Long;
typedef const signed char       H_CSChar;
typedef const signed short      H_CShort;
typedef const signed int        H_CInt;
typedef const signed long       H_CLong;
typedef volatile signed char    H_VSChar;
typedef volatile signed int     H_VInt;
typedef volatile signed short   H_VShort;
typedef volatile signed long    H_VLong;

#define H_SCHAR_MIN             (-127 - 1)
#define H_SHORT_MIN             (-32767 - 1)
#define H_INT_MIN               (-2147483647 - 1)
#define H_LONG_MIN              (-2147483647 - 1)
#define H_SCHAR_MAX             (127)
#define H_SHORT_MAX             (32767)
#define H_INT_MAX               (2147483647)
#define H_LONG_MAX              (2147483647)

typedef unsigned char           H_UChar;
typedef unsigned short          H_UShort;
typedef unsigned int            H_UInt;
typedef unsigned long           H_ULong;
typedef const unsigned long     H_CULong;
typedef const unsigned int      H_CUInt;
typedef const unsigned short    H_CUShort;
typedef const unsigned char     H_CUChar;
typedef volatile unsigned long  H_VULong;
typedef volatile unsigned int   H_VUInt;
typedef volatile unsigned short H_VUShort;
typedef volatile unsigned char  H_VUChar;

#define H_USCHAR_MAX            (255U)
#define H_USHORT_MAX            (65535U)
#define H_UINT_MAX              (4294967295U)
#define H_ULONG_MAX             (4294967295U)

typedef float                   H_Float;
typedef double                  H_Double;

typedef char                    H_Char;
typedef volatile char           H_VChar;
typedef const char              H_CChar;
typedef void                    H_Void;
typedef const void              H_CVoid;

/*@}*/

/*----------------------------------------------------------------------------*/
/** @name Defs for Integral Types with Exactly x Bits */
/*@{*/

typedef signed char             H_Int8;
typedef signed short            H_Int16;
typedef signed int              H_Int32;
typedef const signed char       H_CInt8;
typedef const signed short      H_CInt16;
typedef const signed int        H_CInt32;
typedef volatile signed char    H_VInt8;
typedef volatile signed short   H_VInt16;
typedef volatile signed int     H_VInt32;

#define H_INT_8_MIN             (-127 - 1)
#define H_INT_16_MIN            (-32767 - 1)
#define H_INT_32_MIN            (-2147483647 - 1)
#define H_INT_8_MAX             (127)
#define H_INT_16_MAX            (32767)
#define H_INT_32_MAX            (2147483647)

typedef unsigned char           H_UInt8;
typedef unsigned short          H_UInt16;
typedef unsigned int            H_UInt32;
typedef const unsigned char     H_CUInt8;
typedef const unsigned short    H_CUInt16;
typedef const unsigned int      H_CUInt32;
typedef volatile unsigned char  H_VUInt8;
typedef volatile unsigned short H_VUInt16;
typedef volatile unsigned int   H_VUInt32;

#define H_UINT_8_MAX            (255U)
#define H_UINT_16_MAX           (65535U)
#define H_UINT_32_MAX           (4294967295U)

/*@}*/

/*----------------------------------------------------------------------------*/
/** @name Defs for Integral Types with at Least x Bits */
/*@{*/

typedef signed char             H_IntL8;
typedef signed short            H_IntL16;
typedef signed int              H_IntL32;
typedef const signed char       H_CIntL8;
typedef const signed short      H_CIntL16;
typedef const signed int        H_CIntL32;
typedef volatile signed char    H_VIntL8;
typedef volatile signed short   H_VIntL16;
typedef volatile signed int     H_VIntL32;

#define H_INT_L8_MIN            (-127 - 1)
#define H_INT_L16_MIN           (-32767 - 1)
#define H_INT_L32_MIN           (-2147483647 - 1)
#define H_INT_L8_MAX            (127)
#define H_INT_L16_MAX           (32767)
#define H_INT_L32_MAX           (2147483647)

typedef unsigned char           H_UIntL8;
typedef unsigned short          H_UIntL16;
typedef unsigned int            H_UIntL32;
typedef const unsigned char     H_CUIntL8;
typedef const unsigned short    H_CUIntL16;
typedef const unsigned int      H_CUIntL32;
typedef volatile unsigned char  H_VUIntL8;
typedef volatile unsigned short H_VUIntL16;
typedef volatile unsigned int   H_VUIntL32;

#define H_UINT_L8_MAX  (255U)
#define H_UINT_L16_MAX (65535U)
#define H_UINT_L32_MAX (4294967295U)

/*@}*/

/*----------------------------------------------------------------------------*/
/** @name Defs for Fastest Integral Types with at Least x Bits */
/*@{*/

typedef signed char             H_IntF8;
typedef signed short            H_IntF16;
typedef signed int              H_IntF32;
typedef const signed char       H_CIntF8;
typedef const signed short      H_CIntF16;
typedef const signed int        H_CIntF32;
typedef volatile signed char    H_VIntF8;
typedef volatile signed short   H_VIntF16;
typedef volatile signed int     H_VIntF32;

#define H_INT_F8_MIN            (-127 - 1)
#define H_INT_F16_MIN           (-32767 - 1)
#define H_INT_F32_MIN           (-2147483647 - 1)
#define H_INT_F8_MAX            (127)
#define H_INT_F16_MAX           (32767)
#define H_INT_F32_MAX           (2147483647)

typedef unsigned char           H_UIntF8;
typedef unsigned short          H_UIntF16;
typedef unsigned int            H_UIntF32;
typedef const unsigned char     H_CUIntF8;
typedef const unsigned short    H_CUIntF16;
typedef const unsigned int      H_CUIntF32;
typedef volatile unsigned char  H_VUIntF8;
typedef volatile unsigned short H_VUIntF16;
typedef volatile unsigned int   H_VUIntF32;

#define H_UINT_F8_MAX  (255U)
#define H_UINT_F16_MAX (65535U)
#define H_UINT_F32_MAX (4294967295U)

/*@}*/

/*----------------------------------------------------------------------------*/
/** @name Misc. Types */
/*@{*/

typedef signed long    H_IntMax;
typedef unsigned long  H_UIntMax;
typedef signed long    H_IntPtr;
typedef unsigned long  H_UIntPtr;

/*@}*/

/*----------------------------------------------------------------------------*/
/** @name Defs for a Boolean Type */
/*@{*/

/**  The \c H_Bool type represents a boolean value, one that can only take on
 *   the values 0 and 1.  A set of pairs of defs is provided for common
 *   boolean cases.
 */ 
typedef H_Int  H_Bool;

#define H_FAILURE  0
#define H_SUCCESS  1
#define H_NO       0
#define H_YES      1
#define H_INVALID  0
#define H_VALID    1
#define H_FALSE    0
#define H_TRUE     1

/*@}*/

/*----------------------------------------------------------------------------*/
/** @name Defs for a Status Type */
/*@{*/

/**  The \c H_Status type should be a signed integral value returned by
 *   functions that can incur an error during processing.  It has
 *   value 0 to indicate success and -1 to indicate an unspecified error.
 *   Specific errors are indicated by positive values.  The macros \c H_OK
 *   and \c H_NOT_OK are provided for the successful and unspecified
 *   error cases respectively.
 */
typedef H_Int  H_Status;

#define H_OK  0
#define H_NOT_OK  -1

/*@}*/

/*----------------------------------------------------------------------------*/
/** @name Macros Assisting with String Comparisons */
/*@{*/

/**  Checks if the 1st string is equal to the 2nd string. */
#define H_IS_STR_EQ(s1, s2)  (strcmp(s, t) == 0)

/**  Checks if the 1st string is greater than the 2nd string. */
#define H_IS_STR_GT(s1, s2)  (strcmp(s, t) > 0)

/**  Checks if the 1st string is less than the 2nd string. */
#define H_IS_STR_LT(s1, s2)  (strcmp(s, t) < 0)

/*@}*/

/*----------------------------------------------------------------------------*/
/** @name Macros Assisting with Stringizing and Token Pasting */
/*@{*/

/**  Token pastes its unexpanded arguments. */
#define H_GLUE(a, b)  a ## b

/**  Token pastes its expanded arguments. */
#define H_XGLUE(a, b)  G_GLUE(a, b)

/**  Stringizes its unexpanded arguments. */ 
#define H_STR(s)  #s

/**  Stringizes its expanded arguments. */ 
#define H_XSTR(s)  G_STR(s) 

/*@}*/

/*----------------------------------------------------------------------------*/
/** @name Macros Assisting with Logging */
/*@{*/

/**  Expands to a static variable definition of \c U_fileName.
 *   This variable is relied upon by the \c HLog_print() argument macros.
 *   Use this macro once at file scope in all files that use
 *   \c HLog_print().
 */
#define H_FILE(name)  static H_CChar* U_fileName = H_STR(name)

/**  Expands to a static variable definition of \c L_funcName.
 *   This variable is relied upon by the \c HLog_print() argument macros.
 *   Use this macro once at function scope in all functions that use
 *   \c Hlog_print().
 */
#define H_FUNC(name)  static H_CChar* L_funcName = H_STR(name)

/**  This type represents an event type.  
 */
typedef enum H_EvType {
   H_EVTYPE_FERR,     /**< Fatal error  */
   H_EVTYPE_SERR,     /**< System error */
   H_EVTYPE_ERR,      /**< Error        */
   H_EVTYPE_WRN,      /**< Warning      */
   H_EVTYPE_INFO,     /**< Information  */
   H_EVTYPE_DBG,      /**< Debug        */
   H_EVTYPE_AST,      /**< Assertion    */
   H_EVTYPE_RAW,      /**< Raw          */
} H_EvType;

/**  These convenience macros can be used in calls 
 *   to reduce the number of arguments specified by the user.  They
 *   expand to the associated event record type, preceded by
 *   3 location arguments.
 */
#define H_TF  L_funcName, U_fileName, __LINE__, H_EVTYPE_FERR  
#define H_TS  L_funcName, U_fileName, __LINE__, H_EVTYPE_SERR  
#define H_TE  L_funcName, U_fileName, __LINE__, H_EVTYPE_ERR
#define H_TW  L_funcName, U_fileName, __LINE__, H_EVTYPE_WRN
#define H_TI  L_funcName, U_fileName, __LINE__, H_EVTYPE_INFO
#define H_TD  L_funcName, U_fileName, __LINE__, H_EVTYPE_DBG 
#define H_TA  L_funcName, U_fileName, __LINE__, H_EVTYPE_AST
#define H_TR  L_funcName, U_fileName, __LINE__, H_EVTYPE_RAW 

/*@}*/

/*----------------------------------------------------------------------------*/
/** @name Miscellaneous Macros */
/*@{*/

#define H_SYS_ERR_HDR_LEN_MAX 128

/**  Swaps the values of its first two args, using third as temp. */
#define H_SWAP(a, b, t)  ((t) = (a), (a) = (b), (b) = (t))

/**  Returns true or false regarding if n within specified range. */
#define H_IS_IN_RANGE(n, lo, hi)  ((lo) <= (n) && (n) <= (hi))

/**  Returns the dimension of its array argument. */
#define H_DIM(a)  (sizeof(a) / sizeof(a[0]))

/*@}*/


/*============================================================================*/

/** */
extern H_Char*
H_pkg;



/** @} */

#endif /* ! HCommon_H */
