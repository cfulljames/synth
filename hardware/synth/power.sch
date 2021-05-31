EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 2 13
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	2900 3700 4250 3700
Wire Wire Line
	3000 2550 2700 2550
Connection ~ 3000 2550
Connection ~ 4250 2550
Wire Wire Line
	4250 2550 3850 2550
Wire Wire Line
	4250 3300 4250 2550
Wire Wire Line
	4250 3700 4250 3600
Connection ~ 4250 3700
Wire Wire Line
	4250 3800 4250 3700
Wire Wire Line
	4250 4200 4250 4100
$Comp
L power:GND #PWR0102
U 1 1 5F51FAFA
P 4250 4200
F 0 "#PWR0102" H 4250 3950 50  0001 C CNN
F 1 "GND" H 4255 4027 50  0000 C CNN
F 2 "" H 4250 4200 50  0001 C CNN
F 3 "" H 4250 4200 50  0001 C CNN
	1    4250 4200
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R4
U 1 1 5F51ED56
P 4250 3950
F 0 "R4" H 4318 3996 50  0000 L CNN
F 1 "13k" H 4318 3905 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4290 3940 50  0001 C CNN
F 3 "~" H 4250 3950 50  0001 C CNN
	1    4250 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R3
U 1 1 5F51E126
P 4250 3450
F 0 "R3" H 4318 3496 50  0000 L CNN
F 1 "39.4k" H 4318 3405 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4290 3440 50  0001 C CNN
F 3 "~" H 4250 3450 50  0001 C CNN
	1    4250 3450
	1    0    0    -1  
$EndComp
Connection ~ 3450 2550
Wire Wire Line
	3550 2550 3450 2550
$Comp
L Device:D_Schottky D3
U 1 1 5F51A6EB
P 3700 2550
F 0 "D3" H 3700 2334 50  0000 C CNN
F 1 "PMEG2010BELD" H 3700 2425 50  0000 C CNN
F 2 "Diode_SMD:D_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 3700 2550 50  0001 C CNN
F 3 "~" H 3700 2550 50  0001 C CNN
	1    3700 2550
	-1   0    0    1   
$EndComp
Wire Wire Line
	3450 3450 3450 3350
$Comp
L power:GND #PWR0103
U 1 1 5F51A112
P 3450 3450
F 0 "#PWR0103" H 3450 3200 50  0001 C CNN
F 1 "GND" H 3455 3277 50  0000 C CNN
F 2 "" H 3450 3450 50  0001 C CNN
F 3 "" H 3450 3450 50  0001 C CNN
	1    3450 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 2550 3350 2550
Wire Wire Line
	3450 2650 3450 2550
Wire Wire Line
	3450 3050 3450 2950
$Comp
L Device:D_Schottky D1
U 1 1 5F51910B
P 3450 3200
F 0 "D1" V 3404 3279 50  0000 L CNN
F 1 "PMEG2010BELD" V 3495 3279 50  0000 L CNN
F 2 "Diode_SMD:D_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 3450 3200 50  0001 C CNN
F 3 "~" H 3450 3200 50  0001 C CNN
	1    3450 3200
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R1
U 1 1 5F51858D
P 3450 2800
F 0 "R1" H 3518 2846 50  0000 L CNN
F 1 "1" H 3518 2755 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 3490 2790 50  0001 C CNN
F 3 "~" H 3450 2800 50  0001 C CNN
	1    3450 2800
	1    0    0    -1  
$EndComp
Connection ~ 2700 2550
Wire Wire Line
	3050 2550 3000 2550
$Comp
L Device:C C3
U 1 1 5F517BB5
P 3200 2550
F 0 "C3" V 2948 2550 50  0000 C CNN
F 1 "2.2u" V 3039 2550 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 3238 2400 50  0001 C CNN
F 3 "~" H 3200 2550 50  0001 C CNN
	1    3200 2550
	0    1    1    0   
$EndComp
Text HLabel 1400 3500 0    50   Input ~ 0
5V_EN
Wire Wire Line
	1550 2550 2300 2550
Wire Wire Line
	2500 4050 2500 3950
Wire Wire Line
	1900 4200 1900 4150
$Comp
L power:GND #PWR0104
U 1 1 5F51635A
P 1900 4200
F 0 "#PWR0104" H 1900 3950 50  0001 C CNN
F 1 "GND" H 1905 4027 50  0000 C CNN
F 2 "" H 1900 4200 50  0001 C CNN
F 3 "" H 1900 4200 50  0001 C CNN
	1    1900 4200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0105
