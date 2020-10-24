/* 
 * File:   bme280.h
 * Author: aleks
 *
 * Created on August 23, 2020, 2:04 PM
 */

#include <stdint.h>

#ifndef BME280_H
#define	BME280_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    typedef struct {
        int32_t pressure;
        int32_t temperature;
        int32_t humidity;
    } bme_data;
    
    bool bme280_exists(void);
    void bme280_Initialize(void);


#ifdef	__cplusplus
}
#endif

#endif	/* BME280_H */

