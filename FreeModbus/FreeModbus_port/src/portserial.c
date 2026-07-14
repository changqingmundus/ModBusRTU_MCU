/*
 * FreeModbus Libary: ATMega168 Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *   - Initial version and ATmega168 support
 * Modfications Copyright (C) 2006 Tran Minh Hoang:
 *   - ATmega8, ATmega16, ATmega32 support
 *   - RS485 support for DS75176
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

#include "port.h"
#include "pins.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include <stdbool.h>

#include <xc.h>
#include "uart1.h"

#define UART_BAUD_RATE          38400

//#define UART_UCSRB  UCSR0B

void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    if( xRxEnable )
    {
        IEC0bits.U1RXIE = 1;  //開啟RX中斷
    }
    else
    {
        IEC0bits.U1RXIE = 0;  //關閉RX中斷
    }

    if( xTxEnable )
    {
        IEC0bits.U1TXIE = 1;  //開啟TX中斷
    }
    else
    {
        IEC0bits.U1TXIE = 0;  //關閉TX中斷
    }
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    IEC0bits.U1TXIE = 0;
    IEC0bits.U1RXIE = 0;

    return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    UART1_Write(0x99);
    UART1_Write(ucByte);
    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    *pucByte = UART1_Read();
    return TRUE;
}

void UART1_TxCompleteCallback(void)
{
    pxMBFrameCBTransmitterEmpty(  );
}

void UART1_RxCompleteCallback(void)
{
    pxMBFrameCBByteReceived(  );
}