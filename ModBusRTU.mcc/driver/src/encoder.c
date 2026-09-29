#include "encoder.h"
#include "pins.h"
#include "spi1.h"
#include "mu_1sf_driver.h"
#include "pz_1sf_driver.h"
#include "pvl.h"

int64_t Position_Offset;

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

uint16_t Data_Temp = 0;
uint8_t BissData;
static uint8_t biss_crc_reg = 0; // 4 位 CRC 寄存器

ENCODER_CONFIG Encoder_Config;

static void Biss_SendCDM(uint8_t bit, EncoderFrameMode_t mode);
static uint8_t MU_Get_ModeMT(uint8_t multi_turn_bits);

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
      DEE_Read(DEE_SENSOR_TYPE, &Sensor_Type);
      DEE_Read(DEE_SENSOR_ST_CHIP, &Sensor_Chip_ST);
      DEE_Read(DEE_SENSOR_MT_CHIP, &Sensor_Chip_MT);

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
   uint16_t llowposition = 0;
   uint16_t hhighposition = 0;

   DEE_Read(DEE_POSITION_OFFSET_L, &lowposition);
   DEE_Read(DEE_POSITION_OFFSET_H, &highposition);
   DEE_Read(DEE_POSITION_OFFSET_LL, &llowposition);
   DEE_Read(DEE_POSITION_OFFSET_HH, &hhighposition);

   Position_Offset = (int64_t)(((uint64_t)hhighposition << 48) | ((uint64_t)llowposition << 32) | ((uint64_t)highposition << 16) | lowposition);
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

   Encoder_SSI_Read(data_bits, rx_data);

   uint16_t byte_num = (data_bits + 1U + 7U) / 8U;

   // 把SPI收到的byte拼成一个64bit数据
   for (uint16_t i = 0; i < byte_num; i++)
   {
      spi_data = (spi_data << 8) | rx_data[i];
   }

   /*
    * 丢掉无效bit
    */
   spi_data >>= (byte_num * 8U - (data_bits + 1U));

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

   // if (Encoder_Config.Warning_Data || Encoder_Config.Error_Data == 1)
   if (Encoder_Config.Error_Data == 0)
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

   /* 保存低32位 */
   Encoder_Save_to_DEE(DEE_POSITION_OFFSET_L,
                       DEE_POSITION_OFFSET_H,
                       (uint32_t)(Position_Offset & 0xFFFFFFFFULL));

   /* 保存高32位 */
   Encoder_Save_to_DEE(DEE_POSITION_OFFSET_LL,
                       DEE_POSITION_OFFSET_HH,
                       (uint32_t)(Position_Offset >> 32));
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

/*Biss-C Communication*/
static void Biss_ID(uint8_t addr, EncoderFrameMode_t mode)
{
   addr &= 0x07;

   for (int i = 2; i >= 0; i--)
   {
      uint8_t bit = (addr >> i) & 0x01;

      Biss_SendCDM(bit, mode);
      Biss_CRC_Update(bit);
   }
}

static void Biss_Address(uint8_t addr, EncoderFrameMode_t mode)
{
   addr &= 0x7F;

   for (int i = 6; i >= 0; i--)
   {
      uint8_t bit = (addr >> i) & 0x01;

      Biss_SendCDM(bit, mode);
      Biss_CRC_Update(bit);
   }
}

static void Biss_SendCDM(uint8_t bit, EncoderFrameMode_t mode)
{
   if (mode == BISS_FRAME_MIN)
   {
      if (bit)
         Biss_CDM1();
      else
         Biss_CDM0();
   }
   else if (mode == BISS_FRAME_SHORT)
   {
      if (bit)
         Biss_Short_CDM1();
      else
         Biss_Short_CDM0();
   }
   else if (mode == BISS_FRAME_LONG)
   {
      if (bit)
         Biss_Long_CDM1();
      else
         Biss_Long_CDM0();
   }
   else if (mode == SSI_FRAME)
   {
      if (bit)
         SSI_CDM1();
      else
         SSI_CDM0();
   }
}

/*CRC Send*/
void Biss_CRC(void) // For Simplified Frame
{
   uint8_t crc = (~biss_crc_reg) & 0x0F;

   for (int i = 3; i >= 0; i--)
   {
      if (crc & (1 << i))
         Biss_CDM1();
      else
         Biss_CDM0();
   }
}

static void Biss_SendCRC(EncoderFrameMode_t mode)
{
   uint8_t crc = (~biss_crc_reg) & 0x0F;

   for (int i = 3; i >= 0; i--)
   {
      Biss_SendCDM((crc >> i) & 0x01, mode);
   }
}