U 1 1 5F515EC6
P 2500 4050
F 0 "#PWR0105" H 2500 3800 50  0001 C CNN
F 1 "GND" H 2505 3877 50  0000 C CNN
F 2 "" H 2500 4050 50  0001 C CNN
F 3 "" H 2500 4050 50  0001 C CNN
	1    2500 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 3700 1900 3800
Wire Wire Line
	2100 3700 1900 3700
Wire Wire Line
	1550 3000 1550 2900
$Comp
L power:GND #PWR0106
U 1 1 5F5155BD
P 1550 3000
F 0 "#PWR0106" H 1550 2750 50  0001 C CNN
F 1 "GND" H 1555 2827 50  0000 C CNN
F 2 "" H 1550 3000 50  0001 C CNN
F 3 "" H 1550 3000 50  0001 C CNN
	1    1550 3000
	1    0    0    -1  
$EndComp
Connection ~ 1550 2550
Connection ~ 2300 2550
Wire Wire Line
	1550 2550 1550 2600
Wire Wire Line
	1300 2550 1550 2550
Wire Wire Line
	1300 2500 1300 2550
$Comp
L power:VBUS #PWR0107
U 1 1 5F5149B8
P 1300 2500
F 0 "#PWR0107" H 1300 2350 50  0001 C CNN
F 1 "VBUS" H 1315 2673 50  0000 C CNN
F 2 "" H 1300 2500 50  0001 C CNN
F 3 "" H 1300 2500 50  0001 C CNN
	1    1300 2500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5F51455C
P 1900 3950
F 0 "C2" H 2015 3996 50  0000 L CNN
F 1 "0.047u" H 2015 3905 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 1938 3800 50  0001 C CNN
F 3 "~" H 1900 3950 50  0001 C CNN
	1    1900 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5F513A5C
P 1550 2750
F 0 "C1" H 1665 2796 50  0000 L CNN
F 1 "2.2u" H 1665 2705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 1588 2600 50  0001 C CNN
F 3 "~" H 1550 2750 50  0001 C CNN
	1    1550 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 2550 2700 3150
Wire Wire Line
	2650 2550 2700 2550
Wire Wire Line
	2300 2550 2350 2550
Wire Wire Line
	2300 3150 2300 2550
$Comp
L Device:L L1
U 1 1 5F512B13
P 2500 2550
F 0 "L1" V 2690 2550 50  0000 C CNN
F 1 "SRU1048-100Y" V 2599 2550 50  0000 C CNN
F 2 "" H 2500 2550 50  0001 C CNN
F 3 "~" H 2500 2550 50  0001 C CNN
	1    2500 2550
	0    -1   -1   0   
$EndComp
$Comp
L synth:LT3467 U1
U 1 1 5F511CB1
P 2500 3500
F 0 "U1" H 2050 3900 50  0000 L CNN
F 1 "LT3467" H 1950 3800 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:TSOT-23-6" H 2000 4100 50  0001 C CNN
F 3 "" H 2000 4100 50  0001 C CNN
	1    2500 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 2450 5550 2550
$Comp
L power:+5V #PWR0109
U 1 1 5F5227AB
P 5550 2450
F 0 "#PWR0109" H 5550 2300 50  0001 C CNN
F 1 "+5V" H 5565 2623 50  0000 C CNN
F 2 "" H 5550 2450 50  0001 C CNN
F 3 "" H 5550 2450 50  0001 C CNN
	1    5550 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 2550 4550 2550
Wire Wire Line
	5050 3050 5050 2950
$Comp
L power:GND #PWR0111
U 1 1 5F435E0D
P 5050 3050
F 0 "#PWR0111" H 5050 2800 50  0001 C CNN
F 1 "GND" H 5055 2877 50  0000 C CNN
F 2 "" H 5050 3050 50  0001 C CNN
F 3 "" H 5050 3050 50  0001 C CNN
	1    5050 3050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C5
U 1 1 5F435E07
P 5050 2800
F 0 "C5" H 5165 2846 50  0000 L CNN
F 1 "22u" H 5165 2755 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5088 2650 50  0001 C CNN
F 3 "~" H 5050 2800 50  0001 C CNN
	1    5050 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 2650 5050 2550
