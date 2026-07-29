#ifndef CLEARDATA_H
#define CLEARDATA_H

#include <stdint.h>

#define ClearData() SET_GetValue()

// 宣告外部可能需要關心的狀態標誌（可選）
extern volatile uint8_t high_time_sec;
extern volatile uint8_t is_counting;

// 提供給中斷服務函式（ISR）呼叫的接口
void ClearData_CN_Callback(void);
void ClearData_Timer_Callback(void);

void Encoder_PowerOn_Reset_Check(void);
void Encoder_Factory_Reset(void);

#endif // CLEARDATA_H