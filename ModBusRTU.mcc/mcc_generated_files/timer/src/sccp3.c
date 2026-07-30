/**
 * SCCP3-TIMER Generated Driver Source File
 * 
 * @file      sccp3.c
 * 
 * @ingroup   timerdriver
 * 
 * @brief     This is the generated driver source file for SCCP3-TIMER driver
 *
 * @skipline @version   Firmware Driver Version 1.6.1
 *
 * @skipline @version   PLIB Version 1.6.7
 *
 * @skipline  Device : dsPIC33CK256MP502
*/

/*
? [2026] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

// Section: Included Files

#include <stddef.h> 
#include "../sccp3.h"
#include "../timer_interface.h"

// Section: Data Type Definitions

#define MASK_32_BIT_LOW 0x0000FFFFU
#define MASK_32_BIT_HIGH 0xFFFF0000U

// Section: File specific functions

static void (*SCCP3_TimeoutHandler)(void) = NULL;

// Section: Driver Interface

// Defines an object for TIMER_INTERFACE

const struct TIMER_INTERFACE Timer3 = {
    .Initialize     = &SCCP3_Timer_Initialize,
    .Deinitialize   = &SCCP3_Timer_Deinitialize,
    .Start          = &SCCP3_Timer_Start,
    .Stop           = &SCCP3_Timer_Stop,
    #if TIMER_PERIODCOUNTSET_API_SUPPORT
    .PeriodCountSet = &SCCP3_Timer_PeriodCountSet,
    #endif
    .PeriodSet      = &SCCP3_Timer_PeriodSet,
    .CounterGet     = &SCCP3_Timer_CounterGet,
    .PeriodGet	    = &SCCP3_Timer_PeriodGet,
    .InterruptPrioritySet = &SCCP3_Timer_InterruptPrioritySet,
    .TimeoutCallbackRegister = &SCCP3_Timer_TimeoutCallbackRegister,
    .Tasks          = NULL,
};

// Section: Driver Interface Function Definitions

void SCCP3_Timer_Initialize(void)
{
    // MOD 16-Bit/32-Bit Timer; CCSEL disabled; TMR32 16 Bit; TMRPS 1:1; CLKSEL FOSC/2; TMRSYNC disabled; CCPSLP disabled; CCPSIDL disabled; CCPON disabled; 
    CCP3CON1L = 0x0U; //The module is disabled, till other settings are configured
    //SYNC None; ALTSYNC disabled; ONESHOT disabled; TRIGEN disabled; IOPS Each Time Base Period Match; RTRGEN disabled; OPSRC Timer Interrupt Event; 
    CCP3CON1H = 0x0U;
    //ASDG 0x0; SSDG disabled; ASDGM disabled; PWMRSEN disabled; 
    CCP3CON2L = 0x0U;
    //ICSEL ; AUXOUT Disabled; ICGSM Level-Sensitive mode; OCAEN disabled; OENSYNC disabled; 
    CCP3CON2H = 0x0U;
    //PSSACE Tri-state; POLACE disabled; OSCNT None; OETRIG disabled; 
    CCP3CON3H = 0x0U;
    //ICOV disabled; ICDIS disabled; SCEVT disabled; ASEVT disabled; TRCLR disabled; TRSET disabled; ICGARM disabled; 
    CCP3STATL = 0x0U;
    //TMRL 0x0000; 
    CCP3TMRL = 0x0U;
    //TMRH 0x0000; 
    CCP3TMRH = 0x0U;
    //PRL 39999; 
    CCP3PRL = 0x9C3FU;
    //PRH 0; 
    CCP3PRH = 0x0U;
    //CMPA 0; 
    CCP3RA = 0x0U;
    //CMPB 0; 
    CCP3RB = 0x0U;
    //BUFL 0x0000; 
    CCP3BUFL = 0x0U;
    //BUFH 0x0000; 
    CCP3BUFH = 0x0U;
    
    SCCP3_Timer_TimeoutCallbackRegister(&SCCP3_TimeoutCallback);

    IFS2bits.CCT3IF = 0;
    // Enabling SCCP3 interrupt
    IEC2bits.CCT3IE = 1;

    CCP3CON1Lbits.CCPON = 1; //Enable Module
}

void SCCP3_Timer_Deinitialize(void)
{
    CCP3CON1Lbits.CCPON = 0;
    
    IFS2bits.CCT3IF = 0;
    IEC2bits.CCT3IE = 0;
    
    CCP3CON1L = 0x0U; 
    CCP3CON1H = 0x0U; 
    CCP3CON2L = 0x0U; 
    CCP3CON2H = 0x100U; 
    CCP3CON3H = 0x0U; 
    CCP3STATL = 0x0U; 
    CCP3TMRL = 0x0U; 
    CCP3TMRH = 0x0U; 
    CCP3PRL = 0xFFFFU; 
    CCP3PRH = 0xFFFFU; 
    CCP3RA = 0x0U; 
    CCP3RB = 0x0U; 
    CCP3BUFL = 0x0U; 
    CCP3BUFH = 0x0U; 
}

void SCCP3_Timer_Start(void)
{
    IFS2bits.CCT3IF = 0;
    // Enable SCCP3 interrupt
    IEC2bits.CCT3IE = 1;
    
    CCP3CON1Lbits.CCPON = 1;
}

void SCCP3_Timer_Stop(void)
{
    CCP3CON1Lbits.CCPON = 0;
    
    IFS2bits.CCT3IF = 0;
    // Disable SCCP3 interrupt
    IEC2bits.CCT3IE = 0;
}

void SCCP3_Timer_PeriodSet(uint32_t count)
{
    if(count > 0xFFFFU)
    {
        CCP3PRL = (uint16_t)(count & MASK_32_BIT_LOW);
        CCP3PRH = (uint16_t)((count & MASK_32_BIT_HIGH) >> 16);
        CCP3CON1Lbits.T32 = 1;
    }
    else
    {
        CCP3PRL = (uint16_t)(count & MASK_32_BIT_LOW);
        CCP3CON1Lbits.T32 = 0;
    }
}

uint32_t SCCP3_Timer_CounterGet(void)
{ 
    uint16_t tmrLow;
    uint16_t tmrHigh;
    uint16_t tmrLowRepeat;
    
    if(CCP3CON1Lbits.T32 == 1)
    {
        tmrLow = CCP3TMRL;
        tmrHigh = CCP3TMRH;
        tmrLowRepeat = CCP3TMRL;
        
        if(tmrLowRepeat < tmrLow)
        {
            return (((uint32_t)CCP3TMRH << 16U) | tmrLowRepeat); 
        }
        else 
        {
            return (((uint32_t)tmrHigh << 16U) | tmrLowRepeat);
        }
    }
    else
    {
        return (uint32_t)CCP3TMRL;
    }
}

void SCCP3_Timer_InterruptPrioritySet(enum INTERRUPT_PRIORITY priority)
{
    IPC9bits.CCT3IP = priority;
}

void SCCP3_Timer_TimeoutCallbackRegister(void (*handler)(void))
{
    if(NULL != handler)
    {
        SCCP3_TimeoutHandler = handler;
    }
}

void SCCP3_TimeoutCallbackRegister(void* handler)
{
    if(NULL != handler)
    {
        SCCP3_TimeoutHandler = handler;
    }
}

void __attribute__ ((weak)) SCCP3_TimeoutCallback (void)
{ 

} 

/* cppcheck-suppress misra-c2012-8.4
*
* (Rule 8.4) REQUIRED: A compatible declaration shall be visible when an object or 
* function with external linkage is defined
*
* Reasoning: Interrupt declaration are provided by compiler and are available
* outside the driver folder
*/
void __attribute__ ( ( interrupt, no_auto_psv ) ) _CCT3Interrupt (void)
{
    if(NULL != SCCP3_TimeoutHandler)
    {
        (*SCCP3_TimeoutHandler)();
    }
    IFS2bits.CCT3IF = 0;
}

#if TIMER_PERIODCOUNTSET_API_SUPPORT
void SCCP3_PeriodCountSet(size_t count)
{
    CCP3PRL = count & MASK_32_BIT_LOW;
    CCP3PRH = (count & MASK_32_BIT_HIGH) >> 16;
}
#endif

/**
 End of File
*/
