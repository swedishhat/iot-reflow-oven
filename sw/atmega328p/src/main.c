/*** main.c ***/

#include "globals.h"

int main(void)
{
    // Globally disable interrupts
    cli();
    
    // Setup oven, timers, USART, SPI
    oven_setup();
    msTimer_setup();
    usart_setup(BAUD_PRESCALE);

    // Something kinda like OOP in C
    max31855 *m = max31855_setup();
    
    // Ready to roll. Flush USART buffer and turn on global interrupt flag
    usart_flush();
    
    // Clear interrupt flag by reading the interrupt register
    // Specify that it's 'unused' so compiler doesn't complain
    uint8_t dummy __attribute__((unused)) = SPSR;
    dummy = SPDR;

    // Turn on global interrupt flag
    sei();


    // "Hello World" startup message
    usart_println("Hot Toaster Action");
    
    // Main program loop
    for(;;)
    {                
        // "Fade" duty cycle in and out with single for loop
        int i = 0;
        int dir = 1;
        for (i = 0; i > -1; i = i + dir)
        {
            // Control power output
            oven_setDutyCycle(i);
            
            // Switch direction at peak and pause for 10ms
            if (i == 100) dir = -1;
            msTimer_delay(10);
            
            // If it's done reading, print the temp and status
            if(max31855_readTempDone(m)) max31855_print(m);
        } 
    }

    return 1;
}