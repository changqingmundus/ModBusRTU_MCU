#include "Free_Mode.h"
#include "Protool_Config.h"

void FreeMode_Task(void)
{
    Encoder_Read_Data();

    
    DELAY_microseconds(Update_Time);
}