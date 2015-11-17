/*
 Name:			main.c
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

#include <stdio.h>
#include <string.h>
#include <stdint.h>				// definitions such as 'uint8_t', etc.
#include <avr/io.h>				
#include <avr/interrupt.h>		// ISR handling.
#include <util/delay.h>			// _delay_ms() routines.
#include "uart.h"				// routines for UART comms.

//#define DEBUG /* uncomment to enable debug information via UART */

// bit manipulation macros.
#define sbi(var, mask)   ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask)   ((var) &= (uint8_t)~(1 << mask)) 

#define F_CPU 8000000			// 8 MHz internal clock freq.
#define USART_BAUDRATE 9600		// 9600 baud, selected for 0.2% error.
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1) 	
#define BUFFER_SIZE 100			// size of Rx ring buffer.

/*
 Function Prototypes:
*/
ISR(USART_RXC_vect);
static void pulseLED(void);
static void ioinit(void);
int main(void);

/*
 Global Variables:
 Variables appearing in both ISR/Main are defined as 'volatile'.
*/
volatile int rxn=0; // buffer 'element' counter.
volatile char rx[BUFFER_SIZE]; // buffer of 'char'.
volatile uint8_t rxFlag = 0; //entire byte wasted for using a single bit

/*
 printf();
*/
static FILE mystdout = FDEV_SETUP_STREAM(uart_putc, NULL, _FDEV_SETUP_WRITE);

/*
 Interrupt Service Routine (ISR):
 When interrupt-driven serial data reception is used, the 
 receive complete routine must read the received data from 
 UDR in order to clear the RXC Flag, otherwise a new interrupt
 will occur once the interrupt routine terminates. 

 That's important to remember - if you are using the RXC
 interrupt, you must read a byte from the UDR register 
 to clear the interrupt flag. Keep it in mind for your future 
 projects! 
*/

ISR(USART_RXC_vect)
{
	while ( !(UCSRA & (1<<RXC)) );
	if (rxn==BUFFER_SIZE) // if BUFFER_SIZE is reached, reset to start of buffer.
		rxn=0;
	rx[rxn++] = UDR; // increment rxn and return new value.   
	rxFlag=1; // notify main of receipt of data.
	
	#ifdef DEBUG
		printf("buff[%d]: %c\r\n", rxn, rx[rxn-1]);
	#endif
}

/*
 pulsing LED connected to PORTB0; indicates code is being executed.
 T=0.2 / f=5 Hz strobe on LED.
 WARNING: Disable this function if you are affected by 'strobe' lighting.
*/
static void pulseLED()
{	
	PORTB ^= 1;		/* toggle the LED (PB0) */	
	_delay_ms(100);	/* wait 0.1 s */
	PORTB ^= 1;		/* toggle the LED (PB0) */	
	_delay_ms(100);	/* wait 0.1 s */
}

static void ioinit()
{
    //1 = output, 0 = input
    //DDRB = 0b11101111; //PB4 = MISO 
    //DDRC = 0b11111111; //
    //DDRD = 0b11111110; //PORTD (RX on PD0)
	DDRB=1;
	
    //USART Baud rate: 9600
	UBRRL = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR register
	UBRRH = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register  
    UCSRB |= (1<<RXEN)|(1<<TXEN); // Turn on the transmission and reception circuitry
    UCSRC |= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);	// Use 8-bit character sizes
												//URSEL bit set to select the UCRSC register
	UCSRB |= (1<<RXCIE); // Enable the USART Recieve Complete interrupt (USART_RXC)	
	sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed 	
    
    stdout = &mystdout; // Required for init of printf();
}

int main()
{
	uint8_t i=0;
	
    ioinit();
	
	printf("\n> Init...OK\n");
	
	while(1) // Loop forever
	{
		if (rxFlag == 1) // process received data in buffer.
		{
			/*
			 Ideally, once data is read from the ring buffer,
			 the buffer should be updated to reflect that it
			 has been taken off it.  This is to ensure that no
			 data would be lost when the buffer counter resets 
			 on 'roll over'.
			 
			 Such functionality is pending implementation.  The
			 following loop simply returns the current contents
			 of the entire buffer, each time the rxFlag goes high.
			 */
			for (i=0;i<rxn;i++)
			{
				printf("[%d]: %c, ", i, rx[i]);
			}
			rxFlag=0;
			printf("\n");
		}
		pulseLED();	// heartbeat LED.	
	}    
}
