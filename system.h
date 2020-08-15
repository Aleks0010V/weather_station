/* 
 * File:   system.h
 * Author: aleks
 *
 * Created on August 14, 2020, 1:09 PM
 */

#ifndef SYSTEM_H
#define	SYSTEM_H

#ifdef	__cplusplus
extern "C" {
#endif

#define interrupt_enable() GIE = 1
#define interrupt_disable() GIE = 0
#define peripheral_int_enable() PEIE = 1
#define peripheral_int_disable() PEIE = 0
void System_Initialize(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_H */

