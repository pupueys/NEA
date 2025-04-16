#include <stdint.h>
#include <timers.h>
#include <callbacks.h>
#include "stm32f303xc.h"


int main(void) {

	// Enable LED Clocks and Pins
	enable_clocks();	// enabling LED clock
	initialise_board();	// enabling led gpio as output

	// PART A
	Timer_Init(1000, led_callback);	/// put in interval in ms
	Timer_Start();


	while(1) {

		static uint32_t counter = 0;
		counter++;

			if(counter == 3000000) {	// this is approx 7s
				// Part B
				Timer_SetPeriod(100);

				// Part C
				//Timer_OneShot(5000, led_oneshot_callback);
		}
	}
}
