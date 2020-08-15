/*
 * File:   main.c
 * Author: aleks
 *
 * Created on August 14, 2020, 12:20 PM
 */


#include <xc.h>
#include "system.h"
#include "timer_0.h"
#include "I2C.h"

void main(void) {
    System_Initialize();
    interrupt_enable();
    peripheral_int_enable();
    t0_int_enable();
    t0_enable();
    
    while (1) {
        
    }
    
    return;
}

void __interrupt() ISR(void)
{
    if (TMR0IF & TMR0IE) {
        t0_ISR();
    }
    if (SSP1IF & SSP1IE) {
        i2c_ISR();
    }
    return;
}