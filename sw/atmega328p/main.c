#include <avr/io.h>
//#include <avr/eeprom.h>
//#include <avr/pgmspace.h>
#include <avr/interrupt.h>
//#include <util/delay.h>

#define PIN_LED_OUT     PD7     // Pin 11
#define PIN_ZX_IN       PD2     // Pin 2 (Interrupt)
#define PIN_TA_OUT      PD3     // Pin 3 (PWM output)

// TRIAC timer runs at 128uS per tick. The triac_pulse_ticks variable defines
// how many ticks TRIAC gate will be activated for. 4 ticks = 512uS
#define TICKS_PER_HALF_CYCLE    65
#define TRIAC_PULSE_TICKS       4
#define MARGIN_TICKS            8

/* Initializations:

 **   IRQ on ZX
 **   UART for debugging (and eventual ESP8266 comms)
 **   Timers for TRIAC-Active */


// Control "dimmness" percentage and counter for delay
volatile uint32_t   _ms_counter = 0;
volatile uint8_t    _percent = 0;
volatile uint8_t    _counter = 0;
volatile uint8_t    _ms_subCounter = 0;

/*
 * Zero crossing interrupt handler
 * awreflow::OvenControlInstance->zeroCrossingHandler();
 */
ISR(INT0_vect)
{
    /* 0 is an off switch. round up or down a percentage that strays into the
     * end-zone where we have a margin wide enough to cater for the minimum
     * pulse width and the delay in the zero crossing firing */
    if(_percent == 0)
    {
        PORTD &= ~(1 << PIN_TA_OUT);
        // GpioActivateOven::reset();
        return;
    }
    // either user asked for 100 or calc rounds up to 100
    else if(_percent == 100 || _counter == 0)
    {
        PORTD |= (1 << PIN_TA_OUT); 
        //GpioActivateOven::set();
    }
    // Comparison to a constant is pretty fast
    else if(_counter > TICKS_PER_HALF_CYCLE - TRIAC_PULSE_TICKS - MARGIN_TICKS)
    {
        // Also a constant comparison so also pretty fast
        if(_counter > (TICKS_PER_HALF_CYCLE - (TRIAC_PULSE_TICKS - MARGIN_TICKS / 2)))
        {
            // round half up to completely off
            PORTD &= ~(1 << PIN_TA_OUT);
            // GpioActivateOven::reset();
            return;
        }
        else 
            _counter = TICKS_PER_HALF_CYCLE - TRIAC_PULSE_TICKS - MARGIN_TICKS;
    }

    // Counter is acceptable, or has been rounded down to be acceptable
    OCR2A = _counter;
    TCNT2 = 0;
    TCCR2B = (1 << CS20) | (1 << CS21) | (1 << CS22);    // start timer: 8MHz/1024 = 128uS/tick
}

/*
 * Timer2 comparison value A matched
 * awreflow::OvenControlInstance->timerComparatorHandler();
 */
ISR(TIMER2_COMPA_vect)
{
    //PORTD |= (1 << PIN_LED_OUT);
    // Turn on oven and hold it active for a minimum amount of time before
    // switching it off
    PORTD |= (1 << PIN_TA_OUT);

    // The overflow interrupt will fire when the minimum pulse width is reached
    TCNT2 = 256 - TRIAC_PULSE_TICKS;
}

/*
 * Timer2 overflow handler 
 * awreflow::OvenControlInstance->timerOverflowHandler()
 */
ISR(TIMER2_OVF_vect)
{
    // Turn off oven
    PORTD &= ~(1 << PIN_TA_OUT);

    // turn off the timer. the zero-crossing handler will restart it
    TCCR2B = 0;
}

/*
 * Timer 0 overflow handler (ms timer)
 */
ISR(TIMER0_OVF_vect)
{
    _ms_subCounter++;
    if((_ms_subCounter & 0x3) == 0) _ms_counter++;
    TCNT0 += 6;
}
 

/*
 * Millisecond timer routine. Very similar to Arudion millis()
 */
uint32_t millis()
{
    
    uint32_t ms;

    // an 8-bit MCU cannot atomically read/write a 32-bit value so we must
    // disable interrupts while retrieving the value to avoid getting a half-
    // written value if an interrupt gets in while we're reading it

    cli();
    ms=_ms_counter;
    sei();

    return ms;
}


void delay(uint32_t waitfor)
{

    uint32_t target;

    target = millis() + waitfor;
    while(_ms_counter < target);
}


void setDutyCycle(uint8_t percent) {

    uint16_t newCounter;

    // percentages between 1 and 99 inclusive use the lookup table to translate a linear
    // demand for power to a position on the phase angle axis

    //if(percentage > 0 && percentage < 100)
    //  percentage = pgm_read_byte(&LookupTable[percentage-1]);

    // calculate the new counter value

    newCounter = ((TICKS_PER_HALF_CYCLE - MARGIN_TICKS - TRIAC_PULSE_TICKS) * (100 - percent)) / 100;

    // set the new state with interrupts off because 16-bit writes are not atomic

    cli();
    _counter = newCounter;
    _percent = percent;
    sei();
}

int main(void)
{
    // Turn off interrupts globally before we even get started
    cli();

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
            delay(10);
        }
        for(i = 100; i >= 0; i--){
            setDutyCycle(i);
            delay(10);
        }
        
    }
    return 1;
}

