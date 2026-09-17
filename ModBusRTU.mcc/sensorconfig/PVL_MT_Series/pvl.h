#ifndef PVL_H
#define PVL_H

#include <stdint.h>

/* ============================================================
 * iC-PVL I2C Slave
 * I2C Slave ID = 0b1100001 = 0x61
 * ============================================================ */

#define IC_PVL_I2C_ADDR 0x61U

/* ============================================================
 * iC-PVL Register Address
 *
 * Table 2: REGISTER MAP
 * ============================================================ */

#define IC_PVL_REG_00 0x00U
#define IC_PVL_REG_01 0x01U
#define IC_PVL_REG_02 0x02U
#define IC_PVL_REG_03 0x03U
#define IC_PVL_REG_04 0x04U
#define IC_PVL_REG_05 0x05U
#define IC_PVL_REG_06 0x06U
#define IC_PVL_REG_07 0x07U
#define IC_PVL_REG_08 0x08U
#define IC_PVL_REG_09 0x09U
#define IC_PVL_REG_0A 0x0AU
#define IC_PVL_REG_0B 0x0BU
#define IC_PVL_REG_0C 0x0CU
#define IC_PVL_REG_0D 0x0DU
#define IC_PVL_REG_0E 0x0EU
#define IC_PVL_REG_0F 0x0FU
#define IC_PVL_REG_10 0x10U
#define IC_PVL_REG_11 0x11U
#define IC_PVL_REG_12 0x12U

/* ============================================================
 * 0x00 - Interface and Hall Signal Processing
 *
 * Bit 7:6  EN_PAR
 * Bit 5:4  EN_ERR
 * Bit 3    DIR
 * Bit 2    ST_GRAY
 * Bit 1    MT_GRAY
 * Bit 0    INT_MODE
 * ============================================================ */

#define IC_PVL_EN_PAR_Pos 6U
#define IC_PVL_EN_PAR_Msk 0xC0U

#define IC_PVL_EN_ERR_Pos 4U
#define IC_PVL_EN_ERR_Msk 0x30U

#define IC_PVL_DIR_Pos 3U
#define IC_PVL_DIR_Msk 0x08U

#define IC_PVL_ST_GRAY_Pos 2U
#define IC_PVL_ST_GRAY_Msk 0x04U

#define IC_PVL_MT_GRAY_Pos 1U
#define IC_PVL_MT_GRAY_Msk 0x02U

#define IC_PVL_INT_MODE_Pos 0U
#define IC_PVL_INT_MODE_Msk 0x01U

/* ============================================================
 * 0x01 - Interface and Hall Signal Processing
 *
 * Bit 7:4  OS
 * Bit 3:0  MT_BW
 * ============================================================ */

#define IC_PVL_OS_Pos 4U
#define IC_PVL_OS_Msk 0xF0U

#define IC_PVL_MT_BW_Pos 0U
#define IC_PVL_MT_BW_Msk 0x0FU

/* ============================================================
 * 0x02 - PCR
 *
 * Bit 7:0  PCR
 * ============================================================ */

#define IC_PVL_PCR_Pos 0U
#define IC_PVL_PCR_Msk 0xFFU

/* ============================================================
 * 0x03 - Battery Monitor, Bias and Oscillators
 *
 * Bit 7    EN_WRN
 * Bit 6    BAT_MON
 * Bit 5:3  A_MAX
 * Bit 2:0  IBIAS
 * ============================================================ */

#define IC_PVL_EN_WRN_Pos 7U
#define IC_PVL_EN_WRN_Msk 0x80U

#define IC_PVL_BAT_MON_Pos 6U
#define IC_PVL_BAT_MON_Msk 0x40U

#define IC_PVL_A_MAX_Pos 3U
#define IC_PVL_A_MAX_Msk 0x38U

#define IC_PVL_IBIAS_Pos 0U
#define IC_PVL_IBIAS_Msk 0x07U

/* ============================================================
 * 0x04 - Miscellaneous
 *
 * Bit 7    0
 * Bit 6:5  NOMAG
 * Bit 4:3  ATHR
 * Bit 2    ONAX
 * Bit 1:0  POLEWID
 * ============================================================ */

#define IC_PVL_NOMAG_Pos 5U
#define IC_PVL_NOMAG_Msk 0x60U

#define IC_PVL_ATHR_Pos 3U
#define IC_PVL_ATHR_Msk 0x18U

#define IC_PVL_ONAX_Pos 2U
#define IC_PVL_ONAX_Msk 0x04U

#define IC_PVL_POLEWID_Pos 0U
#define IC_PVL_POLEWID_Msk 0x03U

/* ============================================================
 * 0x05 - Miscellaneous
 *
 * Bit 7    I2C_POS
 * Bit 6    PCR_OUT
 * Bit 5:4  SYNC_BW
 * Bit 3:2  BAT_THR
 * Bit 1    HYS
 * Bit 0    ABQUAD
 * ============================================================ */

