#include "stm32f303xc.h"
#include <stdint.h>
#include <stddef.h>  // for NULL checking
#include <tasks.h>


static void (*timer_callback)(void) = NULL;   // Static = private to this file
static uint8_t oneshot_enabled = 0;  // 0 = normal mode, 1 = one-shot mode



void Timer_Init(uint32_t interval_ms, void (*callback)(void)) {

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	// Enable timer clock

	TIM2->PSC = 799;         // 8MHz / 800 = 10kHz
	TIM2->ARR = (10* interval_ms) - 1;
	TIM2->CNT = 0;

    TIM2->DIER |= TIM_DIER_UIE;           // Enable update interrupt
    NVIC_EnableIRQ(TIM2_IRQn);            // Enable TIM2 interrupt at CPU/NVIC level

    timer_callback = callback;
    oneshot_enabled = 0;                  // Default = periodic mode

	TIM2->CR1 |= TIM_CR1_CEN;  // Start timer


}



void Timer_SetPeriod(uint32_t new_interval_ms) {
    TIM2->ARR = (10 * new_interval_ms) - 1;  // Update ARR
	TIM2->CNT = 0;
}


uint32_t Timer_GetPeriod(void) {

    return (TIM2->ARR + 1) / 10;  // Return in ms
}

void Timer_OneShot(uint32_t delay_ms, void (*callback)(void)) {
	TIM2->ARR = (10 * delay_ms) - 1;
	TIM2->CNT = 0;

	timer_callback = callback;
    oneshot_enabled = 1;  // Enable one-shot mode

}

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {       // Check if update interrupt flag is set
        TIM2->SR &= ~TIM_SR_UIF;        // Clear UIF flag immediately

        if (timer_callback != NULL) {  // If a callback is registered
            timer_callback();          // Call the user function
        }

        if (oneshot_enabled) {         // If it’s a one-shot timer
            timer_callback = NULL;     // Disable the callback
            TIM2->DIER &= ~TIM_DIER_UIE;  // Disable future timer interrupts
        }
    }
}


