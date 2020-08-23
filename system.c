/*
 * File:   system.c
 * Author: aleks
 *
 * Created on August 14, 2020, 1:05 PM
 */


#include <xc.h>
#include <pic16f18875.h>
#include "config.h"
#include "oscillator.h"
#include "pin_manager.h"
#include "I2C.h"
#include "timer_0.h"
#include "rs3231_i2c.h"
#include "internal_interrupt.h"
#include "SPI.h"
#include "bme280_i2c.h"

void System_Initialize(void)
{
    osc_Initialize();
    pin_Initiasize();
    I2C_Initialize();
    timer_0_Initialize();
    initialize_INT();
    rs3231_Initialize();
    bme280_Initialize();
    if (rs3231_Check())
        {LATDbits.LATD1 = 1;}
    initialize_SPI();
}