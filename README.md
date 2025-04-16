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
For this section, `serial.c` and the corresponding `serial.h` from W06-modular-example was used as a baseline. 

### Set-up
To set-up the STM for serial interfacing with a compter, use a Mini-USB to USB-A cable, connecting Mini-USB connector to the ST-Link port on the STM32F3 and connecting the USB-A connector to the computer. To interface with the computer, use PuTTY if using Windows and Cutecom if using MacOS. Ensure that the terminal is communicating via the correct COM serial device. On Windows, this can be done by checking Device Manager and looking for the COM port the STM32 is connected to. Furthermore, ensure that the baud rate matches that of the STM32.

The baud rate for the STM32 is set in `main.c` as the second argument passed to `SerialInitialise`, with the syntax `BAUD_[baud rate]`. There are four baud rate options available to the user: 9600, 19200, 38400, 57600, 115200. For example, if the user wants to set the baud to 19200, the second argument of `SerialInitialise` should read `BAUD_19200`. Arguments of `baud_19200` or `BAUD_420` are invalid and must not be used.

### Usage
To transmit a message to the board, use a client such as PuTTY or Cutecom, ensuring the correct port and baud rate are set. Simply type the message into the terminal and complete it by appending `TERMINATOR`. If `TERMINATOR` is `'\0'`, input Ctrl+Shift+2 into the terminal (appends the terminating character).

Similarly, to receive messages from the board, use a client such as PuTTY or Cutecom with the correct baud rate set. 

### Receive Functionality
The program utilises USART1 on the STM32F3 to communicate with a computer via a serial terminal. There are two modes for receiving; polling and via interrupts. To enable the polling mode, set `polling_flag = true` in `main.c`. This enables the `SerialReceiveString` function, which is the polling receive function for the STM32. 

Messages are received until a user defined terminating character is received, which is `\0` by default (the null character). To set this terminating character, change the definition of `TERMINATOR` in `main.c`, i.e. `#define TERMINATOR '\0'` such that `TERMINATOR` is the desired terminating character, e.g. if the terminating character is to be set as '!', the line should read `#define TERMINATOR '!'`. 

Essentially, the polling mode "waits" for a character to be received into the recieve data register (RDR), i.e. undergoes an infinite `while` loop until data is received. Once data is received, it is stored into the global receive buffer `rx_buffer` and checks for the terminating character. Note that the size of `rx_buffer` is defind by `BUFFER_SIZE` in `main.c`  which can be changed. If the terminating character is reached, it ends the receiving loop and returns to a callback function. Otherwise, the program remains in the programming loop. 

To deactivate the polling mode, set `polling_flag = false` in `main.c`. This disables the `SerialReceiveString` function and instead now the program relies on interrupts to receive data. In this case, the double buffers within the `SerialPort` struct are used, switching modes every time a full string is received. More specifically, there are two buffers in the `SerialPort` struct, denoted `buffer` and `second_buffer`. Initially, data is read into `buffer` until a terminating character is received. Once the terminating character is received, the two buffers are "swapped" and now the `second_buffer` is used to receive any new characters. In the meantime, the data in `buffer` can be used for other routines such as parsing, transmission, etc. 

### The Double Buffers
As mentioned, double buffers are used to allow multiple actions to occur at once, e.g. receiving and manipluating two different buffers. These are stored as elements of the `SerialPort` struct, and have a user-defined buffer size which is mediated by `malloc` to initialise. 
To define the buffer size, change the value of `BUFFER_SIZE` in `main.c`.

### Transmit Functionality
Similarly to Receive Functionality, the program uses USART1 to communicate via serial. There are also two modes; transmission via polling and transmission via interrupts.

For the polling mode, the `SerialOutputString` function in `serial.c` outputs a character until a there is nothing to transmit, outputting whenever there is data ready to be transmitted in the transmit data register (TDR). The polling logic is exactly the same as in Receive Functionality.

For the interrupt mode, the relevant functions are `tx_enable`, `tx_string` and `tx_function`, which are all found in `interrupts.c`. Unlike the Receive Functionality, however, transmission does not run all the time, and a flag must be set to enable/disable the transmission interrupts.
- `tx_enable` initialises and enables the transmission interrupts when it is flagged.
- `tx_string` initialises the transmission pointer to the beginning of the buffer, then calling `tx_function` to transmit the data.
- `tx_function` sends the data to the TDR to be communicated via serial until a terminating character is reached, at which point transmission is disabled and the tranmission pointer is reset.  

The transmission pointer is another element of the `SerialPort` struct, and is essentially a pointer that is used to point at a buffer that is to be transmitted, iterating through the buffer until specified otherwise. 

### Testing
- In the polling mode, to view where the final string is stored, place a breakpoint after the `do while` loop in the `SerialReceiveString` function in `serial.c`. 
- In the interrupt mode, to view where the final string is stored/to view what is stored in the double buffers when receiving is done, 
```c
		if (serial_port->Buffer[serial_port->Count - 1 ] == TERMINATOR) {

			volatile uint8_t* temp_pt = serial_port->Buffer;
			serial_port->Buffer = serial_port->SecondBuffer;
			serial_port->SecondBuffer = temp_pt;

			// callback with the first buffer since the first buffer is finished reading
			serial_port->callback(temp_pt, serial_port->Count);
			serial_port->Count = 0;
		}
```
place a breakpoint directly before the line `serial_port->callback(temp_pt, serial_port->Count);` in `rx_function`.

## Integration
All the modules culminate into the integration file, which showcases how the modules can be used as one cohesive program. 
### Usage

The user inputs one of the following commands into the terminal:
- led {insert 8-bit binary LED bitmask} (e.g. `led 01010101`)
- serial {insert string here} (e.g. `serial Hello, World!`)
- oneshot {insert time in ms} (e.g. `oneshot 5000`)
- timer {insert time in ms} (e.g. `timer 1000`)

These inputs must all follow the syntax provided, otherwise no output will occur. The inputs must be appended with the null terminating character `\0`. 

### Parsing
The program is quite particular about the input argument. As previously stated, the arguments have to follow the exact syntax provided, or else there will be no output. This is due to the parsing utilising `strncmp` to check whether the first argument of the input is valid, i.e. if it is `led`, `serial`, `timer` or `oneshot`. It particularly checks if there is a space after the first argument. If there is no space, then the program will have no output.

### Functionality
**LED Mode**: The LEDs will display according to the set bitmask, starting from the North-West LED.

**Serial Mode**: The STM32 will transmit the input string back to the terminal.

**Oneshot Mode**: The LEDs on the STM32 will turn on once the oneshot timer is complete, e.g. if the argument is 5000, the lights will turn on after 5000ms.

**Timer Mode**: The LEDs on the STM32 will flash according to the time set by the argument, e.g. if the argument is 1000, the LEDs will flash every 1000ms.