Connection ~ 5050 2550
$Comp
L Regulator_Switching:TPS62142 U2
U 1 1 5F4855F8
P 7800 2550
F 0 "U2" H 8150 3150 50  0000 C CNN
F 1 "TPS62142" H 8150 3050 50  0000 C CNN
F 2 "Package_DFN_QFN:VQFN-16-1EP_3x3mm_P0.5mm_EP1.68x1.68mm_ThermalVias" H 7950 2100 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/TPS62140.pdf" H 7800 2550 50  0001 C CNN
	1    7800 2550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0112
U 1 1 5F48816F
P 7800 3150
F 0 "#PWR0112" H 7800 2900 50  0001 C CNN
F 1 "GND" H 7805 2977 50  0000 C CNN
F 2 "" H 7800 3150 50  0001 C CNN
F 3 "" H 7800 3150 50  0001 C CNN
	1    7800 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 3150 7800 3100
Wire Wire Line
	7800 3100 7900 3100
Wire Wire Line
	7900 3100 7900 3050
Connection ~ 7800 3100
Wire Wire Line
	7800 3100 7800 3050
Wire Wire Line
	7800 3100 7700 3100
Wire Wire Line
	7700 3100 7700 3050
Wire Wire Line
	7700 2000 7700 2050
Wire Wire Line
	7900 2000 7900 2050
Wire Wire Line
	7400 2350 7350 2350
Wire Wire Line
	7350 2350 7350 2000
Connection ~ 7700 2000
Wire Wire Line
	7700 2000 7900 2000
Wire Wire Line
	7350 2000 7700 2000
$Comp
L power:VBUS #PWR0113
U 1 1 5F48C44D
P 6350 1900
F 0 "#PWR0113" H 6350 1750 50  0001 C CNN
F 1 "VBUS" H 6365 2073 50  0000 C CNN
F 2 "" H 6350 1900 50  0001 C CNN
F 3 "" H 6350 1900 50  0001 C CNN
	1    6350 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:C C7
U 1 1 5F495435
P 6600 2250
F 0 "C7" H 6715 2296 50  0000 L CNN
F 1 "10u" H 6715 2205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6638 2100 50  0001 C CNN
F 3 "~" H 6600 2250 50  0001 C CNN
	1    6600 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 2000 6600 2000
Wire Wire Line
	6600 2000 6600 2100
Connection ~ 7350 2000
Wire Wire Line
	6600 2000 6350 2000
Wire Wire Line
	6350 2000 6350 1900
Connection ~ 6600 2000
$Comp
L power:GND #PWR0114
U 1 1 5F49D98B
P 6600 2500
F 0 "#PWR0114" H 6600 2250 50  0001 C CNN
F 1 "GND" H 6605 2327 50  0000 C CNN
F 2 "" H 6600 2500 50  0001 C CNN
F 3 "" H 6600 2500 50  0001 C CNN
	1    6600 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 2500 6600 2400
$Comp
L Device:C C8
U 1 1 5F4A056F
P 6850 2750
F 0 "C8" H 6965 2796 50  0000 L CNN
F 1 "3300p" H 6965 2705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6888 2600 50  0001 C CNN
F 3 "~" H 6850 2750 50  0001 C CNN
	1    6850 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 2600 6850 2450
Wire Wire Line
	6850 2450 7400 2450
Wire Wire Line
	7400 2550 7300 2550
Wire Wire Line
	7300 2550 7300 2650
Wire Wire Line
	7300 2650 7400 2650
$Comp
L power:GND #PWR0115
U 1 1 5F4A6DFF
P 6850 3000
F 0 "#PWR0115" H 6850 2750 50  0001 C CNN
F 1 "GND" H 6855 2827 50  0000 C CNN
F 2 "" H 6850 3000 50  0001 C CNN
F 3 "" H 6850 3000 50  0001 C CNN
	1    6850 3000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0116
U 1 1 5F4A8B3D
P 7300 2750
F 0 "#PWR0116" H 7300 2500 50  0001 C CNN
F 1 "GND" H 7305 2577 50  0000 C CNN
F 2 "" H 7300 2750 50  0001 C CNN
F 3 "" H 7300 2750 50  0001 C CNN
	1    7300 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 2750 7300 2650
Connection ~ 7300 2650
Wire Wire Line
	6850 3000 6850 2900
$Comp
L Device:L L2
U 1 1 5F4B0F4F
P 8600 2350
F 0 "L2" V 8790 2350 50  0000 C CNN
F 1 "IHLP1212BZER2R2M11" V 8699 2350 50  0000 C CNN
F 2 "" H 8600 2350 50  0001 C CNN
F 3 "~" H 8600 2350 50  0001 C CNN
	1    8600 2350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8450 2350 8200 2350
