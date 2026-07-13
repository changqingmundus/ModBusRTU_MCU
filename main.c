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
#include "mbport.h"
#include "encoder.h"
#include "dee.h"
#include "system.h"

/*
    Main application
*/

extern volatile uint8_t debug_event;
extern volatile uint8_t debug_len;
extern volatile uint8_t debug_buf[];

int main(void)
{
    SYSTEM_Initialize();
    DEE_Init();

    Encoder_Init();
    
    eMBErrorCode eStatus;
    eStatus = eMBInit(MB_RTU, 1, 0, 38400, MB_PAR_NONE);
    eStatus = eMBEnable();

    if(eStatus == MB_ENOERR)
    {
        while(1)
       {
        Encoder_Read_Data();
        eMBPoll();
        if(debug_event)
        {
            debug_event = 0;
            UART1_Write(debug_len);
            for(uint8_t i = 0; i < 8; i++)
            {
                UART1_Write(debug_buf[i]);

            }
            
        } 
       }
    } 
}