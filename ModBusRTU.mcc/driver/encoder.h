#ifndef ENCODER_H
#define ENCODER_H

#include "dee.h"
#include "stdbool.h"
#include "stdint.h"
#include "cleardata.h"
#include "delay.h"
#include "pins.h"

#define DEE_Encoder_MagicKey 0          // DEE存放編碼器出廠狀態數據
#define DEE_Encoder_MultiTurnBitSize 1  // DEE存放編碼器多圈位數配置
#define DEE_Encoder_SingleTurnBitSize 2 // DEE存放編碼器單圈位數配置
#define DEE_Encoder_CRCBitSize 3        // DEE存放編碼器CRC位數配置

#define DEE_Speed_Update_Period 4 // DEE存放編碼器速度采集周期

#define DEE_POSITION_OFFSET_L 5  // DEE存放編碼器全部值16位
#define DEE_POSITION_OFFSET_H 6  // DEE存放編碼器全部值32位
#define DEE_POSITION_OFFSET_LL 7 // DEE存放編碼器全部值48位
#define DEE_POSITION_OFFSET_HH 8 // DEE存放編碼器全部值64位
#define DEE_Direction 9          // DEE存放編碼器旋轉方向

#define DEE_MultiTurn_Origin_Mode 10 // DEE存放編碼器多圈原點數據

#define MA_Clear() SCLK_MA_SetLow()         // 設置SLO腳為低電平
#define MA_Set() SCLK_MA_SetHigh()          // 設置SLO腳為高電平
#define SLO_Get_Value() MISO_SLO_GetValue() // 讀取MISO腳的電平值

#define FACTORY_MAGIC_KEY 0x500A // 編碼器出廠特徵碼

#define POSITION_DEAD_BAND 3 // 旋转方向滤波
#define SPEED_SAMPLE_TIME_MS 10
#define SPEED_SAMPLE_FACTOR (60000 / SPEED_SAMPLE_TIME_MS)

typedef struct
{
    uint16_t MultiTurn_Bit;  // 配置編碼器多圈位數
    uint16_t SingleTurn_Bit; // 配置編碼器單圈位數
    bool Warning_Bit;        // 配置編碼器警告位數
    bool Error_Bit;          // 配置編碼器錯誤位數
    uint16_t CRC_Bit;        // 配置編碼器CRC位數
    // ReadoutData
    uint64_t Raw_Data;        // 編碼器原始數據
    uint32_t MultiTurn_Data;  // 編碼器多圈數據
    uint32_t SingleTurn_Data; // 編碼器單圈數據
    uint32_t Warning_Data;    // 編碼器警告數據
    uint32_t Error_Data;      // 編碼器錯誤數據
    uint32_t CRC_Data;        // 編碼器CRC數據
} ENCODER_CONFIG;

/**/
typedef enum
{
    BISS_FRAME_MIN = 0, // Simplified Frame / 精简Biss帧
    BISS_FRAME_SHORT,   // Short Frame      / 无过程数据
    BISS_FRAME_LONG     // Long Frame       / 带过程数据
} BissFrameMode_t;

extern ENCODER_CONFIG Encoder_Config; // 聲明為全局變量
extern uint32_t Zero_SingleTurn_Data; // 當前單圈數據值

extern uint16_t Encoder_Write_Low;
extern uint16_t Encoder_Write_High;

extern uint16_t Direction_Config;
extern uint16_t MultiTurn_Origin_Mode;

extern uint32_t Encoder_RPM;
extern uint8_t Encoder_Direction;

extern uint16_t Speed_Timer_Count;
extern uint16_t Speed_Update_Period;

extern uint16_t Data_Temp;

uint8_t Encoder_Update_Direction(int64_t diff);

uint64_t Encoder_Get_Position(void);
uint64_t Encoder_Get_Total_Position(void);
uint64_t Encoder_Get_Max_Position(void);

bool Encoder_SSI_Read(uint8_t bit_num, uint8_t *rx_data);

void Delay_us(uint16_t us);

void Encoder_Init(void);
void Encoder_Read_Data(void);
void Encoder_Clear_Data(void);

void MU_OutputBit_Config(uint8_t single_turn_bits, uint8_t multi_turn_bits);

void Enable_GPIO(void);

void Biss_CRC_Update(uint8_t bit);

void Biss_CRC(void); // For Simplified Frame

void Biss_CDM0(void);
void Biss_CDM1(void);
void Biss_Short_CDM0(void);
void Biss_Short_CDM1(void);
void Biss_Long_CDM0(void);
void Biss_Long_CDM1(void);

/**
 * @brief This function enables short frame mode for multiple BiSS slaves in a daisy-chain configuration.
 *
 * @retval None
 */
void Biss_Enable_Short_Frame(void); // for multi biss slavers

/**
 * @brief This function can be used to read one byte from a BiSS register.
 *
 * @param mode     BiSS frame mode (short or long).
 * @param cts      Control communication CTS bit.
 * @param bissid   BiSS slave ID.
 * @param bissaddr BiSS register address.
 * @param data_rx  Pointer to the buffer where the received data is written.
 * @retval None
 */
void Biss_ReadByte(BissFrameMode_t mode, uint8_t cts, uint8_t bissid, uint8_t bissaddr, uint8_t *data);

void Biss_WriteByteHeader(BissFrameMode_t mode, uint8_t cts, uint8_t bissid, uint8_t bissaddr);
void Biss_WriteByte(BissFrameMode_t mode, uint8_t *write_data, uint8_t data_len);

void Encoder_Update_Speed(void);
void Encoder_Set_Value(uint32_t set_value);
void Encoder_Load_Position_Offset(void);
void Encoder_Save_to_DEE(uint16_t Addr_L, uint16_t Addr_H, uint32_t Data);

#endif