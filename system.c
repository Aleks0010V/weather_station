/*
 * File:   system.c
 * Author: aleks
 *
 * Created on August 14, 2020, 1:05 PM
 */


#include <xc.h>
#include <pic16f18875.h>
#include <stdint.h>
#include <stdbool.h>
#include "system.h"
#include "config.h"
#include "oscillator.h"
#include "pin_manager.h"
#include "I2C.h"
#include "timer_0.h"
#include "rs3231_i2c.h"
#include "internal_interrupt.h"
#include "SPI.h"
#include "SD.h"

void System_Initialize(void) {
    osc_Initialize();
    pin_Initiasize();
    I2C_Initialize_master();
    initialize_SPI_master(false, false);
    timer_0_Initialize();
    int_i_initialize();
    rs3231_Initialize();
    SD_initialize();
    if (rs3231_Check()) {
        LATBbits.LATB5 = 1;
    }
}

word pack_word(uint8_t lsb, uint8_t msb) {
    word result;
    result.bytes[0] = msb;
    result.bytes[1] = lsb;
    return result;
}
