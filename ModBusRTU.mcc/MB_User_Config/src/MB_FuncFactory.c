#include "MB_FunFactory.h"
#include "encoder.h"
#include "mu_1sf_driver.h"
#include "pz_1sf_driver.h"
#include "pvl.h"
#include "spi1.h"

#define FACTORY_SENSOR_CONFIG 0x0000
#define FACTORY_OUTPUT_CONFIG 0x0001
#define FACTORY_SAVE 0x0002

const struct factory_param FACTORY_SENSOR_TYPE = {.addr = FACTORY_SENSOR_CONFIG, .pos = 0, .len = 4};
const struct factory_param FACTORY_ST_CHIP = {.addr = FACTORY_SENSOR_CONFIG, .pos = 4, .len = 4};
const struct factory_param FACTORY_MT_CHIP = {.addr = FACTORY_SENSOR_CONFIG, .pos = 8, .len = 4};
const struct factory_param FACTORY_SINGLE_TURN_BIT = {.addr = FACTORY_OUTPUT_CONFIG, .pos = 0, .len = 2};
const struct factory_param FACTORY_MULTI_TURN_BIT = {.addr = FACTORY_OUTPUT_CONFIG, .pos = 2, .len = 2};
const struct factory_param FACTORY_CRC_BIT = {.addr = FACTORY_OUTPUT_CONFIG, .pos = 4, .len = 6};
const struct factory_param FACTORY_MB_REG_MODE = {.addr = FACTORY_OUTPUT_CONFIG, .pos = 10, .len = 6};

uint8_t Factory_SingleTurnBit = 0;
uint8_t Factory_MultiTurnBit = 0;
uint8_t Factory_CRCBit = 0;

static uint16_t Factory_GetField(uint16_t value, const struct factory_param *param);

SensorType_t Sensor_Type;
SensorChip_t Sensor_ST_Chip;
SensorChip_t Sensor_MT_Chip;
SensorChip_t Sensor_Chip;

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
        /* =================================================
         * SENSOR CONFIG
         *
         * Bit  3:0   SENSOR_TYPE
         * Bit  7:4   ST_CHIP
         * Bit 11:8   MT_CHIP
         * ================================================= */
        case FACTORY_SENSOR_CONFIG:
        {
            USHORT sensor_type;
            USHORT st_chip;
            USHORT mt_chip;

            sensor_type = Factory_GetField(value, &FACTORY_SENSOR_TYPE);

            st_chip = Factory_GetField(value, &FACTORY_ST_CHIP);
            mt_chip = Factory_GetField(value, &FACTORY_MT_CHIP);

            if (sensor_type != SENSOR_TYPE_SINGLE_TURN && // Sensor Type
                sensor_type != SENSOR_TYPE_MULTI_TURN)
            {
                return MB_EX_ILLEGAL_DATA_VALUE;
            }
            Sensor_Type = (SensorType_t)sensor_type;

            if (st_chip == SENSOR_ST_CHIP_MU) // Sensor ST Chip
            {
                Sensor_ST_Chip = SENSOR_CHIP_MU;
            }
            else if (st_chip == SENSOR_ST_CHIP_PZ)
            {
                Sensor_ST_Chip = SENSOR_CHIP_PZ;
            }
            else
            {
                return MB_EX_ILLEGAL_DATA_VALUE;
            }

            if (mt_chip == SENSOR_MT_CHIP_PVL) // Sensor MT Chip
            {
                Sensor_MT_Chip = SENSOR_CHIP_PVL;
            }
            else if (mt_chip == SENSOR_MT_CHIP_NONE)
            {
                Sensor_MT_Chip = SENSOR_CHIP_NONE;
            }
            else
            {
                return MB_EX_ILLEGAL_DATA_VALUE;
            }

            break;
        }
        /* =================================================
         * OUTPUT CONFIG
         *
         * Bit  1:0   MB_REG_MODE
         * Bit  3:2   CRC
         * Bit  9:4   SINGLE_TURN_BIT
         * Bit 15:10  MULTI_TURN_BIT
         * ================================================= */
        case FACTORY_OUTPUT_CONFIG:
        {
            USHORT mb_reg_mode;
            USHORT crc_code;
            USHORT single_turn_bit;
            USHORT multi_turn_bit;

            mb_reg_mode = Factory_GetField(value, &FACTORY_MB_REG_MODE);
            crc_code = Factory_GetField(value, &FACTORY_CRC_BIT);
            single_turn_bit = Factory_GetField(value, &FACTORY_SINGLE_TURN_BIT);
            multi_turn_bit = Factory_GetField(value, &FACTORY_MULTI_TURN_BIT);

            /*
             * Modbus register mode
             */
            if (mb_reg_mode > MB_REG_FORCE32)
            {
                // return MB_EX_ILLEGAL_DATA_VALUE;
            }

            if (crc_code != 0x01 &&
                crc_code != 0x02)
            {
                // return MB_EX_ILLEGAL_DATA_VALUE;
            }

            if (single_turn_bit > 32 ||
                multi_turn_bit > 32)
            {
                return MB_EX_ILLEGAL_DATA_VALUE;
            }

            MB_Reg_Mode = mb_reg_mode;

            if (crc_code == 0x01)
            {
                Factory_CRCBit = 6;
            }
            else
            {
                Factory_CRCBit = 16;
            }

            Factory_SingleTurnBit = single_turn_bit;
            Factory_MultiTurnBit = multi_turn_bit;

            break;
        }

        case FACTORY_SAVE:
            if (value != FACTORY_Save_KEY)
            {
                return MB_EX_ILLEGAL_DATA_VALUE;
            }

            /*
             * Validate complete configuration
             */
            if (Sensor_Config_Validate() != 0)
            {
                return MB_EX_ILLEGAL_DATA_VALUE;
            }

            if (Sensor_Config_Save() != 0)
            {
                return MB_EX_SLAVE_DEVICE_FAILURE;
            }

            Factory_Config_SaveDEE();

            break;

        default:

            return MB_EX_ILLEGAL_DATA_ADDRESS;
        }

        pData += 2;
        address++;
    }

    /* Response */
    pucFrame[0] = MB_FUNC_FACTORY;
    pucFrame[1] = 0x88;

    pucFrame[2] = (UCHAR)(address >> 8);
    pucFrame[3] = (UCHAR)(address & 0xFF);

    pucFrame[4] = (UCHAR)(quantity_written >> 8);
    pucFrame[5] = (UCHAR)(quantity_written & 0xFF);

    *usLen = 6;

    return MB_EX_NONE;
}

