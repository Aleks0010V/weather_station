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

void initialize_SPI_master(bool CKP, bool CMP) {
    spi_disable();
    SPI_set_pins();
    SSP2CON1bits.SSPM = 0b0000; // SPI Master mode, clock = FOSC/4
    SPI_set_mode(CKP, CMP);
//    SSP2IE = 0;
    SSP2ADD = 0x1;
}

void spi_write(uint8_t *data, uint8_t size) {
    spi_enable();
    uint8_t dummy;
    while(size--) {
        do {
            dummy = SSP2BUF;
        } while(BF);
        SSP2BUF = *data++;
    }
    spi_disable();
}

void spi_read(uint8_t *dest_reg, uint8_t size) {
    spi_enable();
    while(size--) {
        do {
            SSP2BUF = 0;
        } while(!BF);
        *dest_reg++ = SSP2BUF;
    }
    spi_disable();
}

void spi_exchange_block(uint8_t *source, uint8_t *dest_reg) {
    spi_enable();
    SSP2BUF = *source;
    while(BF);
    *dest_reg = SSP2BUF;
    spi_disable();
}

void spi_ISR(void) {
    SSP2IF = 0;
}

static void SPI_set_pins(void) {
//    SSP2CLKPPS = 0x18;  // RD0 - clock input - SCK
    SSP2DATPPS = 0x1A;  TRISDbits.TRISD2 = 1; // RD2 - data input - SDI
    RD3PPS = 0x17;  TRISDbits.TRISD3 = 0; // SDO2 - data output
    RD1PPS = 0x16;  TRISDbits.TRISD1 = 0; // SCK2 - clock output
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