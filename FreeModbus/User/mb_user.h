#ifndef MB_USER_H
#define MB_USER_H

#include <stdint.h>

typedef uint16_t (*MB_ReadFunc)(uint64_t Encoder_Value);

typedef void (*MB_WriteFunc)(uint16_t value);

typedef struct
{
    uint16_t address;

    MB_ReadFunc read;

    MB_WriteFunc write;

} MB_RegMap_t;

extern MB_RegMap_t Holding_Reg_Table_16[];

extern MB_RegMap_t Holding_Reg_Table_32[];

extern uint16_t Holding_Reg_Table_16_Size;

extern uint16_t Holding_Reg_Table_32_Size;

extern MB_RegMap_t *MB_Reg_Table;

extern uint16_t MB_Reg_Table_Size;

extern uint64_t Encoder_Write_Position;

/* ==============================
 * Modbus Holding Register API
 * ============================== */

/* ==============================
 * Encoder Register Read
 * ============================== */

uint16_t MB_Read_SingleTurn_Low(uint64_t Encoder_Value);

uint16_t MB_Read_SingleTurn_High(uint64_t Encoder_Value);

uint16_t MB_Read_MultiTurn_Low(uint64_t Encoder_Value);

uint16_t MB_Read_MultiTurn_High(uint64_t Encoder_Value);

uint16_t MB_Read_SlaveID(uint64_t Encoder_Value);

uint16_t MB_Read_BaudRate(uint64_t Encoder_Value);

uint16_t MB_Read_Parity(uint64_t Encoder_Value);

uint16_t MB_Read_Zero(uint64_t Encoder_Value);

uint16_t MB_Read_Direction(uint64_t Encoder_Value);

uint16_t MB_Read_Protocol(uint64_t Encoder_Value);

uint16_t MB_Read_UpdateTime(uint64_t Encoder_Value);

uint16_t MB_Read_Rotation_Direction(uint64_t Encoder_Value);

uint16_t MB_Read_RPM(uint64_t Encoder_Value);

uint16_t MB_Read_Origin_Mode(uint64_t Encoder_Value);

uint16_t MB_Read_Speed_Period(uint64_t Encoder_Value);

uint16_t MB_Read_Total_0(uint64_t Encoder_Value);

uint16_t MB_Read_Total_1(uint64_t Encoder_Value);

uint16_t MB_Read_Total_2(uint64_t Encoder_Value);

uint16_t MB_Read_Total_3(uint64_t Encoder_Value);

/* ==============================
 * Encoder Register Write
 * ============================== */

void MB_Write_SlaveID(uint16_t value);

void MB_Write_BaudRate(uint16_t value);

void MB_Write_Parity(uint16_t value);

void MB_Write_Clear_Encoder(uint16_t value);

void MB_Write_Direction(uint16_t value);

void MB_Write_Protocol(uint16_t value);

void MB_Write_UpdateTime(uint16_t value);

void MB_Write_Origin_Mode(uint16_t value);

void MB_Write_Speed_Period(uint16_t value);

void MB_Write_Position_0(uint16_t value);

void MB_Write_Position_1(uint16_t value);

void MB_Write_Position_2(uint16_t value);

void MB_Write_Position_3(uint16_t value);

#endif