void Biss_CRC_Update(uint8_t bit)
{
   uint8_t msb = (biss_crc_reg >> 3) & 0x01; // 当前 CRC 最高位
   uint8_t feedback = bit ^ msb;             // 异或

   biss_crc_reg = (biss_crc_reg << 1) & 0x0F; // 左移，保留 4 位
   if (feedback)
      biss_crc_reg ^= 0x03; // 多项式 0x13 去掉最高位后为 0x03
}

/*Biss-C SendCDM With ReceiveCDS */
void Biss_CDM0(void)
{
   MA_Clear();
   Delay_us(1);
   MA_Set(); // LAT
   Delay_us(1);
   MA_Clear();
   Delay_us(1);
   MA_Set(); // ACK
   Delay_us(20);
}

void Biss_CDM1(void)
{
   MA_Clear();
   Delay_us(1);
   MA_Set(); // LAT
   Delay_us(1);
   MA_Clear();
   Delay_us(1);
   MA_Set(); // ACK
   Delay_us(1);
   MA_Clear();
   Delay_us(20);
   MA_Set();
}

void Biss_Short_CDM0(void)
{
   MA_Clear();
   Delay_us(1);
   MA_Set(); // LAT
   Delay_us(1);
   MA_Clear();
   Delay_us(1);
   MA_Set(); // ACK
   Delay_us(1);
   MA_Clear();
   Delay_us(1);
   MA_Set(); // STR
   Delay_us(1);
   MA_Clear();
   Delay_us(1);
   MA_Set(); // CDS

   Delay_us(1);
   BissData = SLO_Get_Value(); // 获取 SLO 值
   Data_Temp = (Data_Temp << 1) | BissData;

   MA_Clear();
   Delay_us(1);
   MA_Set(); // STP
   Delay_us(20);
}

void Biss_Short_CDM1(void)
{
   MA_Clear();
   Delay_us(1);
   MA_Set(); // LAT
   Delay_us(1);
   MA_Clear();
   Delay_us(1);
   MA_Set(); // ACK
   Delay_us(1);
   MA_Clear();
   Delay_us(1);
   MA_Set(); // STR
   Delay_us(1);
   MA_Clear();
   Delay_us(1);
   MA_Set(); // CDS

   Delay_us(1);
   BissData = SLO_Get_Value(); // 获取 SLO 值
   Data_Temp = (Data_Temp << 1) | BissData;

   MA_Clear();
   Delay_us(1);
   MA_Set(); // STP
   Delay_us(1);
   MA_Clear();
   Delay_us(20);
   MA_Set();
}

void Biss_Long_CDM0(void)
{
   MA_Clear();
   Delay_us(1);
   MA_Set(); // LAT
   Delay_us(1);
   MA_Clear();
   Delay_us(1);
   MA_Set(); // ACK
   Delay_us(1);
   MA_Clear();
   Delay_us(1);
   MA_Set(); // STR
   Delay_us(1);
   MA_Clear();
   Delay_us(1);
   MA_Set(); // CDS

   Delay_us(1);
   BissData = SLO_Get_Value(); // 获取 SLO 值
   Data_Temp = (Data_Temp << 1) | BissData;

   for (int i = 0; i < 45; i++) // Process data
   {
      MA_Clear();
      Delay_us(1);
      MA_Set();
      Delay_us(1);
   }
   MA_Clear();
   Delay_us(1);
   MA_Set(); // STP
   Delay_us(20);
}

void Biss_Long_CDM1(void)
{
   MA_Clear();
   Delay_us(1);
   MA_Set(); // LAT
   Delay_us(1);
   MA_Clear();
   Delay_us(1);
   MA_Set(); // ACK
   Delay_us(1);
   MA_Clear();
   Delay_us(1);
   MA_Set(); // STR
   Delay_us(1);
   MA_Clear();
   Delay_us(1);
   MA_Set(); // CDS

   Delay_us(1);
   BissData = SLO_Get_Value(); // 获取 SLO 值
   Data_Temp = (Data_Temp << 1) | BissData;

   for (int i = 0; i < 45; i++) // Process data
   {
      MA_Clear();
      Delay_us(1);
      MA_Set();
      Delay_us(1);
   }
   MA_Clear();
   Delay_us(1);
   MA_Set(); // STP
   Delay_us(1);
   MA_Clear();
   Delay_us(20);
   MA_Set();
}

void SSI_CDM0(void)
{
   MA_Clear();
   Delay_us(1);
   MA_Set();
   Delay_us(1);

   for (int i = 0; i < 60; i++) // Process data
   {
      MA_Clear();
      Delay_us(1);
      MA_Set();
      Delay_us(1);
   }
   MA_Clear();
   Delay_us(1);
   MA_Set();
   Delay_us(20);
}

