#include "digital_io.h"
#include "stm32f303xc.h"

//timer for part 4
volatile uint8_t led_change_flag = 0;
volatile uint8_t button_pressed_flag = 0;

void enable_button_interrupt() {
	// Disable the interrupts while messing around with the settings
	//  otherwise can lead to strange behaviour
	__disable_irq();
    // Configure Timer 3 for periodic interrupt
    TIM3->PSC = 16000 - 1; // Prescaler for 1ms ticks (assuming 16 MHz clock)
    TIM3->ARR = 1000 - 1; // Auto-reload for 500ms interval (every 500ms)
    TIM3->DIER |= TIM_DIER_UIE; // Enable update interrupt
    TIM3->CR1 |= TIM_CR1_CEN; // Start the timer
	NVIC_SetPriority(TIM3_IRQn, 2);  // Set Priority

    NVIC_EnableIRQ(TIM3_IRQn);  // Enable Timer 3 interrupt

	// Re-enable all interrupts (now that we are finished)
	__enable_irq();
}

void TIM3_IRQHandler(void) {
    if (TIM3->SR & TIM_SR_UIF) {
        TIM3->SR &= ~TIM_SR_UIF;

        if (button_pressed_flag) {
            led_change_flag = 1;
            button_pressed_flag = 0;  // Clear the flag after use
        }
    }
}
