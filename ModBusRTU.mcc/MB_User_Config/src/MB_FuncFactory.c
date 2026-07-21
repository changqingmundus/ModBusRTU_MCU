#include "MB_FunFactory.h"
#include "encoder.h"

uint8_t Factory_SingleTurnBit = 0;
uint8_t Factory_MultiTurnBit = 0;
uint8_t Factory_CRCBit = 0;

eMBException eMBFuncFactoryConfig(UCHAR *pucFrame)
{
    UCHAR *pData = &pucFrame[1];

    USHORT key;
    USHORT address;
    USHORT quantity;
    UCHAR byteCount;

    USHORT value;

    // Key
    key = ((USHORT)pData[0] << 8) | pData[1];

    if (key != FACTORY_MAGIC_KEY)
    {
        return MB_EX_ILLEGAL_DATA_VALUE;
    }

    // 起始地址
    address = ((USHORT)pData[2] << 8) | pData[3];

    // 寄存器數量
    quantity = ((USHORT)pData[4] << 8) | pData[5];

    // 字節數
    byteCount = pData[6];

    if (byteCount != quantity * 2)
    {
        return MB_EX_ILLEGAL_DATA_VALUE;
    }

    pData += 7;

    while (quantity--)
    {
        value = ((USHORT)pData[0] << 8) | pData[1];

        switch (address)
        {
        case FACTORY_SINGLE_BIT:

            if (value > 32)
                return MB_EX_ILLEGAL_DATA_VALUE;

            Factory_SingleTurnBit = value;
            break;

        case FACTORY_MULTI_BIT:

            Factory_MultiTurnBit = value;
            break;

        case FACTORY_CRC_BIT:

            Factory_CRCBit = value;
            break;

        case FACTORY_SAVE:

            if (value == FACTORY_Save_KEY)
            {
                Factory_Config_Save();
            }

            break;

        default:
            return MB_EX_ILLEGAL_DATA_ADDRESS;
        }

        pData += 2;
        address++;
    }
    // 回覆
    pucFrame[0] = MB_FUNC_FACTORY;
    pucFrame[1] = 0x88;

    //*pusLength = 2;

    return MB_EX_NONE;
}
void Factory_Config_Save(void)
{
    DEE_Write(DEE_Encoder_MultiTurnBitSize, Factory_MultiTurnBit);
    DEE_Write(DEE_Encoder_SingleTurnBitSize, Factory_SingleTurnBit);
    DEE_Write(DEE_Encoder_CRCBitSize, Factory_CRCBit);
    DEE_Write(DEE_Encoder_MagicKey, FACTORY_MAGIC_KEY);

    UART1_Write(Factory_SingleTurnBit);
    UART1_Write(Factory_MultiTurnBit);
    UART1_Write(Factory_CRCBit);
}