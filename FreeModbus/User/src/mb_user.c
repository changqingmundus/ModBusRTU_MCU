#include "encoder.h"
#include "MB_User_Config.h"
#include "mb.h"


volatile uint8_t BaudRate_Update_Flag = 0;
volatile uint32_t New_BaudRate = 38400;

eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress,
                             USHORT usNRegs, eMBRegisterMode eMode)
{
  uint16_t value;

  if (eMode == MB_REG_READ)
  {
    Encoder_Read_Data();
    uint32_t single_value;
    single_value = (Encoder_Config.SingleTurn_Data - Zero_SingleTurn_Data) &
                   ((1UL << Encoder_Config.SingleTurn_Bit) - 1);
    while (usNRegs--)
    {

      switch (usAddress)
      {

      case 0x0001: // 對應地址0x0000開始
      {
        value = (single_value >> 16);
        break;
      }

      // SingleTurn 低16位
      case 0x0002:
      {
        value = single_value & 0xFFFF;
        break;
      }

      // Multiturn 高16位
      case 0x0003:

        value = (Encoder_Config.MultiTurn_Data >> 16);
        break;

      // Multiturn 低16位
      case 0x0004:
        value = Encoder_Config.MultiTurn_Data & 0xFFFF;
        break;

      // 從機地址
      case 0x0005:

        value = Slave_ID;
        break;

      // 波特率
      case 0x0006:

        value = BaudRate;
        break;

      // 校驗
      case 0x0007:

        value = Parity;
        break;

      // zeroData
      case 0x0008:

        value = 0;
        break;
        
      default:

        return MB_ENOREG;
      }

      // Modbus寄存器高字節在前
      *pucRegBuffer++ = value >> 8;
      *pucRegBuffer++ = value & 0xff;

      usAddress++;
    }

    return MB_ENOERR;
  }
  else if (eMode == MB_REG_WRITE)
  {
    while (usNRegs--)
    {
      value = ((uint16_t)pucRegBuffer[0] << 8) | pucRegBuffer[1];

      switch (usAddress)
      {

      case 0x0006:
        switch (value)
        {
        case 0x0001:
          BaudRate = 4800;
          break;

        case 0x0002:
          BaudRate = 9600;
          break;

        case 0x0003:
          BaudRate = 19200;
          break;

        case 0x0004:
          BaudRate = 38400;
          break;

        case 0x0005:
          BaudRate = 57600;
          break;

        case 0x0006:
          BaudRate = 115200;
          break;

        case 0x0007:
          BaudRate = 230400;
          break;

        case 0x0008:
          BaudRate = 460800;
          break;

        default:
          return MB_EINVAL;
        }

        BaudRate_Update_Flag = 1;
        New_BaudRate = BaudRate;

      // 主站寫地址0x0007
      // callback地址偏移+1
      case 0x0008:

        if (value == 0x00FF)
        {
          Encoder_Clear_Data();
          Encoder_Save_to_DEE(DEE_ENCODER_ZERO_L,
                              DEE_ENCODER_ZERO_H,
                              Zero_SingleTurn_Data);
        }
        break;

        // Zero Offset 高16位
        // 主站地址0x0008
      case 0x0009:
      {
        Zero_SingleTurn_Data &= 0x0000FFFF;
        Zero_SingleTurn_Data |= ((uint32_t)value << 16);
        break;
      }

      // Zero Offset 低16位
      // 主站地址0x0009
      case 0x000A:
      {
        Zero_SingleTurn_Data &= 0xFFFF0000;
        Zero_SingleTurn_Data |= value;
        break;
      }

      default:

        return MB_ENOREG;
      }

      pucRegBuffer += 2;
      usAddress++;
    }
    return MB_ENOERR;
  }

  return MB_ENOREG;
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