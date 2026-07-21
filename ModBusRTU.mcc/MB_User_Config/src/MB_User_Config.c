#include "MB_User_Config.h"
#include "MB_FunFactory.h"
#include "encoder.h"
#include "dee.h"

uint16_t Slave_ID = 1;
uint16_t BaudRate_Index = 0x03;
uint16_t Parity = 0x01;

uint32_t BaudRate = 38400;
eMBParity MB_Parity = MB_PAR_NONE;

void MB_User_Config_Init(void)
{

  uint16_t MagicKey;
  DEE_Read(DEE_Encoder_MagicKey, &MagicKey);

  if (MagicKey != FACTORY_MAGIC_KEY)
  {
    Slave_ID = 1;
    BaudRate_Index = 3;
    Parity = 1;

    DEE_Write(DEE_Encoder_MagicKey, FACTORY_MAGIC_KEY);
    DEE_Write(DEE_SLAVE_ID, Slave_ID);
    DEE_Write(DEE_BAUDRATE_INDEX, BaudRate_Index);
    DEE_Write(DEE_PARITY, Parity);
  }
  else
  {
    DEE_Read(DEE_SLAVE_ID, &Slave_ID);
    DEE_Read(DEE_BAUDRATE_INDEX, &BaudRate_Index);
    DEE_Read(DEE_PARITY, &Parity);
  }

  switch (BaudRate_Index)
  {
  case 0x01:
    BaudRate = 9600;
    break;

  case 0x02:
    BaudRate = 19200;
    break;

  case 0x03:
    BaudRate = 38400;
    break;

  case 0x04:
    BaudRate = 57600;
    break;

  case 0x05:
    BaudRate = 115200;
    break;

  default:
    BaudRate = 38400;
    BaudRate_Index = 0x03;
    break;
  }

  switch (Parity)
  {
  case 0x01:
    MB_Parity = MB_PAR_NONE;
    break;

  case 0x02:
    MB_Parity = MB_PAR_ODD;
    break;

  case 0x03:
    MB_Parity = MB_PAR_EVEN;
    break;

  default:
    MB_Parity = MB_PAR_NONE;
    Parity = 0x01;
    break;
  }
}