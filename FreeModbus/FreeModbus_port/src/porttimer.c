/*
 * FreeModbus Libary: ATMega168 Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
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

/* ----------------------- AVR includes -------------------------------------*/

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include <xc.h>
#include "sccp2.h"
#include "MB_User_Config.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/

/* ----------------------- Static variables ---------------------------------*/
// static USHORT   usTimerOCRADelta;
// static USHORT   usTimerOCRBDelta;
volatile uint16_t usTimer50us;
/* ----------------------- Start implementation -----------------------------*/
BOOL xMBPortTimersInit(USHORT usTim1Timerout50us)
{
    uint32_t usTimer50us;
    uint32_t count;

    if (BaudRate > 19200)
    {
        // Modbus規範：固定1.75ms
        usTimer50us = 35;
    }
    else
    {
        // 45 bit時間，單位50us
        usTimer50us = (900000UL + BaudRate - 1) / BaudRate;
    }

    count = usTimer50us * 125UL;
    SCCP2_Timer_PeriodSet(count - 1);
    vMBPortTimersDisable();
    return TRUE;
}

inline void
vMBPortTimersEnable()
{
    /* Restart the timer. */
    SCCP2_Timer_Restart();
}

void SCCP2_Timer_Restart(void)
{
    CCP2TMRL = 0;
    CCP2TMRH = 0;

    IFS1bits.CCT2IF = 0;

    SCCP2_Timer_Start();
}

inline void
vMBPortTimersDisable()
{
    /* Disable the timer. */
    SCCP2_Timer_Stop();
}

void SCCP2_TimeoutCallback(void)
{
    (void)pxMBPortCBTimerExpired();
}