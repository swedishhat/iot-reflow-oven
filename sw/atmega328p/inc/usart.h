// usart.h
#ifndef _USART_H
#define _USART_H

#define USART_BAUD      9600
#define BAUD_PRESCALE   ((((F_CPU / 16) + (USART_BAUD / 2)) / (USART_BAUD)) - 1)      

#define USART_TX        PD1
#define USART_TX_PORT   PORTD
#define USART_TX_DDR    DDRD
#define USART_TX_PIN    PIND

#define USART_RX        PD0
#define USART_RX_PORT   PORTD
#define USART_RX_DDR    DDRD
#define USART_RX_PIN    PIND

/*** USART Initialization Function ***/
void usart_setup(uint32_t ubrr);

/*** USART Flush Function ***/
void usart_flush(void);

/*** USART Transmit Byte Function ***/
void usart_txb(const char data);

/*** USART Print String Function ***/
void usart_print (const char *data);

/*** USART Print String Function with New Line and Carriage Return ***/
void usart_println (const char *data);

#endif
