#include "Protool_Config.h"
#include "mb.h"
#include "MB_User_Config.h"

void Protool_Init(void)
{
    DEE_Read(DEE_Encoder_Protool,&Protool);

    if (Protool != FreeMode && Protool != ModBusRTU)
    {
        Protool = Protool_ModBusRTU;
    }

    if (Protool == Protool_ModBusRTU)
    {
        ModBusRTU_Init();
    }

    else if (Protool == Protool_FreeMode)
    {
        FreeMode_Init();
    }
}

void ModBusRTU_Init(void)
{
    eMBErrorCode eStatus;
    eMBInit(MB_RTU, Slave_ID, 0, BaudRate, Parity);
    eStatus = eMBEnable();
}

void FreeMode_Init(void)
{
    DEE_Read(DEE_Update_Time,&Update_Time);
    if(Update_Time > 1 && < 3000)
    {
        
    else
    {
        Update_Time = 10; 
    }

}