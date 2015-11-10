// Millisecond timer. Short and sweet. Like you!
#include <stdint.h>
#include <avr/interrupt.h>
#include "ms_timer.h"

/*** Millisecond Timer Variables ***/
volatile uint32_t   _ms_counter = 0;
volatile uint8_t    _ms_subCounter = 0;

/*** Timer 0 Overflow ISR ***/
ISR(TIMER0_OVF_vect)
{
    _ms_subCounter++;
    if((_ms_subCounter & 0x3) == 0) _ms_counter++;
    TCNT0 += 6;
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

/*** Millisecond Delay Function ***/
void msTimer_delay(uint32_t waitfor)
{
    uint32_t target;

    target = msTimer_millis() + waitfor;
    while(_ms_counter < target);
}

/*** Millisecond Timer Initialization Function ***/
void msTimer_start(void)
{
    // Leave everything alone in TCCR0A and just set the prescaler to Clk/8
    TCCR0B |= (1 << CS01);
    
    // Enable interrupt when Timer/Counter0 reaches max value and overflows
    TIMSK0 |= (1 << TOIE0);
}
