
/**
 * SPI1 Generated Driver Source File
 * 
 * @file        spi1.c
 * 
 * @ingroup     spihostdriver
 * 
 * @brief       This is the generated driver source file for SPI1 driver.
 *
 * @skipline @version     Firmware Driver Version 1.1.3
 *
 * @skipline @version     PLIB Version 1.4.4
 *
 * @skipline    Device : dsPIC33CK256MP502
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
#include <xc.h>
#include "../spi1.h"

// Section: File specific data type definitions

/** 
 @ingroup spidriver
  @brief Dummy data to be sent for half duplex communication.
*/
#define SPI1_DUMMY_DATA 0x0

//Defines an object for SPI_HOST_INTERFACE.

const struct SPI_HOST_INTERFACE SPI1_Host = {
    .Initialize         = &SPI1_Initialize,
    .Deinitialize       = &SPI1_Deinitialize,
    .Close              = &SPI1_Close,
    .Open               = &SPI1_Open,
    .BufferExchange     = &SPI1_BufferExchange,
    .BufferRead         = &SPI1_BufferRead,
    .BufferWrite        = &SPI1_BufferWrite,
    .ByteExchange       = &SPI1_ByteExchange,
    .ByteRead           = &SPI1_ByteRead,    
    .ByteWrite          = &SPI1_ByteWrite,
    .IsRxReady          = &SPI1_IsRxReady,
    .IsTxReady          = &SPI1_IsTxReady,
};
        

/**
 @ingroup spihostdriver
 @struct SPI1_CONFIG 
 @brief Defines the SPI1 configuration.
*/
struct SPI1_HOST_CONFIG
{ 
    uint16_t controlRegister1; //SPI1BRGL
    uint16_t controlRegister2; //SPI1CON1L
};

static const struct SPI1_HOST_CONFIG config[] = {  
                                        { 
                                            /*Configuration setting for HOST_CONFIG.
                                            SPI Mode : Mode 1, Sampled at : Middle, Data Width : 8 bits, Clock Frequency : 1000 kHz*/
                                            0x13U,//SPI1BRGL
                                            0x21U,//SPI1CON1L
                                        },
                                    };

// Section: Driver Interface Function Definitions

void SPI1_Initialize (void)
{
    // SPIBRGL 19; 
    SPI1BRGL = 0x13U;
    // AUDEN disabled; FRMEN disabled; AUDMOD I2S; FRMSYPW One clock wide; AUDMONO stereo; FRMCNT 0x0; MSSEN disabled; FRMPOL disabled; IGNROV disabled; SPISGNEXT not sign-extended; FRMSYNC disabled; URDTEN disabled; IGNTUR disabled; 
    SPI1CON1H = 0x0U;
    // WLENGTH 0; 
    SPI1CON2L = 0x0U;
    // SPIROV disabled; FRMERR disabled; 
    SPI1STATL = 0x0U;
    // SPIURDTL 0; 
    SPI1URDTL = 0x0U;
    // SPIURDTH 0; 
    SPI1URDTH = 0x0U;
    // SPIEN disabled; DISSDO disabled; MCLKEN FOSC/2; CKP Idle:Low, Active:High; SSEN disabled; MSTEN Host; MODE16 disabled; SMP Middle; DISSCK disabled; SPIFE Frame Sync pulse precedes; CKE Idle to Active; MODE32 disabled; SPISIDL disabled; ENHBUF enabled; DISSDI disabled; 
    SPI1CON1L = 0xA1U;
}

void SPI1_Deinitialize (void)
{
    SPI1_Close();
    
    SPI1BRGL = 0x0U;
    SPI1CON1L = 0x0U;
    SPI1CON1H = 0x0U;
    SPI1CON2L = 0x0U;
    SPI1STATL = 0x28U;
    SPI1URDTL = 0x0U;
    SPI1URDTH = 0x0U;
}

void SPI1_Close(void)
{
    SPI1CON1Lbits.SPIEN = 0U;
}

bool SPI1_Open(uint8_t spiConfigIndex)
{
    bool status = false;
    if(!SPI1CON1Lbits.SPIEN)
    {
        SPI1BRGL = config[spiConfigIndex].controlRegister1;
        SPI1CON1L = config[spiConfigIndex].controlRegister2;
        SPI1CON1Lbits.SPIEN = 1U;
        
        status = true;
    }
    return status;
}

uint8_t SPI1_ByteExchange(uint8_t byteData)
{
    while(1U == SPI1STATLbits.SPITBF)
    {

    }

    SPI1BUFL = byteData;

    while (1U == SPI1STATLbits.SPIRBE)
    {
    
    }

    return SPI1BUFL;
}

void SPI1_ByteWrite(uint8_t byteData)
{
    while(1U == SPI1STATLbits.SPITBF)
    {

    }
    
    SPI1BUFL = byteData;
}

