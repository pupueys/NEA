#ifndef INTERRUPT_HEADER
#define INTERRUPT_HEADER

#include "stm32f303xc.h"
#include "serial.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


void USART1_EXTI25_IRQHandler(void);
void rx_function(SerialPort *serial_port);
void tx_function(SerialPort *serial_port);


#endif
