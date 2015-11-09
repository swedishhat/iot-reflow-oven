#ifndef _MS_TIMER_H_
#define _MS_TIMER_H_

extern volatile uint32_t  _ms_counter;
extern volatile uint8_t  _ms_subCounter;

uint32_t millis(void);
void delay(uint32_t waitfor);

#endif
