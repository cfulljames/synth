EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 13 13
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
L Device:Rotary_Encoder_Switch SW1
U 1 1 5F550CB8
P 2250 3350
F 0 "SW1" H 2250 3717 50  0000 C CNN
F 1 "Encoder" H 2250 3626 50  0000 C CNN
F 2 "" H 2100 3510 50  0001 C CNN
F 3 "~" H 2250 3610 50  0001 C CNN
	1    2250 3350
	1    0    0    -1  
$EndComp
$Comp
L Device:Rotary_Encoder_Switch SW5
U 1 1 5F555710
P 3350 3350
F 0 "SW5" H 3350 3717 50  0000 C CNN
F 1 "Encoder" H 3350 3626 50  0000 C CNN
F 2 "" H 3200 3510 50  0001 C CNN
F 3 "~" H 3350 3610 50  0001 C CNN
	1    3350 3350
	1    0    0    -1  
$EndComp
$Comp
L Device:Rotary_Encoder_Switch SW9
U 1 1 5F55A920
P 4450 3350
F 0 "SW9" H 4450 3717 50  0000 C CNN
F 1 "Encoder" H 4450 3626 50  0000 C CNN
F 2 "" H 4300 3510 50  0001 C CNN
F 3 "~" H 4450 3610 50  0001 C CNN
	1    4450 3350
	1    0    0    -1  
$EndComp
$Comp
L Device:Rotary_Encoder_Switch SW13
U 1 1 5F55A926
P 5550 3350
F 0 "SW13" H 5550 3717 50  0000 C CNN
F 1 "Encoder" H 5550 3626 50  0000 C CNN
F 2 "" H 5400 3510 50  0001 C CNN
F 3 "~" H 5550 3610 50  0001 C CNN
	1    5550 3350
	1    0    0    -1  
$EndComp
Text HLabel 1750 1750 1    59   Output ~ 0
ENC_1_A
Text HLabel 1900 1750 1    59   Output ~ 0
ENC_1_B
Text HLabel 2050 1750 1    59   Output ~ 0
ENC_1_SW
Text HLabel 1350 3650 0    59   Input ~ 0
~ROW_SEL_1
Wire Wire Line
	5850 3450 5900 3450
Wire Wire Line
	5900 3450 5900 3650
Wire Wire Line
	1900 3450 1950 3450
Wire Wire Line
	1750 3250 1950 3250
Wire Wire Line
	2650 3250 2550 3250
Wire Wire Line
	2050 2800 2650 2800
Wire Wire Line
	2650 2800 2650 3250
Text HLabel 2850 1750 1    59   Output ~ 0
ENC_2_A
Text HLabel 3000 1750 1    59   Output ~ 0
ENC_2_B
Text HLabel 3150 1750 1    59   Output ~ 0
ENC_2_SW
Wire Wire Line
	3000 1750 3000 2250
Wire Wire Line
	2850 1750 2850 1900
Wire Wire Line
	3150 1750 3150 2600
Wire Wire Line
	3050 3250 2850 3250
Wire Wire Line
	3000 3450 3050 3450
Wire Wire Line
	3150 2800 3750 2800
Wire Wire Line
	3750 2800 3750 3250
Wire Wire Line
	3750 3250 3650 3250
Text HLabel 3950 1750 1    59   Output ~ 0
ENC_3_A
Text HLabel 4100 1750 1    59   Output ~ 0
ENC_3_B
Text HLabel 4250 1750 1    59   Output ~ 0
ENC_3_SW
Wire Wire Line
	4100 1750 4100 2250
Wire Wire Line
	3950 1750 3950 1900
Wire Wire Line
	4250 1750 4250 2600
Wire Wire Line
	4100 3450 4150 3450
Wire Wire Line
	4150 3250 3950 3250
Wire Wire Line
	4250 2800 4850 2800
Wire Wire Line
	4850 2800 4850 3250
Wire Wire Line
	4850 3250 4750 3250
Text HLabel 5050 1750 1    59   Output ~ 0
ENC_4_A
Text HLabel 5200 1750 1    59   Output ~ 0
ENC_4_B
Text HLabel 5350 1750 1    59   Output ~ 0
ENC_4_SW
Wire Wire Line
	5200 1750 5200 2250
Wire Wire Line
	5050 1750 5050 1900
Wire Wire Line
	5350 1750 5350 2600
Wire Wire Line
	5200 3450 5250 3450
Wire Wire Line
	5050 3250 5250 3250
Wire Wire Line
	5350 2800 5950 2800
Wire Wire Line
	5950 2800 5950 3250
Wire Wire Line
	5950 3250 5850 3250
$Comp
L Device:R_US R57
U 1 1 5F575AAE
P 2300 1900
F 0 "R57" V 2095 1900 50  0000 C CNN
F 1 "10k" V 2186 1900 50  0000 C CNN
F 2 "" V 2340 1890 50  0001 C CNN
F 3 "~" H 2300 1900 50  0001 C CNN
	1    2300 1900
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R58
U 1 1 5F5773B4
P 2300 2250
F 0 "R58" V 2095 2250 50  0000 C CNN
F 1 "10k" V 2186 2250 50  0000 C CNN
F 2 "" V 2340 2240 50  0001 C CNN
F 3 "~" H 2300 2250 50  0001 C CNN
	1    2300 2250
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R59
U 1 1 5F579C00
P 2300 2600
F 0 "R59" V 2095 2600 50  0000 C CNN
F 1 "10k" V 2186 2600 50  0000 C CNN
F 2 "" V 2340 2590 50  0001 C CNN
F 3 "~" H 2300 2600 50  0001 C CNN
	1    2300 2600
	0    1    1    0   
