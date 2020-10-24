/*
 * File:   bme280.c
 * Author: aleks
 *
 * Created on August 23, 2020, 2:01 PM
 */


#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "system.h"
#include "I2C.h"
#include "bme280_i2c.h"

#define MAIN 0xEC
#define RESERVE 0xEE

#define ID 0xD0
#define RESET_ADDR 0xE0

static bool check_id(uint8_t addr);
static void reset(void);

static uint8_t current_addr = 0;
static int32_t t_fine;  // t_fine carries fine temperature as global value

// data from calibration registers
static uint16_t dig_T1;
static uint16_t dig_P1;
static int16_t dig_t[2];
static int16_t dig_p[8];
static uint8_t dig_H1;
static int16_t dig_H2;
static uint8_t dig_H3;
static int16_t dig_H4;
static int16_t dig_H5;
static int8_t dig_H6;

static bme_data current_data;

enum config {
    CTRL_HUM = 0xF2,
    _STATUS,
    CTRL_MEAS,
    CONFIG,
};

enum data {
    PRESS_MSB = 0xF7,
    PRESS_LSB,
    PRESS_XLSB,
    TEMP_MSB,
    TEMP_LSB,
    TEMP_XLSB,
    HUM_MSB,
    HUM_LSB
};

enum calib_0 {
    c0 = 0x88,
    c1, c2, c3, c4, c5, c6, c7, c8, c9,
    c10, c11, c12, c13, c14, c15, c16, c17, c18, c19,
    c20, c21, c22, c23, c24
};

enum calib_1 {
    c25 = 0xE1,
    c26, c27, c28, c29, c30, c31, c32, c33, c34, c35, c36, c37, c38, c39
};

void bme280_Initialize(void) {
    if (bme280_exists()) {
        reset();
    } else {
        LATBbits.LATB5 = 1;
        return;
    }
}

bool bme280_exists(void) {
    if (check_id(MAIN)) {
        current_addr = MAIN;
        return true;
    } else if (check_id(RESERVE)) {
        current_addr = RESERVE;
        return true;
    } else
        return false;
}

int32_t compensate_temperature(void) {
    // Returns temperature in DegC, resolution is 0.01 DegC. Output value of ?5123?equals 51.23 DegC.
    int32_t var1, var2, T;
    var1 = (( ( (current_data.temperature >> 3) - ( (int32_t)dig_T1 << 1) )) * ((int32_t)dig_t[0])) >> 11;
    var2  = (((((current_data.temperature >> 4) - ((int32_t)dig_T1)) * ((current_data.temperature >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_t[1])) >> 14;
    t_fine = var1 + var2;
    T  = (t_fine * 5 + 128) >> 8;
    return T;
}

uint32_t compensate_pressure(void) {
    // Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
    // Output value of ?24674867?represents 24674867/256 = 96386.2 Pa = 963.862 hPa
    int64_t var1, var2, p;
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)dig_p[4];
    var2 = var2 + ((var1*(int64_t)dig_p[3]) << 17);
    var2 = var2 + (((int64_t)dig_p[2]) << 35);
    var1 = ((var1 * var1 * (int64_t)dig_p[1]) >> 8) + ((var1 * (int64_t)dig_p[0]) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)dig_P1) >> 33;
    if (var1 == 0) {
        return 0;
    }
    p = 1048576 - current_data.pressure;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)dig_p[7]) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)dig_p[6]) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)dig_p[5]) << 4);
    return (uint32_t)p;
}

uint32_t compensate_humidity(void) {
    // Returns humidity in %RHas unsigned 32 bit integer in Q22.10format (22integer and 10fractional bits).
    // Output value of ?47445?represents 47445/1024= 46.333%RH
    int32_t v_x1_u32r;
    v_x1_u32r = (t_fine - ((int32_t)76800));
    v_x1_u32r = (((((current_data.humidity << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * v_x1_u32r)) +
            ((int32_t)16384)) >> 15) * (((((((v_x1_u32r * ((int32_t)dig_H6)) >> 10) * (((v_x1_u32r *
            ((int32_t)dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) *
            ((int32_t)dig_H2) + 8192) >> 14));
    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >>7) * ((int32_t)dig_H1)) >> 4));
    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
    return(uint32_t)(v_x1_u32r >> 12);
}

static bool check_id(uint8_t addr) {
    uint8_t id = 0;
    i2c_read(addr, ID, &id, 1);
    if (id == 0x60)
        return true;
    else
        return false;
}

static void reset(void) {
    uint8_t reset_value = 0xB6;
    i2c_write(current_addr, RESET_ADDR, &reset_value, 1);
}

void update_bme_data(void) {
    uint8_t bytes[8];
    i2c_read(current_addr, PRESS_MSB, bytes, 8);
    current_data.pressure = (bytes[0] << 20) + (bytes[1] << 12) + (bytes[2] << 4);
    current_data.temperature = (bytes[3] << 20) + (bytes[4] << 12) + (bytes[5] << 4);
    current_data.humidity = (bytes[6] << 8) + (bytes[7]);
}