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
#define MAX_ARRAY_SIZE 10 + 2

static void transmit(uint8_t address);
static bool is_iddle(void);
static void transmit(uint8_t data);

static uint8_t *arr_start = NULL;
static uint8_t *arr_ptr = NULL;
static size_t global_size = 0;

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
    if (RCEN == 0)
    {
        for(uint8_t i = 0; i < global_size; i++)
        {
            if (SEN) 
            {
                transmit(arr_ptr++);
            }
            else if (ACKSTAT == 0)
            {
                transmit(arr_ptr++);
            }
        }
        while(is_iddle());  
        stop();  
        while(is_iddle());
        i2c_disable();
    }
}

void i2c_write_data(uint8_t address, uint8_t reg, void *data_ptr, size_t size) 
{
    // allocate needed memory
    uint8_t* ptr = (uint8_t*)data_ptr;
    // ToDo - need to check if there is enough memory
    uint8_t array[MAX_ARRAY_SIZE];
    array[0] = address;
    array[1] = reg;
    
    for(uint8_t i = 2; i < size; i++)
    {
        array[i] = *ptr;
        ptr++;
    }
    arr_start = array;
    arr_ptr = array;
    
    // connect
    global_size = size;
    i2c_enable();
    RCEN = 0;
    start();
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

void i2c_master_write_1Byte(uint8_t address, uint8_t reg, uint8_t data)
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

void i2c_master_read_1Byte(uint8_t address, uint8_t reg, uint8_t* dest_ptr)
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
}