Wire Wire Line
	8200 2550 8850 2550
Wire Wire Line
	8850 2550 8850 2350
Wire Wire Line
	8850 2350 8750 2350
$Comp
L power:GND #PWR0117
U 1 1 5F4B7FCA
P 8300 2850
F 0 "#PWR0117" H 8300 2600 50  0001 C CNN
F 1 "GND" H 8305 2677 50  0000 C CNN
F 2 "" H 8300 2850 50  0001 C CNN
F 3 "" H 8300 2850 50  0001 C CNN
	1    8300 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	8300 2850 8300 2750
Wire Wire Line
	8300 2750 8200 2750
$Comp
L Device:C C9
U 1 1 5F4BE2FE
P 9450 2600
F 0 "C9" H 9565 2646 50  0000 L CNN
F 1 "22u" H 9565 2555 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 9488 2450 50  0001 C CNN
F 3 "~" H 9450 2600 50  0001 C CNN
	1    9450 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 2450 9450 2350
Wire Wire Line
	9450 2350 9000 2350
Connection ~ 8850 2350
$Comp
L power:+3.3V #PWR0118
U 1 1 5F4C11E3
P 9800 2250
F 0 "#PWR0118" H 9800 2100 50  0001 C CNN
F 1 "+3.3V" H 9815 2423 50  0000 C CNN
F 2 "" H 9800 2250 50  0001 C CNN
F 3 "" H 9800 2250 50  0001 C CNN
	1    9800 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	9800 2250 9800 2350
Wire Wire Line
	9800 2350 9550 2350
Connection ~ 9450 2350
$Comp
L power:GND #PWR0119
U 1 1 5F4C43A7
P 9450 2850
F 0 "#PWR0119" H 9450 2600 50  0001 C CNN
F 1 "GND" H 9455 2677 50  0000 C CNN
F 2 "" H 9450 2850 50  0001 C CNN
F 3 "" H 9450 2850 50  0001 C CNN
	1    9450 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 2850 9450 2750
Wire Wire Line
	5050 2550 5250 2550
Text Notes 2550 1950 0    118  ~ 0
+/-5 V REGULATOR
Text Notes 7250 1400 0    118  ~ 0
+3.3 V REGULATOR
Wire Notes Line
	6200 3600 6200 1000
Wire Notes Line
	6200 1000 10000 1000
Wire Notes Line
	10000 1000 10000 3600
Wire Notes Line
	10000 3600 6200 3600
Wire Notes Line
	1000 1450 5800 1450
Wire Notes Line
	5800 1450 5800 5950
Wire Notes Line
	5800 5950 1000 5950
Wire Notes Line
	1000 5950 1000 1450
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5F82A142
P 5250 2550
F 0 "#FLG0101" H 5250 2625 50  0001 C CNN
F 1 "PWR_FLAG" H 5250 2723 50  0000 C CNN
F 2 "" H 5250 2550 50  0001 C CNN
F 3 "~" H 5250 2550 50  0001 C CNN
	1    5250 2550
	1    0    0    -1  
$EndComp
Connection ~ 5250 2550
Wire Wire Line
	5250 2550 5550 2550
$Comp
L power:PWR_FLAG #FLG0103
U 1 1 5F888C49
P 9550 2350
F 0 "#FLG0103" H 9550 2425 50  0001 C CNN
F 1 "PWR_FLAG" H 9550 2523 50  0000 C CNN
F 2 "" H 9550 2350 50  0001 C CNN
F 3 "~" H 9550 2350 50  0001 C CNN
	1    9550 2350
	1    0    0    -1  
$EndComp
Connection ~ 9550 2350
Wire Wire Line
	9550 2350 9450 2350
$Comp
L Device:LED D?
U 1 1 5F98381B
P 8400 5100
AR Path="/5F4DCAE5/5F98381B" Ref="D?"  Part="1" 
AR Path="/5F7D597A/5F98381B" Ref="D?"  Part="1" 
AR Path="/5F2F8A25/5F98381B" Ref="D204"  Part="1" 
F 0 "D204" H 8393 5316 50  0000 C CNN
F 1 "150060RS75000" H 8393 5225 50  0000 C CNN
F 2 "LED_SMD:LED_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 8400 5100 50  0001 C CNN
F 3 "~" H 8400 5100 50  0001 C CNN
	1    8400 5100
	-1   0    0    1   
