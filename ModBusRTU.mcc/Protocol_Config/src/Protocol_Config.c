#include "MB_User_Config.h"
#include "Protocol_Config.h"
#include "sccp2.h"

eMBErrorCode Modbus_Status;
uint8_t Protocol;
uint16_t Update_Time = 10;
bool FreeMode_Enable;

void Protocol_Init(void)
{
    DEE_Read(DEE_Encoder_Protocol,&Protocol);
    DEE_Read(DEE_Update_Time,&Update_Time);

    if (Protocol != FreeMode && Protocol != ModBusRTU)
    {
        Protocol = ModBusRTU;
        DEE_Write(DEE_Encoder_Protocol,Protocol);
    }

    if (Protocol == ModBusRTU)
    {
        ModBusRTU_Init();
    }

    else if (Protocol == FreeMode)
    {
        SCCP2_Timer_Stop();      //stop timer2
        EXIT_CRITICAL_SECTION(); //enable global interrupt

        FreeMode_Init();
    }
}

void ModBusRTU_Init(void)
{
    eMBInit(MB_RTU, Slave_ID, 0, BaudRate, MB_Parity);
    if(Modbus_Status  == MB_ENOERR)
    {
        Modbus_Status  = eMBEnable();
    }
}

void FreeMode_Init(void)
{
    if(Update_Time < 1 || Update_Time > 3000)
    {
        Update_Time = 10;
        DEE_Write(DEE_Update_Time,Update_Time);
        FreeMode_Enable = true;
    }
    else
    {
        FreeMode_Enable = true;
    }
}