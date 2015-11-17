/*
 Name:			uart.c
 Author: 		Michael de Silva
 Email:			mike@bsodmike.com
 Blog:			www.bsodmike.com
 
 Copyright: 	Copyright (C) Michael de Silva, 2009
 
 Permission to use, copy, modify, and distribute this software
 and its documentation for any purpose and without fee is hereby
 granted, provided that the above copyright notice appear in all
 copies and that both that the copyright notice and this
 permission notice and warranty disclaimer appear in supporting
 documentation, and that the name of the author not be used in
 advertising or publicity pertaining to distribution of the
 software without specific, written prior permission.
 
 The author disclaim all warranties with regard to this
 software, including all implied warranties of merchantability
 and fitness.  In no event shall the author be liable for any
 special, indirect or consequential damages or any damages
 whatsoever resulting from loss of use, data or profits, whether
 in an action of contract, negligence or other tortious action,
 arising out of or in connection with the use or performance of
 this software.
*/

#include "uart.h"

/*
 In place only for use with the following in 'main.c':
 static FILE mystdout = FDEV_SETUP_STREAM(uart_putc, NULL, _FDEV_SETUP_WRITE);
*/
int uart_putc(char c, FILE *stream)
{
    if (c == '\n') uart_putc('\r', stream);
	
    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = c;
	
    return 0;
} 

void uart_putchar (char data)
{
	while ( !( UCSRA & (1<<UDRE)) ); // Wait for empty transmit buffer           
	UDR = data;  // Putting data into the buffer, forces transmission                               
}

char uart_getchar (void)
{
	while ( !(UCSRA & (1<<RXC)) );  // Wait for data to be received           
	return (UDR);        // Return Data                         
}

void uart_putstring(char *s)
{
	while(*s) uart_putchar(*s++);
	
}