$EndComp
$Comp
L power:+3.3V #PWR0251
U 1 1 5F57A807
P 2500 1800
F 0 "#PWR0251" H 2500 1650 50  0001 C CNN
F 1 "+3.3V" H 2515 1973 50  0000 C CNN
F 2 "" H 2500 1800 50  0001 C CNN
F 3 "" H 2500 1800 50  0001 C CNN
	1    2500 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 1800 2500 1900
Wire Wire Line
	2500 1900 2450 1900
Wire Wire Line
	2500 1900 2500 2250
Wire Wire Line
	2500 2250 2450 2250
Connection ~ 2500 1900
Wire Wire Line
	2500 2250 2500 2600
Wire Wire Line
	2500 2600 2450 2600
Connection ~ 2500 2250
Wire Wire Line
	2050 1750 2050 2600
Wire Wire Line
	1900 1750 1900 2250
Wire Wire Line
	1750 1750 1750 1900
Wire Wire Line
	2150 1900 1750 1900
Connection ~ 1750 1900
Wire Wire Line
	1750 1900 1750 3250
Wire Wire Line
	2150 2250 1900 2250
Connection ~ 1900 2250
Wire Wire Line
	1900 2250 1900 3450
Wire Wire Line
	2150 2600 2050 2600
Connection ~ 2050 2600
Wire Wire Line
	2050 2600 2050 2800
$Comp
L Device:R_US R60
U 1 1 5F586A2C
P 3400 1900
F 0 "R60" V 3195 1900 50  0000 C CNN
F 1 "10k" V 3286 1900 50  0000 C CNN
F 2 "" V 3440 1890 50  0001 C CNN
F 3 "~" H 3400 1900 50  0001 C CNN
	1    3400 1900
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R61
U 1 1 5F586A32
P 3400 2250
F 0 "R61" V 3195 2250 50  0000 C CNN
F 1 "10k" V 3286 2250 50  0000 C CNN
F 2 "" V 3440 2240 50  0001 C CNN
F 3 "~" H 3400 2250 50  0001 C CNN
	1    3400 2250
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R62
U 1 1 5F586A38
P 3400 2600
F 0 "R62" V 3195 2600 50  0000 C CNN
F 1 "10k" V 3286 2600 50  0000 C CNN
F 2 "" V 3440 2590 50  0001 C CNN
F 3 "~" H 3400 2600 50  0001 C CNN
	1    3400 2600
	0    1    1    0   
$EndComp
$Comp
L power:+3.3V #PWR0252
U 1 1 5F586A3E
P 3600 1800
F 0 "#PWR0252" H 3600 1650 50  0001 C CNN
F 1 "+3.3V" H 3615 1973 50  0000 C CNN
F 2 "" H 3600 1800 50  0001 C CNN
F 3 "" H 3600 1800 50  0001 C CNN
	1    3600 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 1800 3600 1900
Wire Wire Line
	3600 1900 3550 1900
Wire Wire Line
	3600 1900 3600 2250
Wire Wire Line
	3600 2250 3550 2250
Connection ~ 3600 1900
Wire Wire Line
	3600 2250 3600 2600
Wire Wire Line
	3600 2600 3550 2600
Connection ~ 3600 2250
Wire Wire Line
	3250 1900 2850 1900
Wire Wire Line
	3250 2250 3000 2250
Wire Wire Line
	3250 2600 3150 2600
$Comp
L Device:R_US R63
U 1 1 5F5898E9
P 4500 1900
F 0 "R63" V 4295 1900 50  0000 C CNN
F 1 "10k" V 4386 1900 50  0000 C CNN
F 2 "" V 4540 1890 50  0001 C CNN
F 3 "~" H 4500 1900 50  0001 C CNN
	1    4500 1900
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R64
U 1 1 5F5898EF
P 4500 2250
F 0 "R64" V 4295 2250 50  0000 C CNN
F 1 "10k" V 4386 2250 50  0000 C CNN
F 2 "" V 4540 2240 50  0001 C CNN
F 3 "~" H 4500 2250 50  0001 C CNN
	1    4500 2250
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R65
U 1 1 5F5898F5
P 4500 2600
F 0 "R65" V 4295 2600 50  0000 C CNN
F 1 "10k" V 4386 2600 50  0000 C CNN
F 2 "" V 4540 2590 50  0001 C CNN
F 3 "~" H 4500 2600 50  0001 C CNN
	1    4500 2600
	0    1    1    0   
$EndComp
$Comp
L power:+3.3V #PWR0253
U 1 1 5F5898FB
P 4700 1800
F 0 "#PWR0253" H 4700 1650 50  0001 C CNN
F 1 "+3.3V" H 4715 1973 50  0000 C CNN
F 2 "" H 4700 1800 50  0001 C CNN
F 3 "" H 4700 1800 50  0001 C CNN
	1    4700 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 1800 4700 1900
Wire Wire Line
	4700 1900 4650 1900
Wire Wire Line
	4700 1900 4700 2250
Wire Wire Line
	4700 2250 4650 2250
Connection ~ 4700 1900
Wire Wire Line
	4700 2250 4700 2600
Wire Wire Line
	4700 2600 4650 2600
Connection ~ 4700 2250
Wire Wire Line
	4350 1900 3950 1900
Wire Wire Line
	4350 2250 4100 2250
Wire Wire Line
	4350 2600 4250 2600
$Comp
L Device:R_US R66
U 1 1 5F58BF1D
P 5600 1900
F 0 "R66" V 5395 1900 50  0000 C CNN
F 1 "10k" V 5486 1900 50  0000 C CNN
F 2 "" V 5640 1890 50  0001 C CNN
F 3 "~" H 5600 1900 50  0001 C CNN
	1    5600 1900
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R67
U 1 1 5F58BF23
P 5600 2250
F 0 "R67" V 5395 2250 50  0000 C CNN
F 1 "10k" V 5486 2250 50  0000 C CNN
F 2 "" V 5640 2240 50  0001 C CNN
F 3 "~" H 5600 2250 50  0001 C CNN
	1    5600 2250
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R68
U 1 1 5F58BF29
P 5600 2600
F 0 "R68" V 5395 2600 50  0000 C CNN
F 1 "10k" V 5486 2600 50  0000 C CNN
F 2 "" V 5640 2590 50  0001 C CNN
F 3 "~" H 5600 2600 50  0001 C CNN
	1    5600 2600
	0    1    1    0   
