#include "Free_Mode.h"
#include "Protool_Config.h"
#include "encoder.h"
#include <stdint.h>

static uint8_t tx[FreeMode_Data_Length];
static uint8_t tx_len = 0;

void FreeMode_Task(void) {
  if (FreeMode_Enable) {
    Encoder_Read_Data();
    FreeMode_Process();
    FreeMode_Send();
    DELAY_milliseconds(Update_Time);
  }
}

void FreeMode_Process(void) {
  uint8_t index = 0;

  tx[index++] = 0xAB;
  tx[index++] = 0xCD;

  uint8_t data_len = 0;

  // 預留長度位置
  uint8_t len_index = index++;

  // 單圈數據
  if (Encoder_Config.SingleTurn_Bit > 16) {
    tx[index++] = (Encoder_Config.SingleTurn_Data >> 16) & 0xFF;
    data_len++;
  }

  tx[index++] = (Encoder_Config.SingleTurn_Data >> 8) & 0xFF;
  tx[index++] = Encoder_Config.SingleTurn_Data & 0xFF;
  data_len += 2;

  // 多圈數據
  if (Encoder_Config.MultiTurn_Bit > 16) {
    tx[index++] = (Encoder_Config.MultiTurn_Data >> 16) & 0xFF;
    data_len++;
  }

  tx[index++] = (Encoder_Config.MultiTurn_Data >> 8) & 0xFF;
  tx[index++] = Encoder_Config.MultiTurn_Data & 0xFF;
  data_len += 2;

  // 填入數據長度
  tx[len_index] = data_len;

  // 和校驗
  uint8_t sum = 0;
  uint8_t xor = 0;

  for (uint8_t i = len_index; i < index; i++) {
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
    for(uint8_t i=0;i<tx_len;i++)
    {
        while(U1STAHbits.UTXBF);
        U1TXREG = tx[i];
    }
    while(!U1STAbits.TRMT);
}