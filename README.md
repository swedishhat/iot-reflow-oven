# iot-reflow-oven
DIY solder reflow ovens. Internet of Toasters. So hot right now.

## Branches

### master
This is where milestone-level code lives. A rule of thumb is that it will always be behind or equal to __dev__ branch. I generally merge changes into master after each documentation article is published

### dev
This is where the action is. I try to make sane commits but continue with caution... thar be dragons.

## Folder Hierarchy

### [doc] Documentation Files
* __01_triac-dimmer__: [Control Your AC Mains with a Microcontroller](http://www.allaboutcircuits.com/projects/controlling-ac-mains-with-a-microcontroller-for-fun-and-profit/)
* __02_atmega-controller__: [Designing Your Own Controller for a Solder Reflow Oven]()

### [sw] Software Files

#### atmega328p
C files and headers for oven controller

#### esp8266
Either Arduino or NodeMCU files for the controller's web interface

#### python
Scripts for viewing, logging, and interacting with serial data from oven controller

### [hw] Hardware Files

#### most-toast-controller
Contest entry for [Hackaday's](http://hackaday.com/) [Square Inch Project Challenge](http://hackaday.com/2015/10/02/the-square-inch-project-challenges-your-layout-skills/). Small KiCad board containing ATmega328P, MAX31855, headers for ESP-01, thermocouple connector, voltage regulator, and header for CR2032 battery module.

####triac-dimmer
KiCad files for the TRIAC, TRIAC driver circuit, and zero cross detector.

## Attributions
A good chunk of the ATmega328P was modeled after Andy Brown's Bluetooth enabled reflow oven controller. He used and ATmega8 and wrote his project in C++ but a lot of the code is maps pretty easily to C on an ATmega328P. His tutorial page is located [here](http://andybrown.me.uk/2015/07/12/awreflow2/) and his GitHub repository is located [here](https://github.com/andysworkshop/awreflow2/)

The UART code for the ATmega328P was taken from a library writen by [Andy Gock](https://github.com/andygock/avr-uart). It has the added benefit of being generic across numerous AVR platforms and supports multiple UARTs. The library itself contains code from Peter Fleury and others. Andy's license information can be found [here](https://github.com/andygock/avr-uart/blob/master/LICENSE.txt) and there is more complete information located in the uart.h file.