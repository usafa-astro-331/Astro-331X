/*
Connect 4 sun sensors on your spacecraft, 
one pointed in each direction. 
__________
|       ↑|
|       →|
|        |
|        |
|        |
|        |
|←       |
|↓_______|

Each sun sensor should be connected as follows. 

Vcc (3.3 V) ----- (phototransistor) ----- (10 kΩ resistor) ----- GND
.                                     |
.                                     |
.                                  ADC input
.                                 (see below)

*/

int sunpx_reading, sunpy_reading, sunnx_reading, sunny_reading; 
int sun_x, sun_y; 

// pin assignments if using teensy 4.1
#ifdef ARDUINO_TEENSY41
  static int sunpx_pin = 24; 
  static int sunpy_pin = 25;
  static int sunnx_pin = 26;
  static int sunny_pin = 27;

// pin assignments if using arduino MKR Zero
#else  // Arduino MKR Zero
  static int sunpx_pin = 18; // (A3)
  static int sunpy_pin = 19; // (A4)
  static int sunnx_pin = 20; // (A5)
  static int sunny_pin = 21; // (A6)
#endif