$EndComp
$Comp
L power:+3.3V #PWR0254
U 1 1 5F58BF2F
P 5800 1800
F 0 "#PWR0254" H 5800 1650 50  0001 C CNN
F 1 "+3.3V" H 5815 1973 50  0000 C CNN
F 2 "" H 5800 1800 50  0001 C CNN
F 3 "" H 5800 1800 50  0001 C CNN
	1    5800 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 1800 5800 1900
Wire Wire Line
	5800 1900 5750 1900
Wire Wire Line
	5800 1900 5800 2250
Wire Wire Line
	5800 2250 5750 2250
Connection ~ 5800 1900
Wire Wire Line
	5800 2250 5800 2600
Wire Wire Line
	5800 2600 5750 2600
Connection ~ 5800 2250
Wire Wire Line
	5450 1900 5050 1900
Wire Wire Line
	5450 2250 5200 2250
Wire Wire Line
	5450 2600 5350 2600
Connection ~ 5350 2600
Wire Wire Line
	5350 2600 5350 2800
Connection ~ 5200 2250
Wire Wire Line
	5200 2250 5200 3450
Connection ~ 5050 1900
Wire Wire Line
	5050 1900 5050 3250
Connection ~ 3950 1900
Wire Wire Line
	3950 1900 3950 3250
Connection ~ 4100 2250
Wire Wire Line
	4100 2250 4100 3450
Connection ~ 4250 2600
Wire Wire Line
	4250 2600 4250 2800
Connection ~ 3150 2600
Wire Wire Line
	3150 2600 3150 2800
Connection ~ 3000 2250
Wire Wire Line
	3000 2250 3000 3450
Connection ~ 2850 1900
Wire Wire Line
	2850 1900 2850 3250
Wire Wire Line
	2550 3450 2600 3450
Wire Wire Line
	2600 3450 2600 3650
Connection ~ 2600 3650
Wire Wire Line
	3650 3450 3700 3450
Wire Wire Line
	3700 3450 3700 3650
Connection ~ 3700 3650
Wire Wire Line
	4750 3450 4800 3450
Wire Wire Line
	4800 3450 4800 3650
Connection ~ 4800 3650
Wire Wire Line
	4800 3650 5100 3650
Wire Wire Line
	5900 3650 5100 3650
Connection ~ 5100 3650
Wire Wire Line
	5100 3350 5250 3350
Wire Wire Line
	5100 3350 5100 3650
Wire Wire Line
	3700 3650 4000 3650
Wire Wire Line
	4000 3650 4800 3650
Connection ~ 4000 3650
Wire Wire Line
	4000 3350 4150 3350
Wire Wire Line
	4000 3350 4000 3650
Wire Wire Line
	2600 3650 2900 3650
Wire Wire Line
	2900 3650 3700 3650
Connection ~ 2900 3650
Wire Wire Line
	2900 3350 3050 3350
Wire Wire Line
	2900 3350 2900 3650
Wire Wire Line
	1800 3350 1950 3350
Wire Wire Line
	1800 3650 2600 3650
Connection ~ 1800 3650
Wire Wire Line
	1350 3650 1800 3650
Wire Wire Line
	1800 3650 1800 3350
$Comp
L Device:Rotary_Encoder_Switch SW2
U 1 1 5F618CA9
P 2250 4150
F 0 "SW2" H 2250 4517 50  0000 C CNN
F 1 "Encoder" H 2250 4426 50  0000 C CNN
F 2 "" H 2100 4310 50  0001 C CNN
F 3 "~" H 2250 4410 50  0001 C CNN
	1    2250 4150
	1    0    0    -1  
$EndComp
$Comp
L Device:Rotary_Encoder_Switch SW6
U 1 1 5F618CAF
P 3350 4150
F 0 "SW6" H 3350 4517 50  0000 C CNN
F 1 "Encoder" H 3350 4426 50  0000 C CNN
F 2 "" H 3200 4310 50  0001 C CNN
F 3 "~" H 3350 4410 50  0001 C CNN
	1    3350 4150
	1    0    0    -1  
$EndComp
$Comp
L Device:Rotary_Encoder_Switch SW10
U 1 1 5F618CB5
P 4450 4150
F 0 "SW10" H 4450 4517 50  0000 C CNN
F 1 "Encoder" H 4450 4426 50  0000 C CNN
F 2 "" H 4300 4310 50  0001 C CNN
F 3 "~" H 4450 4410 50  0001 C CNN
	1    4450 4150
	1    0    0    -1  
$EndComp
$Comp
L Device:Rotary_Encoder_Switch SW14
U 1 1 5F618CBB
P 5550 4150
F 0 "SW14" H 5550 4517 50  0000 C CNN
F 1 "Encoder" H 5550 4426 50  0000 C CNN
F 2 "" H 5400 4310 50  0001 C CNN
F 3 "~" H 5550 4410 50  0001 C CNN
	1    5550 4150
	1    0    0    -1  
$EndComp
Text HLabel 1350 4450 0    59   Input ~ 0
~ROW_SEL_2
Wire Wire Line
	5850 4250 5900 4250
Wire Wire Line
	5900 4250 5900 4450
Wire Wire Line
	1900 4250 1950 4250
Wire Wire Line
	1750 4050 1950 4050
Wire Wire Line
	2650 4050 2550 4050
