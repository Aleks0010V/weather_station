/*
 * File:   SPI.c
 * Author: aleks
 *
 * Created on August 22, 2020, 6:50 PM
 */


#include <xc.h>
#include <stdint.h>
#include "SPI.h"

#define spi_enable() SSPEN_2 = 1
#define spi_disable() SSPEN_2 = 0

void initialize_SPI(void)
{
    SSP2CLKPPS = 0x1E;
    SSP2DATPPS = 0x1C;
    RD5PPS = 0x17;
    RD7PPS = 0x16;
    
    SSP2CON1bits.SSPM = 0b1010;  // SPI Master mode, clock = FOSC/(4 * (SSPxADD+1))
    SSP2CON1bits.CKP = 0;
    SSP2STATbits.SMP = 0;
    /*
     * clock polarity should be same with slave device,
     * so, it should be an opportunity to change it when slave is switched.
     */
    
    SSP2ADD  = 0x09;
    spi_disable();
}

void spi_master_write_1Byte (uint8_t data)
{
    spi_enable();
    SSP2BUF = data;
    while(SSP2IF);
    spi_disable();
}

void spi_ISR(void)
{
    SSP2IF = 0;
    LATDbits.LATD1 = 1;
}