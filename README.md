# MTRX2700 C Lab
*by Nadeen, Ethan and Anikan
# Project Overview
The MTRX2700 C Lab is an application of the knowledge gained from MTRX2700 - Mechatronics 2. The various submodules in this repo demonstrate the functions of the STM32F3 microcontroller including GPIO, UART, hardware timers, etc. These submodules were integrated together to show all of these features working at once in a simple program. . All the tasks were coded using C programming language. This repository contains all the code we wrote, alongside the minutes of the meetings we had throughout the project. This readme should guide one through the process of using, testing, and understanding all code in our codebase. Our code was written with a focus on modularity, in order for integration to be easier at the end.

Everything was written for and intended for the STM32F3 Discovery Board which utilises the Cortex-M3 ARM processor, and as such, everything is intended for only such hardware.

# Modules and Submodules
## 1.Digital I/O module to interface LEDs and Button
### 1.1 Summary
This project First modularises the given code in week 5 to implement a clean software design that encapsulates digital I/O into reusable modules, allowing LED control and callback function for a button press on initialisation. It also has a module which allows the user to set the led in a certain pattern or get the current pattern using set/get function. Later timer interrupts are used to restrict the speed at which the LEDs change.

### 1.2 Usage 
- Clone the repo into STM32CubeIDE and run the main file in the digitalIO folder (inside SRC).
- Enter the pattern of LEDs you want to start with in the `set_led_state` function of main file. Use 0b before entering the pattern if you want to enter the pattern in binary.
- Run the code using the run button in the STM32 interface.
- When the user button is pressed in the microcontroller the led pattern change to next possition with a rate limited by the timer.
- The rate at which the LEDs change can be modified by changing the `TIM3->ARR  =  1; ` to a different value.

### 1.3 Valid Input
- Blue User button in the STM32 board acts as the user input for changing the pattern of the LEDs
- `uint8_t` starting with `0b` (if you want to enter the pattern in binary) is the user input in `set_led_state` function which sets the initial pattern of the LEDs.
- Interger is the input in `TIM3->ARR  =  1` to set the reload value which changes the      speed at which the LED pattern changes. 

###  1.4 Functions and Modularity 
## Serial Interface
For this section, `serial.c` and the corresponding `serial.h` was used as a baseline. 
