/* 
 * File:   I2C.h
 * Author: aleks
 *
 * Created on August 14, 2020, 1:08 PM
 */
#include <stdint.h>
#include <stdbool.h>

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

#define SEN SSP1CON2bits.SEN
#define RSEN SSP1CON2bits.RSEN
#define PEN SSP1CON2bits.PEN
#define RCEN SSP1CON2bits.RCEN
#define ACKEN SSP1CON2bits.ACKEN
#define RW SSP1STATbits.RW
#define BF SSP1STATbits.BF
#define SSPEN_1 SSP1CON1bits.SSPEN
#define ACKDT SSP1CON2bits.ACKDT
#define ACKSTAT SSP1CON2bits.ACKSTAT

    void I2C_Initialize_master(void);
    void i2c_ISR(void);
    void i2c_write(uint8_t address, uint8_t reg, uint8_t *data, uint8_t size);
    void i2c_read(uint8_t address, uint8_t reg, uint8_t* dest_ptr, uint8_t size);
    bool i2c_open(uint8_t address, uint8_t reg);


#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

