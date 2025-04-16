#ifndef TIMERS_H
#define TIMERS_H
#include <stdint.h>


// Part a functions
void Timer_Init(uint32_t interval_ms, void (*callback)(void));
void Timer_Start(void);

// Part b functions
void Timer_SetPeriod(uint32_t new_interval_ms);
uint32_t Timer_GetPeriod(void);

// Part c function
void Timer_OneShot(uint32_t delay_ms, void (*callback)(void));


// Interrupt handler
void TIM2_IRQHandler(void);


#endif
