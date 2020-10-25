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
    typedef long long signed int int64_t;
    
    bool bme280_exists(void);
    void bme280_Initialize(void);
    void update_bme_data(void);
    void initialize_compensate_values(void);
    
    // these 3 functions take 33% of PIC16F18875 memory - 4730.88 bytes.
    // compensate_pressure - most of it.
    int32_t compensate_temperature(void);
    uint32_t compensate_pressure(void);
    uint32_t compensate_humidity(void);


#ifdef	__cplusplus
}
#endif

#endif	/* BME280_H */

