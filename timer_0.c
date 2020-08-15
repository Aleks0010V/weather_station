/*
 * File:   timer.c
 * Author: aleks
 *
 * Created on August 14, 2020, 1:18 PM
 */


#include <xc.h>
#include "I2C.h"

void timer_0_Initialize(void) {
    T0CON1bits.T0ASYNC = 0;
    T0CON1bits.T0CS = 0b010;
    T0CON1bits.T0CKPS = 0b0100;
    T0CON0bits.T016BIT = 1;
    T0CON0bits.T0OUTPS = 0;
    // timer pre-load
    TMR0H = 0b00001011; // pre = 16, post = 1, out = 1 sec
    TMR0L = 0b11011100;
}

void t0_ISR(void) {
    TMR0IF = 0;
    // timer pre-load
    TMR0H = 0b00001011; // pre = 16, post = 1, out = 1 sec
    TMR0L = 0b11011100;
    
    LATD0 = ~LATD0;
    master_write_1Byte(0x76, 0xD0);
}