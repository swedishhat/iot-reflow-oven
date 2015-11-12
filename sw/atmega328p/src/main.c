#include "globals.h"

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
    
    // Setup oven, timers, USART, SPI
    oven_setup();
    msTimer_setup();
    usart_setup(BAUD_PRESCALE);

    max31855 *m = max31855_setup();
    
    // Ready to roll. Flush USART buffer and turn on global interrupt flag
    usart_flush();
    sei();

    usart_println("Hot Toaster Action");
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
            if(max31855_readTempDone(m)) max31855_print(m);
        } 
        
    }

    return 1;
}

