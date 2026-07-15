 /**
 * PINS Generated Driver Source File 
 * 
 * @file      pins.c
 *            
 * @ingroup   pinsdriver
 *            
 * @brief     This is the generated driver source file for PINS driver.
 *
 * @skipline @version   Firmware Driver Version 1.0.2
 *
 * @skipline @version   PLIB Version 1.4.1
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

// Section: Includes
#include <xc.h>
#include <stddef.h>
#include "../pins.h"

// Section: File specific functions
static void (*SET_InterruptHandler)(void) = NULL;

// Section: Driver Interface Function Definitions
void PINS_Initialize(void)
{
    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATA = 0x0000U;
    LATB = 0x0004U;

    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    TRISA = 0x0016U;
    TRISB = 0xF7F9U;


    /****************************************************************************
     * Setting the Weak Pull Up and Weak Pull Down SFR(s)
     ***************************************************************************/
    CNPUA = 0x0009U;
    CNPUB = 0x0000U;
    CNPDA = 0x0010U;
    CNPDB = 0x0000U;


    /****************************************************************************
     * Setting the Open Drain SFR(s)
     ***************************************************************************/
    ODCA = 0x0000U;
    ODCB = 0x0000U;


    /****************************************************************************
     * Setting the Analog/Digital Configuration SFR(s)
     ***************************************************************************/
    ANSELA = 0x0006U;
    ANSELB = 0x0090U;

    /****************************************************************************
     * Set the PPS
     ***************************************************************************/
     __builtin_write_RPCON(0x0000); // unlock PPS

        RPINR18bits.U1RXR = 0x0023U; //RB3->UART1:U1RX;
        RPOR1bits.RP34R = 0x0001U;  //RB2->UART1:U1TX;

     __builtin_write_RPCON(0x0800); // lock PPS

    /*******************************************************************************
    * Interrupt On Change: positive
    *******************************************************************************/
    CNEN0Abits.CNEN0A4 = 1; //Pin : RA4U; 

    /****************************************************************************
     * Interrupt On Change: flag
     ***************************************************************************/
    CNFAbits.CNFA4 = 0;    //Pin : SET

    /****************************************************************************
     * Interrupt On Change: config
     ***************************************************************************/
    CNCONAbits.CNSTYLE = 1; //Config for PORTA
    CNCONAbits.ON = 1; //Config for PORTA

    /* Initialize IOC Interrupt Handler*/
    SET_SetInterruptHandler(&SET_CallBack);

    /****************************************************************************
     * Interrupt On Change: Interrupt Enable
     ***************************************************************************/
    IFS0bits.CNAIF = 0; //Clear CNAI interrupt flag
    IEC0bits.CNAIE = 1; //Enable CNAI interrupt
}

void __attribute__ ((weak)) SET_CallBack(void)
{

}

void SET_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC0bits.CNAIE = 0; //Disable CNAI interrupt
    SET_InterruptHandler = InterruptHandler; 
    IEC0bits.CNAIE = 1; //Enable CNAI interrupt
}

/* Interrupt service function for the CNAI interrupt. */
/* cppcheck-suppress misra-c2012-8.4
*
* (Rule 8.4) REQUIRED: A compatible declaration shall be visible when an object or 
* function with external linkage is defined
*
* Reasoning: Interrupt declaration are provided by compiler and are available
* outside the driver folder
*/
void __attribute__ (( interrupt, no_auto_psv )) _CNAInterrupt (void)
{
    if(CNFAbits.CNFA4 == 1)
    {
        if(SET_InterruptHandler != NULL) 
        { 
            SET_InterruptHandler(); 
        }
        
        CNFAbits.CNFA4 = 0;  //Clear flag for Pin - SET
    }
    
    // Clear the flag
    IFS0bits.CNAIF = 0;
}

