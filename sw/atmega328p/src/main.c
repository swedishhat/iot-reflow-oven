#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "oven_control.h"
#include "ms_timer.h"
//#include "macros.h"

int main(void)
{
    // Globally disable interrupts
    cli();

    // Clear interrupt flag by reading the interrupt register
    uint8_t dummy __attribute__((unused)) = SPSR;
    dummy = SPDR;
    
    // Configure external interrupt registers (Eventually move into macros.h)
    EICRA |= (1 << ISC01);      // Falling edge of INT0 generates an IRQ
    EIMSK |= (1 << INT0);       // Enable INT0 external interrupt mask
    
    // Setup I/O direction & values, start timers
    oven_start();
    msTimer_start();

    // Ready to roll. Turn on global interrupt flag
    sei();

    // Main program loop
    for(;;)
    {                
        // "Fade" duty cycle with single FOR loop
        int i = 0;
        int dir = 1;
        for (i = 0; i > -1; i = i + dir)
        {
            oven_setDutyCycle(i);
            if (i == 100) dir = -1;             // switch direction at peak
            msTimer_delay(10);
        } 
    }

    return 1;
}

