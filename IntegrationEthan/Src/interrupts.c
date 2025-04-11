#include "interrupts.h"

#define TERMINATOR '\n'

void USART1_EXTI25_IRQHandler(void) {

	/*  Handler for the USART1 interrupts
	 *  Activates rx_function if interrupt is triggered
	 	Activates tx_function when transmit interrupt is triggered */

	// when interrupt is reached, call the rx_function
	rx_function(&USART1_PORT);

	// transmit if and only if the transmit interrupt TXEIE is triggered
	if (!(USART1->CR1 & USART_CR1_TXEIE) == 0) {
		tx_function(&USART1_PORT);
	}
}

void rx_function(SerialPort *serial_port) {

	/* This function is called when the RXNE interrupt is triggered.
	 * If the buffer is filled, the double buffers will switch, with the second buffer
	 * being able to receive, while the other buffer can be used */

	// checking if receiving is working properly
	if (!((serial_port->UART->ISR & USART_ISR_RXNE) == 0) &&
		(serial_port->UART->ISR & USART_ISR_ORE) == 0 &&
		(serial_port->UART->ISR & USART_ISR_FE) == 0)  {

		// reading the character into the buffer
		serial_port->Buffer[serial_port->Count] = (uint8_t)(serial_port->UART->RDR);
		serial_port->Count++;

		// if the buffer has been filled, append the terminating character
		if (serial_port->Count + 1 == serial_port->BufferSize) {

			serial_port->Buffer[serial_port->Count] = TERMINATOR;
			serial_port->Count++;
		}

		// if the terminating character has been read, reading is complete and callback occurs
		// (extension) swap the buffers
		if (serial_port->Buffer[serial_port->Count - 1 ] == TERMINATOR) {

			volatile uint8_t* temp_pt = serial_port->Buffer;
			serial_port->Buffer = serial_port->SecondBuffer;
			serial_port->SecondBuffer = temp_pt;

			// callback with the first buffer since the first buffer is finished reading
			serial_port->callback(serial_port->SecondBuffer, serial_port->Count);
			serial_port->Count = 0;
		}

	}	else {
		// clear error flags
			serial_port->UART->ICR |= USART_ICR_ORECF | USART_ICR_FECF;

	}

}

void tx_enable(bool flag, SerialPort *serial_port) {
	/* Given a flag, the transmission interrupt will be enabled/disabled */
	__disable_irq();

	if (flag == true) {
		serial_port->UART->CR1 |= USART_CR1_TXEIE;
	} else {
		serial_port->UART->CR1 &= ~USART_CR1_TXEIE;
	}

	__enable_irq();
}

void tx_string(uint8_t *str, SerialPort *serial_port) {

	serial_port->TxPointer = str;
	tx_enable(true, serial_port);

}

void tx_function(SerialPort *serial_port) {

	/* This function is called when the TXE interrupt is enabled.
	   If this was triggered intentionally, a character will be placed in the TDR
	 */

	// checking for terminating character
	if (*(serial_port->TxPointer) == TERMINATOR) {

		serial_port->UART->TDR = TERMINATOR;
		tx_enable(false, serial_port);
	}



	// transmit character
	serial_port->UART->TDR = *serial_port->TxPointer;
	serial_port->TxPointer++;
}


