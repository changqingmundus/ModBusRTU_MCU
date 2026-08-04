#include "Free_Mode.h"
#include "Protocol_Config.h"
#include "encoder.h"
#include <stdint.h>

static uint8_t tx[FreeMode_Data_Length];
static uint8_t tx_len = 0;

void FreeMode_Task(void)
{
  if (FreeMode_Enable)
  {
    Encoder_Read_Data();
    FreeMode_Process();
    FreeMode_Send();
    DELAY_milliseconds(Update_Time);
  }
}

void FreeMode_Process(void)
{
  uint8_t index = 0;

  tx[index++] = 0xAB;
  tx[index++] = 0xCD;

  uint8_t data_len = 0;
  uint32_t position;
  position = Encoder_Get_Position();

  // 預留長度位置
  uint8_t len_index = index++;
  uint32_t multiturn;

  multiturn = position >> Encoder_Config.SingleTurn_Bit;
  if (Encoder_Config.MultiTurn_Bit > 16)
  {
    tx[index++] = (multiturn >> 16) & 0xFF;
    data_len++;
  }
  tx[index++] = (multiturn >> 8) & 0xFF;
  tx[index++] = multiturn & 0xFF;
  data_len += 2;

  uint32_t singleturn;
  singleturn = position &
               ((1UL << Encoder_Config.SingleTurn_Bit) - 1);

  if (Encoder_Config.SingleTurn_Bit > 16)
  {
    tx[index++] = (singleturn >> 16) & 0xFF;
    data_len++;
  }
  tx[index++] = (singleturn >> 8) & 0xFF;
  tx[index++] = singleturn & 0xFF;
  data_len += 2;

  // 填入數據長度
  tx[len_index] = data_len;

  // 和校驗
  uint8_t sum = 0;
  uint8_t xor = 0;

  for (uint8_t i = len_index; i < index; i++)
  {
    sum += tx[i];
    xor ^= tx[i];
  }

  tx[index++] = sum;
  tx[index++] = xor;
  tx[index++] = 0x3D;

  tx_len = index;
}

void FreeMode_Send(void)
{
  for (uint8_t i = 0; i < tx_len; i++)
  {
    while (U1STAHbits.UTXBF)
      ;
    U1TXREG = tx[i];
  }
  while (!U1STAbits.TRMT)
    ;
}