EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 13
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
S 2900 2850 1150 1600
U 633284AA
F0 "LEDS_1" 59
F1 "leds.sch" 59
F2 "SDA" B L 2900 3050 59 
F3 "SCL" I L 2900 3200 59 
F4 "~INTB" O L 2900 3450 59 
F5 "~SDB" I L 2900 3600 59 
F6 "IICRST" I L 2900 3750 59 
F7 "SYNC" B L 2900 4250 59 
F8 "ADDR" U L 2900 4000 59 
$EndSheet
$Sheet
S 4950 2850 1150 1600
U 633284B3
F0 "LEDS_2" 59
F1 "leds.sch" 59
F2 "SDA" B L 4950 3050 59 
F3 "SCL" I L 4950 3200 59 
F4 "~INTB" O L 4950 3450 59 
F5 "~SDB" I L 4950 3600 59 
F6 "IICRST" I L 4950 3750 59 
F7 "SYNC" B L 4950 4250 59 
F8 "ADDR" U L 4950 4000 59 
$EndSheet
$Sheet
S 7000 2850 1150 1600
U 633284BC
F0 "LEDS_3" 59
F1 "leds.sch" 59
F2 "SDA" B L 7000 3050 59 
F3 "SCL" I L 7000 3200 59 
F4 "~INTB" O L 7000 3450 59 
F5 "~SDB" I L 7000 3600 59 
F6 "IICRST" I L 7000 3750 59 
F7 "SYNC" B L 7000 4250 59 
F8 "ADDR" U L 7000 4000 59 
$EndSheet
$Sheet
S 9050 2850 1150 1600
U 633284C5
F0 "LEDS_4" 59
F1 "leds.sch" 59
F2 "SDA" B L 9050 3050 59 
F3 "SCL" I L 9050 3200 59 
F4 "~INTB" O L 9050 3450 59 
F5 "~SDB" I L 9050 3600 59 
F6 "IICRST" I L 9050 3750 59 
F7 "SYNC" B L 9050 4250 59 
F8 "ADDR" U L 9050 4000 59 
$EndSheet
Wire Wire Line
	8950 3050 9050 3050
Wire Wire Line
	8950 3200 9050 3200
Wire Wire Line
	8950 3450 9050 3450
Wire Wire Line
	8950 3600 9050 3600
Wire Wire Line
	8950 3750 9050 3750
Wire Wire Line
	8950 4250 9050 4250
Wire Wire Line
	6900 3050 7000 3050
Wire Wire Line
	6900 3200 7000 3200
Wire Wire Line
	6900 3450 7000 3450
Wire Wire Line
	6900 3600 7000 3600
Wire Wire Line
	6900 3750 7000 3750
Wire Wire Line
	6900 4250 7000 4250
Wire Wire Line
	4850 3050 4950 3050
Wire Wire Line
	4850 3200 4950 3200
Wire Wire Line
	4850 3600 4950 3600
Wire Wire Line
	4850 3750 4950 3750
Wire Wire Line
	4850 4250 4950 4250
Wire Wire Line
	1550 3050 2550 3050
Wire Wire Line
	1550 3200 2150 3200
Wire Wire Line
	1550 3450 1700 3450
Wire Wire Line
	1550 3600 1700 3600
Wire Wire Line
	1550 3750 2100 3750
Wire Wire Line
	2800 4250 2900 4250
Text HLabel 8950 3050 0    59   BiDi ~ 0
SDA
Text HLabel 8950 3200 0    59   Input ~ 0
SCL
Text HLabel 8950 3600 0    59   Input ~ 0
~SDB
Text HLabel 8950 3750 0    59   Input ~ 0
IIRST
Text HLabel 6900 3050 0    59   BiDi ~ 0
SDA
Text HLabel 6900 3200 0    59   Input ~ 0
SCL
Text HLabel 6900 3600 0    59   Input ~ 0
~SDB
Text HLabel 6900 3750 0    59   Input ~ 0
IIRST
Text HLabel 4850 3050 0    59   BiDi ~ 0
SDA
Text HLabel 4850 3200 0    59   Input ~ 0
SCL
Text HLabel 4850 3600 0    59   Input ~ 0
~SDB
Text HLabel 4850 3750 0    59   Input ~ 0
IIRST
Text HLabel 1550 3050 0    59   BiDi ~ 0
SDA
Text HLabel 1550 3200 0    59   Input ~ 0
SCL
Text HLabel 1550 3450 0    59   Output ~ 0
~INTB
Text HLabel 1550 3600 0    59   Input ~ 0
~SDB
Text HLabel 1550 3750 0    59   Input ~ 0
IIRST
Wire Wire Line
	2800 4250 2800 4650
Wire Wire Line
	2800 4650 4850 4650
Wire Wire Line
	4850 4650 4850 4250
Wire Wire Line
	4850 4650 6900 4650
Wire Wire Line
	6900 4650 6900 4250
Connection ~ 4850 4650
Wire Wire Line
	6900 4650 8950 4650
Wire Wire Line
	8950 4650 8950 4250
