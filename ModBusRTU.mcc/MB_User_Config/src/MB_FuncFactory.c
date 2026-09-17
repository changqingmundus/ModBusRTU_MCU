#include "MB_FunFactory.h"
#include "encoder.h"
#include "mu_1sf_driver.h"
#include "pz_1sf_driver.h"
#include "spi1.h"

uint8_t Factory_SingleTurnBit = 0;
uint8_t Factory_MultiTurnBit = 0;
uint8_t Factory_CRCBit = 0;

eMBException eMBFuncFactoryConfig(UCHAR *pucFrame, USHORT *usLen)
{
    UCHAR *pData = &pucFrame[1];

    USHORT key;
    USHORT address;
    USHORT quantity;
    USHORT quantity_written;
    UCHAR byteCount;
    USHORT value;

    /*
     * Minimum request:
     *
     * Key       2 bytes
     * Address   2 bytes
     * Quantity  2 bytes
     * ByteCount 1 byte
     *
     * Total = 7 bytes
     */
    if (*usLen < 8)
    {
        return MB_EX_ILLEGAL_DATA_VALUE;
    }

    /* Key */
    key = ((USHORT)pData[0] << 8) | pData[1];

    if (key != FACTORY_MAGIC_KEY)
    {
        return MB_EX_ILLEGAL_DATA_VALUE;
    }

    /* Start address */
    address = ((USHORT)pData[2] << 8) | pData[3];

    /* Register quantity */
    quantity = ((USHORT)pData[4] << 8) | pData[5];

    /* Quantity must not be zero */
    if (quantity == 0)
    {
        return MB_EX_ILLEGAL_DATA_VALUE;
    }
    quantity_written = quantity;

    /* Byte count */
    byteCount = pData[6];

    if (byteCount != quantity * 2)
    {
        return MB_EX_ILLEGAL_DATA_VALUE;
    }

    /*
     * Check complete request length.
     *
     * 1 byte function
     * 7 bytes header
     * quantity * 2 bytes data
     */
    if (*usLen < (USHORT)(8 + byteCount))
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
            {
                return MB_EX_ILLEGAL_DATA_VALUE;
            }

            Factory_SingleTurnBit = value;
            break;

        case FACTORY_MULTI_BIT:

            if (value > 32)
            {
                return MB_EX_ILLEGAL_DATA_VALUE;
            }

            Factory_MultiTurnBit = value;
            break;

        case FACTORY_CRC_BIT:

            if (value != 6 && value != 16)
            {
                return MB_EX_ILLEGAL_DATA_VALUE;
            }

            Factory_CRCBit = value;
            break;

        case FACTORY_MB_REG_MODE:

            if (value <= MB_REG_FORCE32)
            {
                MB_Reg_Mode = value;
            }
            else
            {
                return MB_EX_ILLEGAL_DATA_VALUE;
            }

            break;

        case FACTORY_SAVE:

            if (value == FACTORY_Save_KEY)
            {
                Factory_Config_SaveDEE();
                Sensor_MU_Config();
                if (Sensor_MU_Config() != 0)
                {
                    return MB_EX_SLAVE_DEVICE_FAILURE;
                }
            }
            else
            {
                return MB_EX_ILLEGAL_DATA_VALUE;
            }

            break;

        default:

            return MB_EX_ILLEGAL_DATA_ADDRESS;
        }

        pData += 2;
        address++;
    }

    pucFrame[0] = MB_FUNC_FACTORY;
    pucFrame[1] = 0x88;

    pucFrame[2] = (UCHAR)(address >> 8);
    pucFrame[3] = (UCHAR)(address & 0xFF);

    pucFrame[4] = (UCHAR)(quantity_written >> 8);
    pucFrame[5] = (UCHAR)(quantity_written & 0xFF);

    *usLen = 6;

    return MB_EX_NONE;
}

void Factory_Config_SaveDEE(void)
{
    DEE_Write(DEE_Encoder_MultiTurnBitSize, Factory_MultiTurnBit);
    DEE_Write(DEE_Encoder_SingleTurnBitSize, Factory_SingleTurnBit);
    DEE_Write(DEE_Encoder_CRCBitSize, Factory_CRCBit);
    DEE_Write(DEE_Encoder_MagicKey, FACTORY_MAGIC_KEY);

    DEE_Write(DEE_MB_Reg_Mode, MB_Reg_Mode);
}

uint8_t Sensor_MU_Config(void)
{
    uint8_t out_lsb;
    uint8_t out_msb;
    uint8_t status1;

    Enable_GPIO();

    MU_OutputBit_Config(Factory_SingleTurnBit, Factory_MultiTurnBit);

    // Read back parameters to verify configuration
    out_lsb = mu_read_param(&MU_OUT_LSB);
    out_msb = mu_read_param(&MU_OUT_MSB);

    // Verify configuration
    if (out_lsb != (19 - Factory_SingleTurnBit) ||
        out_msb != (Factory_MultiTurnBit + 5))
    {
        SPI1_Open(0);
        return 1;
    }

    mu_read_param(&MU_STATUS1);         // read status1 to update the output bit configuration
    mu_write_command(CMD_MU_WRITE_ALL); // write all parameters to EEPROM
    status1 = mu_read_param(&MU_STATUS1);

    if (status1 & MU_STATUS1_CRC_ERR)
    {
        return 1;
    }

    SPI1_Open(0);
    return 0;
}