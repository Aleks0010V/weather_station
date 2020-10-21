/* 
 * File:   timer_0.h
 * Author: aleks
 *
 * Created on August 14, 2020, 1:37 PM
 */

#ifndef TIMER_0_H
#define	TIMER_0_H

#ifdef	__cplusplus
extern "C" {
#endif

#define t0_enable() T0EN = 1
#define t0_disable() T0EN = 0
#define t0_int_enable() TMR0IE = 1
#define t0_int_disable() TMR0IE = 0
    void timer_0_Initialize(void);
    void t0_ISR(void);


#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_0_H */

