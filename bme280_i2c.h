/* 
 * File:   bme280.h
 * Author: aleks
 *
 * Created on August 23, 2020, 2:04 PM
 */

#ifndef BME280_H
#define	BME280_H

#ifdef	__cplusplus
extern "C" {
#endif

    bool bme280_exists(void);
    void bme280_Initialize(void);


#ifdef	__cplusplus
}
#endif

#endif	/* BME280_H */

