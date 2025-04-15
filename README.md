# MTRX2700 C Lab
*by Nadeen, Ethan and Anikan
# Project Overview
The MTRX2700 C Lab is an application of the knowledge gained from MTRX2700 - Mechatronics 2. The various submodules in this repo demonstrate the functions of the STM32F3 microcontroller including GPIO, UART, hardware timers, etc. These submodules were integrated together to show all of these features working at once in a simple program. . All the tasks were coded using C programming language. This repository contains all the code we wrote, alongside the minutes of the meetings we had throughout the project. This readme should guide one through the process of using, testing, and understanding all code in our codebase. Our code was written with a focus on modularity, in order for integration to be easier at the end.

Everything was written for and intended for the STM32F3 Discovery Board which utilises the Cortex-M3 ARM processor, and as such, everything is intended for only such hardware.

# Modules and Submodules
## 1.Digital I/O module to interface LEDs and Button
### 1.1 Summary
---
This project First modularises the given code in week 5 to implement a clean software design that encapsulates digital I/O into reusable modules, allowing LED control and callback function for a button press on initialisation. It also has a module which allows the user to set the led in a certain pattern or get the current pattern using set/get function. Later timer interrupts are used to restrict the speed at which the LEDs change.

### 1.2 Usage 
---
- Clone the repo into STM32CubeIDE and run the main file in the digitalIO folder (inside SRC).
- Enter the pattern of LEDs you want to start with in the `set_led_state` function of main file. Use 0b before entering the pattern if you want to enter the pattern in binary.
- Run the code using the run button in the STM32 interface.
- When the user button is pressed in the microcontroller the led pattern change to next possition with a rate limited by the timer.
- The rate at which the LEDs change can be modified by changing the `TIM3->ARR  =  1; ` to a different value.

### 1.3 Valid Input
---
- Blue User button in the STM32 board acts as the user input for changing the pattern of the LEDs
- `uint8_t` starting with `0b` (if you want to enter the pattern in binary) is the user input in `set_led_state` function which sets the initial pattern of the LEDs.
- Interger is the input in `TIM3->ARR  =  1` to set the reload value which changes the      speed at which the LED pattern changes. 
---

###  1.4 Functions and Modularity 
#### 1.4.1 Digital_io.c / Digital_io.h
This module sets up GPIO pins, enables the button interrupt, and allows passing of a callback function to respond to button presses.
- `void  digital_io(callback  chase_led)` - This function is the entry point to the digital_io.c module. It takes the address of the callback function as the input and then uses the function pointer on_button_press to point at the address of the callback function that was passed in. This function then calls the `enable_clocks()`,  `initialise_board()`, `enable_timer_interrupt()`, and `enable_interrupt()` functions.

- `void  enable_clocks()` - Enables the clock for GPIOA, GPIOC, GPIOE for Button and LEDs and tim3 for timer functions.
- `void  initialise_board()` - initialise the discovery board I/O pins.
- `void  enable_interrupt()`-  This enables the user input button `(PA0)`  as a interrupt. Everytime the user button is pressed interrupt handler `EXTI0_IRQHandler` is called.
- `void  EXTI0_IRQHandler(void)`- This function first sets the `button_pressed_flag` to high indicating a button press has occured. Then it checks `on_button_press` which is a function pointer that points to the callback function `chase_led` is not null. Then it calls the on_button_press function and since this just points at chase_led function, chase_led function is now runned.
- `void  chase_led()`- Chase_led function first checks if the led_change_flag is high and then it left shifts the led pattern stored in `led_state` by 1 bit essentially moving the leds. It also checks if the led_state is 0 so that it can reset the pattern back to default. Then the new led state stored in led_state is stored back to ODR of the GPIOE using the following code, `GPIOE->ODR  =  (GPIOE->ODR  &  0x00FF)  |  (led_state  <<  8);`. Finally led_change_flag is set to 0 so that the leds only turn on again when there is a change in timer.
## Serial Interface
For this section, `serial.c` and the corresponding `serial.h` was used as a baseline. 
