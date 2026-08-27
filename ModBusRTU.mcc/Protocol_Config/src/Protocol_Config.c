#include "MB_User_Config.h"
#include "Protocol_Config.h"
#include "sccp2.h"
#include "mb_user.h"

eMBErrorCode Modbus_Status;

uint16_t Protocol;
uint16_t Update_Time = 10;

uint16_t MB_Reg_Mode;

bool FreeMode_Enable;

void Protocol_Init(void)
{
    DEE_Read(DEE_Encoder_Protocol, &Protocol);
    DEE_Read(DEE_Update_Time, &Update_Time);

    if (Protocol != FreeMode && Protocol != ModBusRTU)
    {
        Protocol = ModBusRTU;
        DEE_Write(DEE_Encoder_Protocol, Protocol);
    }

    if (Protocol == ModBusRTU)
    {
        ModBusRTU_Init();
    }

    else if (Protocol == FreeMode)
    {
        SCCP2_Timer_Stop();      // stop timer2
        EXIT_CRITICAL_SECTION(); // enable global interrupt

        FreeMode_Init();
    }
}

void ModBusRTU_Init(void)
{
    eMBInit(MB_RTU, Slave_ID, 0, BaudRate, MB_Parity);
    if (Modbus_Status == MB_ENOERR)
    {
        Modbus_Status = eMBEnable();
    }
}

void MB_Reg_Init(void)
{
    switch(MB_Reg_Mode)
    {
        case MB_REG_FORCE16:
        {
            MB_Reg_Table = Holding_Reg_Table_16;
            MB_Reg_Table_Size = Holding_Reg_Table_16_Size;
            break;
        }

        case MB_REG_FORCE32:
        {
            MB_Reg_Table = Holding_Reg_Table_32;
            MB_Reg_Table_Size = Holding_Reg_Table_32_Size;
            break;
        }

        case MB_REG_AUTO:
        default:
        {
            if ((Encoder_Config.SingleTurn_Bit <= 16) && (Encoder_Config.MultiTurn_Bit <= 16))
            {
                MB_Reg_Table = Holding_Reg_Table_16;
                MB_Reg_Table_Size = Holding_Reg_Table_16_Size;
            }
            else
            {
                MB_Reg_Table = Holding_Reg_Table_32;
                MB_Reg_Table_Size = Holding_Reg_Table_32_Size;
            }

            break;
        }
    }
}

void FreeMode_Init(void)
{
    BaudRate = BaudRate_Get_Value(BaudRate_Index);
    UART1_BaudRateSet(BaudRate);
    if (Update_Time < 1 || Update_Time > 3000)
    {
        Update_Time = 10;
        DEE_Write(DEE_Update_Time, Update_Time);
        FreeMode_Enable = true;
    }
    else
    {
        FreeMode_Enable = true;
    }
}