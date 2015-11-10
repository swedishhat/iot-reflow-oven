// ms_timer.h
#ifndef _MS_TIMER_H_
#define _MS_TIMER_H_

/*** Millisecond Timer Variables ***/
extern volatile uint32_t  _ms_counter;
extern volatile uint8_t  _ms_subCounter;

/*** Millisecond Counter Function ***/
uint32_t msTimer_millis(void);

/*** Millisecond Delay Function ***/
void msTimer_delay(uint32_t waitfor);

/*** Millisecond Timer Initialization Function ***/
void msTimer_start(void);

#endif
