![Look beyond the Arduino exterior, it's a raw ATmega underneath!](images/ardpromini-thumb.jpg)

# Designing the Controller for a DIY Solder Reflow Oven

## Summary
In the last installment, we built the hardware to control an AC waveform with a TRIAC as the first step in a DIY solder reflow oven. This tutorial continues the project by showing you how to set up the low-level hardware to measure temperature, read the zero-cross detector, drive the TRIAC, and print to the serial terminal using a USART.

#### Technical Concepts
* C programming
* Serial peripheral interface
* USART
* Internal and external interrupts
* Timers / counters

#### Introduction
**[See Part 1: Control Your AC Mains with a Microcontroller ](http://www.allaboutcircuits.com/projects/controlling-ac-mains-with-a-microcontroller-for-fun-and-profit/)**

Last time we built the TRIAC driver and zero-cross detection circuitry to interface with 120VAC mains voltages. It's a very capable bit of circuitry but without a proper controller, the end result wasn't all that interesting since it could only turn on or off the waveform, and not dim it. In this project, we are  writing C code on an Atmel ATmega328P microcontroller to accomplish several key tasks:
1. Read zero-cross signal with external interrupt and drive TRIAC with a special form of pulse-width modulation
2. Use the _Universal Synchronous and Asynchronous serial Receiver and Transmitter_ (USART) to display debug data
3. Interface with MAX31855 thermocouple amplifier over the _Serial Peripheral Interface_ (SPI)
4. Create a general purpose millisecond timer to help facilitate timeouts, timestamps, and non-blocking delays

Bare metal C means that we are writing very low-level code -- C is just a single step up from assembly language as far as abstraction goes. This means we'll be manipulating bits in specific registers, specifying interrupt vectors directly in our interrupt service routines (ISRs), and sometimes dealing with raw memory allocation with `malloc()`. There are some macros that make this process a little easier for us (and make the code cleaner to read) but familiarity with some of the actual inner workings of the ATmega328P and the names it uses for different registers and components is very important. The [complete datasheet](http://www.atmel.com/images/Atmel-8271-8-bit-AVR-Microcontroller-ATmega48A-48PA-88A-88PA-168A-168PA-328-328P_datasheet_Complete.pdf)(PDF) for the chip has all that info in it and is worth keeping on hand. [Programming from the Gound Up](http://programminggroundup.blogspot.com/) may be a helpful resource as well for getting comfortable with low-level development.

_I borrowed some of my code from [Andy Brown](http://andybrown.me.uk/) and his [ATmega8 oven controller](https://github.com/andysworkshop/awreflow2/tree/master/atmega8l). There is some drop-in code reuse, some tweaked bits, and some totally different implementations. In addition to having a different controller, he wrote his code in C++ and uses a different build system but I still want to give him full credit for the previous work he's done._

#### What You Need for this Project
This project is mostly software so the parts count is relatively small. You'll need:
* 3.3V ATmega328P microcontroller board with crystal oscillator (necessary for propery USART functionality)
  * [Arduino Pro Mini (3.3V)](http://www.digikey.com/product-detail/en/DEV-11114/1568-1054-ND/5140819) - I got a Chinese knock-off from Ebay instead of DigiKey. There are a bunch of small variations between boards so be aware when connecting your pins.
  * [Built your own](https://www.arduino.cc/en/Tutorial/ArduinoToBreadboard) - Plenty of tutorials out there on breadboarding your chip and doing a true barebones solution
* In-circuit Serial Programmer (ICSP)
  * [AVR Dragon](http://www.atmel.com/tools/AVRDRAGON.aspx) - I use this one. Lots of features and relatively cheap
  * [Arduino Uno](https://www.arduino.cc/en/Tutorial/ArduinoToBreadboard) - Other main Arduino boards can be used as a programmer as well.
* USB-Serial Adapter
  * [FT232RL](https://www.sparkfun.com/products/10275) - **Needs to work at 3.3v!** I have this 5V model but I cut trace on the back and added a switch:

![Modified FTDI breakout](images/ftdi-basic-tracecut.jpg)
  * [CH340/CH341](http://www.seeedstudio.com/wiki/USB_To_Uart_5V/3V3)
* MAX31855 breakout
  * [Home grown](http://www.allaboutcircuits.com/projects/build-a-thermocouple-amplifier-and-custom-kicad-libraries/)
  * [Adafruit](http://www.adafruit.com/product/269)
* Functioning TRIAC AC controller
* Computer running Linux with `avrdude`, `binutils-avr`, `gcc-avr`, `avr-libc`, and `gdb-avr` installed. It's possible to do this on Windows or Mac but that is outside the scope of this project.

#### TRIAC Controller
This is the bread and butter of the controller. The `oven_control.c` file consist of several parts: an `oven_setup()` function to configure GPIO and interrupt conditions, an `oven_setDutyCycle(percent)` function to update the  output power variable, and three ISRs to deal with different timing-critical events.  

**ISR(INT0_vect)**
```
ISR(INT0_vect)
{
    /* 0 is an off switch. round up or down a percentage that strays into the
     * end-zone where we have a margin wide enough to cater for the minimum
     * pulse width and the delay in the zero crossing firing */
    if(_percent == 0)
    {
        OVEN_OFF();
        return;
    }
    // either user asked for 100 or calc rounds up to 100
    else if(_percent == 100 || _counter_t2 == 0)
    {
        OVEN_ON();
    }
    // Comparison to a constant is pretty fast
    else if(_counter_t2 > TICKS_PER_HALF_CYCLE - TRIAC_PULSE_TICKS - MARGIN_TICKS)
    {
        // Also a constant comparison so also pretty fast
        if(_counter_t2 > (TICKS_PER_HALF_CYCLE - (TRIAC_PULSE_TICKS - MARGIN_TICKS / 2)))
        {
            // round half up to completely off
            OVEN_OFF();
            return;
        }
        else
            _counter_t2 = TICKS_PER_HALF_CYCLE - TRIAC_PULSE_TICKS - MARGIN_TICKS;
    }

    // Counter is acceptable, or has been rounded down to be acceptable
    OCR2A = _counter_t2;
    TCNT2 = 0;
    TCCR2B = (1 << CS20) | (1 << CS21) | (1 << CS22);    // start timer: 8MHz/1024 = 128uS/tick
}
```

This triggers on the falling edge of pin PD2. Depending on what the global `_percent` variable is set to, it will either turn the oven full on, full off, or if the

has It uses Timer2 to

#### USART
In normal C or C++ programming on a computer, functions like `assert()` and `sprintf()` can print formatted text to the terminal and help with debugging.

When dealing with microcontrollers, there is rarely a

#### Temperature Sensor

#### General Purpose Timer

#### Putting it All Together
The `main.c` file is just a small test file that initializes all the other parts through the `(device)_setup` command, flushes anything in the USART and then goes into an endless loop. In the loop, it fades the TRIAC drive intensity in and out and constantly tries to read the temperature. Since there's a poll interval specified in the `max31855_readTempDone()` function, it will only update and print status and temperature at that rate.

```
/*** main.c ***/

#include "globals.h"

int main(void)
{
    // Globally disable interrupts
    cli();

    // Setup oven, timers, USART, SPI
    oven_setup();
    msTimer_setup();
    usart_setup(BAUD_PRESCALE);

    // Something kinda like OOP in C
    max31855 *m = max31855_setup();

    // Flush USART buffer
    usart_flush();

    // Clear interrupt flag by reading the interrupt register
    // Specify that it's 'unused' so compiler doesn't complain
    uint8_t dummy __attribute__((unused)) = SPSR;
    dummy = SPDR;

    // Turn on global interrupt flag
    sei();

    // "Hello World" startup message
    usart_println("Hot Toaster Action");

    // Main program loop
    for(;;)
    {                
        // "Fade" duty cycle in and out with single for loop
        int i = 0;
        int dir = 1;
        for (i = 0; i > -1; i = i + dir)
        {
            // Control power output
            oven_setDutyCycle(i);

            // Switch direction at peak and pause for 10ms
            if (i == 100) dir = -1;
            msTimer_delay(10);

            // If it's done reading, print the temp and status
            if(max31855_readTempDone(m)) max31855_print(m);
        }
    }

    return 1;
}
```

To finally compile and upload the code to the board, we use _GNU Make_. Make allows you to specify compiler and programmer options with a somewhat cryptic syntax. I borrowed the makefile template from [Pat Deegan at electrons.psychogenic.com](http://electrons.psychogenic.com/modules/arms/view.php?w=art&idx=8&page=2) and modified it to suit my needs. You may need to do the same if your setup differs from mine at all. The main things you should be concerned with are:

```
# Name of target controller
# ...
MCU=atmega328p
```
```
# ID to use with programmer
# ...
PROGRAMMER_MCU=atmega328p
```
```
# Name of our project
# ...
PROJECTNAME=iot-reflow-oven
```
```
# programmer id
# ...
AVRDUDE_PROGRAMMERID=dragon_isp
```
```
# port
# ...
AVRDUDE_PORT=usb
```

Once everything is to your liking, type `make` to compile and `sudo make writeflash` to upload to your board. If everything went according to plan, it should look something like this:

[![Click for video!](images/video-thumb.png)](https://drive.google.com/file/d/0B3jahN8xZtrpVVhKWXJoeHd2cU0/view)
