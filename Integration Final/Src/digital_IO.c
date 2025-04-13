#include "digital_io.h"
#include "stm32f303xc.h"
#include "set_get_leds.h"


// enable the clocks for desired peripherals (GPIOA, C and E)
void enable_clocks() {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOEEN;
}


// initialise the discovery board I/O (just outputs: inputs are selected by default)
void initialise_board() {
	// get a pointer to the second half word of the MODER register (for outputs pe8-15)
	uint16_t *led_output_registers = ((uint16_t *)&(GPIOE->MODER)) + 1;
	*led_output_registers = 0x5555;
}

uint8_t led_case(char *restofwords) {
    uint8_t value = 0;
    int i = 0;

    while (restofwords[i] != '\0' && i < 8) { // Only process 8 bits
        value <<= 1; // Shift left
        if (restofwords[i] == '1') {
            value |= 1;
        }
        i++;
    }

    return value;
}
