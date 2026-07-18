#include "Protool_Config.h"
#include "MB_User_Config.h"

eMBErrorCode Modbus_Status;
uint8_t Protocol;
uint8_t Update_Time;

void Protool_Init(void)
{
    DEE_Read(DEE_Encoder_Protool,&Protocol);

    if (Protocol != FreeMode && Protocol != ModBusRTU)
    {
        Protocol = ModBusRTU;
    }

    if (Protocol == ModBusRTU)
    {
        ModBusRTU_Init();
    }

    else if (Protocol == FreeMode)
    {
        FreeMode_Init();
    }
}

void ModBusRTU_Init(void)
{
    eMBInit(MB_RTU, Slave_ID, 0, BaudRate, Parity);
    if(Modbus_Status  == MB_ENOERR)
    {
        Modbus_Status  = eMBEnable();
    }
}

void FreeMode_Init(void)
{
    DEE_Read(DEE_Update_Time,&Update_Time);
    if(Update_Time > 1 && Update_Time < 3000)
    {
        Update_Time = 10;
    }
    else
    {
        Update_Time = 10; 
    }

}