#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define BUFFER_SIZE 128

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

uint32_t timers_case(char* restofwords) {
    uint32_t value = 0;

    for(int i=0; restofwords[i] != '\0'; i++) {
        value = value * 10 + (restofwords[i] - '0');
    }

    return value;

}



int main() {

    char buffer[BUFFER_SIZE];   // total string length is 127 characters + \0
    char firstword[8];  //the largest operation word is "oneshot" + 1 (\0)
    char restofwords[BUFFER_SIZE - 8];  // allows up to 119 characters for words (for serial function) + \0

    // TODO
    // Need to implement logic to write serial outputted string into buffer
    // Probably something as simple as memcpy() from wherever its stored into buffer
    // Remember to clear buffer, firstword and restofwords after it has been used


    int i=0;
    while(buffer[i] != ' ') {
        firstword[i] = buffer[i];
        i++;
    }

    firstword[i] = '\0';
    restofwords[BUFFER_SIZE - 8 - 1] = '\0';

    if(strcmp(firstword, "led") == 0) {
        uint8_t led_pattern = led_case(restofwords);
        //TODO

    }

    if(strcmp(firstword, "serial") == 0) {
        //TODO (just pass restofwords)
    }

    if(strcmp(firstword, "oneshot") == 0) {
        uint32_t oneshot_delay = timers_case(restofwords);
        Timer_OneShot(oneshot_delay, led_oneshot_callback); //requires uint32
    }

    if(strcmp(firstword, "timer") == 0) {
        uint32_t timer_delay = timers_case(restofwords);    //requires uint32
        Timer_Init(timer_delay, led_callback);
    }

    
}
