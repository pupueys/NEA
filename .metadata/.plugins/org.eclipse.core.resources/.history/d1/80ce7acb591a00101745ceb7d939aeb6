#include "parse.h"

void parse_buffer(volatile uint8_t *buffer, uint32_t size) {

	/*  This callback function parses through the buffer
	 *  to determine what is required to be done.
	 */

	if ((strncmp(buffer, "serial ", 7) == 0)) {

		tx_string(pt + 7, &USART1_PORT);
		return;
	}

}
