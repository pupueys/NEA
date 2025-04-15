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
#### _1.4.1 Digital_io.c / Digital_io.h_
This is the independent module that encapsulates the digital I/O interface  
(button and LED) that was asked in task (a). This module sets up GPIO pins, enables the button interrupt, and allows passing of a callback function to respond to button presses.
- `void  digital_io(callback  chase_led)` - This function is the entry point to the digital_io.c module. It takes the address of the callback function as the input and then uses the function pointer on_button_press to point at the address of the callback function that was passed in. Just changing the input address to a different function will change the function that is called by the button press. This function then calls the `enable_clocks()`,  `initialise_board()`, `enable_timer_interrupt()`, and `enable_interrupt()` functions.

- `void  enable_clocks()` - Enables the clock for GPIOA, GPIOC, GPIOE for Button and LEDs and tim3 for timer functions.
- `void  initialise_board()` - initialise the discovery board I/O pins.
- `void  enable_interrupt()`-  This enables the user input button `(PA0)`  as a interrupt. Everytime the user button is pressed interrupt handler `EXTI0_IRQHandler` is called.
- `void  EXTI0_IRQHandler(void)`- This function first sets the `button_pressed_flag` to high indicating a button press has occured. Then it checks `on_button_press` which is a function pointer that points to the callback function `chase_led` is not null. Then it calls the on_button_press function and since this just points at chase_led function, chase_led function is now runned.
- `void  chase_led()`- Chase_led function first checks if the led_change_flag is high and then it left shifts the led pattern stored in `led_state` by 1 bit essentially moving the leds. It also checks if the led_state is 0 so that it can reset the pattern back to default. Then the new led state stored in led_state is stored back to ODR of the GPIOE using the following code, `GPIOE->ODR  =  (GPIOE->ODR  &  0x00FF)  |  (led_state  <<  8);`. Finally led_change_flag is set to 0 so that the leds only turn on again when there is a change in timer.

In the header file for this module `void  digital_io(callback);` and `void  chase_led(void);` are defined. Also the type of callback which is a function pointer that takes no input and gives no output is defined. This is used as an input type for the digital_io function. The code for the same is given here `typedef  void  (*callback)(void);`

#### _1.4.2 Set_get_leds.c / Set_get_leds.h_
This module incorporates the LED state into the module such that the only way to access it is through the header file using `get_led_state` and `set_led_state` functions.

This module first uses `static volatile uint8_t led_state;` to define led_state as a uint8_t static variable. led_state set to static so that it is only accessible in this file. This led_state stores the state of the LEDs and is used to manipulate the LED pattern in other modules.

- ` uint8_t  get_led_state(void)`- This function is used to fetch the current state of the LEDs from memory. The output of the function is a uint8_t value and there is no input for this function. This function just returns the current patteren stored in the `led_state` which represents the pattern in which the LEDs are on.

- `void  set_led_state(uint8_t  state)`- This function takes the input of type uint8_t which represents the pattern the user enters. This user input is then assigned to the `led_state` which over writes the current state of the LEDs to the user sent pattern. This pattern is then stored back to the ODR of the GPIOE.

The header file for this module defines the functions mentioned above and additionally also defines the led_state as an extern variable. `static  volatile  uint8_t led_state ;` This is done so that led_state can be used in other functions like digital_io.c

#### _1.4.3 Timer.c / Timer.h_
Timer module is used to restrict the speed at which the LEDs can change. In this module we use Interrupts rather than polling to restrict the speed. It sets a flag after a certain period of time using interrupts which is then checked in the callback function before changing the LED pattern.

`volatile  uint8_t led_change_flag =  0;` and `volatile  uint8_t button_pressed_flag =  0;` is used to define 2 variables which are used as flags to decide if the led is ready to be changed and if the button is pressed respectively. Both of them are set to 0 during initialisation.

- `void  enable_timer_interrupt()`- This sets the timer 3 to generate periodic interrupts. When the value of `TIM3->ARR  =  1;` which is the reload register is changed to something different like 1000, the interrupt handler is called every 1 second as the timer is set to increment every 1 ms using the following code, `TIM3->PSC  =  16000  -  1;`. The reload is currently set at 1 so that the delay is not visible to human eyes. During demonstration the value should be changed to see the difference. So currently when ever the timer is reloaded the interrupt handler `TIM3_IRQn` is called which calls the function `void  TIM3_IRQHandler(void) `.

- `void  TIM3_IRQHandler(void)`-  This is the interrupt handler function which is called when the interrupt occurs. This function first checks if the button is pressed using the `button_pressed_flag` and then sets the `led_change_flag` to high so that this can be used in the callback function in digital_io.c to change the led pattern. By using these flags we are ensuring that the led pattern ONLY changes when the timer interrupt decided by the ARR is called ensuring there is a rate at which the LEDs can change.

The header file for this module just defines the above mentioned functions and the above mentioned variables as extern so that it can be used in other modules as well. 

#### _1.4.4 Main.c_
This is the entry point of this excercise where the `digital_io` function is called and an input of the address of callback function using `&chase_led` is sent. The digital_io then later will initialise and run the other important functions of the excercise. The main function also sets the initial LED pattern to 0b00000001 such that one led is turned on during initialisation. This is done using the following code, `set_led_state(0b0000011);`. Then the code loops for ever waiting for interuupts to oocur.


### 1.5 Testing
Individual Module testing for each module is given bellow,

#### _1.5.1  Digital_io.c_
- Pressing the button repeatedly and check if it causes LEDs to shift correctly.
- Check if LEDs respond only once per press.
- Try using a different address as the input to the digital_io function to see if changing the callback function changes the function performed while button is pressed.
- Ensure callback is triggered only when a valid function pointer is passed.
- Try using a different left shift value to see if the `chase_led` function is working as expected.
- Mannualy set the led_change_flag to 0 and make sure the LEDs are not shifting when the button is pressed.

#### _1.5.2  Set_get_leds.c_
- After few button presses try using the `get_led_state` to fetch the current state of the LEDs.
- Try different inputs with the `set_led_state` function and make sure the LEDs turn on as expected.

#### _1.5.3  timer.c_
- Try using a different pre scaller value to see if the time between pattern change is changing accordingly. 
- Do the same with the ARR to change the reload value to a different number to change the rate at which the interrupt is trigerred.
- In the interrupt handler function mannualy set the button pressed flag to 0 and make sure the led_change_flag does not go up even when the timer interrupt is running.


**Overall testing**
- Run the program and check when the button is pressed the LED pattern change as expected.
- Keep pressing the button continuously and but make sure LEDs change only after a certain intervel of time regardless of the button press.
- Use `set_led_state` to set a random pattern of LEDs and make sure it is reflected at the start of the program.








## Serial Interface
For this section, `serial.c` and the corresponding `serial.h` was used as a baseline. 
