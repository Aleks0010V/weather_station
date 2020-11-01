/* 
 * File:   one_wire.h
 * Author: aleks
 *
 * Created on October 30, 2020, 8:59 PM
 */

#include <stdint.h>

#ifndef ONE_WIRE_H
#define	ONE_WIRE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    // readonly pointer to I/O status
    extern const uint8_t * const read_write_status; // read - 0; write - 1
    
    // buffer for timer ISR
    extern uint8_t oneWire_buf;
    uint8_t oneWire_buf_size = 8;
    
    uint8_t oneWire_write(void *data);


#ifdef	__cplusplus
}
#endif

#endif	/* ONE_WIRE_H */

