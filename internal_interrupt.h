/* 
 * File:   internal_interrupt.h
 * Author: aleks
 *
 * Created on August 19, 2020, 1:53 PM
 */

#ifndef INTERNAL_INTERRUPT_H
#define	INTERNAL_INTERRUPT_H

#ifdef	__cplusplus
extern "C" {
#endif

    void int_i_initialize(void);
    void int_ISR(void);

#define INT_enable() INTE = 1
#define INT_disable() INTE = 0


#ifdef	__cplusplus
}
#endif

#endif	/* INTERNAL_INTERRUPT_H */

