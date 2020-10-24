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

// to port this lib you should map these directives to registers of your MCU
// also you need to replace SSP1BUF register
#define i2c_enable() SSPEN_1 = 1
#define i2c_disable() SSPEN_1 = 0
#define start() SEN = 1
#define stop() PEN = 1
#define restart() RSEN = 1
#define BF SSP1STATbits.BF

static bool is_iddle(void);
static void transmit(uint8_t *data);
static void I2C_set_pins(void);
static void receive(uint8_t *dest_reg);

void I2C_Initialize_master(void) {
    SSPEN_1 = 0;
    I2C_set_pins();

    SSP1STAT = 0x00;
    SSP1CON1 = 0x08;
    SSP1CON2 = 0x00;
    SSP1ADD = 0x09;
    SSP1IE = 0;
    SSPEN_1 = 1;
}

void i2c_ISR(void)
// ISR handler for slave mode
{
    SSP1IF = 0;
    if (RCEN == 0) {

    } else {

    }
}

static bool is_iddle(void) {
    if (SEN || RSEN || PEN || RCEN || ACKEN || RW) {
        return true;
    } else {
        return false;
    }
}

void i2c_write(uint8_t address, uint8_t reg, void *data, uint8_t size) {
    if (size <= 0)
        return;
    uint8_t *ptr = (uint8_t *)data;
    if (SSPEN_1 == 0)
        i2c_enable();

    RCEN = 0;
    start();

    if (i2c_open(address, reg) == true) {
        while(size--)
            transmit(ptr++);
    }

    stop();
    while (is_iddle());
}

bool i2c_open(uint8_t address, uint8_t reg) {
    transmit(&address);
    if (ACKSTAT == 1)
        return false;
    transmit(&reg);
    if (ACKSTAT == 1)
        return false;
    return true;
}

void i2c_read(uint8_t address, uint8_t reg, void* dest_ptr, uint8_t size) {
    if (size <= 0)
        return;
    
    uint8_t *ptr = (uint8_t *)dest_ptr;
    if (SSPEN_1 == 0)
        i2c_enable();

    RCEN = 0; // prepare to read specific register
    start();
    if (i2c_open(address, reg) == false) {
        stop();
        while (is_iddle());
        return;
    }

    restart();
    uint8_t read_addr = address + 1;
    transmit(&read_addr);
    RCEN = 1;
    while(size--) {
        receive(ptr++);
        ACKDT = 1;
        ACKEN = 1;
    }

    stop();
    while (is_iddle());
}

static void transmit(uint8_t *data) {
    while (is_iddle());
    SSP1BUF = *data;
    while (is_iddle());
}

static void receive(uint8_t *dest_reg) {
    while (is_iddle() && !BF);
    *dest_reg = SSP1BUF;
    while (is_iddle());
}

static void I2C_set_pins(void) {
    SSP1CLKPPS = 0x12; //RC2->MSSP1:SCL1;    
    RC3PPS = 0x15; //RC3->MSSP1:SDA1;    
    RC2PPS = 0x14; //RC2->MSSP1:SCL1;    
    SSP1DATPPS = 0x13; //RC3->MSSP1:SDA1;
}