static uint16_t Factory_GetField(uint16_t value, const struct factory_param *param)
{
    uint16_t mask;

    mask = (uint16_t)((1UL << param->len) - 1UL);

    return (value >> param->pos) & mask;
}

uint8_t Sensor_Config_Validate(void)
{
    /*
     * Single-turn resolution
     */
    if (Factory_SingleTurnBit == 0 ||
        Factory_SingleTurnBit > 32)
    {
        return 1;
    }

    /*
     * Multi-turn configuration
     */
    if (Sensor_Type == SENSOR_TYPE_SINGLE_TURN)
    {
        if (Factory_MultiTurnBit != 0)
        {
            return 1;
        }

        if (Sensor_MT_Chip != SENSOR_CHIP_NONE)
        {
            return 1;
        }
    }
    else if (Sensor_Type == SENSOR_TYPE_MULTI_TURN)
    {
        if (Factory_MultiTurnBit == 0 ||
            Factory_MultiTurnBit > 32)
        {
            return 1;
        }

        if (Sensor_MT_Chip == SENSOR_CHIP_NONE)
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }

    /*
     * Single-turn chip
     */
    if (Sensor_ST_Chip != SENSOR_CHIP_MU &&
        Sensor_ST_Chip != SENSOR_CHIP_PZ)
    {
        return 1;
    }

    /*
     * CRC
     */
    if (Factory_CRCBit != 6 &&
        Factory_CRCBit != 16)
    {
        return 1;
    }

    /*
     * Modbus register mode
     */
    if (MB_Reg_Mode > MB_REG_FORCE32)
    {
        return 1;
    }

    return 0;
}

void Factory_Config_SaveDEE(void)
{
    DEE_Write(DEE_Encoder_MultiTurnBitSize, Factory_MultiTurnBit);
    DEE_Write(DEE_Encoder_SingleTurnBitSize, Factory_SingleTurnBit);
    DEE_Write(DEE_Encoder_CRCBitSize, Factory_CRCBit);
    DEE_Write(DEE_Encoder_MagicKey, FACTORY_MAGIC_KEY);

    DEE_Write(DEE_MB_Reg_Mode, MB_Reg_Mode);
}

uint8_t Sensor_Config_Save(void)
{
    switch (Sensor_Chip)
    {
    case SENSOR_CHIP_MU:
        return Sensor_MU_Config();

    case SENSOR_CHIP_PZ:
        return Sensor_PZ_Config();

    default:
        return 1;
    }
}

