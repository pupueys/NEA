#ifndef CALLBACKS_H
#define CALLBACKS_H
#include <stdint.h>


void enable_clocks();
void initialise_board();
void led_callback(void);
void led_oneshot_callback(void);
void turn_leds_off(void);


#endif
