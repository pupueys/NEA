#ifndef DIGITAL_IO_H
#define DIGITAL_IO_H
#include <stdint.h>

// define the type of callback which is a function pointer that takes no input and gives no output
typedef void (*callback)(void);


// Function declarations
void digital_io(callback);
void chase_led(void);


#endif
