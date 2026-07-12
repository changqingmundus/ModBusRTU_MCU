#include "encoder.h"
#include "mb.h"

uint16_t Slave_ID = 1;
uint16_t BaudRate = 0x01;
uint16_t Parity = 0x01;
uint16_t Direction = 0x01;
uint16_t Protocol = 0x01;

eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress,
                             USHORT usNRegs, eMBRegisterMode eMode) 
{
  uint16_t value;

  if (eMode == MB_REG_READ) {

    while (usNRegs--) {

      switch (usAddress) {

      // SingleTurn 高16位
      case 0x0000:

        value = (Encoder_Config.SingleTurn_Data >> 16);
        break;

      // SingleTurn 低16位
      case 0x0001:

        value = Encoder_Config.SingleTurn_Data & 0xFFFF;
        break;

      // Multiturn 高16位
      case 0x0002:

        value = (Encoder_Config.Multiturn_Data >> 16);
        break;

      // Multiturn 低16位
      case 0x0003:

        value = Encoder_Config.Multiturn_Data & 0xFFFF;
        break;

      // Status
      case 0x0004:

        value = 0;

        if (Encoder_Config.Warning_Data)
          value |= 0x01;

        if (Encoder_Config.Error_Data)
          value |= 0x02;

        break;

      // Encoder CRC
      case 0x0005:

        value = Encoder_Config.CRC_Data;
        break;

      // 從機地址
      case 0x0006:

        value = Slave_ID;
        break;

      // 波特率
      case 0x0007:

        value = BaudRate;
        break;

      // 校驗
      case 0x0008:

        value = Parity;
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

  return MB_ENOREG;
}