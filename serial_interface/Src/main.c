#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "serial.h"

#include "stm32f303xc.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

// Defining constants
#define TERMINATOR '!'
#define BUFFER_SIZE 64

volatile uint8_t rx_buffer[BUFFER_SIZE];

void rx_complete(uint8_t *pt, uint32_t counter) {
	// This function will be called after a transmission is complete

	volatile uint32_t test = 0;
	// make a very simple delay
	for (volatile uint32_t i = 0; i < 0x8ffff; i++) {
		// waste time !
	}
}

int main(void)
{

	bool polling_flag = true;

 	SerialInitialise(BUFFER_SIZE, BAUD_115200, &USART1_PORT, &rx_complete);

    /* Loop forever */
	for(;;) {
		if (polling_flag == true) {
			SerialReceiveString(rx_buffer, &USART1_PORT, BUFFER_SIZE, TERMINATOR);
		}
	}
}
