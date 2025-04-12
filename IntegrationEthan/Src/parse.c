#include "parse.h"

#define SERIAL_ARG 7
#define LED_ARG 4

void parse_buffer(volatile uint8_t *buffer, uint32_t size) {

	// Checks if the argument is to transmit via serial
	if ((size >= SERIAL_ARG && strncmp((volatile uint8_t*)buffer, "serial ", SERIAL_ARG) == 0)) {

		tx_string((volatile uint8_t*)buffer + SERIAL_ARG, &USART1_PORT);
		return;
	}

	// Checks if the argument is to display leds
	if ((size >= LED_ARG && strncmp((volatile uint8_t*)buffer, "led ", LED_ARG) == 0)) {

                uint8_t led_pattern = led_case((volatile uint8_t*)buffer + LED_ARG);
                enable_clocks();                         // enable the clocks
                initialise_board();                      // initialise the boards
                set_led_state(led_pattern);
	}

}
