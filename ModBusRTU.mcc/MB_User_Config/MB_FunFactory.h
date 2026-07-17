#ifndef MB_FACTORY_H
#define MB_FACTORY_H

#include "mb.h"
#include "stdint.h"

#define MB_FUNC_FACTORY       0x66

#define FACTORY_Save_KEY      0x505A


#define FACTORY_SINGLE_BIT    0x0001
#define FACTORY_MULTI_BIT     0x0002
#define FACTORY_CRC_BIT       0x0003
#define FACTORY_SAVE          0x00FF

extern uint8_t Factory_SingleTurnBit;
extern uint8_t Factory_MultiTurnBit;
extern uint8_t Factory_CRCBit;
extern uint16_t Factory_MagicKey;

eMBException eMBFuncFactoryConfig(UCHAR *pucFrame);
void Factory_Config_Save(void);


#endif