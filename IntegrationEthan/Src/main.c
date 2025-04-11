#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "set_get_leds.h"
#include "digital_IO.h"
#include "parse.h"

#define BUFFER_SIZE 128
#define TERMINATOR '\0'


uint32_t timers_case(char* restofwords) {
    uint32_t value = 0;

    for(int i=0; restofwords[i] != '\0'; i++) {
        value = value * 10 + (restofwords[i] - '0');
    }

    return value;

}

int main() {

	SerialInitialise(BUFFER_SIZE, BAUD_115200, &USART1_PORT, &parse_buffer);

//	// Manual test buffer NEED TO CHANGE THIS TO INPUT FROM SERIAL PORTS
//	char buffer[BUFFER_SIZE] = "led 10101010";  // Manual test input
//
//    char firstword[8];  //the largest operation word is "oneshot" + 1 (\0)
//    char restofwords[BUFFER_SIZE - 8];  // allows up to 119 characters for words (for serial function) + \0
//
//    // TODO
//    // Need to implement logic to write serial outputted string into buffer
//    // Probably something as simple as memcpy() from wherever its stored into buffer
//    // Remember to clear buffer, firstword and restofwords after it has been used
//
//
//    int i=0;
//    while(buffer[i] != ' ') {
//        firstword[i] = buffer[i];
//        i++;
//    }
//
//    firstword[i] = '\0';
//    restofwords[BUFFER_SIZE - 8 - 1] = '\0';
//    strcpy(restofwords, buffer + i + 1);
//
//    if(strcmp(firstword, "led") == 0) {
//        uint8_t led_pattern = led_case(restofwords);
//        enable_clocks();                         // enable the clocks
//        initialise_board();                      // initialise the boards
//        set_led_state(led_pattern);
//
//    }
//
//    for (volatile int j = 0; j < 1000000; j++);

	for (;;) {

	}

}

