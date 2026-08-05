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
#include "Free_Mode.h"
#include "MB_User_Config.h"
#include "Protocol_Config.h"
#include "mb.h"
#include "sccp1.h"
#include "system.h"
#include "uart1.h"

/*
    Main application
*/

int main(void)
{
  SYSTEM_Initialize();
  DEE_Init();
  SET_SetInterruptHandler(ClearData_CN_Callback);
  Timer1_TimeoutCallbackRegister(ClearData_Timer_Callback);

  Encoder_PowerOn_Reset_Check();
  MB_User_Config_Init();
  Protocol_Init();
  Encoder_Init();

  if (Protocol == ModBusRTU)
  {
    while (1)
    {
      if (Modbus_Status == MB_ENOERR)
      {
        eMBPoll();
        LED1_SetHigh();
        ModBusRTU_Update();
      }
    }
  }

  else if (Protocol == FreeMode)
  {
    if (FreeMode_Enable)
    {
      while (1)
      {
        FreeMode_Task();
        LED1_SetHigh();
      }
    }
  }
}