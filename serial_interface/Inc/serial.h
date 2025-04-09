#ifndef SERIAL_PORT_HEADER
#define SERIAL_PORT_HEADER

#include "stm32f303xc.h"

#include <stdint.h>
#include <stdio.h>

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


// SerialInitialise - initialise the serial port
// Input: baud rate as defined in the enum
void SerialInitialise(uint32_t buffer_size, uint32_t baudRate, SerialPort *serial_port, void (*callback)(volatile uint8_t*, uint32_t));
void SerialOutputChar(uint8_t data, SerialPort *serial_port);
void SerialReceiveChar(uint8_t *pt, SerialPort *serial_port);
void SerialReceiveString(uint8_t *buffer,
						 SerialPort *serial_port,
						 uint8_t buffer_size,
						 uint8_t terminator);

#endif