#define IC_PVL_I2C_POS_Pos 7U
#define IC_PVL_I2C_POS_Msk 0x80U

#define IC_PVL_PCR_OUT_Pos 6U
#define IC_PVL_PCR_OUT_Msk 0x40U

#define IC_PVL_SYNC_BW_Pos 4U
#define IC_PVL_SYNC_BW_Msk 0x30U

#define IC_PVL_BAT_THR_Pos 2U
#define IC_PVL_BAT_THR_Msk 0x0CU

#define IC_PVL_HYS_Pos 1U
#define IC_PVL_HYS_Msk 0x02U

#define IC_PVL_ABQUAD_Pos 0U
#define IC_PVL_ABQUAD_Msk 0x01U

/* ============================================================
 * 0x06 - CRC Configuration
 *
 * CRC_CFG(7:0)
 *
 * Register is inverted according to datasheet.
 * ============================================================ */

#define IC_PVL_CRC_CFG_Pos 0U
#define IC_PVL_CRC_CFG_Msk 0xFFU

/* ============================================================
 * 0x07 ~ 0x0B
 *
 * PCR_OUT = 0:
 *     Multiturn Counter Current Count
 *
 * 0x07 MT_COUNT(7:0)
 * 0x08 MT_COUNT(15:8)
 * 0x09 MT_COUNT(23:16)
 * 0x0A MT_COUNT(31:24)
 * 0x0B MT_COUNT(39:32)
 *
 * PCR_OUT = 1:
 *     0x07 PCR(7:0)
 *     0x08 MT_COUNT(7:0)
 *     0x09 MT_COUNT(15:8)
 *     0x0A MT_COUNT(23:16)
 *     0x0B MT_COUNT(31:24)
 * ============================================================ */

#define IC_PVL_MT_COUNT_0 0x07U
#define IC_PVL_MT_COUNT_1 0x08U
#define IC_PVL_MT_COUNT_2 0x09U
#define IC_PVL_MT_COUNT_3 0x0AU
#define IC_PVL_MT_COUNT_4 0x0BU

#define IC_PVL_PCR_OUT_DATA 0x07U

/* ============================================================
 * 0x0C - CRC Counter
 *
 * CRC_CTR(7:0)
 *
 * Register is inverted according to datasheet.
 * ============================================================ */

#define IC_PVL_CRC_CTR_Pos 0U
#define IC_PVL_CRC_CTR_Msk 0xFFU

/* ============================================================
 * 0x0D - Synchronization Bits
 *
 * I2C slave mode: READ ONLY
 *
 * Bit 7:3  0
 * Bit 2:0  SYNC
 * ============================================================ */

#define IC_PVL_SYNC_Pos 0U
#define IC_PVL_SYNC_Msk 0x07U

/* ============================================================
 * 0x0E - Reserved
 * ============================================================ */

/* ============================================================
 * 0x0F - Chip Release
 *
 * I2C slave mode: READ ONLY
 *
 * CHIP_REL(7:0)
 * ============================================================ */

#define IC_PVL_CHIP_REL_Pos 0U
#define IC_PVL_CHIP_REL_Msk 0xFFU

/* ============================================================
 * 0x10 - Status Register
 *
 * I2C slave mode
 *
 * Bit 7  PRESET
 * Bit 6  PDR
 * Bit 5  BAT_WRN
 * Bit 4  BAT_ERR
 * Bit 3  POS_ERR
 * Bit 2  CTR_ERR
 * Bit 1  CFG_ERR
 * Bit 0  STUP_ERR
 * ============================================================ */

#define IC_PVL_PRESET_Pos 7U
#define IC_PVL_PRESET_Msk 0x80U

#define IC_PVL_PDR_Pos 6U
#define IC_PVL_PDR_Msk 0x40U

#define IC_PVL_BAT_WRN_Pos 5U
#define IC_PVL_BAT_WRN_Msk 0x20U

#define IC_PVL_BAT_ERR_Pos 4U
#define IC_PVL_BAT_ERR_Msk 0x10U

#define IC_PVL_POS_ERR_Pos 3U
#define IC_PVL_POS_ERR_Msk 0x08U

#define IC_PVL_CTR_ERR_Pos 2U
#define IC_PVL_CTR_ERR_Msk 0x04U

#define IC_PVL_CFG_ERR_Pos 1U
#define IC_PVL_CFG_ERR_Msk 0x02U

#define IC_PVL_STUP_ERR_Pos 0U
#define IC_PVL_STUP_ERR_Msk 0x01U

/* ============================================================
 * 0x11 - Command Register
 *
 * I2C slave mode: WRITE ONLY
 *
 * CMD(7:0)
 * ============================================================ */

#define IC_PVL_CMD_Pos 0U
#define IC_PVL_CMD_Msk 0xFFU

