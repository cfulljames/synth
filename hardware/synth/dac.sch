EESchema Schematic File Version 4
LIBS:synth-cache
EELAYER 30 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 4 12
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 9700 2450 2    118  ~ 0
DAC OUTPUT BUFFER
$Comp
L Amplifier_Operational:RC4580 U?
U 1 1 5F912DC9
P 9050 3050
AR Path="/5F912DC9" Ref="U?"  Part="1" 
AR Path="/5F8FEF97/5F912DC9" Ref="U?"  Part="1" 
F 0 "U?" H 9050 3417 50  0000 C CNN
F 1 "RC4580" H 9050 3326 50  0000 C CNN
F 2 "" H 9050 3050 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/rc4580.pdf" H 9050 3050 50  0001 C CNN
	1    9050 3050
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:RC4580 U?
U 2 1 5F912DCF
P 9050 5350
AR Path="/5F912DCF" Ref="U?"  Part="2" 
AR Path="/5F8FEF97/5F912DCF" Ref="U?"  Part="2" 
F 0 "U?" H 9050 5717 50  0000 C CNN
F 1 "RC4580" H 9050 5626 50  0000 C CNN
F 2 "" H 9050 5350 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/rc4580.pdf" H 9050 5350 50  0001 C CNN
	2    9050 5350
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:RC4580 U?
U 3 1 5F912DD5
P 9100 4050
AR Path="/5F912DD5" Ref="U?"  Part="3" 
AR Path="/5F8FEF97/5F912DD5" Ref="U?"  Part="3" 
F 0 "U?" H 9058 4096 50  0000 L CNN
F 1 "RC4580" H 9058 4005 50  0000 L CNN
F 2 "" H 9100 4050 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/rc4580.pdf" H 9100 4050 50  0001 C CNN
	3    9100 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 3050 9350 3050
Wire Wire Line
	9450 3050 9450 3350
Connection ~ 9450 3050
$Comp
L Device:C C?
U 1 1 5F912DDE
P 9400 3850
AR Path="/5F912DDE" Ref="C?"  Part="1" 
AR Path="/5F8FEF97/5F912DDE" Ref="C?"  Part="1" 
F 0 "C?" H 9515 3896 50  0000 L CNN
F 1 "0.1u" H 9515 3805 50  0000 L CNN
F 2 "" H 9438 3700 50  0001 C CNN
F 3 "~" H 9400 3850 50  0001 C CNN
	1    9400 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 3700 9400 3700
Wire Wire Line
	9400 4400 9000 4400
$Comp
L power:GND #PWR?
U 1 1 5F912DE6
P 9400 4000
AR Path="/5F912DE6" Ref="#PWR?"  Part="1" 
AR Path="/5F8FEF97/5F912DE6" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 9400 3750 50  0001 C CNN
F 1 "GND" H 9405 3827 50  0000 C CNN
F 2 "" H 9400 4000 50  0001 C CNN
F 3 "" H 9400 4000 50  0001 C CNN
	1    9400 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 3650 9000 3700
Connection ~ 9000 3700
Wire Wire Line
	9000 3700 9000 3750
Wire Wire Line
	9000 4350 9000 4400
Connection ~ 9000 4400
Wire Wire Line
	9000 4400 9000 4450
$Comp
L power:GND #PWR?
U 1 1 5F912DFE
P 9400 4700
AR Path="/5F912DFE" Ref="#PWR?"  Part="1" 
AR Path="/5F8FEF97/5F912DFE" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 9400 4450 50  0001 C CNN
F 1 "GND" H 9405 4527 50  0000 C CNN
F 2 "" H 9400 4700 50  0001 C CNN
F 3 "" H 9400 4700 50  0001 C CNN
	1    9400 4700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F912E04
P 9400 4550
AR Path="/5F912E04" Ref="C?"  Part="1" 
AR Path="/5F8FEF97/5F912E04" Ref="C?"  Part="1" 
F 0 "C?" H 9515 4596 50  0000 L CNN
F 1 "0.1u" H 9515 4505 50  0000 L CNN
F 2 "" H 9438 4400 50  0001 C CNN
F 3 "~" H 9400 4550 50  0001 C CNN
	1    9400 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 3050 9550 3050
Wire Wire Line
	8750 3150 8750 3350
Wire Wire Line
	9350 5350 9450 5350
Wire Wire Line
	8750 3350 9450 3350
Wire Wire Line
	8750 5450 8750 5650
Wire Wire Line
	8750 5650 9450 5650
Wire Wire Line
	9450 5650 9450 5350
Connection ~ 9450 5350
Wire Wire Line
	9450 5350 9550 5350
