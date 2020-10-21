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

static msg *start_msg = NULL;
static msg *last_msg = NULL;

void initialize_SPI(void) {
    //Setup PPS Pins
    SSP2CLKPPS = 0x1E;
    SSP2DATPPS = 0x1C;
    RD6PPS = 0x16;
    RD5PPS = 0x17;

    SSP2CON1bits.SSPM = 0b1010; // SPI Master mode, clock = FOSC/(4 * (SSPxADD+1))
    SSP2CON1bits.CKP = 0;
    SSP2STATbits.SMP = 0;
    /*
     * clock polarity should be same with slave device,
     * so, it should be an opportunity to change it when slave is switched.
     */

    SSP2ADD = 0x09;
    spi_disable();
}

void spi_master_write_1Byte(uint8_t *data, uint8_t len, void (*preload_func)(void), void (*postload_func)(void)) {
    // add msgs to the quie
    for (uint8_t i = 0; i < len; i++) {
        msg next_msg = {.data = data[i],
            .preload_func = preload_func,
            .postload_func = postload_func};
        if (last_msg == NULL) {
            last_msg = &next_msg;
            start_msg = last_msg;
        } else {
            last_msg->next_msg = &next_msg;
            last_msg = &next_msg;
        }
    }

    // send first message if SPI is not running
    if (SSPEN_2 == 0) {
        spi_enable();
        if (start_msg != NULL) {
            start_msg->preload_func();
            SSP2BUF = start_msg->data;
        } else
            spi_disable();
    }
}

void spi_ISR(void) {
    SSP2IF = 0;

    // if data was received - execute specific postprocess function
    if (SSP2STATbits.BF == 1) {
        if (start_msg != NULL)
            start_msg->postload_func(SSP2BUF);
    }

    // we started from the first message before coming here
    if (start_msg != NULL) {
        // so we need to check that next message exists
        if (start_msg->next_msg != NULL) {
            // if yes than we switch pointer to it
            start_msg = start_msg->next_msg;
            start_msg->preload_func(); // executing it's pre-load function
            SSP2BUF = start_msg->data; // finally, sending data
        } else {
            start_msg = NULL;
            last_msg = NULL;
            spi_disable();
        }
    }
}