//  message_fields.h defines a structure, field_tab, which
//  contains the detailed field layout of all JK messages
//
//  NOTE! this file must be modified whenever the definitions
//        in message_structs are changed
//
//                                 rjc  2005.10.31

#include "mk4_typedefs.h"
#include "mk4_sizes.h"
#include "message_codes.h"
                                    // define tokens that represent each of the
                                    // variable (i.e. field) types
#define CHAR_    },{1,
#define U8_      },{2,
#define SHORT_   },{3,
#define U16_     },{4,
#define INT_     },{5,
#define U32_     },{6,
#define DOUBLE_  },{7,
#define LLINT_   },{8,
#define NAME_    },{9,
#define STRUCT8_ },{10,               // start of structure repeat/alignment blocks
#define STRUCT4_ },{11,
#define STRUCT2_ },{12,
#define STRUCT1_ },{13,
#define STREND_  },{14,0

                                    // initialize structure that contains field
                                    // information about all messages
struct ftabtag
    {
    U16 type;
    U16 num;
    }
field_tab[] = {
{     0, 0

/*******************************************************************************
*                                                                              *
*                 t o / f r o m    g e n a r o o t                             *
*                                                                              *
*******************************************************************************/


NAME_   GENERATE_ROOT
U32_    2                           // header
CHAR_   MAX_LEN_KEY
CHAR_   MAX_LEN_KEY
CHAR_   MAX_LEN_KEY
CHAR_   MAX_LEN_KEY
CHAR_   MAX_NUM_STN
U16_    2

NAME_   ROOT_COMPLETE
U32_    2                           // header
CHAR_   MAX_LEN_FILNAM

NAME_   ROOT_FAILURE
U32_    2                           // header
U16_    1

/*******************************************************************************
*                                                                              *
*                 t o / f r o m    c o r r m a n                               *
*                                                                              *
*******************************************************************************/

NAME_   INITIALIZE_CORRELATOR
U32_    2                           // header
CHAR_   MAX_LEN_KEY
U8_     MAX_NUM_SU

NAME_   CORRELATOR_INITIALIZED
U32_    2                           // header
U8_     4*MAX_NUM_SU

NAME_   CONFIGURE_CORRELATOR
U32_    2                           // header
CHAR_   MAX_LEN_FILNAM
CHAR_   MAX_NUM_STN
DOUBLE_ 1
U8_     2
                                    // SU table
STRUCT8_ 1
    U8_     1
    STRUCT8_ MAX_NUM_SU
        U32_     1
        U16_     3
        U8_      3
        CHAR_    22
        SHORT_   1
        U16_     2
        SHORT_   2
        U16_     4
        DOUBLE_  6
        STRUCT2_ 16
           CHAR_    9
           U16_     1
           CHAR_    1
       STREND_
    STREND_
STREND_

NAME_   CU_CONFIGURATION
U32_    2                           // header
INT_    2

NAME_   CU_RESOURCE_ERROR
U32_    2                           // header
U16_    1

NAME_   CORRELATOR_ABORT
U32_    2                           // header
U16_    1
CHAR_   1

NAME_   CORRTASK_COMPLETE
U32_    2                           // header
U16_    2

NAME_   REALTIME_DISPLAY
U32_    2                           // header
CHAR_   2
CHAR_   8
U8_     1

NAME_   REALTIME_DATA
U32_    2                           // header
CHAR_   8
                                    // type_120 struct
CHAR_   6
SHORT_  1
CHAR_   8
INT_    6
INT_    128                         // NOTE: only correct for 32 lag case
                                    // need to support variable # of lags
                                    // in order to be robust!!!

NAME_   CORRELATOR_STATS
U32_    2                           // header
U8_     3
U8_     MAX_CHAN_RT
STRUCT1_ MAX_CHAN_RT
  U8_   12
STREND_
STRUCT1_ MAX_CHAN_RT
  U8_   4
STREND_

/*******************************************************************************
*                                                                              *
*                 t o / f r o m    s u m a n                                   *
*                                                                              *
*******************************************************************************/

NAME_   INITIALIZE_SUS
U32_    2                           // header
CHAR_   MAX_LEN_KEY
U8_     MAX_NUM_SU

NAME_   SUS_INITIALIZED
U32_    2                           // header
U16_    MAX_NUM_SU

NAME_   CONFIGURE_SUS
U32_    2                           // header
CHAR_   MAX_LEN_FILNAM
U8_     1
U16_    1
U8_     1

NAME_   SUS_CONFIGURED
U32_    2                           // header
                                    // SU table
STRUCT8_ 1
    U8_     1
    STRUCT8_ MAX_NUM_SU
        U32_     1
        U16_     3
        U8_      3
        CHAR_    22
        SHORT_   1
        U16_     2
        SHORT_   2
        U16_     4
        DOUBLE_  6
        STRUCT2_ 16
           CHAR_    9
           U16_     1
           CHAR_    1
        STREND_
    STREND_
STREND_

NAME_   SU_CONFIG_ERROR
U32_    2                           // header
U16_    1

NAME_   CANT_MOUNT_TAPE
U32_    2                           // header
U16_    1
CHAR_   1

NAME_   ALL_MOUNTED
U32_    2                           // header

NAME_   POS_TO_TOT
U32_    2                           // header

NAME_   REPOS_TO_TOT
U32_    2                           // header
U8_     2

NAME_   VALID_TIME
U32_    2                           // header
DOUBLE_ 1
U8_     1

NAME_   CANT_FIND_TIME
U32_    2                           // header
DOUBLE_ 1
U16_    1
CHAR_   1
U8_     1

NAME_   SYNCHRONIZE_TAPES
U32_    2                           // header
DOUBLE_ MAX_NUM_SU

NAME_   SEND_TAPE_INFO
U32_    2                           // header
U16_    1

NAME_   TAPE_INFO
U32_    2                           // header
                                    // SU table
STRUCT8_ 1
    U8_     1
    STRUCT8_ MAX_NUM_SU
        U32_     1
        U16_     3
        U8_      3
        CHAR_    22
        SHORT_   1
        U16_     2
        SHORT_   2
        U16_     4
        DOUBLE_  6
        STRUCT2_ 16
            CHAR_    9
            U16_     1
            CHAR_    1
        STREND_
    STREND_
STREND_

NAME_   END_TASK
U32_    2                           // header

NAME_   TRANSMIT_FFS
U32_    2                           // header



/*******************************************************************************
*                                                                              *
*                 t o / f r o m    d o m i n o                                 *
*                                                                              *
*******************************************************************************/

NAME_   INITIALIZE_MK5
U32_    2                           // header
U32_    1
U8_     4
U16_    1

NAME_   MOUNT_CHANGE  
U32_    2                           // header
U8_     2*8
U8_     2*8*20
U8_     1

NAME_   SEND_M5_STATUS
U32_    2                           // header
U16_    1

NAME_   M5_STATUS
U32_    2                           // header
U32_    2
U8_     1

NAME_   LOOKUP_SCAN       
U32_    2                           // header
CHAR_   51
U8_     1

NAME_   SCAN_BOUNDS       
U32_    2                           // header
LLINT_  2
U8_     1

NAME_   FIND_NEXT_HEADER
U32_    2                           // header
LLINT_  1

NAME_   HEADER_CONTENTS
U32_    2                           // header
LLINT_  1
U32_    4
U8_     1

NAME_   CONFIGURE_SCAN
U32_    2                           // header
LLINT_  1
DOUBLE_ 2
U32_    2
U8_     32
U8_     4

NAME_   MODEL_DATA
U32_    2                           // header
DOUBLE_ 104
U16_    1
U8_     3

NAME_   PCAL_REQUEST
U32_    2                           // header
DOUBLE_ 16
U8_     2

NAME_   PCAL_DATA
U32_    2                           // header
DOUBLE_ 1
U32_    16*16*2
U8_     1

NAME_   SCOUNT_REQUEST
U32_    2                           // header
U8_     1

NAME_   SCOUNT_DATA
U32_    2                           // header
DOUBLE_ 1
U32_    16*4
U8_     1

NAME_   ABORT_SCAN
U32_    2                           // header

NAME_   SEND_DIRECTORY
U32_    2                           // header
INT_    2

NAME_   DIRECTORY_CHUNK
U32_    2                           // header
INT_    4
STRUCT8_ 50
    CHAR_  80
    LLINT_  2
STREND_

NAME_   PLAYBACK_CONTROL
U32_    2                           // header
INT_    1
LLINT_  1
U8_     3

/*******************************************************************************
*                                                                              *
*                 t o / f r o m    r o t m a n                                 *
*                                                                              *
*******************************************************************************/

NAME_   SET_COT_CLOCK
U32_    2                           // header
DOUBLE_ 1
INT_    2

NAME_   START_ROT_CLOCK
U32_    2                           // header
DOUBLE_ 4
U16_    1
U8_     1

NAME_   SEND_TIMES
U32_    2                           // header
INT_    1

NAME_   CURRENT_TIMES
U32_    2                           // header
DOUBLE_ 5

/*******************************************************************************
*                                                                              *
*                 t o / f r o m    o p e r a                                   *
*                                                                              *
*******************************************************************************/

NAME_   INITIALIZE_HARDWARE
U32_    2                           // header
CHAR_   MAX_LEN_KEY
U8_     1 + MAX_NUM_SU

NAME_   REMOVE_STATION
U32_    2                           // header
CHAR_   1

NAME_   SEND_COND_STATUS
U32_    2                           // header
U16_    1

NAME_   COND_STATUS
U32_    2                           // header
STRUCT4_ 4
    U32_    1
    U16_    3
    CHAR_   MAX_LEN_FILNAM
    CHAR_   16
STREND_

NAME_   MOUNT_REQUEST
U32_    2                           // header
U8_     1
STRUCT1_ MAX_NUM_TAPES
    U8_     1
    CHAR_   14
    U8_     1
STREND_

NAME_   MOUNT_STATUS
U32_    2                           // header
U8_     1
CHAR_   14
U8_     1

NAME_   DISMOUNT_TAPE
U32_    2                           // header
U8_     1

NAME_   STREAM_CONTROL
U32_    2                           // header
U8_     2
U16_    1
CHAR_   MAX_LEN_FILNAM

NAME_   REORDER_STREAMS
U32_    2                           // header
U8_     4

/*******************************************************************************
*                                                                              *
*                 t o / f r o m    m c b                                       *
*                                                                              *
*******************************************************************************/

NAME_   SEND_TAPE_STATUS
U32_    2                           // header
U16_    1
U8_     MAX_NUM_SU
U8_     17

NAME_   TAPE_STATUS
U32_    2                           // header
U8_     MAX_NUM_SU
U8_     17
U16_    MAX_NUM_SU * 16

NAME_   DPU_COMMAND
U32_    2                           // header
CHAR_   1
U8_     2
U16_    1



/*******************************************************************************
*                                                                              *
*                 m i s c e l l a n y                                          *
*                                                                              *
*******************************************************************************/

NAME_   TERMINATE_SELF
U32_    2                           // header
U16_    1

NAME_   PROGRAM_CONTROL
U32_    2                           // header
INT_    1
U32_    1

NAME_   ERROR_MESSAGE
U32_    2                           // header
U16_    1
CHAR_   MAX_LEN_ERR
                                    // mark end of table
} , {999, 999}
};
