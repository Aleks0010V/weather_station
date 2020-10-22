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

static void SPI_set_pins(void);
static void SPI_set_mode(bool CKP, bool CMP);

void initialize_SPI_master(bool CKP, bool CMP) {
    SPI_set_pins();
    SSP2CON1bits.SSPM = 0b0000; // SPI Master mode, clock = FOSC/4
    SPI_set_mode(CKP, CMP);
//    SSP2IE = 0;
//    SSP2ADD = 0x09;
    spi_disable();
}

void spi_write(uint8_t *data, uint8_t size) {
    
}

void spi_read(uint8_t *dest_reg, uint8_t size) {
    
}

void spi_exchange_block(uint8_t *source, uint8_t *dest_reg) {
    
}

void spi_ISR(void) {
    SSP2IF = 0;
}

static void SPI_set_pins(void) {
    SSP2CLKPPS = 0x18;  // RD0 - clock input - SCK
    SSP2DATPPS = 0x1A;  // RD2 - data input - SDI
    RD3PPS = 0x17;  // SDO2 - data output
    RD1PPS = 0x16;  // SCK2 - clock output
}

static void SPI_set_mode(bool CKP, bool CMP) {
    /*
     * clock polarity should be same with slave device,
     * so, it should be an opportunity to change it when slave is switched.
     */
    SSP2CON1bits.CKP = CKP;
    SSP2STATbits.SMP = CMP;
}