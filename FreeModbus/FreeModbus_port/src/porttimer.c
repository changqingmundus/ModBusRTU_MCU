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
    /* Calculate overflow counter an OCR values for Timer2. */

    usTimer50us = usTim1Timerout50us;
    vMBPortTimersDisable();

    return TRUE;
}

inline void
vMBPortTimersEnable()
{
    uint32_t count;

    count = (uint32_t)usTimer50us * 125U;

    //SCCP2_Timer_PeriodSet(count - 1);
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