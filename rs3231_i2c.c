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
#include <stdio.h>

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

void read_hours(uint8_t *dest_reg)
{
    fetch_hours_reg(dest_reg);
}

void get_time_string(char* str_ptr)
{
    uint8_t seconds = 0;
    uint8_t minutes = 0;
    uint8_t hours = 0;
    
    read_seconds(&seconds);
    read_minutes(&minutes);
    read_hours(&hours);
    
//    char* str_ptr = NULL;
    if (hours & 0x40)
    {
        char time_string[11];
        str_ptr = time_string;
        
        time_string[0] = ((hours & 0x10) >> 4) + 48;
        time_string[8] = ' ';
        time_string[9] = (hours & 0x20) ? 'P' : 'A';
        time_string[10] = 'M';
    }
    else
    {
        char time_string[8];
        str_ptr = time_string;
        
        time_string[0] = ((hours & 0x30) >> 4) + 48;
    }
    str_ptr[1] = (hours & 0x0F) + 48;
    str_ptr[2] = ':';
    str_ptr[3] = ((minutes & 0xF0) >> 4) + 48;
    str_ptr[4] = (minutes & 0x0F) + 48;
    str_ptr[5] = ':';
    str_ptr[6] = ((seconds & 0xF0) >> 4) + 48;
    str_ptr[7] = (seconds & 0x0F) + 48;
}

void set_seconds(uint8_t seconds)
{
    if (seconds > 60)
        return;
    
    seconds = ((seconds / 10) << 4) + seconds % 10;
    
    master_write_1Byte(MAIN, SECONDS, seconds);
}

void set_minutes(uint8_t minutes)
{
    if (minutes > 60)
        return;
    
    minutes = ((minutes / 10) << 4) + minutes % 10;
    
    master_write_1Byte(MAIN, MINUTES, minutes);
}

void set_hours(bool mode_12h, uint8_t hours)
{
    bool PM = false;
    if (mode_12h && hours > 12)
    {
        PM = true;
        hours -= 12;
    }
    if (!mode_12h && hours > 24)
        return;
    
    hours = ((hours / 10) << 4) + hours % 10;
    if (mode_12h)
        hours += 64;
    if (PM){
        hours += 32;
    }
    
    master_write_1Byte(MAIN, HOURS, hours);
}

static void fetch_hours_reg(uint8_t *dest_reg)
{
    master_read_1Byte(MAIN, HOURS, dest_reg);
}

static void read_status(uint8_t *dest_reg)
{
    master_read_1Byte(MAIN, CONTROL_STATUS, dest_reg);
}

static void read_control(uint8_t *dest_reg)
{
    master_read_1Byte(MAIN, CONTROL, dest_reg);
}