$EndComp
$Comp
L Device:R_US R?
U 1 1 5F983821
P 7900 5900
AR Path="/5F4DCAE5/5F983821" Ref="R?"  Part="1" 
AR Path="/5F7D597A/5F983821" Ref="R?"  Part="1" 
AR Path="/5F2F8A25/5F983821" Ref="R76"  Part="1" 
F 0 "R76" V 7695 5900 50  0000 C CNN
F 1 "1.6k" V 7786 5900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 7940 5890 50  0001 C CNN
F 3 "~" H 7900 5900 50  0001 C CNN
	1    7900 5900
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_US R?
U 1 1 5F983827
P 7900 5100
AR Path="/5F4DCAE5/5F983827" Ref="R?"  Part="1" 
AR Path="/5F7D597A/5F983827" Ref="R?"  Part="1" 
AR Path="/5F2F8A25/5F983827" Ref="R74"  Part="1" 
F 0 "R74" V 7695 5100 50  0000 C CNN
F 1 "1.6k" V 7786 5100 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 7940 5090 50  0001 C CNN
F 3 "~" H 7900 5100 50  0001 C CNN
	1    7900 5100
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D?
U 1 1 5F98382D
P 8400 5900
AR Path="/5F4DCAE5/5F98382D" Ref="D?"  Part="1" 
AR Path="/5F7D597A/5F98382D" Ref="D?"  Part="1" 
AR Path="/5F2F8A25/5F98382D" Ref="D206"  Part="1" 
F 0 "D206" H 8393 6116 50  0000 C CNN
F 1 "150060RS75000" H 8393 6025 50  0000 C CNN
F 2 "LED_SMD:LED_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 8400 5900 50  0001 C CNN
F 3 "~" H 8400 5900 50  0001 C CNN
	1    8400 5900
	-1   0    0    1   
$EndComp
Wire Wire Line
	8050 5100 8250 5100
Wire Wire Line
	8050 5900 8250 5900
Wire Wire Line
	7750 5900 7650 5900
Wire Wire Line
	7650 5100 7750 5100
$Comp
L power:GND #PWR?
U 1 1 5F983837
P 8850 4700
AR Path="/5F7D597A/5F983837" Ref="#PWR?"  Part="1" 
AR Path="/5F2F8A25/5F983837" Ref="#PWR0259"  Part="1" 
F 0 "#PWR0259" H 8850 4450 50  0001 C CNN
F 1 "GND" H 8855 4527 50  0000 C CNN
F 2 "" H 8850 4700 50  0001 C CNN
F 3 "" H 8850 4700 50  0001 C CNN
	1    8850 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 5900 8550 5900
Text Label 8300 2650 0    59   ~ 0
3V3_GOOD
Wire Wire Line
	8300 2650 8200 2650
$Comp
L Device:LED D?
U 1 1 5F98AE4D
P 8400 4700
AR Path="/5F4DCAE5/5F98AE4D" Ref="D?"  Part="1" 
AR Path="/5F7D597A/5F98AE4D" Ref="D?"  Part="1" 
AR Path="/5F2F8A25/5F98AE4D" Ref="D203"  Part="1" 
F 0 "D203" H 8393 4916 50  0000 C CNN
F 1 "150060RS75000" H 8393 4825 50  0000 C CNN
F 2 "LED_SMD:LED_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 8400 4700 50  0001 C CNN
F 3 "~" H 8400 4700 50  0001 C CNN
	1    8400 4700
	-1   0    0    1   
$EndComp
$Comp
L Device:R_US R?
U 1 1 5F98AE53
P 7900 4700
AR Path="/5F4DCAE5/5F98AE53" Ref="R?"  Part="1" 
AR Path="/5F7D597A/5F98AE53" Ref="R?"  Part="1" 
AR Path="/5F2F8A25/5F98AE53" Ref="R73"  Part="1" 
F 0 "R73" V 7695 4700 50  0000 C CNN
F 1 "1.6k" V 7786 4700 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 7940 4690 50  0001 C CNN
F 3 "~" H 7900 4700 50  0001 C CNN
	1    7900 4700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7650 4700 7750 4700
Wire Wire Line
	8850 4700 8550 4700
