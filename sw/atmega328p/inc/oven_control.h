// oven_control.h
#ifndef _OVEN_CONTROL_H_
#define _OVEN_CONTROL_H_

/*** I/O pins ***/ 
// Zero Cross Input
#define ZERO_CROSS              PD2     
#define ZERO_CROSS_PORT         PORTD
#define ZERO_CROSS_DDR          DDRD
#define ZERO_CROSS_PIN          PIND 

// TRIAC Enable Output
#define TRIAC_EN                PD3 
#define TRIAC_EN_PORT           PORTD
#define TRIAC_EN_DDR            DDRD
#define TRIAC_EN_PIN            PIND 

/*** TRIAC Timing Definitions ***/
/* 
 * TRIAC timer runs at 128uS per 'tick' of TIMER2. The triac_pulse_ticks
 * variable defines how many ticks TRIAC gate will be activated for.
 * 4 ticks = 512uS
 */
#define TICKS_PER_HALF_CYCLE    65
#define TRIAC_PULSE_TICKS       4
#define MARGIN_TICKS            8

/*** Oven ON/OFF Macros ***/ 
#define OVEN_OFF()              SET_LOW(TRIAC_EN)
#define OVEN_ON()               SET_HIGH(TRIAC_EN) 

/*** Oven Control Variables ***/
extern volatile uint8_t _percent;
extern volatile uint8_t _counter_t2;

void oven_setDutyCycle(uint8_t percent);

void oven_start(void);
#endif
