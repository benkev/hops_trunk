/* @(#)$Id$ */
#ifndef HErr_H
#define HErr_H


/*============================================================================*/
#include <HCommon.h>


/*============================================================================*/
/** @defgroup HErr  HErr Interface
 *  @qsem
 *   HErr_E_...
 *  @qsef
 *   HErr_set
 *   HErr_get
 */
/** @{ */


/*============================================================================*/

#define HErr_E_NOT_IMPL   -99   /* function or feature not implemented      */
#define HErr_E_FAILED      -1   /* general failure                          */
#define HErr_E_OK           0   /* success (no error)                       */
#define HErr_E_INVAL        1   /* invalid argument                         */
#define HErr_E_SOCKET       2   /* socket() error                           */
#define HErr_E_BAD_STATE    3   /* call not valid in this state             */
#define HErr_E_ADDR_INFO    4   /* unable to resolve network address        */
#define HErr_E_BIND         5   /* bind() error                             */
#define HErr_E_BLOCKED      6   /* operation would block                    */
#define HErr_E_ACCEPT       7   /* accept() error                           */
#define HErr_E_BAD_TYPE     8   /* wrong socket type for operation          */
#define HErr_E_NO_MEM       9
#define HErr_E_LISTEN      11   /* listen() error                           */
#define HErr_E_SOCK_INFO   12   /* unable to obtain socket info             */
#define HErr_E_FCNTL       13   /* fcntl() error                            */
#define HErr_E_LOST_CONN   14   /* connection to peer lost                  */
#define HErr_E_FDOPEN      15   /* fdopen() error                           */
#define HErr_E_CONNECT     16   /* connect() error                          */
#define HErr_E_NO_CONN     17   /* no connection available                  */
#define HErr_E_SEND        18   /* send() error                             */
#define HErr_E_RECV        19   /* recv() error                             */
#define HErr_E_MSG_2_BIG   20   /* UDP message too big                      */
#define HErr_E_SENDTO      21   /* sendto() error                           */
#define HErr_E_RECVFROM    22   /* recvfrom() error                         */
#define HErr_E_TIMEOUT     23   /* connection or I/O timed out              */
#define HErr_E_SVC_INFO    24   /* unable to obtain service info            */
#define HErr_E_FORK        25   /* unable to fork                           */
#define HErr_E_TBL_FULL    26   /* table full (depends on context)          */
#define HErr_E_SELECT      27   /* select() error                           */
#define HErr_E_IOCTL       28   /* ioctl() error                            */
#define HErr_E_TCATTR      29   /* unable to get/set tty attributes         */
#define HErr_E_NO_TTY      30   /* descriptor does not refer to a tty       */
#define HErr_E_GETPTY      31   /* getpty() error                           */
#define HErr_E_OPEN        32   /* open() error                             */
#define HErr_E_PTY         33   /* general pseudoterminal error             */
#define HErr_E_EXECV       34   /* execv() error                            */


/*============================================================================*/

   extern H_Void
HErr_set(H_Char* pkg, H_Int errN);

   extern H_Int
HErr_get(H_Void);


/** @} */

#endif /* ! HErr_H */
