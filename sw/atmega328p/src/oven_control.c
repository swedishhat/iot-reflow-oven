#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "oven_control.h"


volatile uint8_t    _percent = 0;
volatile uint8_t    _counter_t2 = 0;

static const PROGMEM uint8_t powerLUT[99] = 
{
        // 0% isn't required (oven is off at zero)
    6,  // 1%
    9,  // 2% etc...
    11,
    13,
    14,
    16,
    17,
    18,
    19,
    21,
    22,
    23,
    23,
    24,
    26,
    26,
    27,
    28,
    29,
    29,
    31,
    31,
    32,
    33,
    33,
    34,
    35,
    36,
    36,
    37,
    38,
    38,
    39,
    39,
    41,
    41,
    42,
    42,
    43,
    43,
    44,
    45,
    46,
    46,
    47,
    47,
    48,
    49,
    49,
    50,
    51,
    51,
    52,
    53,
    53,
    54,
    54,
    55,
    56,
    57,
    57,
    58,
    58,
    59,
    59,
    61,
    61,
    62,
    62,
    63,
    64,
    64,
    65,
    66,
    67,
    67,
    68,
    69,
    69,
    71,
    71,
    72,
    73,
    74,
    74,
    76,
    77,
    77,
    78,
    79,
    81,
    82,
    83,
    84,
    86,
    87,
    89,
    91, // 98%
    94  // 99%   
        // 100% is not required (oven fully on)
};

/*
 * Zero crossing interrupt handler
 * awreflow::OvenControlInstance->zeroCrossingHandler();
 */
ISR(INT0_vect)
{
    /* 0 is an off switch. round up or down a percentage that strays into the
     * end-zone where we have a margin wide enough to cater for the minimum
     * pulse width and the delay in the zero crossing firing */
    if(_percent == 0)
    {
        OVEN_OFF;
        // GpioActivateOven::reset();
        return;
    }
    // either user asked for 100 or calc rounds up to 100
    else if(_percent == 100 || _counter_t2 == 0)
    {
        OVEN_ON; 
        //GpioActivateOven::set();
    }
    // Comparison to a constant is pretty fast
    else if(_counter_t2 > TICKS_PER_HALF_CYCLE - TRIAC_PULSE_TICKS - MARGIN_TICKS)
    {
        // Also a constant comparison so also pretty fast
        if(_counter_t2 > (TICKS_PER_HALF_CYCLE - (TRIAC_PULSE_TICKS - MARGIN_TICKS / 2)))
        {
            // round half up to completely off
            OVEN_OFF;
            // GpioActivateOven::reset();
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

/*
 * Timer2 comparison value A matched
 * awreflow::OvenControlInstance->timerComparatorHandler();
 */
ISR(TIMER2_COMPA_vect)
{
    //PORTD |= (1 << PIN_LED_OUT);
    // Turn on oven and hold it active for a minimum amount of time before
    // switching it off
    OVEN_ON;

    // The overflow interrupt will fire when the minimum pulse width is reached
    TCNT2 = 256 - TRIAC_PULSE_TICKS;
}

/*
 * Timer2 overflow handler 
 * awreflow::OvenControlInstance->timerOverflowHandler()
 */
ISR(TIMER2_OVF_vect)
{
    // Turn off oven
    OVEN_OFF;

    // turn off the timer. the zero-crossing handler will restart it
    TCCR2B = 0;
}


void setDutyCycle(uint8_t percent)
{

    uint16_t newCounter;
    
    if(percent > 0 && percent < 100)
        _percent = pgm_read_byte(&powerLUT[percent - 1]);

    // percentages between 1 and 99 inclusive use the lookup table to translate a linear
    // demand for power to a position on the phase angle axis

    //if(percentage > 0 && percentage < 100)
    //  percentage = pgm_read_byte(&LookupTable[percentage-1]);

    // calculate the new counter value

    newCounter = ((TICKS_PER_HALF_CYCLE - MARGIN_TICKS - TRIAC_PULSE_TICKS) * (100 - percent)) / 100;

    // set the new state with interrupts off because 16-bit writes are not atomic

    cli();
    _counter_t2 = newCounter;
    _percent = percent;
    sei();
}