uint8_t SPI1_ByteRead(void)
{
    while (1U == SPI1STATLbits.SPIRBE)
    {
    
    }
    
    return SPI1BUFL;
}

void SPI1_BufferExchange(void *bufferData, size_t bufferSize)
{
    uint16_t dataSentCount = 0U;
    uint16_t dataReceivedCount = 0U;

    while(1U == SPI1STATLbits.SPITBF)
    {

    }

    if (SPI1CON1Lbits.MODE32 == 1U)
    {
        // ---------------- 32-bit Mode  ----------------
        uint16_t *data = (uint16_t *)bufferData;
        uint16_t sendAddressIncrement = 0U;
        uint16_t receiveAddressIncrement = 0U;

        bufferSize >>= 2U;

        while (dataSentCount < bufferSize)
        {
            if ( 1U != SPI1STATLbits.SPITBF )
            {
                SPI1BUFL = data[sendAddressIncrement];
                SPI1BUFH = data[sendAddressIncrement + 1U];
                sendAddressIncrement += 2U;
                dataSentCount++;
            }

            if (0U == SPI1STATLbits.SPIRBE)
            {
                data[receiveAddressIncrement] = SPI1BUFL;
                data[receiveAddressIncrement + 1U] = SPI1BUFH;
                receiveAddressIncrement += 2U;
                dataReceivedCount++;
            }
        }
        while (dataReceivedCount < bufferSize)
        {
            if (0U == SPI1STATLbits.SPIRBE)
            {
                data[receiveAddressIncrement] = SPI1BUFL;
                data[receiveAddressIncrement + 1U] = SPI1BUFH;
                receiveAddressIncrement += 2U;
                dataReceivedCount++;
            }
        }
    }
    else if(SPI1CON1Lbits.MODE16 == 1U)
    {
            // ---------------- 16-bit Mode --------------------
            uint16_t *data = (uint16_t *)bufferData;
            bufferSize >>= 1U;

            while (dataSentCount < bufferSize)
            {
                if ( 1U != SPI1STATLbits.SPITBF )
                {
                    SPI1BUFL = data[dataSentCount];
                    dataSentCount++;
                }

                if (0U == SPI1STATLbits.SPIRBE)
                {
                    data[dataReceivedCount] = SPI1BUFL;
                    dataReceivedCount++;
                }
            }
            while (dataReceivedCount < bufferSize)
            {
                if (0U == SPI1STATLbits.SPIRBE)
                {
                    data[dataReceivedCount] = SPI1BUFL;
                    dataReceivedCount++;
                }
            }
    }
    else
    {
        // ---------------- 8-bit Mode --------------------
        uint8_t *data = (uint8_t *)bufferData;

        while (dataSentCount < bufferSize)
        {
            if ( 1U != SPI1STATLbits.SPITBF )
            {
                SPI1BUFL = data[dataSentCount];
                dataSentCount++;
            }

            if (0U == SPI1STATLbits.SPIRBE)
            {
                data[dataReceivedCount] = SPI1BUFL;
                dataReceivedCount++;
            }
        }
        while (dataReceivedCount < bufferSize)
        {
            if (0U == SPI1STATLbits.SPIRBE)
            {
                data[dataReceivedCount] = SPI1BUFL;
                dataReceivedCount++;
            }
        }
    }
}

