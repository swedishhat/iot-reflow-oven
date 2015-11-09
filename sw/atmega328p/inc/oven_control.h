// oven_control.h
#ifndef _OVEN_CONTROL_H_
#define _OVEN_CONTROL_H_


// TRIAC timer runs at 128uS per 'tick' of TIMER2. The triac_pulse_ticks
// variable defines how many ticks TRIAC gate will be activated for.
// 4 ticks = 512uS
#define TICKS_PER_HALF_CYCLE    65
#define TRIAC_PULSE_TICKS       4
#define MARGIN_TICKS            8

// Specify the I/O pins to the AC waveform dimmer. Because of the macros right
// below this, these pins need to be in PORTD for the time being.
#define PIN_ZX_IN               PD2     // Pin 2 (Interrupt)
#define PIN_TA_OUT              PD3     // Pin 3 (PWM output)

// Macros to turn the oven on and off. Not generic (only applies to PORTD)
// Would be nice in the future to have it know  PORT(x) based on P(x)(#) input
#define OVEN_OFF                PORTD &= ~(1 << PIN_TA_OUT)
#define OVEN_ON                 PORTD |= (1 << PIN_TA_OUT); 

// Control "dimmness" percentage and counter for delay

/* Lookup table to more accurately represent power output percentage.
 * Compensates for curvature of sinusoid at peaks and troughs. Values are
 * stored in FLASH because why waste the space anywhere else? Check out this
 * doc for more info on PROGMEM attribute:
 *
 * http://www.github.com/abcminiuser/avr-tutorials/blob/master/Progmem/Output/Progmem.pdf?raw=true */
extern volatile uint8_t _percent;
extern volatile uint8_t _counter_t2;

void setDutyCycle(uint8_t percent);

#endif
