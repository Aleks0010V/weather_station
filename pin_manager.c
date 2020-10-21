/*
 * File:   pin_manager.c
 * Author: aleks
 *
 * Created on August 14, 2020, 12:28 PM
 */


#include <xc.h>

void pin_Initiasize(void) {

    // clear output register
    LATA = 0;
    LATB = 0;
    LATC = 0;
    LATD = 0;

    // 0 - digital, 1 - analog
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;

    // 0 - output, 1 - input
    TRISA = 0b01000000;
    TRISB = 0;
    // MSSP1 as I2C pins should be inputs, MSSP2 SPI RC4 - CLK input
    TRISC = 0b00001100;
    // RD4 - MSSP2 SPI input, RD6 - output, RD7 - clock input, 
    // RD6 - clock output
    TRISD = 0b10010000;
}
