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
#define BF SSP2STATbits.BF

static void SPI_set_pins(void);
static void SPI_set_mode(bool CKP, bool CKE);

void initialize_SPI_master(bool CKP, bool CKE) {
    spi_disable();
    SPI_set_pins();
    SSP2CON1bits.SSPM = 0b1010; // SPI Master mode, clock = FOSC/16
    SPI_set_mode(CKP, CKE);
//    SSP2IE = 0;
    SSP2ADD = 0x09;
    spi_enable();
}

void spi_write(void *data, uint8_t size) {
    if (SSPEN_2 == 0)
        spi_enable();
    uint8_t dummy;
    uint8_t *ptr = (uint8_t *)data;
    while(size--) {
        SSP2BUF = *ptr++;
        while(!BF);
        dummy = SSP2BUF;
    }
}

void spi_read(void *dest_reg, uint8_t size, uint8_t dummy) {
    if (SSPEN_2 == 0)
        spi_enable();
    uint8_t *ptr = (uint8_t *)dest_reg;
    while(size--) {
        SSP2BUF = dummy;
        while(!BF);
        *ptr++ = SSP2BUF;
    }
}

void spi_exchange_block(uint8_t *block, uint8_t size) {
    if (SSPEN_2 == 0)
        spi_enable();
    while(size--) {
        SSP2BUF = *block;
        while(BF);
        *block = SSP2BUF;
    }
}

void spi_ISR(void) {
    SSP2IF = 0;
}

static void SPI_set_pins(void) {
//    SSP2CLKPPS = 0x18;  // RD0 - clock input - SCK
    SSP2DATPPS = 0x1A; // RD2 - data input - SDI
    RD3PPS = 0x17; // SDO2 - data output
    RD1PPS = 0x16; // SCK2 - clock output
}

static void SPI_set_mode(bool CKP, bool CKE) {
    /*
     * clock polarity should be same with slave device,
     * so, it should be an opportunity to change it when slave is switched.
     */
    SSP2CON1bits.CKP = CKP;
    SSP2STATbits.CKE = CKE;
    SSP2STATbits.SMP = 1;
}