uint8_t Sensor_MU_Config(void)
{
    uint8_t out_lsb;
    uint8_t out_msb;
    uint8_t status1;

    Enable_GPIO();
    Change_To_Biss(); // change protocol to biss-c

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

    /*
     * If multiturn is enabled, write PVL configuration
     * to the external EEPROM through iC-MU I2C.
     */
    if (Sensor_MT_Config(SENSOR_CHIP_PVL) != 0)
    {
        SPI1_Open(0);
        return 1;
    }

    mu_read_param(&MU_STATUS1); // read status1 to update the output bit configuration
    mu_write_command(CMD_MU_I2C_COM);

    Sensor_SetProtocol(SENSOR_PROTOCOL_EXTSSI); // change to extssi mode

    mu_read_param(&MU_STATUS1);
    mu_write_command(CMD_MU_WRITE_ALL); // write all parameters to EEPROM
    status1 = mu_read_param(&MU_STATUS1);

    if (status1 & MU_STATUS1_CRC_ERR)
    {
        return 1;
    }

    SPI1_Open(0);
    return 0;
}

uint8_t Sensor_PZ_Config(void)
{
    uint8_t out_lsb;
    uint8_t out_msb;

    Enable_GPIO();
    Change_To_Biss(); // change protocol to biss-c

    pz_write_param(&PZ_ST_PDL, Factory_SingleTurnBit);
    pz_write_param(&PZ_MT_PDL, Factory_MultiTurnBit);

    // Read back parameters to verify configuration
    out_lsb = pz_read_param(&PZ_ST_PDL);
    out_msb = pz_read_param(&PZ_MT_PDL);

    // Verify configuration
    if (out_lsb != (Factory_SingleTurnBit) ||
        out_msb != (Factory_MultiTurnBit))
    {
        SPI1_Open(0);
        return 1;
    }

    pz_write_param(&PZ_BISS_ST_DL, Factory_SingleTurnBit);
    pz_write_param(&PZ_BISS_MT_DL, Factory_MultiTurnBit);

    // Read back parameters to verify configuration
    out_lsb = pz_read_param(&PZ_BISS_ST_DL);
    out_msb = pz_read_param(&PZ_BISS_MT_DL);

    // Verify configuration
    if (out_lsb != (Factory_SingleTurnBit) ||
        out_msb != (Factory_MultiTurnBit))
    {
        SPI1_Open(0);
        return 1;
    }

    /*
     * If multiturn is enabled, write PVL configuration
     * to the external EEPROM through I2C.
     */
    if (Sensor_MT_Config(SENSOR_CHIP_PVL) != 0)
    {
        SPI1_Open(0);
        return 1;
    }

    //pz_read_param(&); // read status1 to update the output bit configuration
    // pz_write_command(CMD_MU_I2C_COM);

    Sensor_SetProtocol(SENSOR_PROTOCOL_EXTSSI); // change to extssi mode

    pz_write_command(PZ_COMMAND_CONF_WRITE_ALL); // write all parameters to EEPROM

    uint32_t diag;
    diag = (uint32_t)pz_read_param(&PZ_DIAG);

    if (diag != 0)
    {
        return 1;
    }

    SPI1_Open(0);
    return 0;
}

uint8_t Sensor_MT_Config(SensorChip_t chip)
{
    switch (chip)
    {
    case SENSOR_CHIP_PVL:

        if (Factory_MultiTurnBit != 0)
        {

            PVL_OutputBit_Config(Factory_MultiTurnBit); // Configure PVL output bits

            if (MU_Load_PVL_Config() != 0) // Load PVL configuration to MU USER_EXCHANGE_REGISTERS
            {
                return 1;
            }

            MU_I2C_Transfer(0xA0, 0x60, 0x6C, 0x40); // Write PVL configuration to EEPROM 0x40 ~ 0x4C

            /*if (MU_WriteRegister_Verify(0x6D, 0x05) != 0) // Send SCLR command to iC-PVL
            {
                return 1;
            }
            MU_I2C_Transfer(0xC0, 0x6D, 0x6D, 0x11);

            if (MU_WriteRegister_Verify(0x6D, 0x03) != 0) // Send REBOOT command to iC-PVL
            {
                return 1;
            }
            MU_I2C_Transfer(0xC0, 0x6D, 0x6D, 0x11);

            if (PVL_Check_Status() != 0) // Check PVL status after reboot
            {
                return 1;
            }*/
        }

        break;

    default:
        return 1;
    }

    return 0;
}