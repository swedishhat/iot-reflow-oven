/*** uart.c ***/

#include "globals.h"

/*** USART Initialization Function ***/
void uart_setup(uint32_t ubrr)
{
    // Set baud rate by loading high and low bytes of ubrr into UBRR0 register
    UBRR0H = (ubrr >> 8);
    UBRR0L = ubrr;

    // Turn on the transmission and reception circuitry
    UCSR0B = (1 << RXCIE0) | (1 << RXEN0 ) | (1 << TXEN0 );

    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1<<USBS0) | (3<<UCSZ00);

    // Use 8-N-1 -> Eight (8) data bits, No (N) partiy bits, one (1) stop bit
    // The initial vlaue of USCR0C is 0b00000110 which implements 8N1 by
    // Default. Setting these bits is for Paranoid Patricks and people that
    // Like to be reeeeeally sure that the hardware is doing what you say
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
}

/*** USART Flush Function ***/
void uart_flush(void)
{
    // Do a dummy read of the data register until it's clear
    uint8_t dummy __attribute__((unused));
    while (UCSR0A & (1 << RXC0))
        dummy = UDR0;
}

/*** USART Transmit Byte Function ***/
void uart_txb(const char data)
{
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));

    // Put data into buffer, sends the data
    UDR0 = data;
}

/*** USART Print String Function ***/
void uart_print (const char *data)
{
    while (*data != '\0')
        uart_txb(*data++);
}

/*** USART Print String Function with New Line and Carriage Return ***/
void uart_println (const char *data)
{
    uart_print(data);
    uart_print("\n\r");    // GNU screen demands \r as well as \n :(
}

/*** Echo on Receive ISR ***/
ISR(USART_RX_vect)
{
    unsigned char ReceivedByte;
    ReceivedByte = UDR0;
    UDR0 = ReceivedByte;
}