$Comp
L power:VBUS #PWR0260
U 1 1 5F98D320
P 7650 4700
F 0 "#PWR0260" H 7650 4550 50  0001 C CNN
F 1 "VBUS" H 7665 4873 50  0000 C CNN
F 2 "" H 7650 4700 50  0001 C CNN
F 3 "" H 7650 4700 50  0001 C CNN
	1    7650 4700
	1    0    0    -1  
$EndComp
$Comp
L power:-5V #PWR0261
U 1 1 5F996B93
P 8850 5900
F 0 "#PWR0261" H 8850 6000 50  0001 C CNN
F 1 "-5V" H 8865 6073 50  0000 C CNN
F 2 "" H 8850 5900 50  0001 C CNN
F 3 "" H 8850 5900 50  0001 C CNN
	1    8850 5900
	-1   0    0    1   
$EndComp
$Comp
L Device:LED D?
U 1 1 5F99ACFC
P 8400 5500
AR Path="/5F4DCAE5/5F99ACFC" Ref="D?"  Part="1" 
AR Path="/5F7D597A/5F99ACFC" Ref="D?"  Part="1" 
AR Path="/5F2F8A25/5F99ACFC" Ref="D205"  Part="1" 
F 0 "D205" H 8393 5716 50  0000 C CNN
F 1 "150060RS75000" H 8393 5625 50  0000 C CNN
F 2 "LED_SMD:LED_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 8400 5500 50  0001 C CNN
F 3 "~" H 8400 5500 50  0001 C CNN
	1    8400 5500
	-1   0    0    1   
$EndComp
$Comp
L Device:R_US R?
U 1 1 5F99AD02
P 7900 5500
AR Path="/5F4DCAE5/5F99AD02" Ref="R?"  Part="1" 
AR Path="/5F7D597A/5F99AD02" Ref="R?"  Part="1" 
AR Path="/5F2F8A25/5F99AD02" Ref="R75"  Part="1" 
F 0 "R75" V 7695 5500 50  0000 C CNN
F 1 "1.6k" V 7786 5500 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 7940 5490 50  0001 C CNN
F 3 "~" H 7900 5500 50  0001 C CNN
	1    7900 5500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8050 5500 8250 5500
Wire Wire Line
	7650 5500 7750 5500
$Comp
L power:+5V #PWR0262
U 1 1 5F99AD0B
P 7650 5500
F 0 "#PWR0262" H 7650 5350 50  0001 C CNN
F 1 "+5V" H 7665 5673 50  0000 C CNN
F 2 "" H 7650 5500 50  0001 C CNN
F 3 "" H 7650 5500 50  0001 C CNN
	1    7650 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 4700 8150 4700
Text Label 8150 4550 0    59   ~ 0
3V3_GOOD
Wire Wire Line
	8150 4550 8150 4700
Connection ~ 8150 4700
Wire Wire Line
	8150 4700 8250 4700
$Comp
L power:VBUS #PWR0263
U 1 1 5F9B148D
P 7650 5100
F 0 "#PWR0263" H 7650 4950 50  0001 C CNN
F 1 "VBUS" H 7665 5273 50  0000 C CNN
F 2 "" H 7650 5100 50  0001 C CNN
F 3 "" H 7650 5100 50  0001 C CNN
	1    7650 5100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F9BCF27
P 8850 5100
AR Path="/5F7D597A/5F9BCF27" Ref="#PWR?"  Part="1" 
AR Path="/5F2F8A25/5F9BCF27" Ref="#PWR0264"  Part="1" 
F 0 "#PWR0264" H 8850 4850 50  0001 C CNN
F 1 "GND" H 8855 4927 50  0000 C CNN
F 2 "" H 8850 5100 50  0001 C CNN
F 3 "" H 8850 5100 50  0001 C CNN
	1    8850 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 5100 8550 5100
$Comp
L power:GND #PWR?
U 1 1 5F9BF43D
P 8850 5500
AR Path="/5F7D597A/5F9BF43D" Ref="#PWR?"  Part="1" 
AR Path="/5F2F8A25/5F9BF43D" Ref="#PWR0265"  Part="1" 
F 0 "#PWR0265" H 8850 5250 50  0001 C CNN
F 1 "GND" H 8855 5327 50  0000 C CNN
F 2 "" H 8850 5500 50  0001 C CNN
F 3 "" H 8850 5500 50  0001 C CNN
	1    8850 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 5500 8550 5500
