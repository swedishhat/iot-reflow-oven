#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "oven_control.h"
#include "ms_timer.h"

#define PIN_LED_OUT     PD7     // Pin 11


/* Initializations:

 **   IRQ on ZX
 **   UART for debugging (and eventual ESP8266 comms)
 **   Timers for TRIAC-Active */

int main(void)
{
    // Turn off interrupts globally before we even get started
    //cli();

    // Clear interrupt flag by reading the interrupt register
    uint8_t dummy __attribute__((unused)) = SPSR;
    dummy = SPDR;

    /* GPIO Config *
    ================
    * DDRD: The Port D Data Direction Register
        PIN_LED_OUT, PIN_TA_OUT are outputs -> set 1
        PIN_ZX_IN is an input -> set 0
    * PORTD: The Port D Data Register
        PIN_LED_OUT, PIN_TA_OUT outputs are cleared -> set 0 */
    
    DDRD |= (1 << PIN_LED_OUT) | (1 << PIN_TA_OUT);     // OUTPUTS      (1)
    DDRD &= ~(1 << PIN_ZX_IN);                          // INPUTS       (0) 
    PORTD &= ~((1 << PIN_LED_OUT) | (1 << PIN_TA_OUT));  // Clear OUTPUTS (0)

    /* External Interrupt Config *
    ===============================
    * EICRA: External Interrupt Control Register A
        The falling edge of INT0 generates an interrupt request
    * EIMSK: External Interrupt Mask Register
        The interrupt INT0 is enabled
    * EIFR: External Interrupt Flag Register
        Set whenever interrupt condition me. Can be manually cleared.
        Don't think this needs fucking with right now though... */
    
    EICRA |= (1 << ISC01);
    EIMSK |= (1 << INT0);
    

    /* Just Timer Things... ;) *
    ============================
    * TCCR0B: Timer/Counter0 Control Register B
        Leave everything alone in TCCR0A and just set the prescaler to Clk/8
    * TIMSK0: Timer/Counter0 Interrupt Mask Register
        Enable interrupt when Timer/Counter0 reaches max value and overflows
    * TIMSK2: Timer/Counter2 Interrupt Mask Register
        Enable interrupt when Timer/Counter2 overflows & when it equals OC2A */
    
    TCCR0B |= (1 << CS01);
    TIMSK0 |= (1 << TOIE0);
    TIMSK2 |= (1 << OCIE2A) | (1 << TOIE2);

    // Ready to roll. Turn on global interrupt flag
    sei();

    int i;
    setDutyCycle(50);
    while(1)
    {                
        
        //PORTB |= (1 << PIN_LED_OUT); // digitalWrite HIGH
        for(i = 0; i <= 100; i++)
        {
            setDutyCycle(i);
            delay(60);
        }
        for(i = 100; i >= 0; i--){
            setDutyCycle(i);
            delay(60);
        }
        
    }
    return 1;
}

