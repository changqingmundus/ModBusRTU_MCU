#ifndef ENCODER_H
#define ENCODER_H

#include "dee.h"
#include "stdbool.h"
#include "stdint.h"
#include "delay.h"
#include "pins.h"

#define DEE_Encoder_MagicKey 0            //DEE存放編碼器出廠狀態數據
#define DEE_Encoder_MultiTurnBitSize 1    //DEE存放編碼器多圈位數配置
#define DEE_Encoder_SingleTurnBitSize 2   //DEE存放編碼器單圈位數配置
#define DEE_Encoder_CRCBitSize 3          //DEE存放編碼器CRC位數配置

#define DEE_Encoder_MultiTurn_Data_L 4    //DEE存放編碼器多圈低位數據
#define DEE_Encoder_MultiTurn_Data_H 5    //DEE存放編碼器多圈高位數據
#define DEE_Encoder_SingleTurn_Data_L 6   //DEE存放編碼器單圈低位數據
#define DEE_Encoder_SingleTurn_Data_H 7   //DEE存放編碼器單圈高位數據


#define MA_Clear() SCLK_MA_SetLow()     //設置SLO腳為低電平
#define MA_Set() SCLK_MA_SetHigh()      //設置SLO腳為高電平
#define SLO_Get_Value() MISO_SLO_GetValue()  //讀取MISO腳的電平值

#define Factory_Magic_Value 0x500A        //編碼器出廠特徵碼

typedef struct
{
    uint8_t MultiTurn_Bit;    //配置編碼器多圈位數
    uint8_t SingleTurn_Bit;   //配置編碼器單圈位數
    uint32_t Warning_Bit;           //配置編碼器警告位數
    bool Error_Bit;            //配置編碼器錯誤位數
    bool CRC_Bit;          //配置編碼器CRC位數
    //ReadoutData
    uint64_t Raw_Data;         //編碼器原始數據
    uint32_t MultiTurn_Data;   //編碼器多圈數據
    uint32_t SingleTurn_Data;  //編碼器單圈數據
    uint32_t Warning_Data;          //編碼器警告數據
    uint32_t Error_Data;           //編碼器錯誤數據
    uint32_t CRC_Data;         //編碼器CRC數據
}ENCODER_CONFIG;

extern ENCODER_CONFIG Encoder_Config;  //聲明為全局變量

void Delay_us(uint16_t us);

void Encoder_Init(void);
void Encoder_SSI_Read(uint8_t bit_num, uint32_t *data);
void Encoder_Read_Data(void);
void Encoder_Clear_Data(void);


#endif