#include "encoder.h"
#include "uart1.h"
#include "sccp1.h"

#define FreeMode_Data_Length 24

void FreeMode_Task(void);
void FreeMode_Process(void);
void FreeMode_Send(void);