Wire Wire Line
	3050 4050 2850 4050
Wire Wire Line
	3000 4250 3050 4250
Wire Wire Line
	3750 4050 3650 4050
Wire Wire Line
	4100 4250 4150 4250
Wire Wire Line
	4150 4050 3950 4050
Wire Wire Line
	4850 4050 4750 4050
Wire Wire Line
	5200 4250 5250 4250
Wire Wire Line
	5050 4050 5250 4050
Wire Wire Line
	5950 4050 5850 4050
Wire Wire Line
	2550 4250 2600 4250
Wire Wire Line
	2600 4250 2600 4450
Connection ~ 2600 4450
Wire Wire Line
	3650 4250 3700 4250
Wire Wire Line
	3700 4250 3700 4450
Connection ~ 3700 4450
Wire Wire Line
	4750 4250 4800 4250
Wire Wire Line
	4800 4250 4800 4450
Connection ~ 4800 4450
Wire Wire Line
	4800 4450 5100 4450
Wire Wire Line
	5900 4450 5100 4450
Connection ~ 5100 4450
Wire Wire Line
	5100 4150 5250 4150
Wire Wire Line
	5100 4150 5100 4450
Wire Wire Line
	3700 4450 4000 4450
Wire Wire Line
	4000 4450 4800 4450
Connection ~ 4000 4450
Wire Wire Line
	4000 4150 4150 4150
Wire Wire Line
	4000 4150 4000 4450
Wire Wire Line
	2600 4450 2900 4450
Wire Wire Line
	2900 4450 3700 4450
Connection ~ 2900 4450
Wire Wire Line
	2900 4150 3050 4150
Wire Wire Line
	2900 4150 2900 4450
Wire Wire Line
	1800 4150 1950 4150
Wire Wire Line
	1800 4450 2600 4450
Connection ~ 1800 4450
Wire Wire Line
	1350 4450 1800 4450
Wire Wire Line
	1800 4450 1800 4150
$Comp
L Device:Rotary_Encoder_Switch SW3
U 1 1 5F63B24E
P 2250 4950
F 0 "SW3" H 2250 5317 50  0000 C CNN
F 1 "Encoder" H 2250 5226 50  0000 C CNN
F 2 "" H 2100 5110 50  0001 C CNN
F 3 "~" H 2250 5210 50  0001 C CNN
	1    2250 4950
	1    0    0    -1  
$EndComp
$Comp
L Device:Rotary_Encoder_Switch SW7
U 1 1 5F63B254
P 3350 4950
F 0 "SW7" H 3350 5317 50  0000 C CNN
F 1 "Encoder" H 3350 5226 50  0000 C CNN
F 2 "" H 3200 5110 50  0001 C CNN
F 3 "~" H 3350 5210 50  0001 C CNN
	1    3350 4950
	1    0    0    -1  
$EndComp
$Comp
L Device:Rotary_Encoder_Switch SW11
U 1 1 5F63B25A
P 4450 4950
F 0 "SW11" H 4450 5317 50  0000 C CNN
F 1 "Encoder" H 4450 5226 50  0000 C CNN
F 2 "" H 4300 5110 50  0001 C CNN
F 3 "~" H 4450 5210 50  0001 C CNN
	1    4450 4950
	1    0    0    -1  
$EndComp
$Comp
L Device:Rotary_Encoder_Switch SW15
U 1 1 5F63B260
P 5550 4950
F 0 "SW15" H 5550 5317 50  0000 C CNN
F 1 "Encoder" H 5550 5226 50  0000 C CNN
F 2 "" H 5400 5110 50  0001 C CNN
F 3 "~" H 5550 5210 50  0001 C CNN
	1    5550 4950
	1    0    0    -1  
$EndComp
Text HLabel 1350 5250 0    59   Input ~ 0
~ROW_SEL_3
Wire Wire Line
	5850 5050 5900 5050
Wire Wire Line
	5900 5050 5900 5250
Wire Wire Line
	1900 5050 1950 5050
Wire Wire Line
	1750 4850 1950 4850
Wire Wire Line
	2650 4850 2550 4850
Wire Wire Line
	3050 4850 2850 4850
Wire Wire Line
	3000 5050 3050 5050
Wire Wire Line
	3750 4850 3650 4850
Wire Wire Line
	4100 5050 4150 5050
Wire Wire Line
	4150 4850 3950 4850
Wire Wire Line
	4850 4850 4750 4850
Wire Wire Line
	5200 5050 5250 5050
Wire Wire Line
	5050 4850 5250 4850
Wire Wire Line
	5950 4850 5850 4850
Wire Wire Line
	2550 5050 2600 5050
Wire Wire Line
	2600 5050 2600 5250
Connection ~ 2600 5250
Wire Wire Line
	3650 5050 3700 5050
Wire Wire Line
	3700 5050 3700 5250
Connection ~ 3700 5250
Wire Wire Line
	4750 5050 4800 5050
Wire Wire Line
	4800 5050 4800 5250
Connection ~ 4800 5250
Wire Wire Line
	4800 5250 5100 5250
Wire Wire Line
	5900 5250 5100 5250
Connection ~ 5100 5250
Wire Wire Line
	5100 4950 5250 4950
Wire Wire Line
	5100 4950 5100 5250
Wire Wire Line
	3700 5250 4000 5250
Wire Wire Line
	4000 5250 4800 5250
Connection ~ 4000 5250
Wire Wire Line
	4000 4950 4150 4950
Wire Wire Line
	4000 4950 4000 5250
Wire Wire Line
	2600 5250 2900 5250
Wire Wire Line
	2900 5250 3700 5250
Connection ~ 2900 5250
Wire Wire Line
	2900 4950 3050 4950
Wire Wire Line
	2900 4950 2900 5250
Wire Wire Line
	1800 4950 1950 4950
