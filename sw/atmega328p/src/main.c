/*** main.c ***/

#include "globals.h"
//#define __AVR_ATmega328P__  // Our board

void fadeoven(uint8_t fadedelay);


int main(void)
{
    // Globally disable interrupts
    cli();
    
    // Setup oven, timers, UART, SPI
    oven_setup();
    msTimer_setup();
    
    uart0_init(UART_BAUD_SELECT(9600, F_CPU));

    // Something kinda like OOP in C
    //max31855 *m = max31855_setup();
    
    // Ready to roll. Flush UART buffer and turn on global interrupt flag
    uart0_flush();
    
    // Clear interrupt flag by reading the interrupt register
    // Specify that it's 'unused' so compiler doesn't complain
    uint8_t dummy __attribute__((unused)) = SPSR;
    dummy = SPDR;

    // Turn on global interrupt flag
    sei();


    // "Hello World" startup message
    uart0_puts("Hot Toaster Action\r\n");
    
    // Main program loop
    uint16_t cmdchar = 0;

    for(;;)
    {                
        if(uart0_available() > 0)
        {
            cmdchar = uart0_getc();
            switch((char)(cmdchar & 0x00FF))
            {
                case 'f':
                    uart0_puts("FUCK!\n\r");
                    break;
                case 's':
                    uart0_puts("SHIT!\n\r");
                    break;
                default:
                    uart0_puts("????\n\r");
            }
        }

        // If it's done reading, print the temp and status
        //if(max31855_readTempDone(m)) max31855_print(m); 
    }

    return 1;
}

void fadeoven(uint8_t fadedelay)
{
    uint8_t i = 0;
    uint8_t dir = 1;
    for (i = 0; i > -1; i = i + dir)
    {
        // Control power output
        oven_setDutyCycle(i);
        
        // Switch direction at peak and pause for 10ms
        if (i == 100) dir = -1;
        msTimer_delay(fadedelay);
    }
}