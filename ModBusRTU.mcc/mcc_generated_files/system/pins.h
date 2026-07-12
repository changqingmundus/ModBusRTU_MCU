/**
 * PINS Generated Driver Header File 
 * 
 * @file      pins.h
 *            
 * @defgroup  pinsdriver Pins Driver
 *            
 * @brief     The Pin Driver directs the operation and function of 
 *            the selected device pins using dsPIC MCUs.
 *
 * @skipline @version   Firmware Driver Version 1.0.2
 *
 * @skipline @version   PLIB Version 1.4.1
 *
 * @skipline  Device : dsPIC33CK256MP502
*/

/*
© [2026] Microchip Technology Inc. and its subsidiaries.

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

#ifndef PINS_H
#define PINS_H
// Section: Includes
#include <xc.h>

// Section: Device Pin Macros

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RA0 GPIO Pin which has a custom name of LED0 to High
 * @pre      The RA0 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED0_SetHigh()          (_LATA0 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RA0 GPIO Pin which has a custom name of LED0 to Low
 * @pre      The RA0 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED0_SetLow()           (_LATA0 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RA0 GPIO Pin which has a custom name of LED0
 * @pre      The RA0 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED0_Toggle()           (_LATA0 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RA0 GPIO Pin which has a custom name of LED0
 * @param    none
 * @return   none  
 */
#define LED0_GetValue()         _RA0

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RA0 GPIO Pin which has a custom name of LED0 as Input
 * @param    none
 * @return   none  
 */
#define LED0_SetDigitalInput()  (_TRISA0 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RA0 GPIO Pin which has a custom name of LED0 as Output
 * @param    none
 * @return   none  
 */
#define LED0_SetDigitalOutput() (_TRISA0 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RA3 GPIO Pin which has a custom name of LED1 to High
 * @pre      The RA3 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED1_SetHigh()          (_LATA3 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RA3 GPIO Pin which has a custom name of LED1 to Low
 * @pre      The RA3 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED1_SetLow()           (_LATA3 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RA3 GPIO Pin which has a custom name of LED1
 * @pre      The RA3 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED1_Toggle()           (_LATA3 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RA3 GPIO Pin which has a custom name of LED1
 * @param    none
 * @return   none  
 */
#define LED1_GetValue()         _RA3

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RA3 GPIO Pin which has a custom name of LED1 as Input
 * @param    none
 * @return   none  
 */
#define LED1_SetDigitalInput()  (_TRISA3 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RA3 GPIO Pin which has a custom name of LED1 as Output
 * @param    none
 * @return   none  
 */
#define LED1_SetDigitalOutput() (_TRISA3 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RA4 GPIO Pin which has a custom name of SET to High
 * @pre      The RA4 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define SET_SetHigh()          (_LATA4 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RA4 GPIO Pin which has a custom name of SET to Low
 * @pre      The RA4 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define SET_SetLow()           (_LATA4 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RA4 GPIO Pin which has a custom name of SET
 * @pre      The RA4 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define SET_Toggle()           (_LATA4 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RA4 GPIO Pin which has a custom name of SET
 * @param    none
 * @return   none  
 */
#define SET_GetValue()         _RA4

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RA4 GPIO Pin which has a custom name of SET as Input
 * @param    none
 * @return   none  
 */
#define SET_SetDigitalInput()  (_TRISA4 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RA4 GPIO Pin which has a custom name of SET as Output
 * @param    none
 * @return   none  
 */
#define SET_SetDigitalOutput() (_TRISA4 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB11 GPIO Pin which has a custom name of SCLK_MA to High
 * @pre      The RB11 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define SCLK_MA_SetHigh()          (_LATB11 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB11 GPIO Pin which has a custom name of SCLK_MA to Low
 * @pre      The RB11 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define SCLK_MA_SetLow()           (_LATB11 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RB11 GPIO Pin which has a custom name of SCLK_MA
 * @pre      The RB11 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define SCLK_MA_Toggle()           (_LATB11 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RB11 GPIO Pin which has a custom name of SCLK_MA
 * @param    none
 * @return   none  
 */
#define SCLK_MA_GetValue()         _RB11

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB11 GPIO Pin which has a custom name of SCLK_MA as Input
 * @param    none
 * @return   none  
 */
#define SCLK_MA_SetDigitalInput()  (_TRISB11 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB11 GPIO Pin which has a custom name of SCLK_MA as Output
 * @param    none
 * @return   none  
 */
#define SCLK_MA_SetDigitalOutput() (_TRISB11 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB13 GPIO Pin which has a custom name of MISO_SLO to High
 * @pre      The RB13 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define MISO_SLO_SetHigh()          (_LATB13 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB13 GPIO Pin which has a custom name of MISO_SLO to Low
 * @pre      The RB13 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define MISO_SLO_SetLow()           (_LATB13 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RB13 GPIO Pin which has a custom name of MISO_SLO
 * @pre      The RB13 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define MISO_SLO_Toggle()           (_LATB13 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RB13 GPIO Pin which has a custom name of MISO_SLO
 * @param    none
 * @return   none  
 */
#define MISO_SLO_GetValue()         _RB13

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB13 GPIO Pin which has a custom name of MISO_SLO as Input
 * @param    none
 * @return   none  
 */
#define MISO_SLO_SetDigitalInput()  (_TRISB13 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB13 GPIO Pin which has a custom name of MISO_SLO as Output
 * @param    none
 * @return   none  
 */
#define MISO_SLO_SetDigitalOutput() (_TRISB13 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Initializes the PINS module
 * @param    none
 * @return   none  
 */
void PINS_Initialize(void);

/**
 * @ingroup  pinsdriver
 * @brief    This function is callback for SET Pin
 * @param    none
 * @return   none   
 */
void SET_CallBack(void);


/**
 * @ingroup    pinsdriver
 * @brief      This function assigns a function pointer with a callback address
 * @param[in]  InterruptHandler - Address of the callback function 
 * @return     none  
 */
void SET_SetInterruptHandler(void (* InterruptHandler)(void));


#endif
