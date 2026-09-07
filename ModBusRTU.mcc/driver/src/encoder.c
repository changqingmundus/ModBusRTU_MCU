#include "encoder.h"
#include "pins.h"
#include "spi1.h"
#include "cleardata.h"

int32_t Position_Offset;

uint16_t Encoder_Write_Low = 0;
uint16_t Encoder_Write_High = 0;
volatile static uint64_t Last_Position = 0;
static uint8_t Speed_Init_Flag = 0;

uint16_t Direction_Config = 1;
uint16_t MultiTurn_Origin_Mode = 1;

uint32_t Encoder_RPM = 0;
uint8_t Encoder_Direction = 0;
static uint8_t Dir_Last = 0;
static uint8_t Dir_Count = 0;
static uint32_t rpm_buf[3];
static uint8_t rpm_index = 0;

uint16_t Speed_Timer_Count = 0;
uint16_t Speed_Update_Period = 1;

ENCODER_CONFIG Encoder_Config;

uint64_t Encoder_Get_Max_Position(void)
{
   uint64_t single_max;
   uint64_t multi_max;

   single_max = ((uint64_t)1 << Encoder_Config.SingleTurn_Bit);
   multi_max = ((uint64_t)1 << Encoder_Config.MultiTurn_Bit);

   return single_max * multi_max - 1;
}

/*處理位置數據*/
uint64_t Encoder_Get_Position(void)
{
   int64_t position;
   uint64_t max_position;

   max_position = Encoder_Get_Max_Position();

   position = (int64_t)Encoder_Get_Total_Position() +
              (int64_t)Position_Offset;

   while (position < 0)
   {
      position += max_position + 1;
   }

   while (position > max_position)
   {
      position -= max_position + 1;
   }

   if (Direction_Config == 0x02)
   {
      position = max_position - position;
   }

   return (uint64_t)position;
}

/*原始位置數據*/
uint64_t Encoder_Get_Total_Position(void)
{
   uint32_t multi1;
   uint32_t multi2;
   uint32_t single;

   do
   {
      multi1 = Encoder_Config.MultiTurn_Data;

      single = Encoder_Config.SingleTurn_Data;

      multi2 = Encoder_Config.MultiTurn_Data;

   } while (multi1 != multi2);

   return ((uint64_t)multi1 << Encoder_Config.SingleTurn_Bit) | single;
}

uint8_t Encoder_Update_Direction(int64_t diff)
{
   uint8_t dir;

   if (diff > POSITION_DEAD_BAND)
   {
      dir = 1; // CW
   }
   else if (diff < -POSITION_DEAD_BAND)
   {
      dir = 2; // CCW
   }
   else
   {
      dir = 0; // STOP
   }

   if (dir == Dir_Last)
   {
      if (Dir_Count < 3)
      {
         Dir_Count++;
      }
   }
   else
   {
      Dir_Count = 0;
   }

   if (Dir_Count >= 3)
   {
      Encoder_Direction = dir;
   }

   Dir_Last = dir;

   return Encoder_Direction;
}

uint32_t Median3(uint32_t a, uint32_t b, uint32_t c)
{
   uint32_t max;
   uint32_t min;

   max = a;
   if (b > max)
      max = b;
   if (c > max)
      max = c;

   min = a;
   if (b < min)
      min = b;
   if (c < min)
      min = c;

   return a + b + c - max - min;
}

bool Encoder_SSI_Read(uint8_t bit_num, uint8_t *rx_data)
{
   if (rx_data == NULL)
   {
      return false;
   }

   uint16_t total_bits = bit_num + 1U;

   // SPI按8 bit传输，向上取整
   uint16_t byte_num = (total_bits + 7U) / 8U;

   SPI1_BufferRead(rx_data, byte_num);

   return true;
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
   }
   else
   {
      Encoder_Config.MultiTurn_Bit = 8;   // 配置默認多圈位數
      Encoder_Config.SingleTurn_Bit = 12; // 配置默認單圈位數
      Encoder_Config.Warning_Bit = 1;
      Encoder_Config.Error_Bit = 1;
      Encoder_Config.CRC_Bit = 6; // 配置默認CRC位數

      Position_Offset = 0;
   }
   DEE_Read(DEE_Speed_Update_Period, &Speed_Update_Period);
   Encoder_Load_Position_Offset();
   DEE_Read(DEE_Direction, &Direction_Config);

   if (Direction_Config != 0x01 && Direction_Config != 0x02)
   {
      Direction_Config = 0x01;

      DEE_Write(DEE_Direction, Direction_Config);
   }
   DEE_Read(DEE_MultiTurn_Origin_Mode, &MultiTurn_Origin_Mode);

   if (MultiTurn_Origin_Mode != 1 && MultiTurn_Origin_Mode != 2)
   {
      MultiTurn_Origin_Mode = 1;
      DEE_Write(DEE_MultiTurn_Origin_Mode, MultiTurn_Origin_Mode);
   }
   if (Speed_Update_Period < 1 || Speed_Update_Period > 20)
   {
      Speed_Update_Period = 1;

      DEE_Write(DEE_Speed_Update_Period, Speed_Update_Period);
   }

   SPI1_Open(0); // open ssi port
}

void Encoder_Load_Position_Offset(void)
{
   uint16_t lowposition = 0;
   uint16_t highposition = 0;

   DEE_Read(DEE_POSITION_OFFSET_L, &lowposition);
   DEE_Read(DEE_POSITION_OFFSET_H, &highposition);

   Position_Offset = (int32_t)(((uint32_t)highposition << 16) | lowposition);
}

