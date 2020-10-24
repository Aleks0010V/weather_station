/*
 * File:   oscillator.c
 * Author: aleks
 *
 * Created on August 14, 2020, 1:05 PM
 */


#include <xc.h>

void osc_Initialize(void) {
    OSCCON1bits.NOSC = 0b110;
    OSCCON1bits.NDIV = 0;
    OSCFRQbits.HFFRQ = 0b101;
}
