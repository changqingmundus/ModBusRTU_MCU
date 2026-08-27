#include "encoder.h"
#include "MB_User_Config.h"
#include "Protocol_Config.h"
#include "mb.h"
#include "mb_user.h"

MB_RegMap_t *MB_Reg_Table;

uint16_t MB_Reg_Table_Size;

uint64_t Encoder_Write_Position;

static MB_RegMap_t *MB_Reg_Find(uint16_t address);

volatile uint8_t BaudRate_Update_Flag = 0;
volatile uint8_t Slave_ID_Update_Flag = 0;
volatile uint8_t Parity_Update_Flag = 0;
volatile uint32_t New_Slave_ID;
volatile uint32_t New_BaudRate;

eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress,
                             USHORT usNRegs, eMBRegisterMode eMode)
{
  uint16_t value;
  uint64_t Encoder_Value = 0;

  if (eMode == MB_REG_READ)
  {
    LED1_SetLow(); // LED1指示EncoderReadData

    Encoder_Value = Encoder_Get_Position(); // 一次读取
    MB_RegMap_t *reg;

    while (usNRegs--)
    {
      reg = MB_Reg_Find(usAddress);

      if (reg == NULL ||
          reg->read == NULL)
      {
        return MB_ENOREG;
      }

      value = reg->read(Encoder_Value);

      pucRegBuffer[0] = value >> 8;
      pucRegBuffer[1] = value & 0xff;
      pucRegBuffer += 2;

      usAddress++;
    }

    return MB_ENOERR;
  }
  else if (eMode == MB_REG_WRITE)
  {
    MB_RegMap_t *reg;

    while (usNRegs--)
    {
      value = ((uint16_t)pucRegBuffer[0] << 8) | pucRegBuffer[1];
      reg = MB_Reg_Find(usAddress);

      if (reg == NULL ||
          reg->write == NULL)
      {
        return MB_ENOREG;
      }

      reg->write(value);
      pucRegBuffer += 2;
      usAddress++;
    }
    return MB_ENOERR;
  }
  return MB_EINVAL;
}

eMBErrorCode eMBRegInputCB(UCHAR *pucRegBuffer,
                           USHORT usAddress,
                           USHORT usNRegs)
{
  return MB_ENOREG;
}

eMBErrorCode eMBRegCoilsCB(UCHAR *pucRegBuffer,
                           USHORT usAddress,
                           USHORT usNCoils,
                           eMBRegisterMode eMode)
{
  return MB_ENOREG;
}

eMBErrorCode eMBRegDiscreteCB(UCHAR *pucRegBuffer,
                              USHORT usAddress,
                              USHORT usNDiscrete)
{
  return MB_ENOREG;
}

MB_RegMap_t Holding_Reg_Table_16[] =
    {
        {0x0001, MB_Read_SingleTurn_Low, NULL},
        {0x0002, MB_Read_MultiTurn_Low, NULL},
        {0x0003, MB_Read_SlaveID, MB_Write_SlaveID},
        {0x0004, MB_Read_BaudRate, MB_Write_BaudRate},
        {0x0005, MB_Read_Parity, MB_Write_Parity},
        {0x0006, NULL, MB_Write_Clear_Encoder},
        {0x0007, MB_Read_Direction, MB_Write_Direction},
        {0x0008, MB_Read_Protocol, MB_Write_Protocol},
        {0x0009, MB_Read_UpdateTime, MB_Write_UpdateTime},

        {0x000A, MB_Read_Total_1, NULL},
        {0x000B, MB_Read_Total_0, NULL},

        {0x000C, MB_Read_Rotation_Direction, NULL},
        {0x000D, MB_Read_RPM, NULL},
        {0x0024, MB_Read_Origin_Mode, MB_Write_Origin_Mode},
        {0x0025, MB_Read_Speed_Period, MB_Write_Speed_Period},

        {0x0401, MB_Read_Total_0, NULL},
        {0x0402, MB_Read_Total_1, NULL},

        {0x0033, NULL, MB_Write_Position_0},
        {0x0034, NULL, MB_Write_Position_2},
};

