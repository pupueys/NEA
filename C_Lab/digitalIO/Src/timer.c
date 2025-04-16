#include "digital_io.h"
#include "stm32f303xc.h"

//timer for part 4
volatile uint8_t led_change_flag = 0;
volatile uint8_t button_pressed_flag = 0;

void enable_timer_interrupt() {
	// Disable the interrupts while messing around with the settings
	//  otherwise can lead to strange behaviour
	__disable_irq();
    // Configure Timer 3 for periodic interrupt
    TIM3->PSC = 16000 - 1;              // Timer increments every 1 ms
    //TIM3->ARR = 1000    ;             // uncomment this to set the reload time to 1 sec
    TIM3->ARR = 1;                      // The reload by default is set at 1 so that is not noticible
    TIM3->DIER |= TIM_DIER_UIE;         // Enable update interrupt
    TIM3->CR1 |= TIM_CR1_CEN;           // Start the timer
	NVIC_SetPriority(TIM3_IRQn, 2);     // Set Priority as no 2

    NVIC_EnableIRQ(TIM3_IRQn);          // Enable Timer 3 interrupt

	// Re-enable all interrupts
	__enable_irq();
}

void TIM3_IRQHandler(void) {
    if (TIM3->SR & TIM_SR_UIF) {
        TIM3->SR &= ~TIM_SR_UIF;

        if (button_pressed_flag) {
            led_change_flag = 1;        // Set led change flag to high so that when button is pressed the LED pattern can change
            button_pressed_flag = 0;    // Clear the button pressed flag so that it only switches during next press
        }
    }
}
