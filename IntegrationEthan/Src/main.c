#include <serial.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>


#include "stm32f303xc.h"
#include "set_get_leds.h"
#include "digital_IO.h"
#include "serial.h"
#include "parse.h"

#define BUFFER_SIZE 128
#define TERMINATOR '\0'

int main() {

	SerialInitialise(BUFFER_SIZE, BAUD_115200, &USART1_PORT, &parse_buffer);

	for (;;) {

	}

}