uint16_t Holding_Reg_Table_16_Size = sizeof(Holding_Reg_Table_16) /
                                     sizeof(Holding_Reg_Table_16[0]);

MB_RegMap_t Holding_Reg_Table_32[] =
    {
        {0x0001, MB_Read_SingleTurn_Low, NULL},
        {0x0002, MB_Read_SingleTurn_High, NULL},

        {0x0003, MB_Read_MultiTurn_Low, NULL},
        {0x0004, MB_Read_MultiTurn_High, NULL},

        // 参数区
        {0x0005, MB_Read_SlaveID, MB_Write_SlaveID},
        {0x0006, MB_Read_BaudRate, MB_Write_BaudRate},
        {0x0007, MB_Read_Parity, MB_Write_Parity},
        {0x0008, NULL, MB_Write_Clear_Encoder},
        {0x0009, MB_Read_Direction, MB_Write_Direction},
        {0x000A, MB_Read_Protocol, MB_Write_Protocol},
        {0x000B, MB_Read_UpdateTime, MB_Write_UpdateTime},

        // Total Position 64bit
        {0x000C, MB_Read_Total_3, NULL},
        {0x000D, MB_Read_Total_2, NULL},
        {0x000E, MB_Read_Total_1, NULL},
        {0x000F, MB_Read_Total_0, NULL},

        {0x0010, MB_Read_Rotation_Direction, NULL},
        {0x0011, MB_Read_RPM, NULL},

        {0x0024, MB_Read_Origin_Mode, MB_Write_Origin_Mode},
        {0x0025, MB_Read_Speed_Period, MB_Write_Speed_Period},

        // 设置位置
        {0x0033, NULL, MB_Write_Position_0},
        {0x0034, NULL, MB_Write_Position_1},
        {0x0035, NULL, MB_Write_Position_2},
        {0x0036, NULL, MB_Write_Position_3},

        {0x0401, MB_Read_Total_0, NULL},
        {0x0402, MB_Read_Total_1, NULL},
        {0x0403, MB_Read_Total_2, NULL},
        {0x0404, MB_Read_Total_3, NULL},

};

uint16_t Holding_Reg_Table_32_Size = sizeof(Holding_Reg_Table_32) /
                                     sizeof(Holding_Reg_Table_32[0]);

static MB_RegMap_t *MB_Reg_Find(uint16_t address)
{
  uint16_t i;

  for (i = 0; i < MB_Reg_Table_Size; i++)
  {
    if (MB_Reg_Table[i].address == address)
    {
      return &MB_Reg_Table[i];
    }
  }

  return NULL;
}

uint16_t MB_Read_SingleTurn_Low(uint64_t value)
{
  return value & ((1UL << Encoder_Config.SingleTurn_Bit) - 1);
}

uint16_t MB_Read_SingleTurn_High(uint64_t value)
{
  return value >> Encoder_Config.SingleTurn_Bit;
}

uint16_t MB_Read_MultiTurn_Low(uint64_t value)
{
  value = value >> Encoder_Config.SingleTurn_Bit;
  return value & 0xffff;
}

uint16_t MB_Read_MultiTurn_High(uint64_t value)
{
  value = value >> Encoder_Config.SingleTurn_Bit;
  return value >> 16;
}

uint16_t MB_Read_SlaveID(uint64_t Encoder_Value)
{
  return Slave_ID;
}

uint16_t MB_Read_BaudRate(uint64_t Encoder_Value)
{
  return BaudRate_Index;
}

uint16_t MB_Read_Parity(uint64_t Encoder_Value)
{
  return Parity;
}

uint16_t MB_Read_Direction(uint64_t Encoder_Value)
{
  return Direction_Config;
}

uint16_t MB_Read_Protocol(uint64_t Encoder_Value)
{
  return Protocol;
}

uint16_t MB_Read_UpdateTime(uint64_t Encoder_Value)
{
  return Update_Time;
}

