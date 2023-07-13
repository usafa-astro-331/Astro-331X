#include "input.h"
#include "global_var.h"

#include <Adafruit_INA219.h>

 Adafruit_INA219 ina219;


  // Initialize the INA219.
  // ina219.begin();
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).

  // To use a slightly lower 32V, 1A range (higher precision on amps):
  // ina219.setCalibration_32V_1A();

void get_command_from_pc() {
  int space, do_it = 0;         // For reading in a space from the PC
  // unsigned long int value = 0;  // Secondary value for items that have a 'set' functionality

  String what_to_do = Serial1.readString();  // Find out what the user wants the MKR1000 to do.
  if (what_to_do.length() != 0) {            // As long as the string contains "something"
    Serial.print("Received: ");
    Serial.println(what_to_do);

    what_to_do.trim();                                   // Remove all leading and trailing spaces
    space = what_to_do.indexOf(' ');                     // See if there is a space somewhere after removing leading and trailing spaces
    // if (space > -1) {                                    // If there is a space, then we have a value to 'set'
      // value = (what_to_do.substring(space)).toInt();     // Get the 'set' value as an integer
      // do_it = (what_to_do.substring(0, space)).toInt();  // Get the 'command' value as an integer
    // } else {
      do_it = (what_to_do.substring(0, space)).toInt();
    // }
    // Serial.print(do_it);
    // Serial.print(" ");
    // Serial.println(value);
    switch (do_it) {  // Determine what we are supposed to do
      case 1:
        Serial1.print(" help \n");
        Serial1.print("2 get RSSI \n");
        Serial1.print("3 toggle LED \n");
        Serial1.print("4 report LED current \n\n"); 
        break;

      case 2:
        get_sat_rssi();
        break;

        case 3: 

        digitalWrite(LED_pin, !(digitalRead(LED_pin)));
        
        if (digitalRead(LED_pin)){Serial1.println(" LED on");}
        else {Serial1.println(" LED off");}
        break;

      case 4: 
        float current_mA; 
        ina219.begin();
        current_mA = ina219.getCurrent_mA();

        Serial1.print(" ");
        Serial1.print(current_mA); 
        Serial1.print(" mA \n");
        break;

    }
  }
  delay(25);
}