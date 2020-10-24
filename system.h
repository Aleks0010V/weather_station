/* 
 * File:   system.h
 * Author: aleks
 *
 * Created on August 14, 2020, 1:09 PM
 */
#include <stdint.h>

#ifndef SYSTEM_H
#define	SYSTEM_H

#ifdef	__cplusplus
extern "C" {
#endif

#define interrupt_enable() GIE = 1
#define interrupt_disable() GIE = 0
#define peripheral_int_enable() PEIE = 1
#define peripheral_int_disable() PEIE = 0
#define _XTAL_FREQ 16000000

    typedef union {
        // BE CAREFULL WITH IT
        // https://stackoverflow.com/a/2313676/11189805
        uint16_t data;
        uint8_t bytes[2];
    } word;

    void System_Initialize(void);
    word pack_word(uint8_t lsb, uint8_t msb);


#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_H */

