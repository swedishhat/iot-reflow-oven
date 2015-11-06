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
LIBS:kicad-triac-uc-cache
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
S 2800 1000 1300 500 
U 561BC3C5
F0 "Zero Cross Detector" 60
F1 "1-zero-cross.sch" 60
F2 "ZERO_CROSS" O R 4100 1250 60 
F3 "MAINS_L" B L 2800 1150 60 
F4 "MAINS_N" B L 2800 1350 60 
$EndSheet
$Sheet
S 2800 1800 1300 500 
U 561FF7D1
F0 "TRIAC Gate Controller" 60
F1 "triac-gate-ctrl.sch" 60
F2 "TRIAC_ACTIVE" I R 4100 2050 60 
F3 "MAINS_N" B L 2800 2050 60 
F4 "OVEN_L" B L 2800 2150 60 
F5 "MAINS_L" B L 2800 1950 60 
$EndSheet
$Comp
L CONN_01X06 P?
U 1 1 563CC7E0
P 2100 2100
F 0 "P?" H 2100 2450 50  0000 C CNN
F 1 "CONN_01X06" V 2200 2100 50  0000 C CNN
F 2 "" H 2100 2100 60  0000 C CNN
F 3 "" H 2100 2100 60  0000 C CNN
	1    2100 2100
	-1   0    0    -1  
$EndComp
$Comp
L CONN_01X06 P?
U 1 1 563CC881
P 4950 2100
F 0 "P?" H 4950 2450 50  0000 C CNN
F 1 "CONN_01X06" V 5050 2100 50  0000 C CNN
F 2 "" H 4950 2100 60  0000 C CNN
F 3 "" H 4950 2100 60  0000 C CNN
	1    4950 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 1850 2650 1850
Wire Wire Line
	2650 1850 2650 1950
Wire Wire Line
	2650 1950 2800 1950
Wire Wire Line
	2300 2050 2800 2050
Wire Wire Line
	2800 2150 2300 2150
Wire Wire Line
	2300 2250 2450 2250
Wire Wire Line
	2450 2250 2450 2500
Wire Wire Line
	2300 2350 2450 2350
Connection ~ 2450 2350
$Comp
L GND #PWR?
U 1 1 563CD65C
P 2450 2500
F 0 "#PWR?" H 2450 2250 50  0001 C CNN
F 1 "GND" H 2450 2350 50  0000 C CNN
F 2 "" H 2450 2500 60  0000 C CNN
F 3 "" H 2450 2500 60  0000 C CNN
	1    2450 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 1150 2350 1150
Wire Wire Line
	2350 1150 2350 1850
Connection ~ 2350 1850
Wire Wire Line
	2800 1350 2500 1350
Wire Wire Line
	2500 1350 2500 2050
Connection ~ 2500 2050
Wire Wire Line
	2300 1950 2400 1950
Wire Wire Line
	2400 1950 2400 2050
Connection ~ 2400 2050
$Comp
L +3V3 #PWR?
U 1 1 563CDED8
P 4700 1750
F 0 "#PWR?" H 4700 1600 50  0001 C CNN
F 1 "+3V3" H 4700 1890 50  0000 C CNN
F 2 "" H 4700 1750 60  0000 C CNN
F 3 "" H 4700 1750 60  0000 C CNN
	1    4700 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 1850 4700 1850
Wire Wire Line
	4700 1850 4700 1750
$Comp
L GND #PWR?
U 1 1 563CDF70
P 4700 2450
F 0 "#PWR?" H 4700 2200 50  0001 C CNN
F 1 "GND" H 4700 2300 50  0000 C CNN
F 2 "" H 4700 2450 60  0000 C CNN
F 3 "" H 4700 2450 60  0000 C CNN
	1    4700 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 1950 4700 1950
Wire Wire Line
	4700 1950 4700 2450
Wire Wire Line
	4300 2050 4750 2050
Wire Wire Line
	4200 2150 4750 2150
Wire Wire Line
	4100 1250 4300 1250
Wire Wire Line
	4300 1250 4300 2050
Wire Wire Line
	4100 2050 4200 2050
Wire Wire Line
	4200 2050 4200 2150
NoConn ~ 4750 2250
NoConn ~ 4750 2350
$EndSCHEMATC
