#ifndef INPUT_H
#define INPUT_H
#include <Arduino.h>
#include "get_rssi.h"



#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;


  // Initialize the INA219.
  ina219.begin();
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).

  // To use a slightly lower 32V, 1A range (higher precision on amps):
  ina219.setCalibration_32V_1A();
  




void get_command_from_pc() ;

#endif