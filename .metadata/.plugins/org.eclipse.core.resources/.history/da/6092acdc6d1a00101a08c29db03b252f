#ifndef INTERRUPT_HEADER
#define INTERRUPT_HEADER

#include "stm32f303xc.h"
#include "serial.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*  Handler for the USART1 interrupts
    *  Activates rx_function if interrupt is triggered
    Activates tx_function when transmit interrupt is triggered */
void USART1_EXTI25_IRQHandler(void);


/* Enables the receiving interrupts */
void rx_enable(SerialPort *serial_port);

/* This function is called when the RXNE interrupt is triggered.
* If the buffer is filled, the double buffers will switch, with the second buffer
* being able to receive, while the other buffer can be used */
void rx_function(SerialPort *serial_port);

/* Given a flag, the transmission interrupt will be enabled/disabled */
void tx_enable(bool flag, SerialPort *serial_port);

/* Transmit the given string*/
void tx_string(uint8_t *str, SerialPort *serial_port);

/* This function is called when the TXE interrupt is enabled.
If this was triggered intentionally, a character will be placed in the TDR*/
void tx_function(SerialPort *serial_port);


#endif
