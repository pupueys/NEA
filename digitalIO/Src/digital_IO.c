#include "digital_io.h"
#include "stm32f303xc.h"
#include "set_get_leds.h"
#include "timer.h"

// part 1 This is the independent module that handles the encapsulates the digital I/O interface

// store a pointer to the function that is called when a button is pressed
// set a default value of NULL so that it won't be called until the
// function pointer is defined
void (*on_button_press)() = 0x00;

// enable the clocks for desired peripherals (GPIOA, C and E)
void enable_clocks() {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOEEN;

	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;           // enable clock for timer 3
}


// initialise the discovery board I/O (just outputs: inputs are selected by default)
void initialise_board() {
	// get a pointer to the second half word of the MODER register (for outputs pe8-15)
	uint16_t *led_output_registers = ((uint16_t *)&(GPIOE->MODER)) + 1;
	*led_output_registers = 0x5555;
}


void enable_interrupt() {
	// Disable the interrupts while messing around with the settings
	//  otherwise can lead to strange behaviour
	__disable_irq();

	// Enable the system configuration controller (SYSCFG in RCC)
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// External Interrupts details on large manual page 294)
	// PA0 is on interrupt EXTI0 large manual - page 250
	// EXTI0 in  SYSCFG_EXTICR1 needs to be 0x00 (SYSCFG_EXTICR1_EXTI0_PA)
	SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI0_PA;

	//  Select EXTI0 interrupt on rising edge
	EXTI->RTSR |= EXTI_RTSR_TR0; // rising edge of EXTI line 0 (includes PA0)

	// set the interrupt from EXTI line 0 as 'not masked' - as in, enable it.
	EXTI->IMR |= EXTI_IMR_MR0;

	// Tell the NVIC module that EXTI0 interrupts should be handled
	NVIC_SetPriority(EXTI0_IRQn, 1);  // Set Priority
	NVIC_EnableIRQ(EXTI0_IRQn);

	// Re-enable all interrupts (now that we are finished)
	__enable_irq();
}


void EXTI0_IRQHandler(void)
{
    button_pressed_flag = 1;  // Raise the button presses flag
	// run the button press handler (make sure it is not null first !)
	if (on_button_press != 0x00) {
		on_button_press();
	}

	// reset the interrupt (so it doesn't keep firing until the next trigger)
	EXTI->PR |= EXTI_PR_PR0;
}


// Accept callback function in digital_io
void digital_io(callback chase_led) {
	on_button_press = chase_led;             // on_button_press is now pointing at the callback function
	enable_clocks();                         // enable the clocks
	initialise_board();                      // initialise the boards
	enable_timer_interrupt();                // enable the interrupt for timers
	enable_interrupt();                      // enable the button press interrupt
}


void chase_led() {
    if (led_change_flag==1) {
        led_state <<= 1;                         // Left shift the LEDs so the next one turns on
        if (led_state == 0) {
            led_state = 1;                       // Reset to the first LED if all LEDs are off
        }
        GPIOE->ODR = (GPIOE->ODR & 0x00FF) | (led_state << 8);  // Update the LED pattern to the ODR
        led_change_flag = 0;                                    // Reset the LED change flag to 0 so that the process can repeat again
    }
}