void SSI_CDM1(void)
{
   MA_Clear();
   Delay_us(1);
   MA_Set();
   Delay_us(1);

   for (int i = 0; i < 60; i++) // Process data
   {
      MA_Clear();
      Delay_us(1);
      MA_Set();
      Delay_us(1);
   }
   MA_Clear();
   Delay_us(20);
   MA_Set();
}

/*Biss-C Simplify BiSS frame Communication*/
void Biss_Enable_Short_Frame(void)
{
   biss_crc_reg = 0;

   Biss_CDM1(); // S
   Biss_CDM0(); // CTS
   Biss_CRC_Update(0x00);
   for (int i = 0; i < 8; i++) // 发送8位ID
   {
      Biss_CDM0();
      Biss_CRC_Update(0x00);
   }

   Biss_CDM0(); // CMD = 00
   Biss_CRC_Update(0x00);
   Biss_CDM0();
   Biss_CRC_Update(0x00);

   Biss_CRC(); // 发送4位CRC
}

/*Biss-C ReadByte Without Process Data*/
void Biss_ReadByte(EncoderFrameMode_t mode, uint8_t cts, uint8_t bissid, uint8_t bissaddr, uint8_t *data)
{
   /* 14bit CDM = 0 */
   for (int i = 0; i < 14; i++)
   {
      Biss_SendCDM(0, mode);
   }

   biss_crc_reg = 0;

   Biss_SendCDM(1, mode); // S

   Biss_SendCDM(cts & 0x01, mode); // CTS
   Biss_CRC_Update(cts & 0x01);

   Biss_ID(bissid, mode);        // ID
   Biss_Address(bissaddr, mode); // Address
   Biss_SendCRC(mode);           // CRC

   Biss_SendCDM(1, mode); // R
   Biss_SendCDM(0, mode); // W

   Biss_SendCDM(1, mode); // S

   Data_Temp = 0;

   for (int i = 0; i < 15; i++)
   {
      Biss_SendCDM(0, mode);
   }

   *data = (uint8_t)((Data_Temp >> 6) & 0xFF);
}

void Biss_WriteByteHeader(EncoderFrameMode_t mode, uint8_t cts, uint8_t bissid, uint8_t bissaddr)
{
   biss_crc_reg = 0;

   // 1. 发送起始位
   Biss_SendCDM(1, mode); // S

   // 2. 发送 CTS 位并更新 CRC
   uint8_t bit = cts & 0x01;
   Biss_SendCDM(bit, mode);
   Biss_CRC_Update(bit);

   // 3. 发送 ID（内部自动更新 CRC）
   Biss_ID(bissid, mode);

   // 4. 发送地址（内部自动更新 CRC）
   Biss_Address(bissaddr, mode);

   // 5. 发送 CRC
   Biss_SendCRC(mode);

   // 6. 发送 R/W
   Biss_SendCDM(0, mode); // R
   Biss_SendCDM(1, mode); // W
}

void Biss_WriteByte(EncoderFrameMode_t mode, uint8_t *write_data, uint8_t data_len)
{
   biss_crc_reg = 0;

   // 1. 发送起始位
   Biss_SendCDM(1, mode); // S

   // 2. 发送数据并更新 CRC
   for (uint8_t i = 0; i < data_len; i++)
   {
      uint8_t byte = write_data[i];

      for (int8_t j = 7; j >= 0; j--)
      {
         uint8_t bit = (byte >> j) & 0x01;

         Biss_SendCDM(bit, mode);
         Biss_CRC_Update(bit);
      }
   }

   // 3. 发送 CRC
   Biss_SendCRC(mode);

   // 4. P
   Biss_SendCDM(0, mode); // P
}

/*Change PinMode From SPI to GPIO*/
void Enable_GPIO(void)
{
   SPI1_Close();

   __builtin_write_RPCON(0x0000);
   __builtin_write_RPCON(0x0800);

   TRISBbits.TRISB11 = 0;
   LATBbits.LATB11 = 1;

   TRISBbits.TRISB13 = 1;
}

void Change_To_Biss(void)
{
   uint8_t cts = 1;
   uint8_t id = 0;
   uint8_t data_rx;
   uint8_t mode_reg;

   // Read MODEA / MODEB register
   Biss_ReadByte(SSI_FRAME, cts, id, 0x0B, &data_rx);

   mode_reg = data_rx;

   // MODEA(2:0) = 0x2 -> BiSS
   mode_reg = (mode_reg & 0xF8) | 0x02;

   // Write back 0x0B
   Biss_WriteByteHeader(SSI_FRAME, cts, id, 0x0B);
   Biss_WriteByte(SSI_FRAME, &mode_reg, 1);
}

