#ifndef PARSE_HEADER
#define PARSE_HEADER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "set_get_leds.h"
#include "digital_IO.h"
#include "serial.h"
#include "interrupts.h"

/*
 *  Callback function for when a string is received; parses the buffer to determine what to do
 */
void parse_buffer(volatile uint8_t *pt, uint32_t size);

#endif
