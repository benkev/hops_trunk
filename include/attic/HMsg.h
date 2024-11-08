/* @(#)$Id$ */
#ifndef HMsg_H
#define HMsg_H


/*============================================================================*/
#include <HCommon.h>


/*============================================================================*/
/** @defgroup HMsg  HMsg Interface
 *   This interface provides access to a message handler.
 *   The messaging function, \c HMsg_snd(), is called by \b HLib
 *   functions when they need to report operational information
 *   (errors, warning, messages, debug, etc. ).  If the requested
 *   level is greater than the message module's reporting threshold,
 *   then the message handler is called.  The message handler prints a short
 *   message on stderr.  You can dynamically set and get the reporting
 *   threshold via \c HMsg_setLvl() and \c HMsg_getLvl(), respectively.
 *   You may find it convenient to adopt the same logging conventions
 *   in your program as in \b HLib.  To log something you need to call
 *   \c HMsg_snd().
 *
 *  @qsef
 *   HMsg_snd
 *   HMsg_getLvl
 *   HMsg_setLvl
 *   HMsg_setProgName
 */
/** @{ */


/*============================================================================*/

/**  This function generates a message record if the requested level \p lvl
 *   is greater than or equal to the current messaging threshold.
 *   The message record consists of a header and a body.  The header is 
 *   determined by a builtin format selected by the type \p type.
 *   The builtin format uses the passed location information 
 *   (\p funcName, \p fileName, \p lineN).  The body is determined by
 *   the format \fmt and any optional args.  The record is written
 *   and flushed on stderr.
 */
   extern H_Void
HMsg_snd(H_CChar* funcName, H_CChar* fileName, H_Int lineN, H_EvType type,
         H_Int lvl, H_CChar* fmt, ...);

/**  This function returns the current messaging threshold. */
   extern H_Int
HMsg_getLvl(H_Void);

/**  This function sets the current messaging threshold level to \p lvl and
 *   returns the previous threshold.
 */
   extern H_Int
HMsg_setLvl(H_Int lvl);

/**  This function sets the program messaging name to \p name. */
   extern H_Void
HMsg_setProgName(H_Char* name);


/** @} */

#endif /* ! HMsg_H */
