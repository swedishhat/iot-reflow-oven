![Good ol' Nick Tesla, Just Chillin'](images/tesla-thumb.jpg)
# Controlling AC Mains with a Microcontroller for Fun and Profit

## Summary
The first part of several on how to put together your own toaster oven controller with a handful of inexpensive, through-hole parts. The emphasis is on safety, low price, and minimal modification of the toaster oven.

## Technical Concepts
* High-voltage circuits
* TRIAC control
* Zero-crossing detectors
* Phase manipulation

## Introduction
The world is getting smaller and so are electronics designs. Through-hole components are less profitable for parts manufacturers and as a result, new products are often times only available in surface mount packages. Being able to solder surface mount parts by hand is a very valuable skill and can come in handy for parts like chip resistors and caps, small outline packages (SOIC/TSSOP), and quad flat packages (QFP). Still, what if the perfect ARM core for your quadcopter controller only comes as a quad-flat no-leads (QFN)? What if that FPGA you need to do vision processing with is only in a ball-grid array (BGA) style? You can take your chances with a hot air gun or you can use a solder reflow oven.

![Oooh shiny... But at what cost!?](images/expensive-reflow-oven.jpg)

_Oooh shiny... But at what cost!?_

Solder reflow ovens are nothing new, nor is making one for yourself out of a standard toaster like this one. The issue is that commercial options are generally very expensive (the one above may cost upwards of $2000 USD) and many home-built options often require the dismantling and modding the toaster which can be error prone and require special tools (my own [Black & Decker Toasr-R-Oven](http://www.amazon.com/gp/product/B00FN3MV88/ref=pd_lpo_sbs_dp_ss_1?pf_rd_p=1944687742&pf_rd_s=lpo-top-stripe-1&pf_rd_t=201&pf_rd_i=B00164O3WU&pf_rd_m=ATVPDKIKX0DER&pf_rd_r=1JG8VGMESTPB12C5T56Z) has security Torx screws in it...). Sparkfun [published a tutorial](https://www.sparkfun.com/commerce/tutorial_info.php?tutorials_id=60) back in 2006, Andy Brown [created a beautiful design](http://andybrown.me.uk/2015/07/12/awreflow2/) on his blog, and even AAC author [Robert Keim](www.allaboutcircuits.com/author/robert-keim) has some tutorials on the basic concepts of oven controlling such as [zero-cross detection](http://www.allaboutcircuits.com/projects/ambient-light-monitor-zero-cross-detection/) and [controlling a TRIAC](http://www.allaboutcircuits.com/projects/ambient-light-monitor-using-a-triac-to-adjust-lamp-brightness/). This tutorial aims to flesh out some of the concepts and provide a different hardware and software approach to this application. It's another “recipe in the cookbook” if you will.

## A Few Notes on Safety
![Danger! Danger! High Voltage!](images/high-voltage-warning.jpg)

_That lightning monster is not messing around..._

A rock climbing instructor jokingly told me once that there are three rules to heed when it comes to safety. In order of priority, they are:

1. Look good.

2. Don't die.

3. If you have to die, look good doing it.

I promise you that you will be in violation of all three if you don't respect how dangerous mains voltages can be. Hundreds of volts are nothing to sneeze at and can cause serious damage if you aren't careful. There are a few things to keep in mind when dealing with high voltages:

* Don't connect high voltages to a breadboard. The risk of wires coming loose or accidentally touching / plugging into the wrong hole on a breadboard is not worth it. Soldering components to perfboard should be OK for prototyping though.

* Absolutely hands off whenever mains are connected. If you have to measure high voltages with your multimeter, don't hold the probes to the board by hand; attach alligator clips and operate the device remotely. Better yet, use an incandescent bulb as a test load and remotely turned on the device with a surge protector switch.

* Above all else, be smart about this. If you don't know what you are doing or don't feel comfortable doing it, ask for help and find someone who does. The AAC forums and local hackerspaces are great resources to leverage.

## System-Level Design
A full oven controller consists of several parts:

![System Flow Chart](images/system-flow.jpg)

_Toaster oven system flowchart_

It's worth mentioning that this is an ad-hoc AC dimmer in that it is designed to controlling __resistive loads only__, like most toaster ovens. Capacitive and inductive loads require some slight modifications (the addition of snubber components) which won't be covered here but information is available online and in the component datasheets. Compact fluorescent lamps (CFLs) have a fairly complicated electronic ballast circuit inside their housing that is not compatible with the AC dimmer at all.

A very common way of implementing an AC controller is with a solid-state relay. These allow the oven to be full ON or full OFF and the signal can be pulsed to get an approximate temperature (known colloquially as BANG-BANG controlling). A huge swath of the world's control systems run perfectly fine on BANG-BANG controllers but it is neither elegant nor super interesting to implement (in my opinion). Inside most solid state relays, however is a device called a TRIAC which can be ordered as a standalone device. Like Robert mentions [in his article](http://www.allaboutcircuits.com/projects/ambient-light-monitor-using-a-triac-to-adjust-lamp-brightness/), it's essentially a bidirectional extension of the thyristor, or can be though of as a solid-state switch that conducts current in both directions.

![The Solid State Relay](images/solid-state-relay.jpg)

_The solid state relay. Bang-bang, baby!_

![The TRIAC](images/triac-glamor-shot.jpg)

_Not as flashy as the SSR but our TRIAC does some super cool stuff_

The whole idea of this oven controller is to use the TRIAC to implement something known as AC  phase control. If you wait for the zero-crossing of the AC waveform and turn the TRIAC at and adjustable about of time later, you are left with an output waveform that retains the same frequency and magnitude of the original waveform for the time that the TRIAC is active. This limits the amount of power to the end device, effectively dimming it. Other methods of dimming exist like [wave packet control](https://de.wikipedia.org/wiki/Schwingungspaketsteuerung) (a sort of synchronous BANG-BANG paradigm; sorry no EN WikiPedia) but they are beyond the scope of this project.

![AC Phase Control in action](images/ac-phase-example.jpg)

_Example of AC phase control from [Andy's Workshop](andybrown.me.uk/wk/2015/07/12/awreflow2/)_

## Plugs, Terminals, and Enclosure
Safe and cheap is the name of the game here so the first order of business in making this was to select a reasonable enclosure. I got an inexpensive plastic bin with a lid and wrote a nice and scary warning label for myself and anyone around. Holes for the the plugs can be cut or drilled in the side but care needs to be taken to keep the material from cracking.

![Inexpensive plastic enclosure with lid and warning](images/enclosure.jpg)

I opted for a plug and socket interface to the dimmer. On the AC mains side, I used a 10A IEC plug with an integrated fuse holder (the fuses I had to source myself) and a power switch, much like this one:

![10A IEC plug with integrated fuse holder and switch](images/iec-plug-fuse-sw.jpg)

_10A IEC plug with integrated fuse holder and switch_

It takes care of over-current protection and allows me to turn the whole thing off without having to unplug the cables. On the toaster side, I just browsed through my local hardware store and came upon this:

![Single Tamper-Resistant Outlet](images/outlet.jpg)

_Single tamper-resistant outlet_

It's a single tamper-resistant outlet that screws into the enclosure. I didn't want to solder the high voltage wires directly to my board so I got a six-position screw terminal block. The positions are for MAINS_L, MAINS_N, OVEN_L, OVEN_N, and two ground wires. I also used one of these terminals for the four microcontroller wires leading outside of the box. A bit overkill but it's what I had on hand.

![Six-position Screw Terminal Block](images/screw-terminal.jpg)

## Isolated Zero-Crossing Detector
When controlling or measuring high voltage circuits with low voltage devices, it's always a good idea to employ some kind of galvanic isolation between the two sides; this can be accomplished either [inductively](https://en.wikipedia.org/wiki/Isolation_transformer), [optically](https://en.wikipedia.org/wiki/Opto-isolator), or  [capacitively](http://www.ti.com/lsds/ti/isolation/digital-isolators-overview.page). There are a few other isolation methods but these are the big ones.

Robert's [TRIAC controller](www.allaboutcircuits.com/projects/ambient-light-monitor-using-a-triac-to-adjust-lamp-brightness/) and [zero-crossing detector](www.allaboutcircuits.com/projects/ambient-light-monitor-zero-cross-detection/) use the transformer in a wall wart to step down the mains to a safer 12V before interacting with it. This application uses opto-isolators to separate the high and low voltages which has the advantage of being far lighter and more compact than a bulky transformer. They're slow to react to fast signal changes compared to some of the other methods but at sub-kilohertz speeds like our application, it doesn't really matter.

![Zero-crossing schematic](images/zero-cross-schem.jpg)

This circuit was [lovingly borrowed from here](http://www.dextrel.net/diyzerocrosser.htm). The author does an excellent job explaining the circuit in detail but a quick rundown goes like this: the mains waveform is first filtered and rectified. It's voltage is divided which then charges the 10uF cap. When the divided voltage drops below the voltage on the capacitor, the comparator transistor turns on, activating the opto-isolator. The output has an [open collector](https://en.wikipedia.org/wiki/Open_collector) which means you can operate it at any VCC your microcontroller supports. My perfboard circuit looks like this:

![Perfboard zero-crossing detector](images/zero-cross-board.jpg)

I tested this circuit in isolation from the rest of the board with a modified power cable and a surge protector. The zero-crossing detector waveform superimposed on an AC sinusoid should look something like this (I used a step-down transformer to get the shot. For the love of God don't hook your mains to your scope!):

![Zero-crossing Detected!](images/zero-cross-scope.jpg)

## TRIAC Driver and Isolated Driver Circuit
Next up is the TRIAC and the isolated driver circuit. I mentioned earlier [Andy Brown's tutorial](http://andybrown.me.uk/wk/2015/07/12/awreflow2/). I adapted his TRIAC protection and driver circuit to work on 120VAC here in the States and followed his thermal considerations for heatsink selection. The TRIAC we're using is the [BTA312](http://www.nxp.com/products/thyristors/3_quadrant_triacs_high_commutation/BTA312-600B.html). We use another opto-isolator to drive the TRIAC called the [MOC310M](https://www.fairchildsemi.com/products/optoelectronics/triac-driver-optocouplers/random-phase-triac-driver/MOC3010M.html) which requires between 30 and 60mA to turn on. Most microcontrollers aren't comfortable sourcing this kind of current so we use a general purpose NPN transistor to provide it.

The schematic looks like this:

![TRIAC Driver Schematic](images/triac-schem.jpg)

VR1 is a varistor. It serves as over-voltage protection in case there's a spike in the AC line. C3 is a 275VAC film cap for emission suppression. That one could be considered optional. The MOC310 driver board circuit looks like this:

![TRIAC Driver Board](images/triac-board.jpg)

The heatsink, TRIAC, varistor, filter cap, and screw terminals all live on a main board separate from the driver. Once everything is attached to the board through headers, it should look something like this:

![Main board annotated](images/board-annotated.jpg)

I used the mounting holes at the corners of the main board to attach it to the enclosure. Once everything is put together you get this:

![Ready to roll!](images/board-enclosure.jpg)

Now you should be ready to roll! Connecting the wires appropriately (), connect up VCC and GND to a bread board power supply, and flip the power switch. If you apply 3.3V to the TRIAC_ACTIVE line, you should get 100% power on the other end.

[VIDEO]

## Next Steps
Okay, I admit it. Turning on a light bulb with a 3.3V switch isn't that impressive. In fact, it's pretty much just a BANG-BANG controller at that point. What we need next is a controller that can measure the zero-cross signal, dim the line accordingly, and read input from a temperature sensor. That will all be covered in the next installment. What's that you ask? Will I use an Arduino? __Absolutely not!__ It may look like that on the outside but we're going to be playing fast and loose with bare-metal C on the Atmega328P. Until next time, happy hacking.
