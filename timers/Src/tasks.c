#include "stm32f303xc.h"
#include <stdint.h>
#include <stddef.h>  // for NULL checking
#include <tasks.h>


static void (*timer_callback)(void) = NULL;   // Static = private to this file
static uint32_t timer_interval_ticks = 0;      // Also private
static uint8_t oneshot_enabled = 0;  // 0 = normal mode, 1 = one-shot mode



void Timer_Init(uint32_t interval_ms, void (*callback)(void)) {

	timer_interval_ticks = 10 * interval_ms;
	timer_callback = callback;

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	// Enable timer clock

	TIM2->PSC = 799;         // 8MHz / 800 = 10kHz
	TIM2->ARR = 0xffffffff;
	TIM2->CNT = 0;

	TIM2->EGR |= TIM_EGR_UG;  // <-- Force update event to apply prescaler NOW!


	TIM2->CR1 |= TIM_CR1_CEN;  // Start timer


}



// Function to call regularly (polling)
void Timer_Update(void) {

	if(TIM2->CNT >= timer_interval_ticks) {
		TIM2->CNT = 0;

		if (timer_callback != NULL) {
		    timer_callback();
		}

		if (oneshot_enabled) {
			oneshot_enabled = 0;
			timer_callback = NULL;
		}
	}
}

void Timer_SetPeriod(uint32_t new_interval_ms) {
	timer_interval_ticks = new_interval_ms * 10;
	TIM2->CNT = 0;
}


uint32_t Timer_GetPeriod(void) {

	return timer_interval_ticks / 10;	// return in ms
}

void Timer_OneShot(uint32_t delay_ms, void (*callback)(void)) {
	timer_interval_ticks = 10 * delay_ms;
	timer_callback = callback;

    oneshot_enabled = 1;  // Enable one-shot mode

    TIM2->CNT = 0;        // Reset timer counter

    NVIC_EnableIRQ();


}