Wire Wire Line
	1800 5250 2600 5250
Connection ~ 1800 5250
Wire Wire Line
	1350 5250 1800 5250
Wire Wire Line
	1800 5250 1800 4950
$Comp
L Device:Rotary_Encoder_Switch SW4
U 1 1 5F64AEAA
P 2250 5750
F 0 "SW4" H 2250 6117 50  0000 C CNN
F 1 "Encoder" H 2250 6026 50  0000 C CNN
F 2 "" H 2100 5910 50  0001 C CNN
F 3 "~" H 2250 6010 50  0001 C CNN
	1    2250 5750
	1    0    0    -1  
$EndComp
$Comp
L Device:Rotary_Encoder_Switch SW8
U 1 1 5F64AEB0
P 3350 5750
F 0 "SW8" H 3350 6117 50  0000 C CNN
F 1 "Encoder" H 3350 6026 50  0000 C CNN
F 2 "" H 3200 5910 50  0001 C CNN
F 3 "~" H 3350 6010 50  0001 C CNN
	1    3350 5750
	1    0    0    -1  
$EndComp
$Comp
L Device:Rotary_Encoder_Switch SW12
U 1 1 5F64AEB6
P 4450 5750
F 0 "SW12" H 4450 6117 50  0000 C CNN
F 1 "Encoder" H 4450 6026 50  0000 C CNN
F 2 "" H 4300 5910 50  0001 C CNN
F 3 "~" H 4450 6010 50  0001 C CNN
	1    4450 5750
	1    0    0    -1  
$EndComp
$Comp
L Device:Rotary_Encoder_Switch SW16
U 1 1 5F64AEBC
P 5550 5750
F 0 "SW16" H 5550 6117 50  0000 C CNN
F 1 "Encoder" H 5550 6026 50  0000 C CNN
F 2 "" H 5400 5910 50  0001 C CNN
F 3 "~" H 5550 6010 50  0001 C CNN
	1    5550 5750
	1    0    0    -1  
$EndComp
Text HLabel 1350 6050 0    59   Input ~ 0
~ROW_SEL_4
Wire Wire Line
	5850 5850 5900 5850
Wire Wire Line
	5900 5850 5900 6050
Wire Wire Line
	1900 5850 1950 5850
Wire Wire Line
	1750 5650 1950 5650
Wire Wire Line
	2650 5650 2550 5650
Wire Wire Line
	3050 5650 2850 5650
Wire Wire Line
	3000 5850 3050 5850
Wire Wire Line
	3750 5650 3650 5650
Wire Wire Line
	4100 5850 4150 5850
Wire Wire Line
	4150 5650 3950 5650
Wire Wire Line
	4850 5650 4750 5650
Wire Wire Line
	5200 5850 5250 5850
Wire Wire Line
	5050 5650 5250 5650
Wire Wire Line
	5950 5650 5850 5650
Wire Wire Line
	2550 5850 2600 5850
Wire Wire Line
	2600 5850 2600 6050
Connection ~ 2600 6050
Wire Wire Line
	3650 5850 3700 5850
Wire Wire Line
	3700 5850 3700 6050
Connection ~ 3700 6050
Wire Wire Line
	4750 5850 4800 5850
Wire Wire Line
	4800 5850 4800 6050
Connection ~ 4800 6050
Wire Wire Line
	4800 6050 5100 6050
Wire Wire Line
	5900 6050 5100 6050
Connection ~ 5100 6050
Wire Wire Line
	5100 5750 5250 5750
Wire Wire Line
	5100 5750 5100 6050
Wire Wire Line
	3700 6050 4000 6050
Wire Wire Line
	4000 6050 4800 6050
Connection ~ 4000 6050
Wire Wire Line
	4000 5750 4150 5750
Wire Wire Line
	4000 5750 4000 6050
Wire Wire Line
	2600 6050 2900 6050
Wire Wire Line
	2900 6050 3700 6050
Connection ~ 2900 6050
Wire Wire Line
	2900 5750 3050 5750
Wire Wire Line
	2900 5750 2900 6050
Wire Wire Line
	1800 5750 1950 5750
Wire Wire Line
	1800 6050 2600 6050
Connection ~ 1800 6050
Wire Wire Line
	1350 6050 1800 6050
Wire Wire Line
	1800 6050 1800 5750
Wire Wire Line
	1750 3250 1750 4050
Connection ~ 1750 3250
Connection ~ 1750 4050
Wire Wire Line
	1750 4050 1750 4850
Connection ~ 1750 4850
Wire Wire Line
	1750 4850 1750 5650
Wire Wire Line
	2650 5650 2650 4850
Connection ~ 2650 3250
Connection ~ 2650 4050
Wire Wire Line
	2650 4050 2650 3250
Connection ~ 2650 4850
Wire Wire Line
	2650 4850 2650 4050
Wire Wire Line
	2850 3250 2850 4050
Connection ~ 2850 3250
Connection ~ 2850 4050
Wire Wire Line
	2850 4050 2850 4850
Connection ~ 2850 4850
Wire Wire Line
	2850 4850 2850 5650
Wire Wire Line
	1900 3450 1900 4250
Connection ~ 1900 3450
Connection ~ 1900 4250
Wire Wire Line
	1900 4250 1900 5050
Connection ~ 1900 5050
Wire Wire Line
	1900 5050 1900 5850
Wire Wire Line
	3000 5850 3000 5050
Connection ~ 3000 3450
Connection ~ 3000 4250
Wire Wire Line
	3000 4250 3000 3450
Connection ~ 3000 5050
Wire Wire Line
	3000 5050 3000 4250
Wire Wire Line
	3750 3250 3750 4050
Connection ~ 3750 3250
Connection ~ 3750 4050
Wire Wire Line
	3750 4050 3750 4850
