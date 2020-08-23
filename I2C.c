/*
 * File:   I2C.c
 * Author: aleks
 *
 * Created on August 14, 2020, 12:20 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "I2C.h"

#define i2c_enable() SSPEN_1 = 1
#define i2c_disable() SSPEN_1 = 0
#define start() SEN = 1
#define stop() PEN = 1
#define restart() RSEN = 1

static void transmit(uint8_t address);
static bool is_iddle(void);
static void transmit(uint8_t data);

void I2C_Initialize(void)
{
    SSP1CLKPPS = 0x12;   //RC2->MSSP1:SCL1;    
    RC3PPS = 0x15;   //RC3->MSSP1:SDA1;    
    RC2PPS = 0x14;   //RC2->MSSP1:SCL1;    
    SSP1DATPPS = 0x13;   //RC3->MSSP1:SDA1;
    
    SSP1STAT = 0x00;
    SSP1CON1 = 0x08;
    SSP1CON2 = 0x00;
    SSP1ADD  = 0x09;
//    SSP1IE = 1;
    i2c_disable();
}

void i2c_ISR(void)
{
    SSP1IF = 0;
    if (SEN || PEN || RSEN) {
    
    }
}

static bool is_iddle(void) 
{
    if (SEN || RSEN || PEN || RCEN || ACKEN || RW) {
        return true;
    }
    else {
        return false;
    }
}

void master_write_1Byte(uint8_t address, uint8_t reg, uint8_t data)
{
    i2c_enable();
    
    RCEN = 0;
    start();
    
    transmit(address);  // connect
    transmit(reg);
    transmit(data);
    
    stop();  while(is_iddle());
    i2c_disable();
}

void master_write_2Bytes(uint8_t address, uint16_t data)
{
    i2c_enable();
    start();
    while(is_iddle());
}

void master_read_1Byte(uint8_t address, uint8_t reg, uint8_t* dest_ptr)
{
    i2c_enable();
    
    RCEN = 0;  // prepare to read specific register
    start();
    transmit(address);
    transmit(reg);
    
    restart();
    transmit(address + 1);
    RCEN = 1;
    while(is_iddle() && !BF);
    *dest_ptr = SSP1BUF; 
    ACKDT = 1;
    ACKEN = 1;  while(is_iddle());
    
    stop();  while(is_iddle());
    i2c_disable();
}

static void transmit(uint8_t data) 
{
    while(is_iddle());
    SSP1BUF = data;
    while(is_iddle());
}