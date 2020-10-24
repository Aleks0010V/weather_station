/*
 * File:   timer.c
 * Author: aleks
 *
 * Created on August 14, 2020, 1:18 PM
 */


#include <xc.h>
#include <stdint.h>
#include "system.h"
#include "timer_0.h"

#define _DESIRED_TIME_SEC 1
static const short pre = (short)(65536 - _DESIRED_TIME_SEC * _XTAL_FREQ / (4 * 64));

void timer_0_Initialize(void) {
    T0CON1bits.T0ASYNC = 0;
    T0CON1bits.T0CS = 0b010;
    T0CON1bits.T0CKPS = 0b0110;
    T0CON0bits.T016BIT = 1;
    T0CON0bits.T0OUTPS = 0;
    // timer pre-load
    TMR0H = 0b00001011; // pre = 16, post = 1, out = 1 sec
    TMR0L = 0b11011100;
}

void t0_ISR(void) {
    TMR0IF = 0;
    t0_disable();
    // timer pre-load
    TMR0H = pre >> 8;
    TMR0L = pre & 0x0F;

    LATA0 = ~LATA0;
    
    t0_enable();
}