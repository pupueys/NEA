#include "digital_io.h"
#include <stdint.h>
#include "stm32f303xc.h"
#include "set_get_leds.h"
#include "timer.h"

int main(void)
{
        //set_led_state(3);                       // uncomment this to test setting the LED state for part 3
	digital_io(&chase_led);                   // Address of the callback function is passed on the initialisation function


	for(;;) {}                                // loop forever
}
