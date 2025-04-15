#include "digital_io.h"
#include <stdint.h>
#include "stm32f303xc.h"
#include "set_get_leds.h"
#include "timer.h"

int main(void)
{

	digital_io(&chase_led);                   // Address of the callback function is passed on the initialisation function
        set_led_state(0b0000011);                 // Change this value to set the LEDs in a different pattern


	for(;;) {}
}