Connection ~ 3750 4850
Wire Wire Line
	3750 4850 3750 5650
Wire Wire Line
	3950 5650 3950 4850
Connection ~ 3950 3250
Connection ~ 3950 4050
Wire Wire Line
	3950 4050 3950 3250
Connection ~ 3950 4850
Wire Wire Line
	3950 4850 3950 4050
Wire Wire Line
	4100 3450 4100 4250
Connection ~ 4100 3450
Connection ~ 4100 4250
Wire Wire Line
	4100 4250 4100 5050
Connection ~ 4100 5050
Wire Wire Line
	4100 5050 4100 5850
Wire Wire Line
	4850 5650 4850 4850
Connection ~ 4850 3250
Connection ~ 4850 4050
Wire Wire Line
	4850 4050 4850 3250
Connection ~ 4850 4850
Wire Wire Line
	4850 4850 4850 4050
Wire Wire Line
	5050 3250 5050 4050
Connection ~ 5050 3250
Connection ~ 5050 4050
Wire Wire Line
	5050 4050 5050 4850
Connection ~ 5050 4850
Wire Wire Line
	5050 4850 5050 5650
Wire Wire Line
	5200 5850 5200 5050
Connection ~ 5200 3450
Connection ~ 5200 4250
Wire Wire Line
	5200 4250 5200 3450
Connection ~ 5200 5050
Wire Wire Line
	5200 5050 5200 4250
Wire Wire Line
	5950 3250 5950 4050
Connection ~ 5950 3250
Connection ~ 5950 4050
Wire Wire Line
	5950 4050 5950 4850
Connection ~ 5950 4850
Wire Wire Line
	5950 4850 5950 5650
$Comp
L Switch:SW_SPST SW17
U 1 1 5F765837
P 6900 3350
F 0 "SW17" H 6900 3585 50  0000 C CNN
F 1 "SW_SPST" H 6900 3494 50  0000 C CNN
F 2 "" H 6900 3350 50  0001 C CNN
F 3 "~" H 6900 3350 50  0001 C CNN
	1    6900 3350
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW21
U 1 1 5F766672
P 7850 3350
F 0 "SW21" H 7850 3585 50  0000 C CNN
F 1 "SW_SPST" H 7850 3494 50  0000 C CNN
F 2 "" H 7850 3350 50  0001 C CNN
F 3 "~" H 7850 3350 50  0001 C CNN
	1    7850 3350
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW25
U 1 1 5F777B9C
P 8900 3350
F 0 "SW25" H 8900 3585 50  0000 C CNN
F 1 "SW_SPST" H 8900 3494 50  0000 C CNN
F 2 "" H 8900 3350 50  0001 C CNN
F 3 "~" H 8900 3350 50  0001 C CNN
	1    8900 3350
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW29
U 1 1 5F777BA2
P 9900 3350
F 0 "SW29" H 9900 3585 50  0000 C CNN
F 1 "SW_SPST" H 9900 3494 50  0000 C CNN
F 2 "" H 9900 3350 50  0001 C CNN
F 3 "~" H 9900 3350 50  0001 C CNN
	1    9900 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 3650 6650 3650
Wire Wire Line
	6650 3650 6650 3350
Wire Wire Line
	6650 3350 6700 3350
Connection ~ 5900 3650
Wire Wire Line
	6650 3650 7600 3650
Wire Wire Line
	7600 3650 7600 3350
Wire Wire Line
	7600 3350 7650 3350
Connection ~ 6650 3650
Wire Wire Line
	7600 3650 8650 3650
Wire Wire Line
	8650 3650 8650 3350
Wire Wire Line
	8650 3350 8700 3350
Connection ~ 7600 3650
Wire Wire Line
	8650 3650 9650 3650
Wire Wire Line
	9650 3650 9650 3350
Wire Wire Line
	9650 3350 9700 3350
Connection ~ 8650 3650
Text HLabel 7150 2600 1    59   Output ~ 0
BTN_1
Wire Wire Line
	7150 2600 7150 2800
Wire Wire Line
	7150 3350 7100 3350
Text HLabel 8100 2600 1    59   Output ~ 0
BTN_2
Wire Wire Line
	8100 2600 8100 2800
Wire Wire Line
	8100 3350 8050 3350
Text HLabel 9150 2600 1    59   Output ~ 0
BTN_3
Wire Wire Line
	9150 2600 9150 2800
Wire Wire Line
	9150 3350 9100 3350
Text HLabel 10150 2600 1    59   Output ~ 0
BTN_4
Wire Wire Line
	10150 2600 10150 2800
Wire Wire Line
	10150 3350 10100 3350
$Comp
L Switch:SW_SPST SW18
U 1 1 5F8623F9
P 6900 4150
F 0 "SW18" H 6900 4385 50  0000 C CNN
F 1 "SW_SPST" H 6900 4294 50  0000 C CNN
F 2 "" H 6900 4150 50  0001 C CNN
F 3 "~" H 6900 4150 50  0001 C CNN
	1    6900 4150
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW22
U 1 1 5F8623FF
P 7850 4150
F 0 "SW22" H 7850 4385 50  0000 C CNN
F 1 "SW_SPST" H 7850 4294 50  0000 C CNN
F 2 "" H 7850 4150 50  0001 C CNN
F 3 "~" H 7850 4150 50  0001 C CNN
	1    7850 4150
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW26
U 1 1 5F862405
P 8900 4150
F 0 "SW26" H 8900 4385 50  0000 C CNN
F 1 "SW_SPST" H 8900 4294 50  0000 C CNN
F 2 "" H 8900 4150 50  0001 C CNN
F 3 "~" H 8900 4150 50  0001 C CNN
	1    8900 4150
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW30
U 1 1 5F86240B
P 9900 4150
F 0 "SW30" H 9900 4385 50  0000 C CNN
F 1 "SW_SPST" H 9900 4294 50  0000 C CNN
F 2 "" H 9900 4150 50  0001 C CNN
F 3 "~" H 9900 4150 50  0001 C CNN
	1    9900 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 4450 6650 4450
