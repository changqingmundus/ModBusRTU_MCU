#include "Protool_Config.h"
#include "MB_User_Config.h"

eMBErrorCode Modbus_Status;
uint8_t Protocol;
uint16_t Update_Time;
bool FreeMode_Enable;

void Protool_Init(void)
{
    DEE_Read(DEE_Encoder_Protool,&Protocol);

    if (Protocol != FreeMode && Protocol != ModBusRTU)
    {
        Protocol = ModBusRTU;
        DEE_Write(DEE_Encoder_Protool,Protocol);
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
    if(Update_Time < 1 || Update_Time > 3000)
    {
        Update_Time = 10;
        DEE_Write(DEE_Update_Time,Update_Time);
        FreeMode_Enable = true;
    }
    FreeMode_Enable = true;
}