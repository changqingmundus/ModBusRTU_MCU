#include "encoder.h"
#include "pins.h"
#include "cleardata.h"

uint32_t Zero_SingleTurn_Data = 0;
uint32_t Position_Offset = 0;

uint16_t Encoder_Write_Low = 0;
uint16_t Encoder_Write_High = 0;
static uint32_t Last_Position = 0;
static uint8_t Speed_Init_Flag = 0;

uint16_t Direction_Config = 1;
uint16_t MultiTurn_Origin_Mode = 1;

uint32_t Encoder_RPM = 0;
uint8_t Encoder_Direction = 0;

volatile uint16_t Speed_Timer_Count = 0;
volatile uint16_t Speed_Sample_Time = 10;

ENCODER_CONFIG Encoder_Config;

uint32_t Encoder_Get_Max_Position(void)
{
   return ((1UL << Encoder_Config.SingleTurn_Bit) *
           (1UL << Encoder_Config.MultiTurn_Bit)) -
          1;
}

uint32_t Encoder_Get_Position(void)
{
   int64_t position;
   uint32_t max_position;

   max_position = Encoder_Get_Max_Position();

   position =
       (int64_t)Encoder_Get_Total_Position() +
       (int64_t)Position_Offset;

   // 环绕处理
   while (position < 0)
   {
      position += ((int64_t)max_position + 1);
   }

   while (position > max_position)
   {
      position -= ((int64_t)max_position + 1);
   }

   // 逆向递增
   if (Direction_Config == 0x02)
   {
      position =
          max_position - position;
   }

   return (uint32_t)position;
}

uint32_t Encoder_Get_Total_Position(void)
{
   uint32_t position;

   position =
       ((uint32_t)Encoder_Config.MultiTurn_Data
        << Encoder_Config.SingleTurn_Bit) |
       Encoder_Config.SingleTurn_Data;

   return position;
}

uint32_t Encoder_Get_SingleTurn_Position(void)
{
   uint32_t single_value;

   single_value =
       (Encoder_Config.SingleTurn_Data -
        Zero_SingleTurn_Data) &
       ((1UL << Encoder_Config.SingleTurn_Bit) - 1);

   return single_value;
}

void Delay_us(uint16_t us)
{
   DELAY_microseconds(us);
}

void Encoder_Init(void)
{
   uint16_t Magic_Value = 0;
   DEE_Read(DEE_Encoder_MagicKey, &Magic_Value);

   if (Magic_Value == FACTORY_MAGIC_KEY)
   {
      Encoder_Config.MultiTurn_Bit = 0;
      Encoder_Config.SingleTurn_Bit = 0;
      Encoder_Config.CRC_Bit = 0;
      DEE_Read(DEE_Encoder_MultiTurnBitSize, &Encoder_Config.MultiTurn_Bit);
      DEE_Read(DEE_Encoder_SingleTurnBitSize, &Encoder_Config.SingleTurn_Bit);
      DEE_Read(DEE_Encoder_CRCBitSize, &Encoder_Config.CRC_Bit);

      uint16_t Zero_LowData;
      uint16_t Zero_HighData;
      DEE_Read(DEE_ENCODER_ZERO_L, &Zero_LowData);
      DEE_Read(DEE_ENCODER_ZERO_H, &Zero_HighData);
      Zero_SingleTurn_Data = ((uint32_t)Zero_HighData << 16) | Zero_LowData;
   }
   else
   {
      Encoder_Config.MultiTurn_Bit = 12;  // 配置默認多圈位數
      Encoder_Config.SingleTurn_Bit = 16; // 配置默認單圈位數
      Encoder_Config.Warning_Bit = 1;
      Encoder_Config.Error_Bit = 1;
      Encoder_Config.CRC_Bit = 6; // 配置默認CRC位數

      Zero_SingleTurn_Data = 0;
   }
   DEE_Read(DEE_Speed_Sample_Time, &Speed_Sample_Time);
   Encoder_Load_Position_Offset();
   DEE_Read(DEE_Direction, &Direction_Config);

   if (Direction_Config != 0x01 &&
       Direction_Config != 0x02)
   {
      Direction_Config = 0x01;

      DEE_Write(DEE_Direction,
                Direction_Config);
   }
   DEE_Read(DEE_MultiTurn_Origin_Mode,
            &MultiTurn_Origin_Mode);

   if (MultiTurn_Origin_Mode != 1 &&
       MultiTurn_Origin_Mode != 2)
   {
      MultiTurn_Origin_Mode = 2;

      DEE_Write(DEE_MultiTurn_Origin_Mode,
                MultiTurn_Origin_Mode);
   }
   if (Speed_Sample_Time < 10 || Speed_Sample_Time > 200)
   {
      Speed_Sample_Time = 10;

      DEE_Write(DEE_Speed_Sample_Time, Speed_Sample_Time);
   }
}

