#include "dee.h"
#include "uart1.h"
#include "mb.h"

#define DEE_Encoder_Protocol  13  //通信模式存儲地址
#define DEE_Update_Time       14  //上傳時間存儲地址

#define ModBusRTU 0x01
#define FreeMode  0x02

extern uint8_t Protocol;
extern uint16_t Update_Time;
extern bool FreeMode_Enable;

extern eMBErrorCode Modbus_Status;

void Protocol_Init(void);
void ModBusRTU_Init(void);
void FreeMode_Init(void);