void MU_OutputBit_Config(uint8_t single_turn_bits, uint8_t multi_turn_bits)
{
   uint8_t out_lsb;
   uint8_t out_msb;
   uint8_t out_zero;
   uint8_t mode_mt;

   out_lsb = 19 - single_turn_bits;
   out_msb = multi_turn_bits + 5;
   out_zero = 0x00;

   mode_mt = MU_Get_ModeMT(multi_turn_bits);

   mu_write_param(&MU_OUT_LSB, out_lsb);   // for singleturn
   mu_write_param(&MU_OUT_MSB, out_msb);   // for multiturn
   mu_write_param(&MU_OUT_ZERO, out_zero); // for outzero
   mu_write_param(&MU_MODE_MT, mode_mt);   // for external multiturn
}

static uint8_t MU_Get_ModeMT(uint8_t multi_turn_bits)
{
   switch (multi_turn_bits)
   {
   case 4:
      return 0x0B;

   case 8:
      return 0x0C;

   case 12:
      return 0x0D;

   case 16:
      return 0x0E;

   case 18:
      return 0x0F;

   default:
      return 0x00; // 无外部多圈
   }
}

uint8_t Sensor_SetProtocol(SensorProtocol_t protocol)
{
   if (protocol > SENSOR_PROTOCOL_EXTSSI)
   {
      return 1;
   }

   switch (Sensor_Chip_ST)
   {
   case SENSOR_CHIP_MU:
      mu_write_param(&MU_MODEA, protocol);
      break;

   case SENSOR_CHIP_PZ:
      switch (protocol)
      {
      case SENSOR_PROTOCOL_EXTSSI:
         pz_write_param(&PZ_SSI_EN, 1);
         pz_write_param(&PZ_SSI_EXT, 1);
         break;

      case SENSOR_PROTOCOL_SSI:
         pz_write_param(&PZ_SSI_EN, 1);
         pz_write_param(&PZ_SSI_EXT, 0);
         break;

      default:
         return 1;
      }
      break;

   default:
      return 1;
   }

   return 0;
}

uint8_t MU_Load_PVL_Config(void)
{
   uint8_t pvl_data[13];

   IC_PVL_ConfigToBytes(pvl_data);

   for (uint8_t i = 0; i < 13; i++)
   {
      if (MU_WriteRegister_Verify(0x60 + i, pvl_data[i]) != 0)
      {
         return 1;
      }
   }

   return 0;
}

uint8_t MU_WriteRegister_Verify(uint8_t addr, uint8_t data)
{
   uint8_t read_back;
   uint8_t retry = 0;

   while (retry < 3)
   {
      mu_write_register(addr, data);

      mu_read_data = 0;
      mu_read_register(addr);
      read_back = mu_read_data;

      if (read_back == data)
      {
         return 0;
      }

      retry++;
   }

   return 1;
}

uint8_t PVL_Check_Status(void)
{
   uint8_t status;
   uint8_t ext_status;

   /* Read PVL Status Register 0x10 -> MU RAM 0x6E */
   MU_I2C_Transfer(0xC1, 0x6E, 0x6E, 0x10);

   /* Read PVL Extended Status Register 0x12 -> MU RAM 0x6F */
   MU_I2C_Transfer(0xC1, 0x6F, 0x6F, 0x12);

   /* Read status */
   mu_read_data = 0;
   mu_read_register(0x6E);
   status = mu_read_data;

   /* Read extended status */
   mu_read_data = 0;
   mu_read_register(0x6F);
   ext_status = mu_read_data;

   /*
    * 0x10:
    * bit7 PRESET : don't care
    * bit6 PDR    : don't care
    * bit5~0      : must be 0
    */
   if ((status & 0x3F) != 0)
   {
      return 1;
   }

   /*
    * 0x12:
    * bit5 ACTIVE_ST : don't care
    * bit7~6, bit4~0 : must be 0
    */
   if ((ext_status & 0xDF) != 0)
   {
      return 1;
   }

   return 0;
}

void MU_I2C_Transfer(uint8_t devid, uint8_t ram_start, uint8_t ram_end, uint8_t dev_start)
{
   /* I2C configuration */
   mu_write_param(&MU_I2C_DEVID, devid);
   mu_write_param(&MU_I2C_RAM_START, ram_start);
   mu_write_param(&MU_I2C_RAM_END, ram_end);
   mu_write_param(&MU_I2C_DEV_START, dev_start);

   mu_write_command(CMD_MU_I2C_COM);
}
