#include "encoder.h"
#include "pins.h"
#include "cleardata.h"



ENCODER_CONFIG Encoder_Config;

void Delay_us(uint16_t us)
{
   DELAY_microseconds(us);
}

void Encoder_Init(void)
{
   uint16_t Magic_Value = 0;
   DEE_Read(DEE_Encoder_MagicKey, &Magic_Value);
   if(Magic_Value == Factory_Magic_Value)
   {
      Encoder_Config.MultiTurn_Bit = 0;
      Encoder_Config.SingleTurn_Bit = 0;
      Encoder_Config.CRC_Bit = 0;
      DEE_Read(DEE_Encoder_MultiTurnBitSize, &Encoder_Config.MultiTurn_Bit);
      DEE_Read(DEE_Encoder_SingleTurnBitSize, &Encoder_Config.SingleTurn_Bit);
      DEE_Read(DEE_Encoder_CRCBitSize, &Encoder_Config.CRC_Bit);



      uint16_t Multi_LowData;
      uint16_t Multi_HighData;

      uint16_t Single_LowData;
      uint16_t Single_HighData;
      
      DEE_Read(DEE_Encoder_MultiTurn_Data_L, &Multi_LowData);
      DEE_Read(DEE_Encoder_MultiTurn_Data_H, &Multi_HighData);
      DEE_Read(DEE_Encoder_SingleTurn_Data_L, &Single_LowData);
      DEE_Read(DEE_Encoder_SingleTurn_Data_H, &Single_HighData);
      Encoder_Config.MultiTurn_Data = ((uint32_t)Multi_HighData << 16) | Multi_LowData;
      Encoder_Config.SingleTurn_Data = ((uint32_t)Single_HighData << 16) | Single_LowData;
      if(Encoder_Config.MultiTurn_Data == 0xFFFFFFFF)
      {
        Encoder_Config.MultiTurn_Data = 0;
      }
      if(Encoder_Config.SingleTurn_Data == 0xFFFFFFFF)
      {
         Encoder_Config.SingleTurn_Data = 0;
      }

   }
   else
   {
      Encoder_Config.MultiTurn_Bit = 12;    //配置默認多圈位數
      Encoder_Config.SingleTurn_Bit = 19;   //配置默認單圈位數
      Encoder_Config.Warning_Bit = 1;
      Encoder_Config.Error_Bit = 1;
      Encoder_Config.CRC_Bit = 6;           //配置默認CRC位數
   }
}

void Encoder_Read_Data(void)
{
   MA_Clear();   //Start Readout Data
   Delay_us(1);
   Encoder_SSI_Read(Encoder_Config.MultiTurn_Bit,&Encoder_Config.MultiTurn_Data);
   Encoder_SSI_Read(Encoder_Config.SingleTurn_Bit,&Encoder_Config.SingleTurn_Data);
   Encoder_SSI_Read(Encoder_Config.Warning_Bit,&Encoder_Config.Warning_Data);
   Encoder_SSI_Read(Encoder_Config.Error_Bit,&Encoder_Config.Error_Data);
   Encoder_SSI_Read(Encoder_Config.CRC_Bit,&Encoder_Config.CRC_Data);
   Delay_us(5);
   MA_Set();
   if(Encoder_Config.Warning_Data || Encoder_Config.Error_Data ==0 )
   {
      LED0_SetLow();
   }
   else {
      LED0_SetHigh();
   }

   Encoder_Config.Raw_Data = ((uint64_t)Encoder_Config.MultiTurn_Data  << (Encoder_Config.SingleTurn_Bit + 2 + Encoder_Config.CRC_Bit)) |
                             ((uint64_t)Encoder_Config.SingleTurn_Data << (2 + Encoder_Config.CRC_Bit)) |
                             ((uint64_t)Encoder_Config.Warning_Data    << (Encoder_Config.CRC_Bit + 1)) |
                             ((uint64_t)Encoder_Config.Error_Data      << (Encoder_Config.CRC_Bit)) |
                             ((uint64_t)Encoder_Config.CRC_Data);
}

void Encoder_SSI_Read(uint8_t bit_num, uint32_t *data)
{
   uint32_t Data_Temp = 0;
   for(int i=0; i<bit_num; i++)
   {
      MA_Set();
      Delay_us(1);
      SLO_Get_Value();
      Data_Temp = (Data_Temp <<= 1) | SLO_Get_Value();
      MA_Clear();
      Delay_us(1);
   }
   if(data != 0)
   {
   *data = Data_Temp;
   }
}

void Encoder_Clear_Data(void) 
{
    Encoder_Read_Data();

    uint32_t Absolute_SingleTurn_Data = 0;
    Absolute_SingleTurn_Data = Encoder_Config.SingleTurn_Data;
}