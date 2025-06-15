/*
This lab is intended to measure current and
voltage produced by a 4-cell solar array
connected in series. This activity is from 
USAFA's Astro 331: Space Systems Engineering. 

Expected voltage: 0-25 V
Expected current: 0-50 mA

Author: Lt Col Jordan Firth
Date: 2025-06

Connections
. +V (~25 V) 
.  |
.  |---- < 1 MΩ > ------- A0 ---- < 100 kΩ > ----|
.  |                                             |
.  |---- < 0-10 kΩ > ---- A1 ---- < 1 Ω > -------|
.                                                | 
.                                               GND

*/

const int voltage_pin = A0; 
const int current_pin = A1; 

int voltage_counts; 
float voltage_V; 

// ratio of voltage divider: 
// R1 = 1 MΩ; R2 = 100 kΩ
float voltage_divider_ratio = 1.0e5/(1.0e5+1.0e6); 

// shunt resistance
float shunt_resistance = 1.0; 

// count conversion
// 10 bit ADC (1024 counts) = 3.3 V
float count_conversion = 3.3/1024; 

int current_counts; 
float current_mA; 

void setup() {
  delay(1000);
    
  Serial.begin(9600);
  Serial1.begin(9600); 

} // end function setup()

  int interval = 200; // writes measurements every XX ms
  int present = millis(); 
  int due = present += interval; 

void loop() {

  present = millis(); 
  if (present >= due){
    String serial_line = "time:";
    serial_line += present; 
		String SD_line; 
    SD_line += present; 
    
    voltage_counts = analogRead(voltage_pin); 
    current_counts = analogRead(current_pin); 

    voltage_V = voltage_counts * count_conversion / voltage_divider_ratio; 
    current_mA = current_counts * count_conversion / shunt_resistance; 
    
    serial_line += ", current_mA:";
		serial_line += current_mA; 
		
    serial_line += ", voltage_V:";
		serial_line += voltage_V;

    serial_line += ", current_counts:"; 
    serial_line += current_counts; 
    serial_line += ", voltage_counts:";
    serial_line += voltage_counts; 

	Serial.println(serial_line);
	Serial1.println(serial_line);

    due += interval; 
    
  } // end if (present >= due)
  
} // end function loop()

