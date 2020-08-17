/*
 * File:   rs3231_i2c.c
 * Author: aleks
 *
 * Created on August 16, 2020, 5:22 PM
 */


#include <xc.h>
#include <stdbool.h>
#include "I2C.h"
#include "rs3231_i2c.h"

#define MAIN 0b11010000

static void fetch_hours_reg(uint8_t *dest_reg);

void read_seconds(uint8_t *dest_reg)
{
    master_read_1Byte(MAIN, SECONDS, dest_reg);
}

void read_minutes(uint8_t *dest_reg)
{
    master_read_1Byte(MAIN, MINUTES, dest_reg);
}

void read_status(uint8_t *dest_reg)
{
    master_read_1Byte(MAIN, CONTROL_STATUS, dest_reg);
}

void read_hours(uint8_t *dest_reg)
{
    fetch_hours_reg(dest_reg);
    
    // 6th bit of HOURS register represents selected mode
    // 12-hours mode with AM/PM bit or 24-hours mode
    bool mode_12h = (*dest_reg & 0x40) >> 6;
    
    if (mode_12h)
    {
        *dest_reg = (*dest_reg & 0x1F) | (*dest_reg & 0x80);
    }
    else
    {
        *dest_reg = (*dest_reg & 0x3F) | (*dest_reg & 0x80);
    }
}

static void fetch_hours_reg(uint8_t *dest_reg)
{
    master_read_1Byte(MAIN, HOURS, dest_reg);
}