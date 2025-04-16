#include "serial.h"
#include "stm32f303xc.h"

#define TERMINATOR '\0'

// instantiate the serial port parameters
//   note: the complexity is hidden in the c file
SerialPort USART1_PORT = {USART1,
		GPIOC,
		RCC_APB2ENR_USART1EN, 	// bit to enable for APB2 bus
		0x00,					// bit to enable for APB1 bus
		RCC_AHBENR_GPIOCEN, 	// bit to enable for AHB bus
		0xA00,					// bits to set alternate function
		0xF00,					// bits to set high speed mode
		0x770000,  				// for USART1 PC10 and 11, this is in the AFR low register
		0x00, 					// no change to the high alternate function register
		0x00, 					// buffer pointer NULL
		0x00,					// second buffer pointer NULL
		0x00, 					// count NULL
		0x00,					// buffer size NULL
		0x00,					// tx pointer NULL
		USART1_IRQn,			// IRQn for USART1
		0x00 					// callback function pointer is NULL
		};


// InitialiseSerial - Initialise the serial port
// Input: baudRate is from an enumerated set
void SerialInitialise(uint32_t buffer_size,
					  uint32_t baudRate,
					  SerialPort *serial_port,
					  void (*rx_complete)(volatile uint8_t*, uint32_t)) {

	// buffer_size: size of the buffers
	// baudRate: the serial baud rate
	// serial_port: address of the serial port to initialise
	// rx_complete_function: pointer to callback function; inputs are pointer to string buffer
	//						 and length of the stored string respectively

	serial_port->callback = rx_complete;


	// enable clock power, system configuration clock and GPIOC
	// common to all UARTs
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// enable the GPIO which is on the AHB bus
	RCC->AHBENR |= serial_port->MaskAHBENR;

	// set pin mode to alternate function for the specific GPIO pins
	serial_port->GPIO->MODER = serial_port->SerialPinModeValue;

	// enable high speed clock for specific GPIO pins
	serial_port->GPIO->OSPEEDR = serial_port->SerialPinSpeedValue;

	// set alternate function to enable USART to external pins
	serial_port->GPIO->AFR[0] |= serial_port->SerialPinAlternatePinValueLow;
	serial_port->GPIO->AFR[1] |= serial_port->SerialPinAlternatePinValueHigh;

	// enable the device based on the bits defined in the serial port definition
	RCC->APB1ENR |= serial_port->MaskAPB1ENR;
	RCC->APB2ENR |= serial_port->MaskAPB2ENR;

	// Get a pointer to the 16 bits of the BRR register that we want to change
	uint16_t *baud_rate_config = (uint16_t*)&serial_port->UART->BRR; // only 16 bits used!

	// Baud rate calculation from datasheet
	switch(baudRate){
	case BAUD_9600:
		*baud_rate_config = 0x342;  // 9600 at 8MHz
		break;
	case BAUD_19200:
		*baud_rate_config = 0x19C;  // 19200 at 8MHz
		break;
	case BAUD_38400:
		*baud_rate_config = 0xD0;  // 38400 at 8MHz
		break;
	case BAUD_57600:
		*baud_rate_config = 0x8B;  // 57600 at 8MHz
		break;
	case BAUD_115200:
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	}


	// enable serial port for tx and rx
	serial_port->UART->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;

	// configuring buffers
	serial_port->BufferSize = buffer_size;
	serial_port->Buffer = (volatile uint8_t*)malloc(serial_port->BufferSize * sizeof(uint8_t));
	serial_port->SecondBuffer = (volatile uint8_t*)malloc(serial_port->BufferSize * sizeof(uint8_t));

	__disable_irq(); // disable all interrupts while changing settings

	// enabling the interrupts
	serial_port->UART->CR1 |= USART_CR1_RXNEIE;		// enabling RXNE interrupts
	serial_port->UART->CR3 |= USART_CR3_EIE;		// enabling the error interrupts

	// activating interrupts and setting priority
	NVIC_SetPriority(serial_port->UART_IRQn, 5);
	NVIC_EnableIRQ(serial_port->UART_IRQn);			//  enable UART interrupts

	__enable_irq(); // re-enable all interrupts

}

void USART1_EXTI25_IRQHandler(void) {

	// when interrupt is reached, call the rx_function
	rx_function(&USART1_PORT);

	// transmit if and only if the transmit interrupt TXEIE is triggered
	if (!(USART1->CR1 & USART_CR1_TXEIE) == 0) {
		tx_function(&USART1_PORT);
	}
}

void rx_function(SerialPort *serial_port) {

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

	__disable_irq();

	// enabling the TXEIE interrupt if transmission is enabled
	if (flag == true) {
		serial_port->UART->CR1 |= USART_CR1_TXEIE;
	} else {
		serial_port->UART->CR1 &= ~USART_CR1_TXEIE;	// ANDing with compliment to disable
	}

	__enable_irq();
}

void tx_string(uint8_t *str, SerialPort *serial_port) {

	// set the transmission pointer to be the beginning of the string array
	serial_port->TxPointer = str;
	tx_enable(true, serial_port);

}

void tx_function(SerialPort *serial_port) {

	// checking for terminating character
	if (*(serial_port->TxPointer) == TERMINATOR) {

		serial_port->UART->TDR = TERMINATOR;
		tx_enable(false, serial_port);		// disable transmission interrupts
	}

	// transmit character
	serial_port->UART->TDR = *serial_port->TxPointer;
	serial_port->TxPointer++;
}
