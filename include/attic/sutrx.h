/* sutrx.h */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <limits.h>

#define MAX_MSG_SIZE                  16384
#define ERROR_SOCKET                  -1
#define COMMAND_CODE_OFFSET           88
#define COMMAND_CODE_RESPONSE_OFFSET  64
#define RW_OFFSET                     136
#define SU_DPU_COMMAND                0x03020002
#define DPUC_READY                    0
#define DPUC_BUSY                     1
#define DPU_READ_TIMEOUT              5


/*----------------------------------------*/
typedef struct
    {
    char                *cHost;
    int                 wTcpSock;
    struct sockaddr_in  myaddr_in;
    struct sockaddr_in  peeraddr_in;
    struct hostent      *hp;
    struct servent      *sp;
    struct linger       stTimeOut;
    } SU_Params, *pSU_Params;


typedef unsigned char  BYTE;
typedef unsigned short UWORD;
/*typedef unsigned short U16; */
// following line changed by rjc 2007.2.12 to facilitate
// move to 64 bit architecture
// typedef unsigned long  ULONG;
typedef unsigned int  ULONG;
