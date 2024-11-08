/*
 * $Id: dls_compat.h 106 2011-01-20 17:39:56Z gbc $
 *
 * SDK8/9 compatibility issues
 */

#ifndef dls_compat_h
#define dls_compat_h

#include "../hops_config.h"

#ifdef XLR_MAX_IP_ADDR
#define SDKVERSION 9
#define UINT UINT32
#else /* XLR_MAX_IP_ADDR */
#define SDKVERSION 8
#endif /* XLR_MAX_IP_ADDR */

#endif /* dls_compat_h */

/*
 * eof
 */
