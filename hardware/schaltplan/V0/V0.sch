EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 1500 1050 0    50   Input ~ 0
Camera
Text GLabel 1500 1150 0    50   Input ~ 0
MotorL
Text GLabel 1500 1250 0    50   Input ~ 0
MotorR
Text GLabel 1500 1350 0    50   Input ~ 0
Servo
Text GLabel 1500 1700 0    50   Input ~ 0
HallSensorL
Text GLabel 1500 1800 0    50   Input ~ 0
HallSensorR
Text Notes 3000 1000 0    50   ~ 0
MainBoard
Wire Notes Line
	2500 1000 4000 1000
Wire Notes Line
	4000 1000 4000 2000
Wire Notes Line
	4000 2000 2500 2000
Wire Notes Line
	2500 2000 2500 1000
Text GLabel 2500 1050 2    50   Input ~ 0
Camera(Spare)
Text GLabel 2500 1150 2    50   Input ~ 0
MotA
Text GLabel 2500 1250 2    50   Input ~ 0
MotB
Text GLabel 2500 1350 2    50   Input ~ 0
Servo1
Text GLabel 2500 1700 2    50   Input ~ 0
SpeedMotA
Text GLabel 2500 1800 2    50   Input ~ 0
SpeedMotB
Text GLabel 1500 1950 0    50   Input ~ 0
Batery
Text GLabel 2500 1950 2    50   Input ~ 0
Power
Wire Wire Line
	1500 1950 2500 1950
Wire Wire Line
	2500 1800 1500 1800
Wire Wire Line
	1500 1700 2500 1700
Wire Wire Line
	2500 1350 1500 1350
Wire Wire Line
	1500 1250 2500 1250
Wire Wire Line
	2500 1150 1500 1150
Wire Wire Line
	1500 1050 2500 1050
Text GLabel 1500 2450 0    50   Input ~ 0
Camera
Text GLabel 1500 2550 0    50   Input ~ 0
MotorL
Text GLabel 1500 2650 0    50   Input ~ 0
MotorR
Text GLabel 1500 2750 0    50   Input ~ 0
ServoData
Text GLabel 1500 3100 0    50   Input ~ 0
HallSensorL
Text GLabel 1500 3200 0    50   Input ~ 0
HallSensorR
Text Notes 3000 2400 0    50   ~ 0
MainBoard
Wire Notes Line
	2500 2400 4000 2400
Wire Notes Line
	4000 3400 2500 3400
Wire Notes Line
	2500 3400 2500 2400
Text GLabel 2500 2450 2    50   Input ~ 0
Camera(Spare)
Text GLabel 2500 2550 2    50   Input ~ 0
MotA
Text GLabel 2500 2650 2    50   Input ~ 0
MotB
Text GLabel 2500 2750 2    50   Input ~ 0
Servo1Data
Text GLabel 2500 3100 2    50   Input ~ 0
SpeedMotA
Text GLabel 2500 3200 2    50   Input ~ 0
SpeedMotB
Text GLabel 1500 3350 0    50   Input ~ 0
Batery
Text GLabel 2500 3350 2    50   Input ~ 0
Power
Wire Wire Line
	1500 3350 1550 3350
Wire Wire Line
	2500 3200 1500 3200
Wire Wire Line
	1500 3100 2500 3100
Wire Wire Line
	2500 2750 1500 2750
Wire Wire Line
	1500 2650 2500 2650
Wire Wire Line
	2500 2550 1500 2550
Wire Wire Line
	1500 2450 2500 2450
NoConn ~ 2000 3100
NoConn ~ 2000 3200
NoConn ~ 2000 3350
Text GLabel 1500 2850 0    50   Input ~ 0
ServoPower
Text GLabel 2500 2850 2    50   Input ~ 0
Servo1Power
Wire Wire Line
	2500 2850 1550 2850
NoConn ~ 2000 2850
Wire Notes Line
	1600 3750 1800 3750
Wire Notes Line
	1800 3500 1600 3500
Text Notes 1450 3850 0    50   ~ 0
DcDcConverter
Wire Wire Line
	1550 3350 1550 3550
Wire Wire Line
	1550 3550 2400 3550
Wire Wire Line
	2400 3550 2400 3350
Connection ~ 1550 3350
Wire Wire Line
	1550 3350 2400 3350
Connection ~ 2400 3350
Wire Wire Line
	2400 3350 2500 3350
Wire Notes Line
	1800 3500 1800 3750
Wire Notes Line
	1600 3500 1600 3750
Wire Notes Line
	2150 3500 2350 3500
Wire Notes Line
	2350 3500 2350 3750
Wire Notes Line
	2350 3750 2150 3750
Wire Notes Line
	2150 3750 2150 3500
Text Notes 2150 3850 0    50   ~ 0
Switch
Wire Wire Line
	1550 2850 1550 3050
Wire Wire Line
	1550 3050 2400 3050
Wire Wire Line
	2400 3050 2400 3350
Connection ~ 1550 2850
Wire Wire Line
	1550 2850 1500 2850
Text GLabel 4500 1050 2    50   Input ~ 0
Programmer
Text GLabel 4000 1050 0    50   Input ~ 0
J13
Wire Wire Line
	4500 1050 4000 1050
Text GLabel 1500 1450 0    50   Input ~ 0
ServoPower
Text GLabel 2500 1450 2    50   Input ~ 0
Servo1Power
Wire Wire Line
	1500 1450 2500 1450
Text GLabel 4500 2450 2    50   Input ~ 0
Programmer
Text GLabel 4000 2450 0    50   Input ~ 0
J13
Wire Wire Line
	4500 2450 4000 2450
Wire Notes Line
	4000 2400 4000 3400
$EndSCHEMATC
