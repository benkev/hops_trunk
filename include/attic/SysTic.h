#ifndef SysTic_INCLUDED
#define SysTic_INCLUDED

struct SysTic_UserMesg {
    int syncCount;   /* Out: Returns the number of systics since boot. */
    int timeoutLen;  /* In : User specified timeout in 1/100 secs.     */
    int flags;       /* Out: Set if call timed out.                    */  
};
typedef struct SysTic_UserMesg SysTic_UserMesg;

/* Ioctl commands */
#define SysTic_WAIT_ON_NEXT  1
#define SysTic_TIME_OUT      1

#endif 
