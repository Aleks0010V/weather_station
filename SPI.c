/*
 * File:   SPI.c
 * Author: aleks
 *
 * Created on August 22, 2020, 6:50 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "SPI.h"

#define spi_enable() SSPEN_2 = 1
#define spi_disable() SSPEN_2 = 0

typedef struct { 
    uint8_t con1; 
    uint8_t stat;
    uint8_t add;
    uint8_t operation;
} spi2_configuration_t;

//con1 == SSPxCON1, stat == SSPxSTAT, add == SSPxADD, operation == Master/Slave
static const spi2_configuration_t spi2_configuration[] = {   
    { 0x0, 0xc0, 0x1, 0 }
};

void initialize_SPI(void)
{
    //Setup PPS Pins
    SSP2CLKPPS = 0x1E;
    SSP2DATPPS = 0x1C;
    RD6PPS = 0x16;
    RD5PPS = 0x17;
    //SPI setup
    SSP2STAT = 0xC0;
    SSP2CON1 = 0x00;
    SSP2ADD = 0x01;
    TRISDbits.TRISD6 = 0;
    spi_disable();
}

bool SPI2_Open(spi2_modes_t spi2UniqueConfiguration)
{
    if(!SSP2CON1bits.SSPEN)
    {
        SSP2STAT = spi2_configuration[spi2UniqueConfiguration].stat;
        SSP2CON1 = spi2_configuration[spi2UniqueConfiguration].con1;
        SSP2CON2 = 0x00;
        SSP2ADD  = spi2_configuration[spi2UniqueConfiguration].add;
        TRISDbits.TRISD6 = spi2_configuration[spi2UniqueConfiguration].operation;
        spi_enable();
        return true;
    }
    return false;
}

void SPI2_Close(void)
{
    spi_disable();
}

uint8_t SPI2_ExchangeByte(uint8_t data)
{
    SSP2BUF = data;
    while(!PIR3bits.SSP2IF);
    PIR3bits.SSP2IF = 0;
    return SSP2BUF;
}

void SPI2_ExchangeBlock(void *block, size_t blockSize)
{
    uint8_t *data = block;
    while(blockSize--)
    {
        SSP2BUF = *data;
        while(!PIR3bits.SSP2IF);
        PIR3bits.SSP2IF = 0;
        *data++ = SSP2BUF;
    }
}

// Half Duplex SPI Functions
void SPI2_WriteBlock(void *block, size_t blockSize)
{
    uint8_t *data = block;
    while(blockSize--)
    {
        SPI2_ExchangeByte(*data++);
    }
}

void SPI2_ReadBlock(void *block, size_t blockSize)
{
    uint8_t *data = block;
    while(blockSize--)
    {
        *data++ = SPI2_ExchangeByte(0);
    }
}

void SPI2_WriteByte(uint8_t byte)
{
    SSP2BUF = byte;
}

uint8_t SPI2_ReadByte(void)
{
    return SSP2BUF;
}