/* ============================================================
 * 0x12 - Extended Status Register
 *
 * I2C slave mode
 *
 * Bit 7  SLEEP_ST
 * Bit 6  NOMAG_ST
 * Bit 5  ACTIVE_ST
 * Bit 4  POWON_ST
 * Bit 3  Unused
 * Bit 2  NOMAG_L
 * Bit 1  MAG_ERR
 * Bit 0  AMPL_ERR
 * ============================================================ */

#define IC_PVL_SLEEP_ST_Pos 7U
#define IC_PVL_SLEEP_ST_Msk 0x80U

#define IC_PVL_NOMAG_ST_Pos 6U
#define IC_PVL_NOMAG_ST_Msk 0x40U

#define IC_PVL_ACTIVE_ST_Pos 5U
#define IC_PVL_ACTIVE_ST_Msk 0x20U

#define IC_PVL_POWON_ST_Pos 4U
#define IC_PVL_POWON_ST_Msk 0x10U

#define IC_PVL_NOMAG_L_Pos 2U
#define IC_PVL_NOMAG_L_Msk 0x04U

#define IC_PVL_MAG_ERR_Pos 1U
#define IC_PVL_MAG_ERR_Msk 0x02U

#define IC_PVL_AMPL_ERR_Pos 0U
#define IC_PVL_AMPL_ERR_Msk 0x01U

/* ============================================================
 * MCU EEPROM Mapping
 *
 * MCU EEPROM:
 *
 * 0x40 ~ 0x4C
 *
 * Corresponding to iC-PVL EEPROM configuration:
 *
 * 0x00 ~ 0x0C
 *
 * ============================================================ */

#define IC_PVL_EE_START 0x40U
#define IC_PVL_EE_END 0x4CU
#define IC_PVL_EE_SIZE 13U

#define IC_PVL_EE_00 0x40U
#define IC_PVL_EE_01 0x41U
#define IC_PVL_EE_02 0x42U
#define IC_PVL_EE_03 0x43U
#define IC_PVL_EE_04 0x44U
#define IC_PVL_EE_05 0x45U
#define IC_PVL_EE_06 0x46U
#define IC_PVL_EE_07 0x47U
#define IC_PVL_EE_08 0x48U
#define IC_PVL_EE_09 0x49U
#define IC_PVL_EE_0A 0x4AU
#define IC_PVL_EE_0B 0x4BU
#define IC_PVL_EE_0C 0x4CU

/* ============================================================
 * Default EEPROM Configuration
 *
 * HEX file:
 *
 * 0x00 = 0x28
 * 0x01 = 0x03
 * 0x02 = 0x1F
 * 0x03 = 0x60
 * 0x04 = 0x00
 * 0x05 = 0x31
 * 0x06 = 0x8B
 * 0x07 = 0xE8
 * 0x08 = 0x03
 * 0x09 = 0x00
 * 0x0A = 0x00
 * 0x0B = 0x00
 * 0x0C = 0x56
 * ============================================================ */

#define IC_PVL_DEFAULT_00 0x28U
#define IC_PVL_DEFAULT_01 0x03U
#define IC_PVL_DEFAULT_02 0x1FU
#define IC_PVL_DEFAULT_03 0x60U
#define IC_PVL_DEFAULT_04 0x00U
#define IC_PVL_DEFAULT_05 0x31U
#define IC_PVL_DEFAULT_06 0x8BU
#define IC_PVL_DEFAULT_07 0xE8U
#define IC_PVL_DEFAULT_08 0x03U
#define IC_PVL_DEFAULT_09 0x00U
#define IC_PVL_DEFAULT_0A 0x00U
#define IC_PVL_DEFAULT_0B 0x00U
#define IC_PVL_DEFAULT_0C 0x56U

/* ============================================================
 * Helper macros
 * ============================================================ */

#define IC_PVL_FIELD_GET(reg, mask, pos) (((reg) & (mask)) >> (pos))

#define IC_PVL_FIELD_SET(reg, mask, pos, value)                                \
  (((reg) & ~(mask)) | (((value) << (pos)) & (mask)))

typedef struct {
  uint8_t EN_PAR;
  uint8_t EN_ERR;
  uint8_t DIR;
  uint8_t ST_GRAY;
  uint8_t MT_GRAY;
  uint8_t INT_MODE;

  uint8_t OS;
  uint8_t MT_BW;

  uint8_t PCR;

  uint8_t EN_WRN;
  uint8_t BAT_MON;
  uint8_t A_MAX;
  uint8_t IBIAS;

  uint8_t NOMAG;
  uint8_t ATHR;
  uint8_t ONAX;
  uint8_t POLEWID;

  uint8_t I2C_POS;
  uint8_t PCR_OUT;
  uint8_t SYNC_BW;
  uint8_t BAT_THR;
  uint8_t HYS;
  uint8_t ABQUAD;

  uint8_t CRC_CFG;

  uint64_t MT_PREL;

  uint8_t CRC_CTR;

} IC_PVL_Config_t;

extern IC_PVL_Config_t IC_PVL_Config;

void IC_PVL_ConfigToBytes(uint8_t *data);

#endif /* PVL_H */