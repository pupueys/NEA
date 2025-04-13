#ifndef SERIAL_HEADER
#define SERIAL_HEADER

#include "stm32f303xc.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// Defining the serial port struct
typedef struct {

	// setup parameters
	USART_TypeDef *UART;
	GPIO_TypeDef *GPIO;
	volatile uint32_t MaskAPB2ENR;	// mask to enable RCC APB2 bus registers
	volatile uint32_t MaskAPB1ENR;	// mask to enable RCC APB1 bus registers
	volatile uint32_t MaskAHBENR;	// mask to enable RCC AHB bus registers
	volatile uint32_t SerialPinModeValue;
	volatile uint32_t SerialPinSpeedValue;
	volatile uint32_t SerialPinAlternatePinValueLow;
	volatile uint32_t SerialPinAlternatePinValueHigh;

	// rx tx parameters
	volatile uint8_t* Buffer;
	volatile uint8_t* SecondBuffer;
	volatile uint32_t Count;
	volatile uint32_t BufferSize;
	volatile uint8_t* TxPointer;
	volatile uint32_t UART_IRQn;
	void (*callback)(volatile uint8_t*, uint32_t);
} SerialPort;

// make any number of instances of the serial port (they are extern because
//   they are fixed, unique values)
extern SerialPort USART1_PORT;


// The user might want to select the baud rate
enum {
  BAUD_9600,
  BAUD_19200,
  BAUD_38400,
  BAUD_57600,
  BAUD_115200
};

/* Initialises the specified serial port and the receiving serial interrupts*/
void SerialInitialise(uint32_t buffer_size, uint32_t baudRate, SerialPort *serial_port, void (*callback)(volatile uint8_t*, uint32_t));

/*  Handler for the USART1 interrupts
 *  Activates rx_function if interrupt is triggered
 	Activates tx_function when transmit interrupt is triggered */
void USART1_EXTI25_IRQHandler(void);

/* This function is called when the RXNE interrupt is triggered.
	 * If the buffer is filled, the double buffers will switch, with the second buffer
	 * being able to receive, while the other buffer can be used */
void rx_function(SerialPort *serial_port);

/* Given a flag, the transmission interrupt will be enabled/disabled */
void tx_enable(bool flag, SerialPort *serial_port);

/* Sets the input string to be transmitted*/
void tx_string(uint8_t *str, SerialPort *serial_port);

/* This function is called when the TXEIE interrupt is enabled*/
void tx_function(SerialPort *serial_port);


#endif
