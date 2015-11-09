// Millisecond timer. Short and sweet. Like you!
#include <stdint.h>
#include <avr/interrupt.h>
#include "ms_timer.h"

volatile uint32_t   _ms_counter = 0;
volatile uint8_t    _ms_subCounter = 0;

/*
 * Timer 0 overflow handler (ms timer)
 */
ISR(TIMER0_OVF_vect)
{
    _ms_subCounter++;
    if((_ms_subCounter & 0x3) == 0) _ms_counter++;
    TCNT0 += 6;
}
/*
 * Millisecond timer routine. Very similar to Arduino millis()
 */
uint32_t millis(void)
{
    
    uint32_t ms;

    // an 8-bit MCU cannot atomically read/write a 32-bit value so we must
    // disable interrupts while retrieving the value to avoid getting a half-
    // written value if an interrupt gets in while we're reading it

    cli();
    ms=_ms_counter;
    sei();

    return ms;
}

/*
 * Delay for 'waitfor' ms. Very similar in functionality to Arduino delay()
 */
void delay(uint32_t waitfor)
{

    uint32_t target;

    target = millis() + waitfor;
    while(_ms_counter < target);
}
