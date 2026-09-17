#include "pvl.h"

IC_PVL_Config_t IC_PVL_Config =
    {
        .EN_PAR = 0,
        .EN_ERR = 2,
        .DIR = 1,
        .ST_GRAY = 0,
        .MT_GRAY = 0,
        .INT_MODE = 0,

        .OS = 0,
        .MT_BW = 3,

        .PCR = 0x1F,

        .EN_WRN = 0,
        .BAT_MON = 1,
        .A_MAX = 4,
        .IBIAS = 0,

        .NOMAG = 0,
        .ATHR = 0,
        .ONAX = 0,
        .POLEWID = 0,

        .I2C_POS = 0,
        .PCR_OUT = 0,
        .SYNC_BW = 3,
        .BAT_THR = 0,
        .HYS = 0,
        .ABQUAD = 1,

        .CRC_CFG = 0x8B,

        .MT_PREL = 1000,

        .CRC_CTR = 0x56};

/* ============================================================
 * Convert IC_PVL_Config to iC-PVL EEPROM data
 *
 * Output:
 *   data[0]  -> 0x00
 *   data[1]  -> 0x01
 *   ...
 *   data[12] -> 0x0C
 * ============================================================ */
void IC_PVL_ConfigToBytes(uint8_t *data)
{
    uint64_t mt_prel;

    /* 0x00 */
    data[0] = 0;
    data[0] = IC_PVL_FIELD_SET(data[0], IC_PVL_EN_PAR_Msk, IC_PVL_EN_PAR_Pos, IC_PVL_Config.EN_PAR);
    data[0] = IC_PVL_FIELD_SET(data[0], IC_PVL_EN_ERR_Msk, IC_PVL_EN_ERR_Pos, IC_PVL_Config.EN_ERR);
    data[0] = IC_PVL_FIELD_SET(data[0], IC_PVL_DIR_Msk, IC_PVL_DIR_Pos, IC_PVL_Config.DIR);
    data[0] = IC_PVL_FIELD_SET(data[0], IC_PVL_ST_GRAY_Msk, IC_PVL_ST_GRAY_Pos, IC_PVL_Config.ST_GRAY);
    data[0] = IC_PVL_FIELD_SET(data[0], IC_PVL_MT_GRAY_Msk, IC_PVL_MT_GRAY_Pos, IC_PVL_Config.MT_GRAY);
    data[0] = IC_PVL_FIELD_SET(data[0], IC_PVL_INT_MODE_Msk, IC_PVL_INT_MODE_Pos, IC_PVL_Config.INT_MODE);

    /* 0x01 */
    data[1] = 0;
    data[1] = IC_PVL_FIELD_SET(data[1], IC_PVL_OS_Msk, IC_PVL_OS_Pos, IC_PVL_Config.OS);
    data[1] = IC_PVL_FIELD_SET(data[1], IC_PVL_MT_BW_Msk, IC_PVL_MT_BW_Pos, IC_PVL_Config.MT_BW);

    /* 0x02 */
    data[2] = IC_PVL_Config.PCR;

    /* 0x03 */
    data[3] = 0;
    data[3] = IC_PVL_FIELD_SET(data[3], IC_PVL_EN_WRN_Msk, IC_PVL_EN_WRN_Pos, IC_PVL_Config.EN_WRN);
    data[3] = IC_PVL_FIELD_SET(data[3], IC_PVL_BAT_MON_Msk, IC_PVL_BAT_MON_Pos, IC_PVL_Config.BAT_MON);
    data[3] = IC_PVL_FIELD_SET(data[3], IC_PVL_A_MAX_Msk, IC_PVL_A_MAX_Pos, IC_PVL_Config.A_MAX);
    data[3] = IC_PVL_FIELD_SET(data[3], IC_PVL_IBIAS_Msk, IC_PVL_IBIAS_Pos, IC_PVL_Config.IBIAS);

    /* 0x04 */
    data[4] = 0;
    data[4] = IC_PVL_FIELD_SET(data[4], IC_PVL_NOMAG_Msk, IC_PVL_NOMAG_Pos, IC_PVL_Config.NOMAG);
    data[4] = IC_PVL_FIELD_SET(data[4], IC_PVL_ATHR_Msk, IC_PVL_ATHR_Pos, IC_PVL_Config.ATHR);
    data[4] = IC_PVL_FIELD_SET(data[4], IC_PVL_ONAX_Msk, IC_PVL_ONAX_Pos, IC_PVL_Config.ONAX);
    data[4] = IC_PVL_FIELD_SET(data[4], IC_PVL_POLEWID_Msk, IC_PVL_POLEWID_Pos, IC_PVL_Config.POLEWID);

    /* 0x05 */
    data[5] = 0;
    data[5] = IC_PVL_FIELD_SET(data[5], IC_PVL_I2C_POS_Msk, IC_PVL_I2C_POS_Pos, IC_PVL_Config.I2C_POS);
    data[5] = IC_PVL_FIELD_SET(data[5], IC_PVL_PCR_OUT_Msk, IC_PVL_PCR_OUT_Pos, IC_PVL_Config.PCR_OUT);
    data[5] = IC_PVL_FIELD_SET(data[5], IC_PVL_SYNC_BW_Msk, IC_PVL_SYNC_BW_Pos, IC_PVL_Config.SYNC_BW);
    data[5] = IC_PVL_FIELD_SET(data[5], IC_PVL_BAT_THR_Msk, IC_PVL_BAT_THR_Pos, IC_PVL_Config.BAT_THR);
    data[5] = IC_PVL_FIELD_SET(data[5], IC_PVL_HYS_Msk, IC_PVL_HYS_Pos, IC_PVL_Config.HYS);
    data[5] = IC_PVL_FIELD_SET(data[5], IC_PVL_ABQUAD_Msk, IC_PVL_ABQUAD_Pos, IC_PVL_Config.ABQUAD);

    /* 0x06 */
    data[6] = IC_PVL_Config.CRC_CFG;

    /* 0x07 ~ 0x0B : 40-bit MT_PREL */
    mt_prel = IC_PVL_Config.MT_PREL;

    data[7] = (uint8_t)(mt_prel >> 0);
    data[8] = (uint8_t)(mt_prel >> 8);
    data[9] = (uint8_t)(mt_prel >> 16);
    data[10] = (uint8_t)(mt_prel >> 24);
    data[11] = (uint8_t)(mt_prel >> 32);

    /* 0x0C */
    data[12] = IC_PVL_Config.CRC_CTR;
}

void PVL_OutputBit_Config(uint8_t multi_turn_bits)
{
    if (multi_turn_bits >= 9 && multi_turn_bits <= 40)
    {
        IC_PVL_Config.MT_BW = multi_turn_bits - 9;
    }
    else
    {
        IC_PVL_Config.MT_BW = PVL_DEFAULT_MT_BW;
    }
}