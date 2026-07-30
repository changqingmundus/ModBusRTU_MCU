#ifndef ENCODER_H
#define ENCODER_H

#include "dee.h"
#include "stdbool.h"
#include "stdint.h"
#include "cleardata.h"
#include "delay.h"
#include "pins.h"

#define DEE_Encoder_MagicKey 0            //DEE存放編碼器出廠狀態數據
#define DEE_Encoder_MultiTurnBitSize 1    //DEE存放編碼器多圈位數配置
#define DEE_Encoder_SingleTurnBitSize 2   //DEE存放編碼器單圈位數配置
#define DEE_Encoder_CRCBitSize 3          //DEE存放編碼器CRC位數配置

#define DEE_ENCODER_ZERO_L 4              //DEE存放編碼器零點低位數據
#define DEE_ENCODER_ZERO_H 5              //DEE存放編碼器零點高位數據

#define DEE_Speed_Sample_Time 6

#define DEE_POSITION_OFFSET_L   7
#define DEE_POSITION_OFFSET_H   8
#define DEE_Direction 9

#define DEE_MultiTurn_Origin_Mode 10


#define MA_Clear() SCLK_MA_SetLow()     //設置SLO腳為低電平
#define MA_Set() SCLK_MA_SetHigh()      //設置SLO腳為高電平
#define SLO_Get_Value() MISO_SLO_GetValue()  //讀取MISO腳的電平值

#define FACTORY_MAGIC_KEY 0x500A        //編碼器出廠特徵碼

#define SPEED_SAMPLE_TIME_MS   10
#define SPEED_SAMPLE_FACTOR (60000 / SPEED_SAMPLE_TIME_MS)


typedef struct
{
    uint16_t MultiTurn_Bit;    //配置編碼器多圈位數
    uint16_t SingleTurn_Bit;   //配置編碼器單圈位數
    bool Warning_Bit;           //配置編碼器警告位數
    bool Error_Bit;            //配置編碼器錯誤位數
    uint16_t CRC_Bit;          //配置編碼器CRC位數
    //ReadoutData
    uint64_t Raw_Data;         //編碼器原始數據
    uint32_t MultiTurn_Data;   //編碼器多圈數據
    uint32_t SingleTurn_Data;  //編碼器單圈數據
    uint32_t Warning_Data;          //編碼器警告數據
    uint32_t Error_Data;           //編碼器錯誤數據
    uint32_t CRC_Data;         //編碼器CRC數據
}ENCODER_CONFIG;

extern ENCODER_CONFIG Encoder_Config;  //聲明為全局變量
extern uint32_t Zero_SingleTurn_Data; //當前單圈數據值

extern uint16_t Encoder_Write_Low;
extern uint16_t Encoder_Write_High;

extern uint16_t Direction_Config;
extern uint16_t MultiTurn_Origin_Mode;

extern uint32_t Encoder_RPM;
extern uint8_t Encoder_Direction;

extern volatile uint16_t Speed_Timer_Count;
extern volatile uint16_t Speed_Sample_Time;

void Delay_us(uint16_t us);

void Encoder_Init(void);
void Encoder_SSI_Read(uint8_t bit_num, uint32_t *data);
void Encoder_Read_Data(void);
void Encoder_Clear_Data(void);
uint32_t Encoder_Get_SingleTurn_Position(void);
uint32_t Encoder_Get_Position(void);
uint32_t Encoder_Get_Total_Position(void);
uint32_t Encoder_Get_Max_Position(void);

void Encoder_Update_Speed(void);
void Encoder_Set_Value(uint32_t set_value);
void Encoder_Load_Position_Offset(void);
void Encoder_Save_to_DEE(uint16_t Addr_L, uint16_t Addr_H, uint32_t Data);

#endif