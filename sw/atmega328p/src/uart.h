// uart.h
#ifndef _UART_H
#define _UART_H

#define UART_BAUD      9600
#define BAUD_PRESCALE   ((((F_CPU / 16) + (UART_BAUD / 2)) / (UART_BAUD)) - 1)      

#define UART_TX        PD1
#define UART_TX_PORT   PORTD
#define UART_TX_DDR    DDRD
#define UART_TX_PIN    PIND

#define UART_RX        PD0
#define UART_RX_PORT   PORTD
#define UART_RX_DDR    DDRD
#define UART_RX_PIN    PIND

/*** UART Initialization Function ***/
void uart_setup(uint32_t ubrr);

/*** UART Flush Function ***/
void uart_flush(void);

/*** UART Transmit Byte Function ***/
void uart_txb(const char data);

/*** UART Print String Function ***/
void uart_print (const char *data);

/*** UART Print String Function with New Line and Carriage Return ***/
void uart_println (const char *data);

/*** RX Receive ISR ***/
ISR(USART_RX_vect)

#endif
