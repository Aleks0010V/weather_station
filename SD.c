/*
 * File:   SD.c
 * Author: aleks
 *
 * Created on August 22, 2020, 6:50 PM
 */


#include <xc.h>
#include "SD.h"
#include "SPI.h"

static const SD_command reset = {CMD0, 0, 0, 0, 0, 149};

void SD_initialize(void) {
    uint8_t dummy[] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
    spi_write(dummy, 10);
    spi_write(&reset, 6);
    uint8_t result = 0;
    spi_read(&result, 1, 255);
    if ((result & 1) && ((result >> 1) == 0)) {
        LATBbits.LATB5 = 1;
    }
}