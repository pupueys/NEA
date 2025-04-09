/*
 * timer.h
 *
 *  Created on: Apr 9, 2025
 *      Author: nadeen
 */

#ifndef TIMER_H_
#define TIMER_H_

void enable_button_interrupt() ;
void TIM3_IRQHandler(void) ;

extern volatile uint8_t led_change_flag;

extern volatile uint8_t button_pressed_flag;

#endif /* TIMER_H_ */
