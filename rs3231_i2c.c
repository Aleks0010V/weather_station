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

enum addresses
{
    SECONDS,
    MINUTES,
    HOURS,
    DAY,
    DATE,
    MONTH_CENTURY,
    YEAR,
    ALARM_1_SECONDS,
    ALARM_1_MINUTES,
    ALARM_1_HOURS,
    ALARM_1_DAY_DATE,
    ALARM_2_MINUTES,
    ALARM_2_HOURS,
    ALARM_2_DAY_DATE,
    CONTROL,
    CONTROL_STATUS,
    AGING_OFFSET,
    TEMP_HIGH,
    TEMP_LOW
}addrs;

static void read_status(uint8_t *dest_reg);
static void read_control(uint8_t *dest_reg);
static void fetch_hours(uint8_t *dest_reg);
static void fetch_minutes(uint8_t *dest_reg);
static void fetch_seconds(uint8_t *dest_reg);
static void bcd_convert(uint8_t* data);

bool rs3231_Check(void)
// returns 0 if RTC is OK
{
//    uint8_t status = 1;
    uint8_t control_reg = 1;
//    read_status(&status);
    read_control(&control_reg);
    return control_reg >> 7;
}

void rs3231_Initialize(void)
{
    // enable alarm 2 once per minute
    alarm2_every_minute();
    // enable interrupt, enable Alarm 2 interrupt
    clear_a2f();
    master_write_1Byte(MAIN, CONTROL, 0x06);
}

void alarm2_every_minute (void)
{   
    clear_a2f();
    master_write_1Byte(MAIN, ALARM_2_MINUTES, 128);
    master_write_1Byte(MAIN, ALARM_2_HOURS, 128);
    master_write_1Byte(MAIN, ALARM_2_DAY_DATE, 128);
}

void set_alarm_2(uint8_t minutes, bool mode_12h, uint8_t hours, bool a2m2, bool a2m3)
{
    if (minutes > 60 || minutes < 0 || hours > 60 || hours < 0)
        return;
    bool PM = false;
    if (mode_12h && hours > 12)
    {
        PM = true;
        hours -= 12;
    }
    
    bcd_convert(&minutes);
    bcd_convert(&hours);
    if (a2m2)
        minutes += 128;
    if (a2m3)
        hours += 128;
    if (mode_12h)
        hours += 64;
    if (PM)
        hours += 32;
    
    master_write_1Byte(MAIN, ALARM_2_MINUTES, minutes);
    master_write_1Byte(MAIN, ALARM_2_HOURS, hours);
    master_write_1Byte(MAIN, ALARM_2_DAY_DATE, 128);
}

void read_seconds(uint8_t *dest_reg)
{
    fetch_seconds(dest_reg);    
    *dest_reg = ((*dest_reg) >> 4) + ((*dest_reg) & 0x0F);
}

void clear_a2f(void)
{
    uint8_t status_control = 0;
    read_status(&status_control);
    master_write_1Byte(MAIN, CONTROL_STATUS, status_control & ~(1 << 1));
}

void read_minutes(uint8_t *dest_reg)
{
    fetch_minutes(dest_reg);
    *dest_reg = ((*dest_reg) >> 4) + ((*dest_reg) & 0x0F);
}

void get_time_string(char* str_ptr)
{
    uint8_t seconds = 0;
    uint8_t minutes = 0;
    uint8_t hours = 0;
    
    fetch_seconds(&seconds);
    fetch_minutes(&minutes);
    fetch_hours(&hours);
    
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
    str_ptr[3] = (minutes >> 4) + 48;
    str_ptr[4] = (minutes & 0x0F) + 48;
    str_ptr[5] = ':';
    str_ptr[6] = (seconds >> 4) + 48;
    str_ptr[7] = (seconds & 0x0F) + 48;
}

void set_seconds(uint8_t seconds)
{
    if (seconds > 60 || seconds < 0)
        return;
    
    bcd_convert(&seconds);
    
    master_write_1Byte(MAIN, SECONDS, seconds);
}

void set_minutes(uint8_t minutes)
{
    if (minutes > 60 || minutes < 0)
        return;
    
    bcd_convert(&minutes);
    
    master_write_1Byte(MAIN, MINUTES, minutes);
}

void set_hours(bool mode_12h, uint8_t hours)
{
    bool PM = false;
    if (hours < 0)
        return;
    if (mode_12h && hours > 12)
    {
        PM = true;
        hours -= 12;
    }
    if (!mode_12h && hours > 24)
        return;
    
    bcd_convert(&hours);
    if (mode_12h)
        hours += 64;
    if (PM){
        hours += 32;
    }
    
    master_write_1Byte(MAIN, HOURS, hours);
}

void set_day(uint8_t day)
{
    if (day > 7 || day < 1)
        return;
    
    master_write_1Byte(MAIN, DAY, day);
}

void set_date(uint8_t date)
{
    if (date > 31 || date < 1)
        return;
    
    bcd_convert(&date);
    master_write_1Byte(MAIN, DATE, date);
}

void set_month(uint8_t month)
{
    if (month > 12 || month < 1)
        return;
    
    uint8_t month_reg = 0;
    master_read_1Byte(MAIN, MONTH_CENTURY, &month_reg);
    month_reg = month_reg & 128;  // save century bit
    
    bcd_convert(&month);
    master_write_1Byte(MAIN, MONTH_CENTURY, month | month_reg);
}

void set_year(uint8_t year, uint8_t century)
{
    if (year > 99 || year < 0)
        return;
    if (century != 20 && century != 21)
        return;
    else if (century == 20)
        century = 0;
    else if (century == 21)
        century = 128;
    
    uint8_t month_reg = 0;
    master_read_1Byte(MAIN, MONTH_CENTURY, &month_reg);
    if (century != month_reg & 128)  // update century bit
    {
        master_write_1Byte(MAIN, MONTH_CENTURY, month_reg | century);
    }
    
    bcd_convert(&year);
    master_write_1Byte(MAIN, YEAR, year);
}

static void read_status(uint8_t *dest_reg)
{
    master_read_1Byte(MAIN, CONTROL_STATUS, dest_reg);
}

static void read_control(uint8_t *dest_reg)
{
    master_read_1Byte(MAIN, CONTROL, dest_reg);
}

static void fetch_seconds(uint8_t *dest_reg)
{
    master_read_1Byte(MAIN, SECONDS, dest_reg);
}

static void fetch_minutes(uint8_t *dest_reg)
{
    master_read_1Byte(MAIN, MINUTES, dest_reg);
}

static void fetch_hours(uint8_t *dest_reg)
{
    master_read_1Byte(MAIN, HOURS, dest_reg);
}

static void bcd_convert(uint8_t* data)
{
    if (*data > 99)
        return;
    
    *data = ((*data / 10) << 4) + *data % 10;
}