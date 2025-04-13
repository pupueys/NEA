/*
 * set_get_leds.h
 *
 *  Created on: Apr 10, 2025
 *      Author: nadeen
 */

#ifndef SET_GET_LEDS_H_
#define SET_GET_LEDS_H_

static volatile uint8_t led_state = 1;

// header files for set_led_state and get_led_state if they were to be used in main.c
uint8_t get_led_state(void);
void set_led_state(uint8_t state);


#endif /* SET_GET_LEDS_H_ */