void Encoder_Load_Position_Offset(void)
{
   uint16_t low;
   uint16_t high;

   DEE_Read(DEE_POSITION_OFFSET_L, &low);
   DEE_Read(DEE_POSITION_OFFSET_H, &high);

   Position_Offset =
       (int32_t)(((uint32_t)high << 16) | low);
}

void Encoder_Read_Data(void)
{
   MA_Clear(); // Start Readout Data
   Delay_us(1);
   Encoder_SSI_Read(Encoder_Config.MultiTurn_Bit, &Encoder_Config.MultiTurn_Data);
   Encoder_SSI_Read(Encoder_Config.SingleTurn_Bit, &Encoder_Config.SingleTurn_Data);
   Encoder_SSI_Read(Encoder_Config.Warning_Bit, &Encoder_Config.Warning_Data);
   Encoder_SSI_Read(Encoder_Config.Error_Bit, &Encoder_Config.Error_Data);
   Encoder_SSI_Read(Encoder_Config.CRC_Bit, &Encoder_Config.CRC_Data);
   Delay_us(5);
   MA_Set();
   if (Encoder_Config.Warning_Data || Encoder_Config.Error_Data == 0)
   {
      LED0_SetLow();
   }
   else
   {
      LED0_SetHigh();
   }

   LED1_SetLow(); // LED1指示EncoderReadData

   Encoder_Config.Raw_Data = ((uint64_t)Encoder_Config.MultiTurn_Data << (Encoder_Config.SingleTurn_Bit + 2 + Encoder_Config.CRC_Bit)) |
                             ((uint64_t)Encoder_Config.SingleTurn_Data << (2 + Encoder_Config.CRC_Bit)) |
                             ((uint64_t)Encoder_Config.Warning_Data << (Encoder_Config.CRC_Bit + 1)) |
                             ((uint64_t)Encoder_Config.Error_Data << (Encoder_Config.CRC_Bit)) |
                             ((uint64_t)Encoder_Config.CRC_Data);
}

void Encoder_SSI_Read(uint8_t bit_num, uint32_t *data)
{
   uint32_t Data_Temp = 0;
   for (int i = 0; i < bit_num; i++)
   {
      MA_Set();
      Delay_us(1);
      SLO_Get_Value();
      Data_Temp = (Data_Temp <<= 1) | SLO_Get_Value();
      MA_Clear();
      Delay_us(1);
   }
   if (data != 0)
   {
      *data = Data_Temp;
   }
}

void Encoder_Update_Speed(void)
{
   uint32_t current_position;
   int64_t diff;
   uint64_t max_position;
   uint32_t single_resolution;

   current_position = Encoder_Get_Total_Position();

   if (Speed_Init_Flag == 0)
   {
      Last_Position = current_position;

      Speed_Init_Flag = 1;

      Encoder_RPM = 0;
      Encoder_Direction = 0;

      return;
   }

   diff = (int64_t)current_position -
          (int64_t)Last_Position;

   // 总位置范围
   max_position =
       ((uint64_t)1 << (Encoder_Config.SingleTurn_Bit +
                        Encoder_Config.MultiTurn_Bit));

   // 正方向跨零
   if (diff > (int64_t)(max_position / 2))
   {
      diff -= max_position;
   }

   // 反方向跨零
   else if (diff < -(int64_t)(max_position / 2))
   {
      diff += max_position;
   }

   if (diff > 0) // 判斷方向
   {
      if (Direction_Config == 0x01)
         Encoder_Direction = 1;
      else
         Encoder_Direction = 2;
   }
   else if (diff < 0)
   {
      if (Direction_Config == 0x01)
         Encoder_Direction = 2;
      else
         Encoder_Direction = 1;
   }
   else
   {
      Encoder_Direction = 0;
   }

   single_resolution =
       (1UL << Encoder_Config.SingleTurn_Bit);

   Encoder_RPM =
       ((uint64_t)llabs(diff) * 60000UL /
        Speed_Sample_Time) /
       single_resolution;

   Last_Position = current_position;
}

void Encoder_Set_Value(uint32_t set_value)
{
   uint32_t current;

   Encoder_Read_Data();

   current = Encoder_Get_Total_Position();

   Position_Offset =
       (int32_t)set_value -
       (int32_t)current;

   Encoder_Save_to_DEE(
       DEE_POSITION_OFFSET_L,
       DEE_POSITION_OFFSET_H,
       (uint32_t)Position_Offset);
}

void Encoder_Clear_Data(void)
{
   uint32_t current;

   Encoder_Read_Data();

   current = Encoder_Get_Total_Position();

   Position_Offset = -(int32_t)current;

   Encoder_Save_to_DEE(
       DEE_POSITION_OFFSET_L,
       DEE_POSITION_OFFSET_H,
       (uint32_t)Position_Offset);
}

void Encoder_Save_to_DEE(uint16_t Addr_L, uint16_t Addr_H, uint32_t Data)
{
   DEE_Write(Addr_L, (uint16_t)(Data & 0xFFFF));
   DEE_Write(Addr_H, (uint16_t)(Data >> 16));
}

void SCCP3_TimeoutCallback(void)
{
   Speed_Timer_Count++;
}