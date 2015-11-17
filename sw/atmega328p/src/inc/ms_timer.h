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
void msTimer_setup(void);

/*** Time Delta Function ***/
uint32_t msTimer_deltaT(uint32_t start);

/*** Timeout Detection Function ***/
bool msTimer_hasTimedOut(uint32_t start,uint32_t timeout);

#endif
