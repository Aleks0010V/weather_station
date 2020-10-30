/*
 * File:   one_wire.c
 * Author: aleks
 *
 * Created on October 30, 2020, 8:58 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "one_wire.h"

// I/O status flag
static uint8_t _Rw = 0; // read - 0; write - 1
const uint8_t * const read_write_status = &_Rw;

// buffer
uint8_t oneWire_buf = 0;

uint8_t oneWire_write(void *data) {
    if (oneWire_buf_size > 0)
        return 1;
    
    _Rw = 0; // output
    oneWire_buf = *((uint8_t *)data);
    oneWire_buf_size = 8;
    // start timer
    return 0;
}

void timer_ISR(void) {
    
    uint8_t pin; // abstract
    // set TRIS reg of pin = read_write_status
    
    if (oneWire_buf_size > 0) {
        if (read_write_status == 1) {
        
        } else {
            pin = oneWire_buf & 0x01;
            oneWire_buf = oneWire_buf >> 1;
            // restart timer
        }
        oneWire_buf_size--;
    }
}