Wire Wire Line
	6650 4450 6650 4150
Wire Wire Line
	6650 4150 6700 4150
Wire Wire Line
	6650 4450 7600 4450
Wire Wire Line
	7600 4450 7600 4150
Wire Wire Line
	7600 4150 7650 4150
Connection ~ 6650 4450
Wire Wire Line
	7600 4450 8650 4450
Wire Wire Line
	8650 4450 8650 4150
Wire Wire Line
	8650 4150 8700 4150
Connection ~ 7600 4450
Wire Wire Line
	8650 4450 9650 4450
Wire Wire Line
	9650 4450 9650 4150
Wire Wire Line
	9650 4150 9700 4150
Connection ~ 8650 4450
Wire Wire Line
	7150 4150 7100 4150
Wire Wire Line
	8100 4150 8050 4150
Wire Wire Line
	9150 4150 9100 4150
Wire Wire Line
	10150 4150 10100 4150
Connection ~ 5900 4450
$Comp
L Switch:SW_SPST SW19
U 1 1 5F8A660D
P 6900 4950
F 0 "SW19" H 6900 5185 50  0000 C CNN
F 1 "SW_SPST" H 6900 5094 50  0000 C CNN
F 2 "" H 6900 4950 50  0001 C CNN
F 3 "~" H 6900 4950 50  0001 C CNN
	1    6900 4950
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW23
U 1 1 5F8A6613
P 7850 4950
F 0 "SW23" H 7850 5185 50  0000 C CNN
F 1 "SW_SPST" H 7850 5094 50  0000 C CNN
F 2 "" H 7850 4950 50  0001 C CNN
F 3 "~" H 7850 4950 50  0001 C CNN
	1    7850 4950
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW27
U 1 1 5F8A6619
P 8900 4950
F 0 "SW27" H 8900 5185 50  0000 C CNN
F 1 "SW_SPST" H 8900 5094 50  0000 C CNN
F 2 "" H 8900 4950 50  0001 C CNN
F 3 "~" H 8900 4950 50  0001 C CNN
	1    8900 4950
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW31
U 1 1 5F8A661F
P 9900 4950
F 0 "SW31" H 9900 5185 50  0000 C CNN
F 1 "SW_SPST" H 9900 5094 50  0000 C CNN
F 2 "" H 9900 4950 50  0001 C CNN
F 3 "~" H 9900 4950 50  0001 C CNN
	1    9900 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 5250 6650 5250
Wire Wire Line
	6650 5250 6650 4950
Wire Wire Line
	6650 4950 6700 4950
Wire Wire Line
	6650 5250 7600 5250
Wire Wire Line
	7600 5250 7600 4950
Wire Wire Line
	7600 4950 7650 4950
Connection ~ 6650 5250
Wire Wire Line
	7600 5250 8650 5250
Wire Wire Line
	8650 5250 8650 4950
Wire Wire Line
	8650 4950 8700 4950
Connection ~ 7600 5250
Wire Wire Line
	8650 5250 9650 5250
Wire Wire Line
	9650 5250 9650 4950
Wire Wire Line
	9650 4950 9700 4950
Connection ~ 8650 5250
Wire Wire Line
	7150 4950 7100 4950
Wire Wire Line
	8100 4950 8050 4950
Wire Wire Line
	9150 4950 9100 4950
Wire Wire Line
	10150 4950 10100 4950
$Comp
L Switch:SW_SPST SW20
U 1 1 5F8BE430
P 6900 5750
F 0 "SW20" H 6900 5985 50  0000 C CNN
F 1 "SW_SPST" H 6900 5894 50  0000 C CNN
F 2 "" H 6900 5750 50  0001 C CNN
F 3 "~" H 6900 5750 50  0001 C CNN
	1    6900 5750
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW24
U 1 1 5F8BE436
P 7850 5750
F 0 "SW24" H 7850 5985 50  0000 C CNN
F 1 "SW_SPST" H 7850 5894 50  0000 C CNN
F 2 "" H 7850 5750 50  0001 C CNN
F 3 "~" H 7850 5750 50  0001 C CNN
	1    7850 5750
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW28
U 1 1 5F8BE43C
P 8900 5750
F 0 "SW28" H 8900 5985 50  0000 C CNN
F 1 "SW_SPST" H 8900 5894 50  0000 C CNN
F 2 "" H 8900 5750 50  0001 C CNN
F 3 "~" H 8900 5750 50  0001 C CNN
	1    8900 5750
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW32
U 1 1 5F8BE442
P 9900 5750
F 0 "SW32" H 9900 5985 50  0000 C CNN
F 1 "SW_SPST" H 9900 5894 50  0000 C CNN
F 2 "" H 9900 5750 50  0001 C CNN
F 3 "~" H 9900 5750 50  0001 C CNN
	1    9900 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 6050 6650 6050
Wire Wire Line
	6650 6050 6650 5750
Wire Wire Line
	6650 5750 6700 5750
Wire Wire Line
	6650 6050 7600 6050
Wire Wire Line
	7600 6050 7600 5750
Wire Wire Line
	7600 5750 7650 5750
Connection ~ 6650 6050
Wire Wire Line
	7600 6050 8650 6050
Wire Wire Line
	8650 6050 8650 5750
Wire Wire Line
	8650 5750 8700 5750
Connection ~ 7600 6050
Wire Wire Line
	8650 6050 9650 6050
Wire Wire Line
	9650 6050 9650 5750
Wire Wire Line
	9650 5750 9700 5750
Connection ~ 8650 6050
Wire Wire Line
	7150 5750 7100 5750