void SPI1_BufferWrite(void *bufferData, size_t bufferSize)
{   
    uint16_t dataSentCount = 0U;
    uint16_t dataReceivedCount = 0U;

    while(1U == SPI1STATLbits.SPITBF)
    {

    }   
    // ---------------- 32-bit Mode  ----------------
    if (SPI1CON1Lbits.MODE32 == 1U)
    {
        uint16_t *data = (uint16_t *)bufferData; 
        uint16_t addressIncrement = 0U;

        bufferSize >>= 2U;

        while (dataSentCount < bufferSize)
        {
            if ( 1U != SPI1STATLbits.SPITBF )
            {
                SPI1BUFL = data[addressIncrement];
                SPI1BUFH = data[addressIncrement + 1U];
                addressIncrement += 2U;
                dataSentCount++;
            }

            if (0U == SPI1STATLbits.SPIRBE)
            {
                (void)SPI1BUFL; //Dummy Read
                (void)SPI1BUFH;
                dataReceivedCount++;
            }

        }
        while (dataReceivedCount < bufferSize)
        {
            if (0U == SPI1STATLbits.SPIRBE)
            {
                (void)SPI1BUFL; //Dummy Read
                (void)SPI1BUFH;
                dataReceivedCount++;
            }
        }  
    } 
    else if(SPI1CON1Lbits.MODE16 == 1U)
    {
        // ---------------- 16-bit Mode --------------------
        uint16_t *data = (uint16_t *)bufferData;    
        bufferSize >>= 1;

        while (dataSentCount < bufferSize)
        {
            if ( 1U != SPI1STATLbits.SPITBF )
            {
                SPI1BUFL = data[dataSentCount];
                dataSentCount++;
            }

            if (0U == SPI1STATLbits.SPIRBE)
            {
                (void)SPI1BUFL; //Dummy Read
                dataReceivedCount++;
            }
        }
        while (dataReceivedCount < bufferSize)
        {
            if (0U == SPI1STATLbits.SPIRBE)
            {
                (void)SPI1BUFL; //Dummy Read
                dataReceivedCount++;
            }
        }  
    }
    else
    {
        // ---------------- 8-bit Mode --------------------
        uint8_t *data = (uint8_t *)bufferData;

        while (dataSentCount < bufferSize)
        {
            if ( 1U != SPI1STATLbits.SPITBF )
            {
                SPI1BUFL = data[dataSentCount];
                dataSentCount++;
            }

            if (0U == SPI1STATLbits.SPIRBE)
            {
                (void)SPI1BUFL; //Dummy Read
                dataReceivedCount++;
            }
        }
        while (dataReceivedCount < bufferSize)
        {
            if (0U == SPI1STATLbits.SPIRBE)
            {
                (void)SPI1BUFL; //Dummy Read
                dataReceivedCount++;
            }
        }  
    }
}

void SPI1_BufferRead(void *bufferData, size_t bufferSize)
{   
    uint16_t dataSentCount = 0U;
    uint16_t dataReceivedCount = 0U;
    
    while(1U == SPI1STATLbits.SPITBF)
    {

    }

    // ---------------- 32-bit Mode  ----------------
    if (SPI1CON1Lbits.MODE32 == 1U)
    {
        uint16_t *data = (uint16_t *)bufferData;
        const uint16_t wData = SPI1_DUMMY_DATA;
        uint16_t addressIncrement = 0U;

        bufferSize >>= 2U;

        while (dataSentCount < bufferSize)
        {
            if ( 1U != SPI1STATLbits.SPITBF )
            {
                SPI1BUFL = wData;
                SPI1BUFH = wData;
                dataSentCount++;
            }

            if (0U == SPI1STATLbits.SPIRBE)
            {
                data[addressIncrement] = SPI1BUFL;
                data[addressIncrement + 1U] = SPI1BUFH;
                addressIncrement += 2U;
                dataReceivedCount++;
            }
        }
        while (dataReceivedCount < bufferSize)
        {
            if (0U == SPI1STATLbits.SPIRBE)
            {
                data[addressIncrement] = SPI1BUFL;
                data[addressIncrement + 1U] = SPI1BUFH;
                addressIncrement += 2U;
                dataReceivedCount++;
            }
        }
    }
    else if(SPI1CON1Lbits.MODE16 == 1U)
    {
        // ---------------- 16-bit Mode --------------------
        uint16_t *data = (uint16_t *)bufferData; 
        const uint16_t wData = SPI1_DUMMY_DATA;  
        bufferSize >>= 1;

        while (dataSentCount < bufferSize)
        {
            if ( 1U != SPI1STATLbits.SPITBF )
            {
                SPI1BUFL = wData;
                dataSentCount++;
            }

            if (0U == SPI1STATLbits.SPIRBE)
            {
                data[dataReceivedCount] = SPI1BUFL;
                dataReceivedCount++;
            }
        }
        while (dataReceivedCount < bufferSize)
        {
            if (0U == SPI1STATLbits.SPIRBE)
            {
                data[dataReceivedCount] = SPI1BUFL;
                dataReceivedCount++;
            }
        } 
    }
    else
    {
        // ---------------- 8-bit Mode --------------------
        uint8_t *data = (uint8_t *)bufferData;
        const uint8_t wData = SPI1_DUMMY_DATA;

        while (dataSentCount < bufferSize)
        {
            if ( 1U != SPI1STATLbits.SPITBF )
            {
                SPI1BUFL = wData;
                dataSentCount++;
            }

            if (0U == SPI1STATLbits.SPIRBE)
            {
                data[dataReceivedCount] = SPI1BUFL;
                dataReceivedCount++;
            }
        }
        while (dataReceivedCount < bufferSize)
        {
            if (0U == SPI1STATLbits.SPIRBE)
            {
                data[dataReceivedCount] = SPI1BUFL;
                dataReceivedCount++;
            }
        } 
    }
}

bool SPI1_IsRxReady(void)
{    
    return (!SPI1STATLbits.SPIRBE);
}

bool SPI1_IsTxReady(void)
{    
    return (!SPI1STATLbits.SPITBF);
}