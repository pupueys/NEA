#include "stm32f303xc.h"
#include <stdint.h>
#include <stddef.h>  // for NULL checking
#include <callbacks.h>

volatile uint32_t seconds_counter = 0;



void enable_clocks() {

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOEEN;	// led clock
}

// initialise the discovery board I/O
void initialise_board() {

	uint16_t *led_output_registers = ((uint16_t *)&(GPIOE->MODER)) + 1;
	*led_output_registers = 0x5555;

}

void led_callback(void) {

    static uint8_t led_state = 0;  // 0 = OFF, 1 = ON
    uint8_t *led_output_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

    if (led_state == 0) {
       *led_output_register = 0b01010101;  // Turn LEDs ON
        led_state = 1;  // Next time, turn OFF

    } else {
        *led_output_register = 0x00;   // Turn LEDs OFF
        led_state = 0;  // Next time, turn ON
    }
}

void led_oneshot_callback(void) {

    uint8_t *led_output_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
    *led_output_register = 0b10101010;  // Turn LEDs ON

}

void turn_leds_off(void) {
	 uint8_t *led_output_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	 *led_output_register = 0b00000000;  // Turn LEDs OFF
}
