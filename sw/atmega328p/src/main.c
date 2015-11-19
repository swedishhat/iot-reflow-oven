/*** main.c ***/

#include "globals.h"
//#define __AVR_ATmega328P__  // Our board

int main(void)
{
    // Globally disable interrupts
    cli();

    // Setup oven, timers, UART, SPI
    oven_setup();
    msTimer_setup();

    uart0_init(UART_BAUD_SELECT(9600, F_CPU));

    // Something kinda like OOP in C
    max31855 *m = max31855_setup();

    // Ready to roll. Flush UART buffer and turn on global interrupt flag
    uart0_flush();

    // Clear interrupt flag by reading the interrupt register
    // Specify that it's 'unused' so compiler doesn't complain
    uint8_t dummy __attribute__((unused)) = SPSR;
    dummy = SPDR;

    // Turn on global interrupt flag
    sei();


    // "Hello World" startup message
    uart0_puts("Hot Toaster Action");
    uart0_puts_P(RETURN_NEWLINE);
    uart0_puts_P("-----");
    uart_puts_P(RETURN_NEWLINE);

    uart0_puts_P("AVR UART");
    uart0_puts_P(RETURN_NEWLINE);
    uart0_puts_P(RETURN_NEWLINE);

    for(;;)
    {
        if(max31855_readTempDone(m));
        cmd_processUART(m);
    }
}
