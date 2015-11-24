EESchema Schematic File Version 2
LIBS:regul
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
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
LIBS:most-toast-controller-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MAX31855KASA U1
U 1 1 56450D43
P 5350 3300
F 0 "U1" H 5000 3700 50  0000 L CNN
F 1 "MAX31855KASA" H 5450 3700 50  0000 L CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 5350 3300 50  0001 C CIN
F 3 "" H 5350 3300 60  0000 C CNN
	1    5350 3300
	1    0    0    -1  
$EndComp
$Comp
L INDUCTOR L1
U 1 1 564BAB3D
P 4300 3100
F 0 "L1" V 4250 3100 50  0000 C CNN
F 1 "INDUCTOR" V 4400 3100 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 4300 3100 60  0001 C CNN
F 3 "" H 4300 3100 60  0000 C CNN
	1    4300 3100
	0    1    -1   0   
$EndComp
$Comp
L C C6
U 1 1 564BAB96
P 4750 3300
F 0 "C6" H 4775 3400 50  0000 L CNN
F 1 "10nF" H 4775 3200 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 4788 3150 30  0001 C CNN
F 3 "" H 4750 3300 60  0000 C CNN
	1    4750 3300
	-1   0    0    -1  
$EndComp
$Comp
L INDUCTOR L2
U 1 1 564BB062
P 4300 3500
F 0 "L2" V 4250 3500 50  0000 C CNN
F 1 "INDUCTOR" V 4400 3500 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 4300 3500 60  0001 C CNN
F 3 "" H 4300 3500 60  0000 C CNN
	1    4300 3500
	0    1    -1   0   
$EndComp
Wire Wire Line
	4600 3500 4850 3500
Wire Wire Line
	4750 3500 4750 3450
Connection ~ 4750 3500
Wire Wire Line
	4600 3100 4850 3100
Wire Wire Line
	4750 3150 4750 3100
Connection ~ 4750 3100
Text HLabel 3950 3100 0    60   Input ~ 0
TC+
Text HLabel 3950 3500 0    60   Input ~ 0
TC-
$Comp
L C C7
U 1 1 564BB456
P 6050 4000
F 0 "C7" H 6075 4100 50  0000 L CNN
F 1 "0.1uF" H 6075 3900 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 6088 3850 30  0001 C CNN
F 3 "" H 6050 4000 60  0000 C CNN
	1    6050 4000
	-1   0    0    -1  
$EndComp
$Comp
L +3.3V #PWR014
U 1 1 564BB5C4
P 5350 2750
F 0 "#PWR014" H 5350 2600 50  0001 C CNN
F 1 "+3.3V" H 5350 2890 50  0000 C CNN
F 2 "" H 5350 2750 60  0000 C CNN
F 3 "" H 5350 2750 60  0000 C CNN
	1    5350 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 3400 6200 3400
$Comp
L GND #PWR015
U 1 1 564BB67E
P 5350 3850
F 0 "#PWR015" H 5350 3600 50  0001 C CNN
F 1 "GND" H 5350 3700 50  0000 C CNN
F 2 "" H 5350 3850 60  0000 C CNN
F 3 "" H 5350 3850 60  0000 C CNN
	1    5350 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 3800 5350 3850
Wire Wire Line
	5350 2750 5350 2800
$Comp
L +3.3V #PWR016
U 1 1 564BBA4F
P 6050 3800
F 0 "#PWR016" H 6050 3650 50  0001 C CNN
F 1 "+3.3V" H 6050 3940 50  0000 C CNN
F 2 "" H 6050 3800 60  0000 C CNN
F 3 "" H 6050 3800 60  0000 C CNN
	1    6050 3800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR017
U 1 1 564BBA6C
P 6050 4200
F 0 "#PWR017" H 6050 3950 50  0001 C CNN
F 1 "GND" H 6050 4050 50  0000 C CNN
F 2 "" H 6050 4200 60  0000 C CNN
F 3 "" H 6050 4200 60  0000 C CNN
	1    6050 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 4200 6050 4150
Wire Wire Line
	6050 3800 6050 3850
Wire Wire Line
	5850 3100 6200 3100
Wire Wire Line
	5850 3200 6200 3200
Text HLabel 6200 3100 2    60   Input ~ 0
MAX_SCK
Text HLabel 6200 3200 2    60   Output ~ 0
MAX_MISO
Text HLabel 6200 3400 2    60   Input ~ 0
MAX_~CS
Text Notes 3600 4450 0    60   ~ 0
NOTE: 10K pullup resistor on ~CS~ pin omitted\nto use internal pullup on ATmega328P. Be sure\nto set in software
Wire Wire Line
	4000 3500 3950 3500
Wire Wire Line
	4000 3100 3950 3100
$EndSCHEMATC
