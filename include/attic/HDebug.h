/* @(#)$Id$ */
#ifndef HDebug_H  
#define HDebug_H


/*============================================================================*/
#include <HCommon.h>
#include <HMsg.h>


/*============================================================================*/
/** @defgroup HDebug HDebug Interface
 *   Functions and macros that simplify debugging.
 *
 *  @qsem
 *   HDebug_assert
 *   HDebug_print
 *   HDebug_DBG
 *  @qsef
 *   HDebug_assert_
 */
/** @{ */


/*============================================================================*/

/**  The assert provided by the standard library displays not only a 
 *   message of assertion failure and its location, but also the assertion
 *   itself.  The version below doesn't do this in order to avoid allocating
 *   the space for the textual version of each and every assertion.  So it
 *   assumes you have access to the source code and can look it up.  It
 *   also attempts to save space by saving the filename only once.  It does
 *   this by having the application programmer call H_FILE(filename)
 *   once at start of each source file.
 */
#ifdef DEBUG 
#define HDebug_assert(expr)                           \
   if (expr)                                          \
      NULL;                                           \
   else                                               \
      HDebug_assert_(L_funcName, U_fileName, __LINE__)
#else
#define HDebug_assert(expr)  void
#endif

/**  General method of entering debug statements in a program.
 *   When DEBUG not defined, they are removed.
 */   
#ifdef DEBUG
#define HDebug_DBG(tokenList)  tokenList
#else
#define HDebug_DBG(tokenList)  /* nothing */
#endif

/**  If DEBUG, print debugging inforamtion using \c HLog_print(),
 *   else nothing.
 */
#ifdef DEBUG
#define HDebug_print  HMsg_send
#else
#define HDebug_print  if (1) {} else (void)
#endif


/*============================================================================*/

/**  Implements local assert version. */
#ifdef DEBUG
   extern H_Void
HDebug_assert_(H_CChar* funcName, H_CChar* fileName, H_Int lineN);
#endif


/** @} */

#endif /* ! HDebug_H */
