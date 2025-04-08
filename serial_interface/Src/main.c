#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "serial.h"

#include "stm32f303xc.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#define TERMINATOR '!'
#define BUFFER_SIZE 64

void finished_transmission(uint32_t bytes_sent) {
	// This function will be called after a transmission is complete

	volatile uint32_t test = 0;
	// make a very simple delay
	for (volatile uint32_t i = 0; i < 0x8ffff; i++) {
		// waste time !
	}
}

void received_new_string(uint8_t *buffer, uint32_t bytes_sent) {

	// This function will be called after a full string is received
	SerialOutputString(buffer, &USART1_PORT);

}

// Buffer for receiving
volatile char buffer[BUFFER_SIZE];
volatile uint32_t index = 0;

int main(void)
{

 	SerialInitialise(BAUD_115200, &USART1_PORT, &finished_transmission, &received_new_string);


    /* Loop forever */
	for(;;) {
	}
}
