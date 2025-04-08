#include "interrupts.h"
#include "stm32f303xc.h"
#include "serial.h"

#include <stdint.h>

#define BUFFER_SIZE 64
#define TERMINATOR '!'

extern volatile uint8_t buffer[BUFFER_SIZE];  // Declare the external buffer
extern volatile uint32_t index;            // Declare the external buffer index

void USART1_IRQHandler(void) {

	uint32_t counter = 0

	// checking if there is a character to be read
	if (USART1->ISR & USART_ISR_RXNE) {
		uint8_t received_byte = (uint8_t)(USART1->RDR);	// reading received byte

		if (index < BUFFER_SIZE) {
			buffer[index++] = received_byte;
			counter++;
		}

		if (received_byte == TERMINATOR || buffer_index == BUFFER_SIZE) {
			index = 0;
		}

		// callback
		serial_port->receive_complete_function(buffer, counter);


	}
}

void usart_interrupt(USART_TypeDef *USART) {

	// disabling all interrupts
	__disable_irq();

    USART->CR1 |= USART_CR1_RXNEIE;  // RXNE interrupt enable

    // Enable the USART peripheral
    USART->CR1 |= USART_CR1_UE;  // USART Enable

    // Telling NVIC module that USART1 interrupts should be handled
    NVIC_SetPriority(USART1_IRQn, 1);
    NVIC_EnableIRQ(USART1_IRQn);

	__enable_irq();

}