$Comp
L Device:CP1 C?
U 1 1 5F912E16
P 8150 2950
AR Path="/5F912E16" Ref="C?"  Part="1" 
AR Path="/5F8FEF97/5F912E16" Ref="C?"  Part="1" 
F 0 "C?" V 8402 2950 50  0000 C CNN
F 1 "3.3u" V 8311 2950 50  0000 C CNN
F 2 "" H 8150 2950 50  0001 C CNN
F 3 "~" H 8150 2950 50  0001 C CNN
	1    8150 2950
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1 C?
U 1 1 5F912E1C
P 8150 5250
AR Path="/5F912E1C" Ref="C?"  Part="1" 
AR Path="/5F8FEF97/5F912E1C" Ref="C?"  Part="1" 
F 0 "C?" V 8402 5250 50  0000 C CNN
F 1 "3.3u" V 8311 5250 50  0000 C CNN
F 2 "" H 8150 5250 50  0001 C CNN
F 3 "~" H 8150 5250 50  0001 C CNN
	1    8150 5250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8300 5250 8400 5250
Wire Wire Line
	8300 2950 8400 2950
$Comp
L Device:R_US R?
U 1 1 5F912E26
P 8400 5500
AR Path="/5F912E26" Ref="R?"  Part="1" 
AR Path="/5F8FEF97/5F912E26" Ref="R?"  Part="1" 
F 0 "R?" H 8332 5454 50  0000 R CNN
F 1 "100k" H 8332 5545 50  0000 R CNN
F 2 "" V 8440 5490 50  0001 C CNN
F 3 "~" H 8400 5500 50  0001 C CNN
	1    8400 5500
	-1   0    0    1   
$EndComp
Wire Wire Line
	8400 5350 8400 5250
Connection ~ 8400 5250
Wire Wire Line
	8400 5250 8750 5250
$Comp
L power:GND #PWR?
U 1 1 5F912E2F
P 8400 5750
AR Path="/5F912E2F" Ref="#PWR?"  Part="1" 
AR Path="/5F8FEF97/5F912E2F" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8400 5500 50  0001 C CNN
F 1 "GND" H 8405 5577 50  0000 C CNN
F 2 "" H 8400 5750 50  0001 C CNN
F 3 "" H 8400 5750 50  0001 C CNN
	1    8400 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	8400 5750 8400 5650
$Comp
L Device:R_US R?
U 1 1 5F912E36
P 8400 3200
AR Path="/5F912E36" Ref="R?"  Part="1" 
AR Path="/5F8FEF97/5F912E36" Ref="R?"  Part="1" 
F 0 "R?" H 8332 3154 50  0000 R CNN
F 1 "100k" H 8332 3245 50  0000 R CNN
F 2 "" V 8440 3190 50  0001 C CNN
F 3 "~" H 8400 3200 50  0001 C CNN
	1    8400 3200
	-1   0    0    1   
$EndComp
Wire Wire Line
	8400 3050 8400 2950
$Comp
L power:GND #PWR?
U 1 1 5F912E3D
P 8400 3450
AR Path="/5F912E3D" Ref="#PWR?"  Part="1" 
AR Path="/5F8FEF97/5F912E3D" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8400 3200 50  0001 C CNN
F 1 "GND" H 8405 3277 50  0000 C CNN
F 2 "" H 8400 3450 50  0001 C CNN
F 3 "" H 8400 3450 50  0001 C CNN
	1    8400 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	8400 3450 8400 3350
Connection ~ 8400 2950
Wire Wire Line
	8400 2950 8750 2950
Text HLabel 9550 5350 2    50   Input ~ 0
OUT_R
Text HLabel 9550 3050 2    50   Input ~ 0
OUT_L
$Comp
L synth:CS4334-DSZ U?
U 1 1 5F278D87
P 5200 4400
F 0 "U?" H 4800 5050 59  0000 C CNN
F 1 "CS4334-DSZ" H 4800 4950 59  0000 C CNN
F 2 "" H 4800 5050 59  0001 C CNN
F 3 "" H 4800 5050 59  0001 C CNN
	1    5200 4400
	1    0    0    -1  
$EndComp
Text HLabel 4550 4400 0    59   Input ~ 0
LRCLK
Wire Wire Line
	4550 4400 4650 4400
Text HLabel 4550 4300 0    59   Input ~ 0
SCLK
Wire Wire Line
	4550 4300 4650 4300
Text HLabel 4550 4200 0    59   Input ~ 0
SDATA
Wire Wire Line
	4550 4200 4650 4200
$Comp
L power:+5V #PWR?
U 1 1 5F27F39B
P 5200 2900
F 0 "#PWR?" H 5200 2750 50  0001 C CNN
F 1 "+5V" H 5215 3073 50  0000 C CNN
F 2 "" H 5200 2900 50  0001 C CNN
F 3 "" H 5200 2900 50  0001 C CNN
	1    5200 2900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F280714
