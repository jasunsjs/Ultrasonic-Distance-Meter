# Ultrasonic-Distance-Meter
A measurement tool for distance using Arduino

Using an ultrasonic sensor, it sends out sound waves and receive them upon waves hitting obstacles and bouncing back, measuring the duration. Then, the duration is converted into distance. The outputs are displayed on a LCD Display. The Arduino Mega microcontroller and the Arduino 4x4 keypad are used.

The meter has three operating modes: MEASURE, HOLD, & REFERENCE. 
1.	MEASURE simply converts the duration into distance and outputs the distance of an object away from the sensor (average distance calculated to ensure precision).
2.	HOLD freezes the LCD screen and displays the last measured distance. 
3.	REFERENCE allows the user to input a total of 5 reference values via a keypad that can be used to compare with measured distances. While in MEASURE, if the measurement is near any one of the 5 references, the display will indicate the particular reference value.

Meter has 2 operational units: inches and centimeters.

Additional information can be found in the data sheet.