$Comp
L power:GND #PWR?
U 1 1 5F9C1B1A
P 7650 5900
AR Path="/5F7D597A/5F9C1B1A" Ref="#PWR?"  Part="1" 
AR Path="/5F2F8A25/5F9C1B1A" Ref="#PWR0266"  Part="1" 
F 0 "#PWR0266" H 7650 5650 50  0001 C CNN
F 1 "GND" H 7655 5727 50  0000 C CNN
F 2 "" H 7650 5900 50  0001 C CNN
F 3 "" H 7650 5900 50  0001 C CNN
	1    7650 5900
	-1   0    0    1   
$EndComp
Text Notes 7250 4700 0    59   ~ 0
3V3
Text Notes 7250 5100 0    59   ~ 0
VBUS
Text Notes 7250 5500 0    59   ~ 0
+5V
Text Notes 7250 5900 0    59   ~ 0
-5V
Text Notes 6900 4250 0    118  ~ 0
POWER STATUS INDICATORS
Wire Notes Line
	6200 3950 10000 3950
Wire Notes Line
	10000 3950 10000 6300
Wire Notes Line
	10000 6300 6200 6300
Wire Notes Line
	6200 6300 6200 3950
$Comp
L Device:C C4
U 1 1 5F41691B
P 3200 5350
F 0 "C4" V 2948 5350 50  0000 C CNN
F 1 "2.2u" V 3039 5350 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 3238 5200 50  0001 C CNN
F 3 "~" H 3200 5350 50  0001 C CNN
	1    3200 5350
	0    1    1    0   
$EndComp
$Comp
L Device:D_Schottky D4
U 1 1 5F416937
P 4150 5350
F 0 "D4" H 4150 5134 50  0000 C CNN
F 1 "PMEG2010BELD" H 4150 5225 50  0000 C CNN
F 2 "Diode_SMD:D_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 4150 5350 50  0001 C CNN
F 3 "~" H 4150 5350 50  0001 C CNN
	1    4150 5350
	1    0    0    1   
$EndComp
$Comp
L Device:R_US R2
U 1 1 5F416921
P 3750 5350
F 0 "R2" V 3950 5350 50  0000 C CNN
F 1 "1" V 3850 5350 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 3790 5340 50  0001 C CNN
F 3 "~" H 3750 5350 50  0001 C CNN
	1    3750 5350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3000 5350 3000 2550
Wire Wire Line
	3000 5350 3050 5350
Wire Wire Line
	3900 5350 4000 5350
Wire Wire Line
	5550 5450 5550 5350
$Comp
L power:-5V #PWR0108
U 1 1 5F4375C6
P 5550 5450
F 0 "#PWR0108" H 5550 5550 50  0001 C CNN
F 1 "-5V" H 5565 5623 50  0000 C CNN
F 2 "" H 5550 5450 50  0001 C CNN
F 3 "" H 5550 5450 50  0001 C CNN
	1    5550 5450
	-1   0    0    1   
$EndComp
Connection ~ 5250 5350
Wire Wire Line
	5250 5350 5550 5350
$Comp
L power:GND #PWR0101
U 1 1 5F416930
P 3500 4700
F 0 "#PWR0101" H 3500 4450 50  0001 C CNN
F 1 "GND" H 3505 4527 50  0000 C CNN
F 2 "" H 3500 4700 50  0001 C CNN
F 3 "" H 3500 4700 50  0001 C CNN
	1    3500 4700
	-1   0    0    1   
$EndComp
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 5F82A9D6
P 5250 5350
F 0 "#FLG0102" H 5250 5425 50  0001 C CNN
F 1 "PWR_FLAG" H 5250 5523 50  0000 C CNN
F 2 "" H 5250 5350 50  0001 C CNN
F 3 "~" H 5250 5350 50  0001 C CNN
	1    5250 5350
	-1   0    0    1   
$EndComp
$Comp
L Device:D_Schottky D2
U 1 1 5F416927
P 3500 4900
F 0 "D2" V 3450 4800 50  0000 R CNN
F 1 "PMEG2010BELD" V 3550 4800 50  0000 R CNN
F 2 "Diode_SMD:D_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 3500 4900 50  0001 C CNN
F 3 "~" H 3500 4900 50  0001 C CNN
	1    3500 4900
	0    -1   1    0   
$EndComp
Wire Wire Line
	3500 4750 3500 4700
Wire Wire Line
	3500 5050 3500 5350
Wire Wire Line
	3350 5350 3500 5350
Wire Wire Line
	3500 5350 3600 5350
