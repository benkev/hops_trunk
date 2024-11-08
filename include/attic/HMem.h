/* @(#)$Id$ */
#ifndef HMem_H
#define HMem_H


/*============================================================================*/
#include <HCommon.h>


/*============================================================================*/
/** @defgroup HMem  HMem Interface
 *   Functions and macros that simplify memory management in C.
 *   These routines provide more convenient interfaces to the standard
 *   malloc() family of library functions.
 * 
 *  @qsem
 *   HMem_new
 *   HMem_newa
 *   HMem_zero
 *   HMem_zeroa
 *  @qsef
 *   HMem_malloc
 *   HMem_realloc
 *   HMem_free
 */
/** @{ */


/*============================================================================*/

/*----------------------------------------------------------------------------*/
/** @name Macros to Zero Memory */
/*@{*/

/**  These macros zero the specified memory.  \c HMem_zero() 
 *   zeroes the element of type \p type at location \p p. 
 *   \c HMem_zeroa() zeroes the \p n element array of type \p type
 *   at location \p p.
 */
#define HMem_zero(p, type)      memset((H_Void*)p, 0, sizeof(type))
#define HMem_zeroa(p, n, type)  memset((H_Void*)p, 0, (n) * sizeof(type))

/*@}*/

/*----------------------------------------------------------------------------*/
/** @name Macros to Allocate Memory */
/*@{*/

/**  These are additional convenience macros for allocating memory.
 *   \c HMem_new() allocates space for one element of the specified
 *   type.  \c HMem_newa() allocates space for an arrayof \p n elements
 *   of the specified type.  The variable \p n is assumed
 *   to be of type \c size_t.  These macros all expand to 
 *   expressions involving \c HMem_malloc().
 */
#define HMem_new(type)      HMem_malloc(sizeof(type))
#define HMem_newa(n, type)  HMem_malloc(n * sizeof(type))

/*@}*/


/*============================================================================*/

/**  This function is wrapper for \c malloc().  */
   extern H_Void*
HMem_malloc(size_t sz);

/**  This function is wrapper for \c calloc().  */
   H_Void*
HMem_calloc(size_t nElem, size_t sz);

/**  This function is wrapper for \c realloc().  */
   extern H_Void*
HMem_realloc(H_Void* mem, size_t szNew);

/**  This function is wrapper for \c free(). */
   extern H_Void
HMem_free(H_Void* memv);

/**  This function is wrapper for \c free(). */
   extern H_Void
HMem_freeVec(H_Char** vec);


/** @} HMem */

#endif /* ! HMem_H */