P 5200 5100
F 0 "#PWR?" H 5200 4850 50  0001 C CNN
F 1 "GND" H 5205 4927 50  0000 C CNN
F 2 "" H 5200 5100 50  0001 C CNN
F 3 "" H 5200 5100 50  0001 C CNN
	1    5200 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 5100 5200 5000
$Comp
L Device:R_US R?
U 1 1 5F281B84
P 5850 4800
F 0 "R?" H 5918 4846 50  0000 L CNN
F 1 "267k" H 5918 4755 50  0000 L CNN
F 2 "" V 5890 4790 50  0001 C CNN
F 3 "~" H 5850 4800 50  0001 C CNN
	1    5850 4800
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R?
U 1 1 5F284BB4
P 6200 4800
F 0 "R?" H 6268 4846 50  0000 L CNN
F 1 "267k" H 6268 4755 50  0000 L CNN
F 2 "" V 6240 4790 50  0001 C CNN
F 3 "~" H 6200 4800 50  0001 C CNN
	1    6200 4800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F285863
P 5850 5050
F 0 "#PWR?" H 5850 4800 50  0001 C CNN
F 1 "GND" H 5855 4877 50  0000 C CNN
F 2 "" H 5850 5050 50  0001 C CNN
F 3 "" H 5850 5050 50  0001 C CNN
	1    5850 5050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F285FFA
P 6200 5050
F 0 "#PWR?" H 6200 4800 50  0001 C CNN
F 1 "GND" H 6205 4877 50  0000 C CNN
F 2 "" H 6200 5050 50  0001 C CNN
F 3 "" H 6200 5050 50  0001 C CNN
	1    6200 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 4950 5850 5050
Wire Wire Line
	6200 4950 6200 5050
Wire Wire Line
	5750 4500 5850 4500
Wire Wire Line
	5850 4500 5850 4650
Wire Wire Line
	5750 4300 6200 4300
Wire Wire Line
	6200 4300 6200 4650
Connection ~ 6200 4300
Connection ~ 5850 4500
$Comp
L Device:C C?
U 1 1 5F293461
P 5450 3300
F 0 "C?" H 5565 3346 50  0000 L CNN
F 1 "0.1u" H 5565 3255 50  0000 L CNN
F 2 "" H 5488 3150 50  0001 C CNN
F 3 "~" H 5450 3300 50  0001 C CNN
	1    5450 3300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F29461C
P 5850 3300
F 0 "C?" H 5965 3346 50  0000 L CNN
F 1 "1u" H 5965 3255 50  0000 L CNN
F 2 "" H 5888 3150 50  0001 C CNN
F 3 "~" H 5850 3300 50  0001 C CNN
	1    5850 3300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F29670D
P 5450 3550
F 0 "#PWR?" H 5450 3300 50  0001 C CNN
F 1 "GND" H 5455 3377 50  0000 C CNN
F 2 "" H 5450 3550 50  0001 C CNN
F 3 "" H 5450 3550 50  0001 C CNN
	1    5450 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 3550 5450 3450
$Comp
L power:GND #PWR?
U 1 1 5F29FD86
P 5850 3550
F 0 "#PWR?" H 5850 3300 50  0001 C CNN
F 1 "GND" H 5855 3377 50  0000 C CNN
F 2 "" H 5850 3550 50  0001 C CNN
F 3 "" H 5850 3550 50  0001 C CNN
	1    5850 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 3550 5850 3450
Wire Wire Line
	5450 3150 5450 3050
Wire Wire Line
	5850 3050 5850 3150
Wire Wire Line
	5200 2900 5200 3050
Wire Wire Line
	5450 3050 5850 3050
Wire Wire Line
	5450 3050 5200 3050
Connection ~ 5450 3050
Connection ~ 5200 3050
Wire Wire Line
	5200 3050 5200 3800
Text Notes 5300 2450 0    118  ~ 0
DAC
Text Notes 1500 2500 0    118  ~ 0
MCLK MULTIPLIER
Wire Wire Line
	1600 3800 1900 3800
Text HLabel 1600 3800 0    50   Input ~ 0
SCLK
NoConn ~ 1900 3900
Connection ~ 1800 4100
Wire Wire Line
	1700 4100 1700 4150
Wire Wire Line
	1800 4100 1700 4100
Wire Wire Line
	1800 4100 1900 4100
Wire Wire Line
	1800 4200 1800 4100
Wire Wire Line
	1900 4200 1800 4200
