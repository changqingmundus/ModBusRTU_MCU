#include "MB_User_Config.h"
#include "MB_FunFactory.h"

uint16_t Slave_ID = 1;
uint16_t BaudRate_Index = 0x01;
uint16_t Parity = 0x01;

uint32_t BaudRate;
eMBParity MB_Parity = MB_PAR_NONE;

uint32_t BaudRate_Get_Value(uint16_t index)
{
  switch (index)
  {
  case 0x01:
    return 9600;

  case 0x02:
    return 19200;

  case 0x03:
    return 38400;

  case 0x04:
    return 57600;

  case 0x05:
    return 115200;

  case 0x06:
    return 4800;

  case 0x07:
    return 2400;

  case 0x08:
    return 1200;

  default:
    return 9600;
  }
}

void MB_User_Config_Init(void)
{

  uint16_t MagicKey;
  DEE_Read(DEE_MODBUS_MagicKey, &MagicKey);

  if (MagicKey != MODBUS_MAGIC_KEY)
  {
    Slave_ID = 1;
    BaudRate_Index = 1;
    Parity = 1;
    BaudRate = BaudRate_Get_Value(BaudRate_Index);

    DEE_Write(DEE_MODBUS_MagicKey, MODBUS_MAGIC_KEY);
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

void UART1_Parity_Set(uint8_t parity)
{
  uint16_t mode;

  U1MODEbits.UARTEN = 0;

  mode = U1MODE;

  // 清 PDSEL<1:0>
  mode &= ~(0x06);

  switch (parity)
  {
  case 0x01: // None
  {
    // bit2:1 = 00
    break;
  }

  case 0x02: // Odd
  {
    // bit2:1 = 10
    mode |= 0x04;
    break;
  }

  case 0x03: // Even
  {
    // bit2:1 = 01
    mode |= 0x02;
    break;
  }

  default:
  {
    break;
  }
  }

  U1MODE = mode;

  U1MODEbits.UARTEN = 1;
}

void ModBusRTU_Update(void)
{
  if (debug_flag == 1)
  {
    debug_flag = 0;
    UART1_Write(debug_data);
  }

  if (Speed_Timer_Count >= Speed_Update_Period * 10)
  {
    Speed_Timer_Count = 0;

    Encoder_Read_Data();
    Encoder_Update_Speed();
  }

  if (BaudRate_Update_Flag)
  {
    if (UART1_IsTxDone())
    {
      BaudRate_Update_Flag = 0;
      UART1_BaudRateSet(New_BaudRate);
    }
  }

  if (Parity_Update_Flag)
  {
    if (UART1_IsTxDone())
    {
      Parity_Update_Flag = 0;
      UART1_Parity_Set(Parity);
    }
  }
}