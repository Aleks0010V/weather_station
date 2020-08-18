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
#define SSPEN SSP1CON1bits.SSPEN
#define ACKDT SSP1CON2bits.ACKDT

#define serial_port_enable() SSPEN = 1
#define serial_port_disable() SSPEN = 0
#define start() SEN = 1
#define stop() PEN = 1
#define restart() RSEN = 1
    
void I2C_Initialize(void);
void i2c_ISR(void);
void master_write_1Byte(uint8_t address, uint8_t reg, uint8_t data);
//void master_write_2Bytes(uint8_t address, uint16_t data);
void master_read_1Byte(uint8_t address, uint8_t reg, uint8_t* dest_ptr);


#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

