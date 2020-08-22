/*
 * File:   internal_interrupt.c
 * Author: aleks
 *
 * Created on August 19, 2020, 1:50 PM
 */


#include <xc.h>
#include <stdint.h>
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
    
    uint8_t time_string = 0;
    get_time_string(&time_string);
    
    RD2 = ~RD2;  // indicate interrupt occurrence
}
