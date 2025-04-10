#include "digital_io.h"
#include "stm32f303xc.h"


// LED state set to static so that it is only accessible in this file
static volatile uint8_t led_state = 1;

// Get the current LED state
uint8_t get_led_state(void) {
	return led_state;                         // return the current led state stored in led_state
}

// Set the LED state manually
void set_led_state(uint8_t state) {                // pass in a uint8_t value which represents the LED state
	led_state = state;                             // set the input of the function to the led_state
	*((uint8_t*)&(GPIOE->ODR) + 1) = led_state;    // update the LED state to the ODR
}