void Encoder_Read_Data(void)
{
   uint16_t data_bits;

   uint8_t rx_data[8] = {0};
   uint64_t spi_data = 0;

   /*
    * SSI有效数据：
    *
    * MultiTurn
    * SingleTurn
    * Warning
    * Error
    * CRC
    */
   data_bits =
       Encoder_Config.MultiTurn_Bit +
       Encoder_Config.SingleTurn_Bit +
       2U +
       Encoder_Config.CRC_Bit;

   /* SSI前面的启动clock */
   uint16_t total_bits = data_bits + 1U;

   /* SPI按8bit传输 */
   uint16_t byte_num = (total_bits + 7U) / 8U;
    
   Encoder_SSI_Read(data_bits, rx_data);

   // 把SPI收到的byte拼成一个64bit数据
   for (uint16_t i = 0; i < (data_bits + 1U + 7U) / 8U; i++)
   {
      spi_data = (spi_data << 8) | rx_data[i];
   }

   /*
    * 丢掉无效bit
    */
   spi_data >>= (byte_num * 8U - total_bits);

   /*
    * 保存完整SSI数据
    */
   Encoder_Config.Raw_Data = spi_data;

   /*
    * 从Raw_Data拆分各字段
    */

   // CRC
   Encoder_Config.CRC_Data =
       spi_data &
       ((1ULL << Encoder_Config.CRC_Bit) - 1ULL);

   // Error
   Encoder_Config.Error_Data =
       (spi_data >> Encoder_Config.CRC_Bit) & 0x01U;

   // Warning
   Encoder_Config.Warning_Data =
       (spi_data >> (Encoder_Config.CRC_Bit + 1U)) & 0x01U;

   // SingleTurn
   Encoder_Config.SingleTurn_Data =
       (spi_data >> (Encoder_Config.CRC_Bit + 2U)) &
       ((1ULL << Encoder_Config.SingleTurn_Bit) - 1ULL);

   // MultiTurn
   Encoder_Config.MultiTurn_Data =
       spi_data >> (Encoder_Config.CRC_Bit + 2U +
                    Encoder_Config.SingleTurn_Bit);

   if (Encoder_Config.Warning_Data || Encoder_Config.Error_Data == 0)
   {
      LED0_SetLow();
   }
   else
   {
      LED0_SetHigh();
   }
}

void Encoder_Update_Speed(void)
{
   uint64_t current_position;
   int64_t diff;
   uint64_t delta;
   uint32_t rpm_raw;
   uint64_t single_resolution;
   uint64_t range;

   uint16_t Speed_Sample_Period_ms =
       (uint32_t)Speed_Update_Period * 10U;

   // 一次读取位置
   current_position = Encoder_Config.SingleTurn_Data;

   // 第一次初始化
   if (Speed_Init_Flag == 0)
   {
      Last_Position = current_position;

      Speed_Init_Flag = 1;

      Encoder_RPM = 0;

      Encoder_Direction = 0;

      return;
   }

   /*
       计算位置差
   */
   diff = (int64_t)current_position - (int64_t)Last_Position;

   /*
       单圈编码器
       需要处理跨零
   */

   range = 1ULL << Encoder_Config.SingleTurn_Bit;

   if (diff > (int64_t)(range / 2))
   {
      diff -= range;
   }
   else if (diff < -(int64_t)(range / 2))
   {
      diff += range;
   }

   /*
       更新方向
   */
   Encoder_Update_Direction(diff);

   /*
       计算RPM
   */
   delta = (diff >= 0) ? diff : -diff;

   single_resolution = 1ULL << Encoder_Config.SingleTurn_Bit;

   rpm_raw = (uint32_t)(((uint64_t)delta * 60000ULL) /
                        ((uint64_t)Speed_Sample_Period_ms * single_resolution));

   /*
       3点中值滤波
   */
   rpm_buf[rpm_index++] = rpm_raw;

   if (rpm_index >= 3)
   {
      rpm_index = 0;
   }

   Encoder_RPM = Median3(rpm_buf[0],
                         rpm_buf[1],
                         rpm_buf[2]);

   /*
       保存位置
   */
   Last_Position = current_position;
}

void Encoder_Set_Value(uint32_t set_value)
{
   int64_t current;

   Encoder_Read_Data();
   current = Encoder_Get_Total_Position();
   Position_Offset = (int32_t)((int64_t)set_value - current);
   Encoder_Save_to_DEE(DEE_POSITION_OFFSET_L,
                       DEE_POSITION_OFFSET_H,
                       (uint32_t)Position_Offset);
}

void Encoder_Clear_Data(void)
{
   int64_t current;
   int64_t target;

   current = Encoder_Get_Total_Position();

   if (MultiTurn_Origin_Mode == 1)
   {
      uint32_t multi_middle;
      uint32_t single_zero;

      multi_middle = ((uint32_t)1 << Encoder_Config.MultiTurn_Bit) / 2;
      single_zero = 0;
      target = ((uint64_t)multi_middle << Encoder_Config.SingleTurn_Bit) | single_zero;
   }
   else
   {
      target = 0;
   }

   Position_Offset = target - current;

   Encoder_Save_to_DEE(DEE_POSITION_OFFSET_L,
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