/*
 * File:   system.c
 * Author: aleks
 *
 * Created on August 14, 2020, 1:05 PM
 */


#include <xc.h>
#include "config.h"
#include "oscillator.h"
#include "pin_manager.h"
#include "I2C.h"
#include "timer_0.h"

void System_Initialize(void) {
    osc_Initialize();
    pin_Initiasize();
    I2C_Initialize();
    timer_0_Initialize();
}
