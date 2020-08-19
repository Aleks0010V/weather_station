/*
 * File:   internal_interrupt.c
 * Author: aleks
 *
 * Created on August 19, 2020, 1:50 PM
 */


#include <xc.h>
#include "rs3231_i2c.h"

void initialize_INT(void) 
{
    INTPPS = 0x06;  // internal interrupt pin is RA6
    INTEDG = 0;
    return;
}

void int_ISR(void)
{
    INTF = 0;
    clear_a2f();
    RD2 = ~RD2;
}