$Comp
L power:GND #PWR?
U 1 1 5F26513A
P 1700 4150
F 0 "#PWR?" H 1700 3900 50  0001 C CNN
F 1 "GND" H 1705 3977 50  0000 C CNN
F 2 "" H 1700 4150 50  0001 C CNN
F 3 "" H 1700 4150 50  0001 C CNN
	1    1700 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 4400 1900 4400
Text HLabel 1600 4400 0    50   Input ~ 0
MCLK_EN
Wire Wire Line
	2450 2800 2450 2900
$Comp
L power:+3.3V #PWR?
U 1 1 5F262BC9
P 2450 2800
F 0 "#PWR?" H 2450 2650 50  0001 C CNN
F 1 "+3.3V" H 2465 2973 50  0000 C CNN
F 2 "" H 2450 2800 50  0001 C CNN
F 3 "" H 2450 2800 50  0001 C CNN
	1    2450 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 4750 2450 4650
$Comp
L power:GND #PWR?
U 1 1 5F260783
P 2450 4750
F 0 "#PWR?" H 2450 4500 50  0001 C CNN
F 1 "GND" H 2455 4577 50  0000 C CNN
F 2 "" H 2450 4750 50  0001 C CNN
F 3 "" H 2450 4750 50  0001 C CNN
	1    2450 4750
	1    0    0    -1  
$EndComp
$Comp
L synth:ICS501MLFT U?
U 1 1 5F25EB6E
P 2450 4050
F 0 "U?" H 2100 4600 50  0000 L CNN
F 1 "ICS501MLFT" H 1900 4500 50  0000 L CNN
F 2 "" H 2100 4600 50  0001 C CNN
F 3 "" H 2100 4600 50  0001 C CNN
	1    2450 4050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F2DDBD6
P 2750 3150
F 0 "C?" H 2865 3196 50  0000 L CNN
F 1 "0.1u" H 2865 3105 50  0000 L CNN
F 2 "" H 2788 3000 50  0001 C CNN
F 3 "~" H 2750 3150 50  0001 C CNN
	1    2750 3150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F2DDBDC
P 2750 3400
F 0 "#PWR?" H 2750 3150 50  0001 C CNN
F 1 "GND" H 2755 3227 50  0000 C CNN
F 2 "" H 2750 3400 50  0001 C CNN
F 3 "" H 2750 3400 50  0001 C CNN
	1    2750 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 3400 2750 3300
Wire Wire Line
	2750 3000 2750 2900
Wire Wire Line
	2450 2900 2750 2900
Connection ~ 2450 2900
Wire Wire Line
	2450 2900 2450 3550
Wire Notes Line
	1000 2000 3500 2000
Wire Notes Line
	3500 2000 3500 6000
Wire Notes Line
	3500 6000 1000 6000
Wire Notes Line
	1000 6000 1000 2000
Text Notes 1450 5600 0    59   ~ 0
4X MULTIPLICATION\nSELECTED WITH:\nS0 = GND\nS1 = GND
Wire Notes Line
	4000 2000 7000 2000
Wire Notes Line
	7000 2000 7000 5950
Wire Notes Line
	7000 6000 4000 6000
Wire Notes Line
	4000 5950 4000 2000
Wire Wire Line
	3750 4050 3750 4650
Wire Wire Line
	3000 4050 3750 4050
Wire Wire Line
	3750 4650 4650 4650
Wire Wire Line
	7250 4300 7250 2950
Wire Wire Line
	7250 2950 8000 2950
Wire Wire Line
	6200 4300 7250 4300
Wire Wire Line
	7250 4500 7250 5250
Wire Wire Line
	7250 5250 8000 5250
Wire Wire Line
	5850 4500 7250 4500
Wire Notes Line
	7500 2000 10000 2000
Wire Notes Line
	10000 2000 10000 6000
Wire Notes Line
	10000 6000 7500 6000
Wire Notes Line
	7500 6000 7500 2000
$Comp
L power:+5V #PWR?
U 1 1 5F50C1DF
P 9000 3650
F 0 "#PWR?" H 9000 3500 50  0001 C CNN
F 1 "+5V" H 9015 3823 50  0000 C CNN
F 2 "" H 9000 3650 50  0001 C CNN
F 3 "" H 9000 3650 50  0001 C CNN
	1    9000 3650
	1    0    0    -1  
$EndComp
$Comp
L power:-5V #PWR?
U 1 1 5F50CC90
P 9000 4450
F 0 "#PWR?" H 9000 4550 50  0001 C CNN
F 1 "-5V" H 9015 4623 50  0000 C CNN
F 2 "" H 9000 4450 50  0001 C CNN
F 3 "" H 9000 4450 50  0001 C CNN
	1    9000 4450
	-1   0    0    1   
$EndComp
$EndSCHEMATC
