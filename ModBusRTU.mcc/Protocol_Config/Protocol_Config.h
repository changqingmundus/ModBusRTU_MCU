#ifndef Protocol_Config_H
#define Protocol_Config_H

#include "dee.h"
#include "uart1.h"
#include "mb.h"

#define DEE_Encoder_Protocol  15  //通信模式存儲地址
#define DEE_Update_Time       16  //上傳時間存儲地址

#define DEE_MB_Reg_Mode       17  //寄存器表选择保存地址

typedef enum
{
    MB_REG_AUTO = 0x00,       // 根据编码器位数自动选择
    MB_REG_FORCE16 = 0x01,    // 强制16bit寄存器格式
    MB_REG_FORCE32 = 0x02     // 强制32bit寄存器格式

} MB_Reg_Mode_t;

extern MB_Reg_Mode_t MB_Reg_Mode;

#define ModBusRTU 0x01
#define FreeMode  0x02

extern uint16_t Protocol;
extern uint16_t Update_Time;
extern bool FreeMode_Enable;

extern eMBErrorCode Modbus_Status;

void Protocol_Init(void);
void ModBusRTU_Init(void);
void FreeMode_Init(void);
void MB_Reg_Init(void);

#endif