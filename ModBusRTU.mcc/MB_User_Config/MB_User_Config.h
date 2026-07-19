#ifndef MB_USER_CONFIG_H
#define MB_USER_CONFIG_H

#include "mb.h"
#include "dee.h"


// Modbus User Config DEE Address
#define DEE_SLAVE_ID       10
#define DEE_BAUDRATE_INDEX 11
#define DEE_PARITY         12


extern uint16_t Slave_ID;
extern uint16_t BaudRate_Index;
extern uint16_t Parity;

extern uint32_t BaudRate;
extern eMBParity MB_Parity;

extern volatile uint8_t BaudRate_Update_Flag;
extern volatile uint32_t New_BaudRate;


void MB_User_Config_Init(void);


#endif