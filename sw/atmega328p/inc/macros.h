/*** macros.h ***/

#ifndef _MACROS_H_
#define _MACROS_H_

/***** Configure IO *****/
#define CONFIG_AS_INPUT(bit)    {bit ## _DDR &= ~(1 << bit);}
#define CONFIG_AS_OUTPUT(bit)   {bit ## _DDR |= (1 << bit);}

#define PULLUP_ON(bit)          {bit ## _PORT |= (1 << bit);}
#define PULLUP_OFF(bit)         {bit ## _PORT &= ~(1 << bit);}

/***** Manipulate Outputs *****/
#define SET_HIGH(bit)           {bit ## _PORT |= (1 << bit);}
#define SET_LOW(bit)            {bit ## _PORT &= ~(1 << bit);}
#define TOGGLE(bit)             {bit ## _PIN |= (1 << bit);}

/***** Test Inputs *****/
#define IS_HIGH(bit)            (bit ## _PIN & (1 << bit))
#define IS_LOW(bit)             (! (bit ## _PIN & (1 << bit)))

/***** Format for I/O *****
#define REED_CONTACT                PA0
#define REED_CONTACT_PORT           PORTA
#define REED_CONTACT_DDR            DDRA
#define REED_CONTACT_PIN            PINA

#define REED_CONTACT_PCMSK          PCMSK0
#define REED_CONTACT_PCINT          PCINT0 */

// Pin MUXing for ADC
// (Analog comparator uses DIDR1)
//#define disable_digital_input(channel)      {DIDR0 |= (1 << channel);}
//#define enable_digital_input(channel)       {DIDR0 &= ~(1 << channel);}

// Setup Pin-Change Interrupts
//#define enable_pin_change_interrupt(pin)    {pin ## _PCMSK |= (1 << pin ## _PCINT);}
//#define disable_pin_change_interrupt(pin)   {pin ## _PCMSK &= ~(1<< pin ## _PCINT);}

#endif
