/* 
 * File:   I2C.h
 * Author: aleks
 *
 * Created on August 14, 2020, 1:08 PM
 */
#include <stdint.h>

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
#define i2c_int_enable() SSP1IE = 1
#define i2c_int_disable() SSP1IE = 0

void I2C_Initialize(void);
void i2c_ISR(void);
void i2c_master_write_1Byte(uint8_t address, uint8_t reg, uint8_t data);
void i2c_master_read_1Byte(uint8_t address, uint8_t reg, uint8_t* dest_ptr);
void i2c_write_data(uint8_t address, uint8_t reg, void *data_ptr, size_t size);


#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

