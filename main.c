/*
� [2026] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip
    software and any derivatives exclusively with Microchip products.
    You are responsible for complying with 3rd party license terms
    applicable to your use of 3rd party software (including open source
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.?
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR
    THIS SOFTWARE.
*/
#include "mb.h"
#include "uart1.h"
#include "encoder.h"
#include "MB_User_Config.h"
#include "sccp1.h"
#include "system.h"

/*
    Main application
*/

extern volatile uint8_t debug_event;
extern volatile uint8_t debug_byte;
extern volatile uint8_t BaudRate_Update_Flag;
extern volatile uint32_t New_BaudRate;

int main(void)
{
    SYSTEM_Initialize();
    DEE_Init();
    SET_SetInterruptHandler(ClearData_CN_Callback);
    Timer1_TimeoutCallbackRegister(ClearData_Timer_Callback);

    Encoder_Init();
    MB_User_Config_Init();

    eMBErrorCode eStatus;
    eMBInit(MB_RTU,Slave_ID,0,BaudRate,Parity);
    eStatus = eMBEnable();

    if (eStatus == MB_ENOERR)
    {
        while (1)
        {
            // Encoder_Read_Data();
            eMBPoll();
            LED1_SetHigh();
            if (debug_event == 1)
            {
                debug_event = 0;
                UART1_Write(debug_byte);
            }
            if (BaudRate_Update_Flag)
            {
                if (UART1_IsTxDone())
                {
                    BaudRate_Update_Flag = 0;

                    UART1_BaudRateSet(New_BaudRate);
                }
            }
        }
    }
}