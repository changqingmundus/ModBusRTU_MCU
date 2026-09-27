#ifndef MB_FACTORY_H
#define MB_FACTORY_H

#include "mb.h"
#include "stdint.h"
#include "encoder.h"

#define MB_FUNC_FACTORY       0x66

#define FACTORY_Save_KEY      0x505A   //config code

#define MU_STATUS1_CRC_ERR    0x80

struct factory_param
{
    uint16_t addr;
    uint8_t  pos;
    uint8_t  len;
};

extern uint8_t Factory_SingleTurnBit;
extern uint8_t Factory_MultiTurnBit;
extern uint8_t Factory_CRCBit;
extern uint16_t Factory_MagicKey;

eMBException eMBFuncFactoryConfig(UCHAR *pucFrame, USHORT *usLen);
void Factory_Config_SaveDEE(void);

uint8_t Sensor_Config_Validate(void);

uint8_t Sensor_Config_Save(void);
uint8_t Sensor_MU_Config(void);
uint8_t Sensor_PZ_Config(void);
uint8_t Sensor_MT_Config(SensorChip_t chip);

#endif