EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 4250 4050 1900 550 
U 56450D3A
F0 "MAX31855 Temperature Sensor" 60
F1 "temp-sense.sch" 60
$EndSheet
$Sheet
S 4000 2650 2000 450 
U 56450DBB
F0 "ATmega328P Controller" 60
F1 "controller.sch" 60
$EndSheet
$Comp
L CONN_02X04 P?
U 1 1 56450EE0
P 7250 4350
F 0 "P?" H 7250 4600 50  0000 C CNN
F 1 "ESP8266" H 7250 4100 50  0000 C CNN
F 2 "" H 7250 3150 60  0000 C CNN
F 3 "" H 7250 3150 60  0000 C CNN
	1    7250 4350
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X06 P?
U 1 1 56450F26
P 7300 3100
F 0 "P?" H 7300 3450 50  0000 C CNN
F 1 "ICSP" V 7400 3100 50  0000 C CNN
F 2 "" H 7300 3100 60  0000 C CNN
F 3 "" H 7300 3100 60  0000 C CNN
	1    7300 3100
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X04 P?
U 1 1 56450F83
P 6300 1700
F 0 "P?" H 6300 1950 50  0000 C CNN
F 1 "OVEN_GPIO" V 6400 1700 50  0000 C CNN
F 2 "" H 6300 1700 60  0000 C CNN
F 3 "" H 6300 1700 60  0000 C CNN
	1    6300 1700
	1    0    0    -1  
$EndComp
$EndSCHEMATC
