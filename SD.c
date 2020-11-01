/*
 * File:   SD.c
 * Author: aleks
 *
 * Created on August 22, 2020, 6:50 PM
 */


#include <xc.h>
#include "SD.h"
#include "SPI.h"

static const SD_command reset = {CMD0, 0, 0, 0, 0, 0x95};
static const SD_command step_1 = {CMD1, 0, 0, 0x01, 0xAA, 0x95};

// 512 byte read buffer
uint8_t SDRdata[512];
// no response flag
uint8_t no_response = 0;
// timeout variable to determine card timeout
uint32_t timeout = SD_TIMEOUT;

static uint8_t SDcard_get_response(uint8_t response);

uint8_t SD_initialize(void) {
    LATCbits.LATC4 = 1; // set CS high to send dummy bytes in SD mode to wake up SD card
    uint8_t dummy[] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
    spi_write(dummy, 10);
    
    LATCbits.LATC4 = 0;
    spi_write(&reset, 6);
    if(SDcard_get_response(0x01) != 0) {
        LATCbits.LATC4 = 1;
        return 1;
    }
    
    no_response = 1;
    timeout = SD_TIMEOUT;
    while(no_response && timeout--){
        // send command 1
        spi_write(&step_1, 6);
        // read back response
        // response time is 0 to 8 bytes
        spi_read(SDRdata, 8, 255);
        for(uint8_t b = 0; b < 7; b++){
            if(SDRdata[b] == 0x00) 
                no_response = 0;
        }
    }
    LATCbits.LATC4 = 1;
    return no_response;
}

static uint8_t SDcard_get_response(uint8_t response){
    // read back response
    // response time is 0 to 8 bytes
    no_response = 1;
    timeout = SD_TIMEOUT;
    while(no_response && timeout--){
        spi_read(SDRdata, 1, 255); // read one byte
        if(SDRdata[0] == response) no_response = 0; // check if response matches
    }
    return no_response;
}