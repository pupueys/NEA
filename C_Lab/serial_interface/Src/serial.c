#include "serial.h"

#include "stm32f303xc.h"

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

}

void SerialOutputChar(uint8_t data, SerialPort *serial_port) {

	while((serial_port->UART->ISR & USART_ISR_TXE) == 0){
	}

	// outputting the data
	serial_port->UART->TDR = data;
}



void SerialOutputString(uint8_t *pt, SerialPort *serial_port) {

	uint32_t counter = 0;

	//  outputting while there is still data to output
	while(*pt) {
		SerialOutputChar(*pt, serial_port);
		counter++;
		pt++;
	}
}

// Function to receive a character
void SerialReceiveChar(uint8_t *pt, SerialPort *serial_port) {

	while((serial_port->UART->ISR & USART_ISR_RXNE) == 0){
	}

	// receiving a character into the buffer
	*pt = (uint8_t)(serial_port->UART->RDR);

}

void SerialReceiveString(uint8_t *buffer,
						 SerialPort *serial_port,
						 uint8_t buffer_size,
						 uint8_t terminator) {

	// Initialising variables for receiving
    uint8_t *buffer_base = buffer;		// to return to the beginning of the pointer
    uint32_t counter = 0;					
    uint8_t received_char;

	// receive while there is no terminating character
    do {
        SerialReceiveChar(&received_char, serial_port);
        *buffer = received_char;

        counter++;
        buffer++;

		// overwrite when buffer is filled
        if (counter == buffer_size) {
            buffer = buffer_base;
            counter = 0;
        }

    } while (received_char != terminator);

	// reset the buffer
    buffer = buffer_base;

	// return to callback function
    serial_port->callback(buffer, counter);
}