Connection ~ 6900 4650
$Comp
L power:GND #PWR0199
U 1 1 6333D298
P 2550 4100
F 0 "#PWR0199" H 2550 3850 50  0001 C CNN
F 1 "GND" H 2555 3927 50  0000 C CNN
F 2 "" H 2550 4100 50  0001 C CNN
F 3 "" H 2550 4100 50  0001 C CNN
	1    2550 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 4100 2550 4000
Wire Wire Line
	2550 4000 2900 4000
Wire Wire Line
	6900 4000 7000 4000
Text HLabel 6900 4000 0    59   BiDi ~ 0
SDA
Wire Wire Line
	8950 4000 9050 4000
Text HLabel 8950 4000 0    59   Input ~ 0
SCL
$Comp
L Device:R_US R48
U 1 1 63350BF3
P 1700 2750
F 0 "R48" H 1768 2796 50  0000 L CNN
F 1 "100k" H 1768 2705 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 1740 2740 50  0001 C CNN
F 3 "~" H 1700 2750 50  0001 C CNN
	1    1700 2750
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R51
U 1 1 63351A7E
P 2150 2750
F 0 "R51" H 2218 2796 50  0000 L CNN
F 1 "1k" H 2218 2705 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2190 2740 50  0001 C CNN
F 3 "~" H 2150 2750 50  0001 C CNN
	1    2150 2750
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R52
U 1 1 63352788
P 2550 2750
F 0 "R52" H 2618 2796 50  0000 L CNN
F 1 "1k" H 2618 2705 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2590 2740 50  0001 C CNN
F 3 "~" H 2550 2750 50  0001 C CNN
	1    2550 2750
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R50
U 1 1 633535B4
P 2100 4050
F 0 "R50" H 2168 4096 50  0000 L CNN
F 1 "100k" H 2168 4005 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2140 4040 50  0001 C CNN
F 3 "~" H 2100 4050 50  0001 C CNN
	1    2100 4050
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R49
U 1 1 6335419C
P 1700 4050
F 0 "R49" H 1768 4096 50  0000 L CNN
F 1 "100k" H 1768 4005 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 1740 4040 50  0001 C CNN
F 3 "~" H 1700 4050 50  0001 C CNN
	1    1700 4050
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0200
U 1 1 633594B9
P 2150 2500
F 0 "#PWR0200" H 2150 2350 50  0001 C CNN
F 1 "+3.3V" H 2165 2673 50  0000 C CNN
F 2 "" H 2150 2500 50  0001 C CNN
F 3 "" H 2150 2500 50  0001 C CNN
	1    2150 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 2500 2150 2550
Wire Wire Line
	2150 2550 2550 2550
Wire Wire Line
	2550 2550 2550 2600
Connection ~ 2150 2550
Wire Wire Line
	2150 2550 2150 2600
Wire Wire Line
	2150 2550 1700 2550
Wire Wire Line
	1700 2550 1700 2600
Wire Wire Line
	2550 2900 2550 3050
Connection ~ 2550 3050
Wire Wire Line
	2550 3050 2900 3050
Wire Wire Line
	2150 2900 2150 3200
Connection ~ 2150 3200
Wire Wire Line
	2150 3200 2900 3200
Wire Wire Line
	1700 2900 1700 3450
Connection ~ 1700 3450
Wire Wire Line
	1700 3450 2900 3450
Wire Wire Line
	2100 3900 2100 3750
Connection ~ 2100 3750
Wire Wire Line
	2100 3750 2900 3750
Wire Wire Line
	1700 3900 1700 3600
Connection ~ 1700 3600
Wire Wire Line
	1700 3600 2900 3600
$Comp
L power:GND #PWR0201
U 1 1 63361D8D
P 1900 4300
F 0 "#PWR0201" H 1900 4050 50  0001 C CNN
F 1 "GND" H 1905 4127 50  0000 C CNN
F 2 "" H 1900 4300 50  0001 C CNN
F 3 "" H 1900 4300 50  0001 C CNN
	1    1900 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 4300 1900 4250
Wire Wire Line
	1900 4250 2100 4250
Wire Wire Line
	2100 4250 2100 4200
Wire Wire Line
	1900 4250 1700 4250
Wire Wire Line
	1700 4250 1700 4200
Connection ~ 1900 4250
Wire Wire Line
	4850 3450 4950 3450
NoConn ~ 8950 3450
NoConn ~ 6900 3450
NoConn ~ 4850 3450
Wire Wire Line
	4350 4000 4950 4000
Wire Wire Line
	4350 3900 4350 4000
$Comp
L power:+3.3V #PWR0202
U 1 1 6333F3C2
P 4350 3900
F 0 "#PWR0202" H 4350 3750 50  0001 C CNN
F 1 "+3.3V" H 4365 4073 50  0000 C CNN
F 2 "" H 4350 3900 50  0001 C CNN
F 3 "" H 4350 3900 50  0001 C CNN
	1    4350 3900
	1    0    0    -1  
$EndComp
Text Notes 5650 1650 0    118  ~ 0
LEDS
$EndSCHEMATC
