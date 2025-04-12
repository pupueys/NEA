#include "parse.h"

void parse_buffer(volatile uint8_t *buffer, uint32_t size) {

	/*  This callback function parses through the buffer
	 *  to determine what is required to be done.
	 */

	if ((size >= 7 && strncmp((volatile uint8_t*)buffer, "serial ", 7) == 0)) {

		tx_string((volatile uint8_t*)buffer + 7, &USART1_PORT);
		return;
	}

	if ((size >= 4 && strncmp((volatile uint8_t*)buffer, "led ", 4) == 0)) {

                uint8_t led_pattern = led_case((volatile uint8_t*)buffer + 4);
                enable_clocks();                         // enable the clocks
                initialise_board();                      // initialise the boards
                set_led_state(led_pattern);
	}

}
