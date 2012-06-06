EESchema Schematic File Version 2  date Sun 27 May 2012 22:27:00 BST
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
LIBS:special
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
EELAYER 43  0
EELAYER END
$Descr A4 11700 8267
Sheet 1 1
Title ""
Date "27 may 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	4950 3050 4950 3350
Connection ~ 4950 3250
Wire Wire Line
	4950 3750 4950 3850
Wire Wire Line
	5550 3850 5650 3850
Wire Wire Line
	6100 4200 6100 4300
Wire Wire Line
	6100 3050 6100 3500
Wire Wire Line
	6100 4800 6100 4900
Wire Wire Line
	6100 4300 6300 4300
Wire Wire Line
	4950 3250 4800 3250
Wire Wire Line
	4800 3850 5050 3850
Connection ~ 4950 3850
$Comp
L +36V #PWR?
U 1 1 4FC299F9
P 4950 3050
F 0 "#PWR?" H 4950 3140 20  0001 C CNN
F 1 "+36V" H 4950 3140 30  0000 C CNN
	1    4950 3050
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 4FC29901
P 6100 3050
F 0 "#PWR?" H 6100 3140 20  0001 C CNN
F 1 "+5V" H 6100 3140 30  0000 C CNN
	1    6100 3050
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 4FC21A50
P 4950 3550
F 0 "C1" H 5000 3650 50  0000 L CNN
F 1 "100p" H 5000 3450 50  0000 L CNN
	1    4950 3550
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 Can
U 0 1 4FC21847
P 4650 3250
F 0 "Can" H 4730 3250 40  0000 L CNN
F 1 "CONN_1" H 4650 3305 30  0001 C CNN
	0    4650 3250
	-1   0    0    -1  
$EndComp
$Comp
L CONN_1 Wire
U 0 1 4FC2183B
P 4650 3850
F 0 "Wire" H 4730 3850 40  0000 L CNN
F 1 "CONN_1" H 4650 3905 30  0001 C CNN
	0    4650 3850
	-1   0    0    1   
$EndComp
$Comp
L CONN_1 Nanode
U 0 1 4FC21812
P 6450 4300
F 0 "Nanode A0" H 6530 4300 40  0000 L CNN
F 1 "CONN_1" H 6450 4355 30  0001 C CNN
	0    6450 4300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4FC216AF
P 6100 4900
F 0 "#PWR?" H 6100 4900 30  0001 C CNN
F 1 "GND" H 6100 4830 30  0001 C CNN
	1    6100 4900
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 4FC21653
P 6100 4550
F 0 "R2" V 6180 4550 50  0000 C CNN
F 1 "47K" V 6100 4550 50  0000 C CNN
	1    6100 4550
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 4FC21560
P 5300 3850
F 0 "R1" V 5380 3850 50  0000 C CNN
F 1 "1M" V 5300 3850 50  0000 C CNN
	1    5300 3850
	0    1    1    0   
$EndComp
$Comp
L FET_N Q1
U 1 1 4FC214BB
P 5950 3850
F 0 "Q1" H 5853 4100 70  0000 C CNN
F 1 "PN4117" H 5803 3603 60  0000 C CNN
	1    5950 3850
	1    0    0    -1  
$EndComp
$EndSCHEMATC
