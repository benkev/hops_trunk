/*----------------------------------------------------------------------------------------------------
 * The general organization of a binary or parsed cvex:$CORR file:
 *    Cvex_Setup       [ 1                                        ]   
 *    Cvex_CorrParms   [ 1                                        ]
 *    Cvex_Mode        [ 1                                        ]
 *    Cvex_BoardMode   [ num defs used from $CORR_BD_MODE         ]
 *    Cvex_SectionMode [ num defs used from $CORR_BD_SECTION_MODE ]
 *    Cvex_ChipMode    [ num defs used from $CORR_CHIP_MODE       ]
 *    Cvex_BlockMode   [ num defs used from $CORR_CHIP_BLOCK_MODE ]
 */  



/*----------------------------------------------------------------------------------------------------
 */

#define MAX_SECTS_P_BRD          4                /* Max sections/board                             */
#define MAX_BRDS_P_CORR          16               /* Max boards/correlator                          */
#define MAX_XPOL_P_BRD           8                /* Max xpol pairs/board                           */
#define MAX_CHIP_INS_P_CHIP      4                /* Max chip inputs/chip                           */
#define MAX_CHIPS_P_BRD          32               /* Max chips/board                                */
#define MAX_MUXS_P_BLK           4                /* Max multiplexers/block                         */
#define MAX_BLKS_P_SNAKE_P_CHIP  10               /* Max blocks/snake/chip                          */
#define MAX_BLKS_P_CHIP          8                /* Max blocks/chip                                */
#define MAX_SNAKES_P_CHIP        8                /* Max snakes/chip                                */



/*----------------------------------------------------------------------------------------------------
 * Header 
 */

typedef struct Cvex_Setup {
   char  rev[ MAX_PVALSIZE ];                     /*:str                                            */
   struct Cvex_CorrParms *corr_parms; 
   struct Cvex_Mode *  mode; 
} Cvex_Setup;



/*----------------------------------------------------------------------------------------------------
 * $CORR_DIVIDE
 */

typedef struct Cvex_CorrParms {
   char   defId[ MAX_NAMESIZE ];                  /*:str                                            */
   short  accumRatio;                             /*:int {1,2,4,8,16}                               */
   short  bocfShsmpRatio;                         /*:int {1,2,4,8,16}                               */
   short  dataShsmpRatio;                         /*:int {1,2,4,8,16}                               */
} Cvex_CorrParms;



/*----------------------------------------------------------------------------------------------------
 * $CORR_MODE
 */

typedef struct Cvex_ChnPair {
   char  refId[ MAX_PVALSIZE ];                   /*:str                                            */
   char  remId[ MAX_PVALSIZE ];                   /*:str                                            */
   char  sectId[ MAX_PVALSIZE ];                  /*:str                                            */
} Cvex_ChnPair;

 
typedef struct Cvex_Board {
   struct Cvex_BoardMode *  mode;      
   char  segId[ MAX_PVALSIZE ];                   /*:str                                            */
   char  sliceId[ MAX_PVALSIZE ];                 /*:str                                            */
   short  numOfChnPairs;                          /*:int {1..MAX_SECTS_P_BRD}                       */
   Cvex_ChnPair  chnPair[ MAX_SECTS_P_BRD ];
} Cvex_Board;


typedef struct Cvex_XPolPair {
   char  chn1Id[ MAX_PVALSIZE ];                  /*:str                                            */
   char  chn2Id[ MAX_PVALSIZE ];                  /*:str                                            */
} Cvex_XPolPair;


typedef struct Cvex_Mode {
   char  defId[ MAX_NAMESIZE ];                   /*:str                                            */
   short  numOfBoards;                            /*:int {1..MAX_BRDS_P_CORR}                       */
   Cvex_Board  board[ MAX_BRDS_P_CORR ];
   short  numOfXPolPairs;                         /*:int {1..MAX_XPOL_P_BRD}                  */
   Cvex_XPolPair  xPolPair[ MAX_XPOL_P_BRD ];
   short  numOfLags;                              /*:int {32..16384}                                */
   short  numOfSlices;                            /*:int {1,2,3,4}                                  */
} Cvex_Mode;


 
/*----------------------------------------------------------------------------------------------------
 * $CORR_BOARD_MODE
 */