Wire Wire Line
	8100 5750 8050 5750
Wire Wire Line
	9150 5750 9100 5750
Wire Wire Line
	10150 5750 10100 5750
Wire Wire Line
	7150 5750 7150 4950
Connection ~ 7150 3350
Connection ~ 7150 4150
Wire Wire Line
	7150 4150 7150 3350
Connection ~ 7150 4950
Wire Wire Line
	7150 4950 7150 4150
Wire Wire Line
	8100 3350 8100 4150
Connection ~ 8100 3350
Connection ~ 8100 4150
Wire Wire Line
	8100 4150 8100 4950
Connection ~ 8100 4950
Wire Wire Line
	8100 4950 8100 5750
Wire Wire Line
	9150 3350 9150 4150
Connection ~ 9150 3350
Connection ~ 9150 4150
Wire Wire Line
	9150 4150 9150 4950
Connection ~ 9150 4950
Wire Wire Line
	9150 4950 9150 5750
Wire Wire Line
	10150 5750 10150 4950
Connection ~ 10150 3350
Connection ~ 10150 4150
Wire Wire Line
	10150 4150 10150 3350
Connection ~ 10150 4950
Wire Wire Line
	10150 4950 10150 4150
$Comp
L Device:R_US R69
U 1 1 5F93DF8E
P 7400 2800
F 0 "R69" V 7195 2800 50  0000 C CNN
F 1 "10k" V 7286 2800 50  0000 C CNN
F 2 "" V 7440 2790 50  0001 C CNN
F 3 "~" H 7400 2800 50  0001 C CNN
	1    7400 2800
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R70
U 1 1 5F95856D
P 8350 2800
F 0 "R70" V 8145 2800 50  0000 C CNN
F 1 "10k" V 8236 2800 50  0000 C CNN
F 2 "" V 8390 2790 50  0001 C CNN
F 3 "~" H 8350 2800 50  0001 C CNN
	1    8350 2800
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R71
U 1 1 5F972BA3
P 9400 2800
F 0 "R71" V 9195 2800 50  0000 C CNN
F 1 "10k" V 9286 2800 50  0000 C CNN
F 2 "" V 9440 2790 50  0001 C CNN
F 3 "~" H 9400 2800 50  0001 C CNN
	1    9400 2800
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R72
U 1 1 5F9A7002
P 10400 2800
F 0 "R72" V 10195 2800 50  0000 C CNN
F 1 "10k" V 10286 2800 50  0000 C CNN
F 2 "" V 10440 2790 50  0001 C CNN
F 3 "~" H 10400 2800 50  0001 C CNN
	1    10400 2800
	0    1    1    0   
$EndComp
Connection ~ 5900 5250
Connection ~ 5900 6050
Wire Wire Line
	10250 2800 10150 2800
Connection ~ 10150 2800
Wire Wire Line
	10150 2800 10150 3350
Wire Wire Line
	9250 2800 9150 2800
Connection ~ 9150 2800
Wire Wire Line
	9150 2800 9150 3350
Wire Wire Line
	8200 2800 8100 2800
Connection ~ 8100 2800
Wire Wire Line
	8100 2800 8100 3350
Wire Wire Line
	7250 2800 7150 2800
Connection ~ 7150 2800
Wire Wire Line
	7150 2800 7150 3350
$Comp
L power:+3.3V #PWR0255
U 1 1 5FA4DA0C
P 7650 2700
F 0 "#PWR0255" H 7650 2550 50  0001 C CNN
F 1 "+3.3V" H 7665 2873 50  0000 C CNN
F 2 "" H 7650 2700 50  0001 C CNN
F 3 "" H 7650 2700 50  0001 C CNN
	1    7650 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7650 2700 7650 2800
Wire Wire Line
	7650 2800 7550 2800
$Comp
L power:+3.3V #PWR0256
U 1 1 5FA6A1B9
P 8600 2700
F 0 "#PWR0256" H 8600 2550 50  0001 C CNN
F 1 "+3.3V" H 8615 2873 50  0000 C CNN
F 2 "" H 8600 2700 50  0001 C CNN
F 3 "" H 8600 2700 50  0001 C CNN
	1    8600 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8600 2700 8600 2800
Wire Wire Line
	8600 2800 8500 2800
$Comp
L power:+3.3V #PWR0257
U 1 1 5FA85E1F
P 9650 2700
F 0 "#PWR0257" H 9650 2550 50  0001 C CNN
F 1 "+3.3V" H 9665 2873 50  0000 C CNN
F 2 "" H 9650 2700 50  0001 C CNN
F 3 "" H 9650 2700 50  0001 C CNN
	1    9650 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	9650 2700 9650 2800
Wire Wire Line
	9650 2800 9550 2800
$Comp
L power:+3.3V #PWR0258
U 1 1 5FAA2064
P 10650 2700
F 0 "#PWR0258" H 10650 2550 50  0001 C CNN
F 1 "+3.3V" H 10665 2873 50  0000 C CNN
F 2 "" H 10650 2700 50  0001 C CNN
F 3 "" H 10650 2700 50  0001 C CNN
	1    10650 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	10650 2700 10650 2800
Wire Wire Line
	10650 2800 10550 2800
Wire Notes Line
	1500 800  1500 6200
Wire Notes Line
	1500 6200 6200 6200
Wire Notes Line
	6200 6200 6200 800 
Wire Notes Line
	6200 800  1500 800 
Wire Notes Line
	6500 1850 6500 6200
Wire Notes Line
	6500 6200 10900 6200
Wire Notes Line
	10900 6200 10900 1850
Wire Notes Line
	10900 1850 6500 1850
Text Notes 8300 2100 0    118  ~ 0
BUTTONS
Text Notes 3050 1050 0    118  ~ 0
ROTARY ENCODERS
$EndSCHEMATC