Connection ~ 3500 5350
Wire Wire Line
	5050 5350 5050 5250
$Comp
L power:GND #PWR0110
U 1 1 5F42EBDA
P 5050 4850
F 0 "#PWR0110" H 5050 4600 50  0001 C CNN
F 1 "GND" H 5055 4677 50  0000 C CNN
F 2 "" H 5050 4850 50  0001 C CNN
F 3 "" H 5050 4850 50  0001 C CNN
	1    5050 4850
	-1   0    0    1   
$EndComp
$Comp
L Device:C C6
U 1 1 5F42EBD3
P 5050 5100
F 0 "C6" H 5165 5146 50  0000 L CNN
F 1 "22u" H 5165 5055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5088 4950 50  0001 C CNN
F 3 "~" H 5050 5100 50  0001 C CNN
	1    5050 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 4950 5050 4850
Wire Wire Line
	4300 5350 4550 5350
Connection ~ 5050 5350
Wire Wire Line
	5050 5350 5250 5350
$Comp
L Device:R_US R79
U 1 1 60B4ACB2
P 1550 3850
F 0 "R79" H 1618 3896 50  0000 L CNN
F 1 "10k" H 1618 3805 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 1590 3840 50  0001 C CNN
F 3 "~" H 1550 3850 50  0001 C CNN
	1    1550 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 3700 1550 3500
Wire Wire Line
	1550 4000 1550 4150
Wire Wire Line
	1550 4150 1900 4150
Connection ~ 1900 4150
Wire Wire Line
	1900 4150 1900 4100
Connection ~ 1550 3500
Wire Wire Line
	1550 3500 2100 3500
Wire Wire Line
	1400 3500 1550 3500
$Comp
L Device:CP1 C21
U 1 1 60B7C318
P 4550 2800
F 0 "C21" H 4665 2846 50  0000 L CNN
F 1 "100u" H 4665 2755 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.4" H 4550 2800 50  0001 C CNN
F 3 "~" H 4550 2800 50  0001 C CNN
	1    4550 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 2650 4550 2550
Connection ~ 4550 2550
Wire Wire Line
	4550 2550 5050 2550
Wire Wire Line
	4550 3050 4550 2950
$Comp
L power:GND #PWR01
U 1 1 60B80322
P 4550 3050
F 0 "#PWR01" H 4550 2800 50  0001 C CNN
F 1 "GND" H 4555 2877 50  0000 C CNN
F 2 "" H 4550 3050 50  0001 C CNN
F 3 "" H 4550 3050 50  0001 C CNN
	1    4550 3050
	1    0    0    -1  
$EndComp
$Comp
L Device:CP1 C22
U 1 1 60B83DF5
P 4550 5100
F 0 "C22" H 4665 5146 50  0000 L CNN
F 1 "100u" H 4665 5055 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.4" H 4550 5100 50  0001 C CNN
F 3 "~" H 4550 5100 50  0001 C CNN
	1    4550 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 4950 4550 4850
Wire Wire Line
	4550 5350 4550 5250
$Comp
L power:GND #PWR02
U 1 1 60B83DFD
P 4550 4850
F 0 "#PWR02" H 4550 4600 50  0001 C CNN
F 1 "GND" H 4555 4677 50  0000 C CNN
F 2 "" H 4550 4850 50  0001 C CNN
F 3 "" H 4550 4850 50  0001 C CNN
	1    4550 4850
	-1   0    0    1   
$EndComp
Connection ~ 4550 5350
Wire Wire Line
	4550 5350 5050 5350
$Comp
L Device:CP1 C23
U 1 1 60BA4794
P 9000 2600
F 0 "C23" H 9115 2646 50  0000 L CNN
F 1 "100u" H 9115 2555 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.4" H 9000 2600 50  0001 C CNN
F 3 "~" H 9000 2600 50  0001 C CNN
	1    9000 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 2450 9000 2350
Wire Wire Line
	9000 2850 9000 2750
$Comp
L power:GND #PWR03
U 1 1 60BA479C
P 9000 2850
F 0 "#PWR03" H 9000 2600 50  0001 C CNN
F 1 "GND" H 9005 2677 50  0000 C CNN
F 2 "" H 9000 2850 50  0001 C CNN
F 3 "" H 9000 2850 50  0001 C CNN
	1    9000 2850
	1    0    0    -1  
$EndComp
Connection ~ 9000 2350
Wire Wire Line
	9000 2350 8850 2350
$EndSCHEMATC