typedef struct Cvex_Section {
   struct Cvex_SectionMode *  mode;             
   short  phyChipNum;                             /*:int {0..31}                                    */
   char  sectionId[ MAX_PVALSIZE ];               /*:str                                            */
} Cvex_Section;


typedef struct Cvex_BoardMode {
   char  defId[ MAX_NAMESIZE ];                   /*:str                                            */
   short  numOfSections;                          /*:int {1..MAX_SECTS_P_BRD}                       */
   Cvex_Section  section[ MAX_SECTS_P_BRD ];
} Cvex_BoardMode;



/*----------------------------------------------------------------------------------------------------
 * $CORR_SECTION_MODE
 */

typedef struct Cvex_Chip {
   struct Cvex_ChipMode *  mode;
   short  relChipNum;                             /*:int {0..31}                                    */
   char  input[ MAX_CHIP_INS_P_CHIP ][ MAX_PVALSIZE ];
                                                  /*:str                                            */
} Cvex_Chip;


typedef struct Cvex_SectionMode {
   char  defId[ MAX_NAMESIZE ];                   /*:str                                            */
   short  numOfChips;                             /*:int {1..32}                                    */
   Cvex_Chip  chip[ MAX_CHIPS_P_BRD ];
} Cvex_SectionMode;



/*----------------------------------------------------------------------------------------------------
 * $CORR_CHIP_MODE
 */

typedef struct Cvex_Block {
   struct Cvex_BlockMode *  mode;
   char  blockId[ MAX_PVALSIZE ];                 /*:str {A0,A1,A2,A3,B1,B2,B3}                     */
   short  mux[ MAX_MUXS_P_BLK ];                  /*:int {0..7}                                     */
} Cvex_Block;


typedef struct Cvex_Snake {
   char  type[ MAX_PVALSIZE ];                    /*:str {auto,complex}                             */
   char  refId[ MAX_PVALSIZE ];                   /*:str {X0,X1,X2,X3}                              */
   char  remId[ MAX_PVALSIZE ];                   /*:str {X0,X1,X2,X3}                              */
   short  lenOfSnakePath;                         /*:int {1..MAX_BLKS_P_SNAKE_P_CHIP}               */
   char  path[ MAX_BLKS_P_SNAKE_P_CHIP ][ MAX_PVALSIZE ];
                                                  /*:str {+?blk} , blk {A0,A1,A2,A3,B1,B2,B3}       */
} Cvex_Snake;
 

typedef struct Cvex_ChipMode {
   char  defId[ MAX_NAMESIZE ];                   /*:str                                            */
   short  numOfBlocks;                            /*:int {1..MAX_BLKS_P_CHIP}                       */
   Cvex_Block  block[ MAX_BLKS_P_CHIP ]; 
   short  numOfSnakes;                            /*:int {1..MAX_SNAKES_P_CHIP}                     */
   Cvex_Snake  snake[ MAX_SNAKES_P_CHIP ];
} Cvex_ChipMode;

 

/*----------------------------------------------------------------------------------------------------
 * $CORR_BLOCK_MODE
 */

typedef struct Cvex_BlockMode {
   char  defId[ MAX_NAMESIZE ];                   /*:str                                            */
   short  invalidateOnTapMove;                    /*:int {0,1}                                      */
   short  headerMode;                             /*:int {0,1,2,3}                                  */
   short  enableTap;                              /*:int {0,1}                                      */
   short  lCellRotrMode;                          /*:int {0,1,2,3}                                  */
   short  lCellXDly;                              /*:int {0,1}                                      */
   short  lCellYDly;                              /*:int {0,1}                                      */
   short  rCellRotrMode;                          /*:int {0,1,2,3}                                  */
   short  rCellXDly;                              /*:int {0,1}                                      */
   short  rCellYDly;                              /*:int {0,1}                                      */
} Cvex_BlockMode;
