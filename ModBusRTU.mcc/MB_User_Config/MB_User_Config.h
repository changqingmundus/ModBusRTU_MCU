#ifndef MB_USER_CONFIG_H
#define MB_USER_CONFIG_H

#include "mb.h"
#include "dee.h"
#include "uart1.h"
#include "encoder.h"


// Modbus User Config DEE Address
#define DEE_MODBUS_MagicKey 9
#define DEE_SLAVE_ID       10
#define DEE_BAUDRATE_INDEX 11
#define DEE_PARITY         12

#define MODBUS_MAGIC_KEY 0x510A

extern uint16_t Slave_ID;
extern uint16_t BaudRate_Index;
extern uint16_t Parity;

extern uint32_t BaudRate;
extern eMBParity MB_Parity;

extern volatile uint16_t usTimer50us;

extern volatile uint8_t BaudRate_Update_Flag;
extern volatile uint32_t New_BaudRate;
extern volatile uint8_t Parity_Update_Flag;

extern volatile uint8_t debug_flag;
extern volatile uint8_t debug_data;

uint32_t BaudRate_Get_Value(uint16_t index);
void MB_User_Config_Init(void);

void ModBusRTU_Update(void);
void MB_Timer_Update(uint32_t baud);
void UART1_Parity_Set(uint8_t parity);


#endif