uint16_t MB_Read_RPM(uint64_t Encoder_Value)
{
  return Encoder_RPM;
}

uint16_t MB_Read_Rotation_Direction(uint64_t Encoder_Value)
{
  return Encoder_Direction;
}

uint16_t MB_Read_Origin_Mode(uint64_t Encoder_Value)
{
  return MultiTurn_Origin_Mode;
}

uint16_t MB_Read_Speed_Period(uint64_t Encoder_Value)
{
  return Speed_Update_Period;
}

uint16_t MB_Read_Total_0(uint64_t value)
{
  return (uint16_t)(value & 0xffff);
}

uint16_t MB_Read_Total_1(uint64_t value)
{
  return (uint16_t)((value >> 16) & 0xffff);
}

uint16_t MB_Read_Total_2(uint64_t value)
{
  return (uint16_t)((value >> 32) & 0xffff);
}

uint16_t MB_Read_Total_3(uint64_t value)
{
  return (uint16_t)((value >> 48) & 0xffff);
}

void MB_Write_SlaveID(uint16_t value)
{
  if (value >= 1 && value <= 127)
  {
    Slave_ID = value;
    DEE_Write(DEE_SLAVE_ID, Slave_ID);
    Slave_ID_Update_Flag = 1;
    New_Slave_ID = Slave_ID;
  }
}

void MB_Write_BaudRate(uint16_t value)
{
  if (value >= 1 && value <= 8)
  {
    BaudRate_Index = value;
    DEE_Write(DEE_BAUDRATE_INDEX, value);
    BaudRate = BaudRate_Get_Value(value);
    BaudRate_Update_Flag = 1;
    New_BaudRate = BaudRate;
    MB_Timer_Update(BaudRate);
  }
}

void MB_Write_Parity(uint16_t value)
{
  if (value >= 1 && value <= 3)
  {
    Parity = value;
    DEE_Write(DEE_PARITY, Parity);
    Parity_Update_Flag = 1;
  }
}

void MB_Write_Clear_Encoder(uint16_t value)
{
  if (value == 0xff)
  {
    Encoder_Clear_Data();
  }
}

void MB_Write_Direction(uint16_t value)
{
  if (value == 1 || value == 2)
  {
    Direction_Config = value;
    DEE_Write(DEE_Direction, value);
  }
}

void MB_Write_Protocol(uint16_t value)
{
  Protocol = value;
  DEE_Write(DEE_Encoder_Protocol, value);
}

void MB_Write_UpdateTime(uint16_t value)
{
  Update_Time = value;
  DEE_Write(DEE_Update_Time, value);
}

void MB_Write_Origin_Mode(uint16_t value)
{
  if (value == 1 || value == 2)
  {
    MultiTurn_Origin_Mode = value;
    DEE_Write(DEE_MultiTurn_Origin_Mode, value);
  }
}

void MB_Write_Speed_Period(uint16_t value)
{
  if (value >= 1 && value <= 200)
  {
    Speed_Update_Period = value;
    DEE_Write(DEE_Speed_Update_Period, value);
  }
}

void MB_Write_Position_0(uint16_t value)
{
  Encoder_Write_Position &= 0xFFFFFFFFFFFF0000ULL;

  Encoder_Write_Position |= value;
}

void MB_Write_Position_1(uint16_t value)
{
  Encoder_Write_Position &= 0xFFFFFFFF0000FFFFULL;

  Encoder_Write_Position |=
      ((uint64_t)value << 16);
}

void MB_Write_Position_2(uint16_t value)
{
  Encoder_Write_Position &= 0xFFFF0000FFFFFFFFULL;

  Encoder_Write_Position |=
      ((uint64_t)value << 32);
}

void MB_Write_Position_3(uint16_t value)
{
  Encoder_Write_Position &= 0x0000FFFFFFFFFFFFULL;

  Encoder_Write_Position |=
      ((uint64_t)value << 48);

  // 四个寄存器收齐以后设置
  Encoder_Set_Value(Encoder_Write_Position);
}