// Millisecond timer. Short and sweet. Like you!
#include "globals.h"

/*** Millisecond Timer Variables ***/
volatile uint32_t   _ms_counter = 0;
volatile uint8_t    _ms_subCounter = 0;

/*** Millisecond Timer Initialization Function ***/
void msTimer_setup(void)
{
    // Leave everything alone in TCCR0A and just set the prescaler to Clk/8
    TCCR0B |= (1 << CS01);

    // Enable interrupt when Timer/Counter0 reaches max value and overflows
    TIMSK0 |= (1 << TOIE0);
}

/*** Millisecond Delay Function ***/
void msTimer_delay(uint32_t waitfor)
{
    uint32_t target;

    target = msTimer_millis() + waitfor;
    while(_ms_counter < target);
}

/*** Millisecond Counter Function ***/
uint32_t msTimer_millis(void)
{
    uint32_t ms;

    // NOTE: an 8-bit MCU cannot atomically read/write a 32-bit value so we
    // must disable interrupts while retrieving the value to avoid getting a
    // half-written value if an interrupt gets in while we're reading it
    cli();
    ms=_ms_counter;
    sei();

    return ms;
}

/*** Timeout Detection Function ***/
bool msTimer_hasTimedOut(uint32_t start,uint32_t timeout)
{
    // Check if a timeout has been exceeded. This is designed to cope with wrap
    // around
    return msTimer_deltaT(start) > timeout;
}

/*** Time Delta Function ***/
uint32_t msTimer_deltaT(uint32_t start)
{
    // Return difference between a starting time and now, taking into account
    // wrap around
    uint32_t now = msTimer_millis();

    if(now > start)
        return now - start;
    else
        return now + (0xffffffff - start + 1);
}

/*** Timer 0 Overflow ISR ***/
ISR(TIMER0_OVF_vect)
{
    _ms_subCounter++;
    if((_ms_subCounter & 0x3) == 0) _ms_counter++;
    TCNT0 += 6;
}
