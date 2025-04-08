#include "interrupts.h"
#include "stm32f303xc.h"
#include "serial.h"

#include <stdint.h>

void USART1_IRQHandler(void) {

	// checking if there is a character to be read
	if (USART1->ISR & USART_ISR_RXNE) {
		uint8_t received_byte = (uint8_t)(USART1->RDR);


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
