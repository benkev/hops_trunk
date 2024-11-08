#ifndef TC_H_INCLUDED
#define TC_H_INCLUDED

#include <inttypes.h> 
#include "MK4.h"

typedef struct TC_Baseline {
   char        baselineId[2];
   uint16_t    startAPCFNum;
   uint16_t    endAPCFNum;
   uint8_t     unused1[2];
   double      startAPTime;
   double      endAPTime;
} TC_Baseline; 

typedef struct TC_Snake {
   uint16_t        snakeNum;
   uint16_t        primarySnakeNum;
   uint16_t        baselineNum;
   uint16_t        t101Num;
   uint16_t        lenInCells;
   uint16_t        snakeCellMapNum;
   uint8_t         type;
   uint8_t         headBlkNum;
   uint8_t         tailBlkNum;
   uint8_t         refSuId;
   uint8_t         refChnId;
   uint8_t         refPhyInBdChnGrp;
   uint8_t         remSuId;
   uint8_t         remChnId;
   uint8_t         remPhyInBdChnGrp;
   uint8_t         unused0[3];
} TC_Snake;

typedef struct TC_CorBrd {
   uint8_t     brdPhyNum;
   uint8_t     segNum;
   uint8_t     sliceNum;
   uint8_t     accumRatio;
   uint8_t     bocfShsmpRatio;
   uint8_t     dataShsmpRatio;
   uint8_t     sampleCntPerLagEnable;
   uint8_t     unused0[1];
   uint32_t    xBarChipMask;
   uint8_t     xBarOutput[640];
   uint32_t    corChipMask;
   uint32_t    staticParm[256];
   uint16_t    snakeCellMap[512];
   uint16_t    numOfSnakes;
   uint8_t     unused1[4];
   TC_Snake    snake[1];
} TC_CorBrd;

typedef struct TC_MsgHdr {
   uint32_t  msgCode;
   uint32_t  taskId;
   uint8_t   streamNum;
   uint8_t   numOfCorBrds;
   char      rootcode[6];
   char      overSample;
   char      sendRecs;
   uint8_t   unused0[4];
   uint16_t  numOfBaselines;
   float     speedUp;
   float     apLength;
   float     sysTempo;
   double    sampleRate;
   uint32_t  bocfPeriod;
   uint32_t  links[2];
   uint32_t  offsetOfBaselines;
   uint32_t  offsetsOfCorBrds[16];
} TC_MsgHdr;

#endif 
