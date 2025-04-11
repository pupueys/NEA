#include "parse.h"

void parse_buffer(volatile uint8_t *buffer, uint32_t size) {

	/*  This callback function parses through the buffer
	 *  to determine what is required to be done.
	 */

	if ((strncmp(buffer, "serial ", 7) == 0)) {

		tx_string(buffer + 7, &USART1_PORT);
		return;
	}

	if ((strncmp(buffer, "led ", 4) == 0)) {

                uint8_t led_pattern = led_case(buffer + 4);
                enable_clocks();                         // enable the clocks
                initialise_board();                      // initialise the boards
                set_led_state(led_pattern);
	}

}
