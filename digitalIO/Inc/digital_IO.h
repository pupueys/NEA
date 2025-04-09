#ifndef DIGITAL_IO_H
#define DIGITAL_IO_H
#include <stdint.h>

// define the type of callback which is a function pointer that takes no input and gives no output
typedef void (*callback)(void);


// Function declarations
void digital_io(callback);
void chase_led(void);

// header files for set_led_state and get_led_state if they were to be used in main.c
uint8_t get_led_state(void);
void set_led_state(uint8_t state);

#endif
