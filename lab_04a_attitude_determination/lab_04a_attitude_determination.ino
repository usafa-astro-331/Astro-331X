// ----- ICM 20948 IMU
#include <ICM_20948.h>  // Click here to get the library: http://librarymanager/All#SparkFun_ICM_20948_IMU
	// data recorded from this file uses gyr/acc coords, not mag coords

#define WIRE_PORT Wire  // Your desired Wire port.      Used when "USE_SPI" is not defined
// The value of the last bit of the I2C address.
// On the SparkFun 9DoF IMU breakout the default is 1, and when the ADR jumper is closed the value becomes 0
#define AD0_VAL 1

ICM_20948_I2C myICM;

#include "IMU_pins.h"
#include "./sun_sensor_pins.h"

// ----- SD card -----
#include <SPI.h>
#include <SD.h>

#ifdef ARDUINO_TEENSY41
const int chipSelect = BUILTIN_SDCARD;
#else  // Arduino MKR Zero
const int chipSelect = SDCARD_SS_PIN;
#endif

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  //  while (!SERIAL_PORT)
  //  {
  //  };

  // ----- ICM 20948 IMU
  WIRE_PORT.begin();
  WIRE_PORT.setClock(400000);

  bool initialized = false;
  while (!initialized) {

    myICM.begin(WIRE_PORT, AD0_VAL);


    Serial.print(F("Initialization of the sensor returned: "));
    Serial.println(myICM.statusString());
    Serial1.print(F("Initialization of the sensor returned: "));
    Serial1.println(myICM.statusString());
    if (myICM.status != ICM_20948_Stat_Ok) {
      Serial.println("Trying again...");
      Serial1.println("Trying again...");
      delay(500);
    } else {
      initialized = true;
    }
  }

  Serial.print("Initializing SD card...");
  Serial1.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    Serial1.println("Card failed, or not present");
    // don't do anything more:
    while (1)
      ;
  }
  Serial.println("card initialized.");
  Serial1.println("card initialized.");
  delay(2000);

  File dataFile = SD.open("04a_att.tsv", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    String SD_line = ""; 
    SD_line += "units:\n" ;
    SD_line += "  time (ms)\n"; 
    SD_line += "  gyr (dps)\n"; 
    SD_line += "  mag (uT)\n"; 
    SD_line += "  sun detector (count)\n"; 
    SD_line_line += "  sun angle (deg)\n";

    dataFile.print(SD_line);
    dataFile.close();
  }
  // if the file isn't open, print an error:
  else { 
    Serial.println("error opening log file"); 
	  Serial1.println("error opening log file"); 
  }

}  // end function setup


int t;
int t0 = millis();  // set start time right before loop

// write accel data (to SD and/or serial) every `write_interval` ms
int last_wrote = 0;
int write_interval = 100;  // ms


String printFormattedFloat(float val, uint8_t leading, uint8_t decimals) {
  String output_line = "";

  float aval = abs(val);
  if (val < 0) {
    output_line += "-";
  } else {
    output_line += "+";
  }
  for (uint8_t indi = 0; indi < leading; indi++) {
    uint32_t tenpow = 0;
    if (indi < (leading - 1)) {
      tenpow = 1;
    }
    for (uint8_t c = 0; c < (leading - 1 - indi); c++) {
      tenpow *= 10;
    }
    if (aval < tenpow) {
      output_line += "0";
    } else {
      break;
    }
  }
  if (val < 0) {
    output_line += -val;
    output_line += decimals;
  } else {
    output_line += val;
    output_line += decimals;
  }
  return output_line;
}  //end printformattedfloat()

String printScaledAGMT(ICM_20948_I2C *sensor) {
  String serial_line = "\t"; //tab to separate fields
	serial_line += "gyrz:";
  serial_line += printFormattedFloat(sensor->gyrZ(), 5, 2);
  serial_line += "\t"; //tab to separate fields
  
  // align magnetometer coordinates to gyro coordinates
  // magx = gyrx
  // magy = -gyry 
  // magz = -gyrz
	serial_line += "magx:";
  serial_line += printFormattedFloat(sensor->magX(), 5, 2);
  serial_line += "\t"; //tab to separate fields
	serial_line += "magy:";
  serial_line += printFormattedFloat(-(sensor->magY() ), 5, 2);

  return serial_line;
}

void loop() {
  t = millis();

  if (t - last_wrote > write_interval) {
    String serial_line = "t:";
    serial_line += t;

    myICM.getAGMT();                        // The values are only updated when you call 'getAGMT'
    serial_line += printScaledAGMT(&myICM);  // This function takes into account the scale settings from when the measurement was made to calculate the values with units

    // read sun sensors
    sunpx_reading = analogRead(sunpx_pin);
    sunpy_reading = analogRead(sunpy_pin);
    sunnx_reading = analogRead(sunnx_pin);
    sunny_reading = analogRead(sunny_pin);    


    // // output raw sun sensor data
    // // comment out these lines after you find sun angle
    // // (highlight them, then press CTRL-/)
	  serial_line += "\t"; //tab to separate fields
		serial_line += "sunpx:"; 
		serial_line += sunpx_reading; 
    serial_line += "\t"; //tab to separate fields
		serial_line += "sunpy:"; 
		serial_line += sunpy_reading; 
    serial_line += "\t"; //tab to separate fields
		serial_line += "sunnx:"; 
		serial_line += sunnx_reading; 
    serial_line += "\t"; //tab to separate fields
		serial_line += "sunny:"; 
		serial_line += sunny_reading; 

    
    // // find sun direction
    // // uncomment these lines to find sund direction
    // // (highlight them, CTRL-/)
    // sun_x = ... ; // you fill in here--remember to end line with ;
    // sun_y = ... ; // you fill in here--remember to end line with ;
    // sun_direction = atan2(sun_y*1.0, sun_x*1.0) * RAD_TO_DEG + 180; 
		// 	/*NOTE: 
    //  *1.0 converts from int to float
		// 	+ 180 changes range to 0--360; 
    //  */
    //
    // serial_line += "\t"; //tab to separate fields
		// serial_line += "sun:"; 
    // serial_line += sun_direction; 



  // OUTPUT DATA  
    Serial.println(serial_line);
	  Serial1.println(serial_line);

    File dataFile = SD.open("04a_att.tsv", FILE_WRITE);
    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(serial_line);
      dataFile.close();

    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening data file");
	  Serial1.println("error opening data file");
    }

    last_wrote += write_interval;
  }

}  // end loop()
