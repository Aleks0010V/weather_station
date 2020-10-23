/* 
 * File:   SPI.h
 * Author: aleks
 *
 * Created on August 22, 2020, 6:51 PM
 */

#include <stdint.h>
#include <stdbool.h>

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

#define SSPEN_2 SSP2CON1bits.SSPEN

    void initialize_SPI_master(bool CKP, bool CKE);
    void spi_write(void *data, uint8_t size);
    void spi_read(void *dest_reg, uint8_t size);
    void spi_exchange_block(uint8_t *block, uint8_t size);
    void spi_ISR(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

