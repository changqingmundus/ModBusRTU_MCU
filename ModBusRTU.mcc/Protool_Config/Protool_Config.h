#include "dee.h"
#include "uart1.h"

#define DEE_Encoder_Protool 15  //通信模式存儲地址
#define DEE_Update_Time     16  //上傳時間存儲地址

#define ModBusRTU = 0x01;
#define FreeMode  = 0x02;

uint8_t Protool;
uint8_t Protool_ModBusRTU;
uint8_t Protool_FreeMode;
uint8_t Update_Time;

void Protool_Init(void);
void ModBus_RTU_Init(void);
void FreeMode_Init(void);