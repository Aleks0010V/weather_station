/* 
 * File:   rs3231_i2c.h
 * Author: aleks
 *
 * Created on August 16, 2020, 5:20 PM
 */
#include <stdint.h>
#include <stdbool.h>

#ifndef RS3231_I2C_H
#define	RS3231_I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

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

bool rs3231_Check(void);
void read_seconds(uint8_t *dest_reg);
void read_minutes(uint8_t *dest_reg);
//void read_hours(uint8_t *dest_reg);
void get_time_string(char* str_ptr);
void set_seconds(uint8_t seconds);
void set_minutes(uint8_t minutes);
void set_hours(bool mode_12h, uint8_t hours);
void set_alarm_2(uint8_t minutes, bool mode_12h, uint8_t hours, bool a2m2, bool a2m3);
void rs3231_Initialize(void);
void clear_a2f(void);

#ifdef	__cplusplus
}
#endif

#endif	/* RS3231_I2C_H */

