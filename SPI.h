/* 
 * File:   SPI.h
 * Author: aleks
 *
 * Created on August 22, 2020, 6:51 PM
 */

#include <stdint.h>
//#include <stdbool.h>

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

#define SSPEN_2 SSP2CON1bits.SSPEN

typedef struct
{
    uint8_t data;
    void (*preload_func)(void);
    void (*postload_func)(uint8_t data);
    struct msg *next_msg;
}msg;
    
void initialize_SPI(void);
void spi_master_write_1Byte (uint8_t *data, uint8_t len, void (*preload_func)(void), void (*postload_func)(void));
void spi_ISR(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

