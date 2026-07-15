#include "cleardata.h"
#include "encoder.h" 
#include "sccp1.h"
#include "dee.h"
#include "delay.h"

// 全局變量定義
volatile uint8_t high_time_sec = 0; // 記錄高電平持續的秒數
volatile uint8_t is_counting = 0;   // 是否正在計時中

/**
 * @brief RA4 引腳上升沿中斷回調函式
 * @note  請在 MCC 的 pin_manager.c 的 _CNInterrupt 中呼叫此函式
 */
void ClearData_CN_Callback(void) {
     DELAY_milliseconds(20); //xms 消抖，根據實際情況調整
     uint8_t ClearDataFlag = ClearData(); // 讀取 RA4 的當前狀態，觸發上升沿檢測
    // 捕捉到上升沿（剛接上 VCC）
    if(ClearDataFlag == 1) {
        if(is_counting == 0) {
            is_counting = 1;
            high_time_sec = 0;   // 秒數計數器清零
            SCCP1_Timer_Start();  // 啟動定時器（每 1 秒進一次中斷）
        }
    }
}

/**
 * @brief SCCP1 定時器 1 秒溢出中斷回調函式
 * @note  請在 MCC 的 sccp1.c 的 SCCP1_TimeoutHandler 中呼叫此函式
 */
void ClearData_Timer_Callback(void) {
    uint8_t ClearDataFlag = ClearData(); // 每秒檢查一次 RA4 的狀態
    if(is_counting == 1) {
        
        // 檢查目前管腳狀態
        if(ClearDataFlag == 1) {
            high_time_sec++; // 高電平依然存在，秒數加 1
            
            // 如果時間已經達到或超過 60 秒，判定為超時無效
            if(high_time_sec > 60) {
                SCCP1_Timer_Stop();  // 停止計時
                is_counting = 0;    // 重置狀態，不清零，直接結束
                high_time_sec = 0;
            }
        } 
        else {
            // 【核心觸發點】：定時器進來時，發現 RA4 已經變低電平（放開）了
            // 判斷放開時的累積秒數是否在 2 秒 到 10 秒之間（即大於等於2s，小於10s）
            if(high_time_sec >= 2 && high_time_sec < 10) {
                
                // ======= 執行編碼器數據清零代碼 =======
                Encoder_Clear_Data(); 
                DEE_Write(DEE_ENCODER_ZERO_L, (uint16_t)(Encoder_Config.SingleTurn_Data & 0xFFFF));
                DEE_Write(DEE_ENCODER_ZERO_H, (uint16_t)(Encoder_Config.SingleTurn_Data >> 16));
                // ===================================
            }
            else if (high_time_sec >= 10 && high_time_sec <= 60 ) {
                DEE_Write(DEE_ENCODER_ZERO_L, 0);
                DEE_Write(DEE_ENCODER_ZERO_H, 0);            
            }
            
            // 不管符不符合時間，只要放開了就停止這次計時
            SCCP1_Timer_Stop();
            is_counting = 0;
            high_time_sec = 0;
        }
    }
}