#include "parse.h"

#define SERIAL_ARG 7
#define LED_ARG 4
#define TIMER_ARG 6
#define ONESHOT_ARG 8

void parse_buffer(volatile uint8_t *buffer, uint32_t size) {
	stop_timer();
	set_led_state(0b00000000);


	if ((size >= SERIAL_ARG && strncmp((volatile uint8_t*)buffer, "serial ", SERIAL_ARG) == 0)) {

		tx_string((volatile uint8_t*)buffer + SERIAL_ARG, &USART1_PORT);
		return;
	}

	// Checks if the argument is to display leds
	if ((size >= LED_ARG && strncmp((volatile uint8_t*)buffer, "led ", LED_ARG) == 0)) {

        uint8_t led_pattern = led_case((volatile uint8_t*)buffer + LED_ARG);
        enable_clocks();                         // enable the clocks
        initialise_board();                      // initialise the boards
        set_led_state(led_pattern);
}

	// Checks if the argument is to start timer
    if ((size >= TIMER_ARG && strncmp((volatile uint8_t*)buffer, "timer ", TIMER_ARG) == 0)) {

		uint32_t timer_delay = timers_case(buffer + 6);
		enable_clocks();
		initialise_board();
		Timer_Init(timer_delay, led_callback);
		Timer_Start();
	}

    // Checks if the argument is to do oneshot
	if ((strncmp(buffer, "oneshot ", 8) == 0)) {
    if ((size >= ONESHOT_ARG && strncmp((volatile uint8_t*)buffer, "oneshot ", ONESHOT_ARG) == 0)) {

		uint32_t oneshot_delay = timers_case(buffer + 8);
		enable_clocks();
		initialise_board();
		Timer_Init(oneshot_delay, NULL);
		Timer_OneShot(oneshot_delay, led_oneshot_callback);
	}

}


}
