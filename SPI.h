/* 
 * File:   SPI.h
 * Author: aleks
 *
 * Created on August 22, 2020, 6:51 PM
*/

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define SSPEN_2 SSP2CON1bits.SSPEN 

/* SPI interfaces */
typedef enum { 
    SPI2_DEFAULT
} spi2_modes_t;

void initialize_SPI(void);
bool SPI2_Open(spi2_modes_t spi2UniqueConfiguration);
void SPI2_Close(void);
uint8_t SPI2_ExchangeByte(uint8_t data);
void SPI2_ExchangeBlock(void *block, size_t blockSize);
void SPI2_WriteBlock(void *block, size_t blockSize);
void SPI2_ReadBlock(void *block, size_t blockSize);
void SPI2_WriteByte(uint8_t byte);
uint8_t SPI2_ReadByte(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

