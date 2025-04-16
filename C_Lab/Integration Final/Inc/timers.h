#ifndef TIMERS_H
#define TIMERS_H
#include <stdint.h>



// parsing function
void Timer_Init(uint32_t interval_ms, void (*callback)(void));

//Led callback
void led_callback(void);

//Oneshot callback
void led_oneshot_callback(void);

//stop timer
void stop_timer(void);

void Timer_Start(void);



// Part a functions
void Timer_Init(uint32_t interval_ms, void (*callback)(void));
void Timer_Update(void);

// Part b functions
void Timer_SetPeriod(uint32_t new_interval_ms);
uint32_t Timer_GetPeriod(void);

// Part c functions
void Timer_OneShot(uint32_t delay_ms, void (*callback)(void));


#endif // MY_TIMER_H
