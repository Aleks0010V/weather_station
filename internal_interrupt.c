/*
 * File:   internal_interrupt.c
 * Author: aleks
 *
 * Created on August 19, 2020, 1:50 PM
 */


#include <xc.h>
#include <stdint.h>
#include "rs3231_i2c.h"
#include "bme280_i2c.h"
#include "SPI.h"

void int_i_initialize(void) {
    INTPPS = 0x06; // internal interrupt pin is RA6
    INTEDG = 0;
    return;
}

void int_ISR(void) {
    INTF = 0;
//    clear_a1f();
    clear_a2f();

    char time_string[8];
    char date_string[10];
    get_time_string(time_string);
    get_date_string(date_string);
    update_bme_data();
    int32_t temp = compensate_temperature();
    uint32_t press = compensate_pressure();
    uint32_t hum = compensate_humidity();
}
