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

#define SSPEN_2 SSP2CON1bits.SSPEN

void initialize_SPI(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

