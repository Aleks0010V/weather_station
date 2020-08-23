/*
 * File:   SPI.c
 * Author: aleks
 *
 * Created on August 22, 2020, 6:50 PM
 */


#include <xc.h>
#include "SPI.h"

#define spi_enable() SSPEN_2 = 1
#define spi_disable() SSPEN_2 = 0

void initialize_SPI(void)
{
    SSP2CLKPPS = 0x14;  // clock input = RC4
    RC5PPS = 0x16;  // clock output = RC5
    SSP2DATPPS = 0x1A;  // data input = RD2
    RD3PPS = 0x17;  // data output = RD3
    
    SSP2CON1bits.SSPM = 0b1010;  // SPI Master mode, clock = FOSC/(4 * (SSPxADD+1))
    SSP2CON1bits.CKP = 0;
    /*
     * clock polarity should be same with slave device,
     * so, it should be an opportunity to change it when slave is switched.
     */
    
    SSP2ADD  = 0x09;
    spi_disable();
}