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
static void read_status(uint8_t *dest_reg);
static void read_control(uint8_t *dest_reg);

bool rs3231_Check(void)
// returns 0 if RTC is OK
{
    uint8_t status = 1;
    uint8_t control_reg = 1;
    read_status(&status);
    read_control(&control_reg);
    return (status >> 7) && (control_reg >> 7);
}

void rs3231_Initialize(void)
/* Should be called in some way to re-init RTC remotely.
 * 
 * rs3231_Check named in this way because I don't want to init RTC 
 * after every POR.
 */
{
    
}

void read_seconds(uint8_t *dest_reg)
{
    master_read_1Byte(MAIN, SECONDS, dest_reg);
}

void read_minutes(uint8_t *dest_reg)
{
    master_read_1Byte(MAIN, MINUTES, dest_reg);
}

static void read_status(uint8_t *dest_reg)
{
    master_read_1Byte(MAIN, CONTROL_STATUS, dest_reg);
}

static void read_control(uint8_t *dest_reg)
{
    master_read_1Byte(MAIN, CONTROL, dest_reg);
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