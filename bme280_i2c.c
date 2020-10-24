/*
 * File:   bme280.c
 * Author: aleks
 *
 * Created on August 23, 2020, 2:01 PM
 */


#include <xc.h>
#include <stdbool.h>
#include "system.h"
#include "I2C.h"
#include "bme280_i2c.h"

#define MAIN 0xEC
#define RESERVE 0xEE

#define ID 0xD0
#define RESET_ADDR 0xE0

static bool check_id(uint8_t addr);
static void reset(void);
static void update_data(void);

static uint8_t current_addr = 0;

bme_data current_data;

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

static void update_data(void) {
    uint8_t bytes[8];
    i2c_read(current_addr, PRESS_MSB, bytes, 8);
    current_data.pressure = (bytes[0] << 20) + (bytes[1] << 12) + (bytes[2] << 4);
    current_data.temperature = (bytes[3] << 20) + (bytes[4] << 12) + (bytes[5] << 4);
    current_data.humidity = (bytes[6] << 8) + (bytes[7]);
}