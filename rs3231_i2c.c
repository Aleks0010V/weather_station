/*
 * File:   rs3231_i2c.c
 * Author: aleks
 *
 * Created on August 16, 2020, 5:22 PM
 */


#include <xc.h>
#include "I2C.h"
#include "rs3231_i2c.h"

#define MAIN 0b11010000


void read_seconds(uint8_t *dest_reg)
{
    master_read_1Byte(MAIN, SECONDS, dest_reg);
}
