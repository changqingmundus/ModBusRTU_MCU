#include "encoder.h"
#include "MB_User_Config.h"
#include "Protocol_Config.h"
#include "mb.h"

volatile uint8_t BaudRate_Update_Flag = 0;
volatile uint32_t New_BaudRate = 38400;
volatile uint8_t Parity_Update_Flag = 0;

eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress,
                             USHORT usNRegs, eMBRegisterMode eMode)
{
  uint16_t value;

  if (eMode == MB_REG_READ)
  {
    LED1_SetLow(); // LED1指示EncoderReadData

    uint32_t Encoder_Value;

    Encoder_Value = Encoder_Get_Position();

    while (usNRegs--)
    {
      switch (usAddress)
      {
      case 0x0001:
      {
        value = Encoder_Value & ((1UL << Encoder_Config.SingleTurn_Bit) - 1);
        break;
      }

      case 0x0002:
      {
        value = Encoder_Value >> Encoder_Config.SingleTurn_Bit;
        break;
      }

      case 0x0003: // 从机号
      {
        value = Slave_ID;
        break;
      }

      case 0x0004: // 波特率
      {
        value = BaudRate_Index;
        break;
      }

      case 0x0005: // 校验位
      {
        value = Parity;
        break;
      }

      case 0x0006: // 编码器位置零
      {
        value = 0;
        break;
      }

      case 0x0007: // 递增方向
      {
        value = Direction_Config;
        break;
      }

      case 0x0008: // 通信协议
      {
        value = Protocol;
        break;
      }

      case 0x0009: // 上传时间
      {
        value = Update_Time;
        break;
      }

      case 0x000A: // 编码器值高16

      {
        value = Encoder_Value >> 16;
        break;
      }

      case 0x000B: // 编码器值低16

      {
        value = Encoder_Value & 0xffff;
        break;
      }

      case 0x000C: // 旋转方向
      {
        value = Encoder_Direction;
        break;
      }

      case 0x000D: // 转速 RPM
      {
        value = Encoder_RPM;
        break;
      }

      case 0x0024: // 原点位置
      {
        value = MultiTurn_Origin_Mode;
        break;
      }

      case 0x0025: // 速度采样周期
      {
        value = Speed_Update_Period;
        break;
      }

      case 0x0401: // 编码器总值低16位
      {
        value = Encoder_Value & 0xffff;
        break;
      }

      case 0x0402: // 编码器总值高16位
      {
        value = Encoder_Value >> 16;
        break;
      }

      default:
      {
        return MB_ENOREG;
      }
      }
      pucRegBuffer[0] = value >> 8;
      pucRegBuffer[1] = value & 0xff;

      pucRegBuffer += 2;
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
      case 0x0003: // 0X0002開始修改地址
      {
        if (value >= 1 && value <= 127)
        {
          Slave_ID = value;
          ucMBAddress = Slave_ID;
          DEE_Write(DEE_SLAVE_ID, Slave_ID);
        }
        else
        {
          return MB_EINVAL;
        }

        break;
      }

      case 0x0004: // 修改波特率
      {
        if (value >= 0x01 && value <= 0x08)
        {
          BaudRate_Index = value;
          DEE_Write(DEE_BAUDRATE_INDEX, BaudRate_Index);
          BaudRate = BaudRate_Get_Value(BaudRate_Index);
          // 通知主程序立即修改UART
          BaudRate_Update_Flag = 1;
          New_BaudRate = BaudRate;

          MB_Timer_Update(BaudRate);
        }
        break;
      }

      case 0x0005: // 校验位
      {
        if (value >= 0x01 && value <= 0x03)
        {
          Parity = value;

          DEE_Write(DEE_PARITY, Parity);
          Parity_Update_Flag = 1;
        }
        else
        {
          return MB_EINVAL;
        }

        break;
      }

      case 0x0006: // 清零
      {
        if (value == 0xFF)
        {
          Encoder_Clear_Data();
        }
        break;
      }

      case 0x0007: // 递增方向
      {
        if (value == 0x01 || value == 0x02)
        {
          Direction_Config = value;
          DEE_Write(DEE_Direction, Direction_Config);
        }

        break;
      }

      case 0x0008: // 协议
      {
        Protocol = value;
        DEE_Write(DEE_Encoder_Protocol, Protocol);
        break;
      }

      case 0x0009: // 上传时间
      {
        Update_Time = value;
        DEE_Write(DEE_Update_Time, Update_Time);
        break;
      }

      case 0x0024: // 原点位置
      {
        if (value == 1 || value == 2)
        {
          MultiTurn_Origin_Mode = value;
          DEE_Write(DEE_MultiTurn_Origin_Mode, MultiTurn_Origin_Mode);
        }

        break;
      }

      case 0x0025: // 速度采样周期 ms
      {
        if (value >= 1 && value <= 200)
        {
          Speed_Update_Period = value;

          DEE_Write(DEE_Speed_Update_Period, Speed_Update_Period);
        }

        break;
      }

      case 0x0033: // 写当前值低16
      {
        Encoder_Write_Low = value;
        break;
      }

      case 0x0034: // 写当前值高16
      {
        Encoder_Write_High = value;

        Encoder_Set_Value(
            ((uint32_t)Encoder_Write_High << 16) | Encoder_Write_Low);

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