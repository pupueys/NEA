#ifndef TIMER_H_
#define TIMER_H_

//Define all headers

void enable_button_interrupt() ;
void TIM3_IRQHandler(void) ;

extern volatile uint8_t led_change_flag;

extern volatile uint8_t button_pressed_flag;

#endif
