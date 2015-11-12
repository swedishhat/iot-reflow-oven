/*** oven_control.c ***/

#include "globals.h"

/*** Oven Control Variables ***/
volatile uint8_t    _percent = 0;
volatile uint8_t    _counter_t2 = 0;

/*** Oven Power Remap LUT ***/
/* Lookup table to more accurately represent power output percentage.
 * Compensates for curvature of sinusoid at peaks and troughs. Values are
 * stored in FLASH because why waste the space anywhere else? Check out this
 * doc for more info on PROGMEM attribute:
 *
 * http://www.github.com/abcminiuser/avr-tutorials/blob/master/Progmem/Output/Progmem.pdf?raw=true */
static const PROGMEM uint8_t powerLUT[99] = 
{
    6, 9, 11,13,14,16,17,18,19,21,
    22,23,23,24,26,26,27,28,29,29,
    31,31,32,33,33,34,35,36,36,37,
    38,38,39,39,41,41,42,42,43,43,
    44,45,46,46,47,47,48,49,49,50,
    51,51,52,53,53,54,54,55,56,57,
    57,58,58,59,59,61,61,62,62,63,
    64,64,65,66,67,67,68,69,69,71,
    71,72,73,74,74,76,77,77,78,79,
    81,82,83,84,86,87,89,91,94  
};

/*** Zero-Crossing External ISR ***/
ISR(INT0_vect)
{
    /* 0 is an off switch. round up or down a percentage that strays into the
     * end-zone where we have a margin wide enough to cater for the minimum
     * pulse width and the delay in the zero crossing firing */
    if(_percent == 0)
    {
        OVEN_OFF();
        return;
    }
    // either user asked for 100 or calc rounds up to 100
    else if(_percent == 100 || _counter_t2 == 0)
    {
        OVEN_ON(); 
    }
    // Comparison to a constant is pretty fast
    else if(_counter_t2 > TICKS_PER_HALF_CYCLE - TRIAC_PULSE_TICKS - MARGIN_TICKS)
    {
        // Also a constant comparison so also pretty fast
        if(_counter_t2 > (TICKS_PER_HALF_CYCLE - (TRIAC_PULSE_TICKS - MARGIN_TICKS / 2)))
        {
            // round half up to completely off
            OVEN_OFF();
            return;
        }
        else 
            _counter_t2 = TICKS_PER_HALF_CYCLE - TRIAC_PULSE_TICKS - MARGIN_TICKS;
    }

    // Counter is acceptable, or has been rounded down to be acceptable
    OCR2A = _counter_t2;
    TCNT2 = 0;
    TCCR2B = (1 << CS20) | (1 << CS21) | (1 << CS22);    // start timer: 8MHz/1024 = 128uS/tick
}

/*** Timer2 Comparison Value A ISR ***/
ISR(TIMER2_COMPA_vect)
{
    // Turn on oven, hold it active for a min latching time before switching it off
    OVEN_ON();

    // The overflow interrupt will fire when the minimum pulse width is reached
    TCNT2 = 256 - TRIAC_PULSE_TICKS;
}

/*** Timer2 Overflow ISR ***/
ISR(TIMER2_OVF_vect)
{
    // Turn off oven
    OVEN_OFF();

    // turn off the timer. the zero-crossing handler will restart it
    TCCR2B = 0;
}

/*** Oven Duty Cycle Function ***/
void oven_setDutyCycle(uint8_t percent)
{

    uint16_t newCounter;
    
    // percentages between 1 and 99 inclusive use the lookup table to translate a linear
    // demand for power to a position on the phase angle axis
    if(percent > 0 && percent < 100)
        _percent = pgm_read_byte(&powerLUT[percent - 1]);

    // calculate the new counter value
    newCounter = ((TICKS_PER_HALF_CYCLE - MARGIN_TICKS - TRIAC_PULSE_TICKS) * (100 - percent)) / 100;

    // set the new state with interrupts off because 16-bit writes are not atomic
    cli();
    _counter_t2 = newCounter;
    _percent = percent;
    sei();
}

/*** Configure Oven GPIO and Begin Timer2 ***/
void oven_setup(void)
{
    // Setup inputs and outputs
    CONFIG_AS_OUTPUT(TRIAC_EN);
    CONFIG_AS_INPUT(ZERO_CROSS);

    // Initial values for outputs
    SET_LOW(TRIAC_EN);
    
    // Configure external interrupt registers (Eventually move into macros.h)
    EICRA |= (1 << ISC01);      // Falling edge of INT0 generates an IRQ
    EIMSK |= (1 << INT0);       // Enable INT0 external interrupt mask

    // Enable Timer/Counter2 and trigger interrupts on both overflows & when
    // it equals OC2A    
    TIMSK2 |= (1 << OCIE2A) | (1 << TOIE2);
}

