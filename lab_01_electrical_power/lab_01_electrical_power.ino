#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;

// QWIIC cable colors
// black = ground
// red   = 3.3 V
// blue  = SDA
// yellow= SCL

#include <SPI.h>
#include <SD.h>

#ifdef ARDUINO_TEENSY41 
  const int chipSelect = BUILTIN_SDCARD;
#else // Arduino MKR Zero
  const int chipSelect = SDCARD_SS_PIN;
#endif


void setup() {
  delay(1000);
    
  Serial.begin(9600);
  Serial1.begin(9600); 

  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219");
	Serial1.println("Failed to find INA219");
	
    while (1) { delay(10); }
  }
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

  
    Serial.print("Initializing SD card...");
	Serial1.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
	Serial1.println("Card failed, or not present");
  
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
  Serial1.println("card initialized.");
     
                File dataFile = SD.open("iv_curve.tsv", FILE_WRITE);
              // if the file is available, write to it:
              if (dataFile) {
                dataFile.println("time (ms), current (mA), voltage (V)");
                dataFile.close();
              }
              // if the file isn't open, pop up an error:
              else { Serial.println("error opening log file");
					 Serial1.println("error opening log file");
			  }

Serial.println("time (ms), current (mA), voltage (V)");
Serial1.println("time (ms), current (mA), voltage (V)");



} // end function setup()


struct electrical_measurement
{
  float current_mA, voltage_V, power_mW;  
};

// prototype function for INA219 reading
electrical_measurement INA219reading() ;

  int interval = 200; // writes measurements every XX ms
  int present = millis(); 
  int due = present += interval; 

  int averaging_index = 0; // average over the last X samples
  int const num_samples = 10; // number of samples to average over

  float ina219curr_samples[num_samples];
  float ina219volt_samples[num_samples];


void loop() {
  electrical_measurement ina219data; 
  ina219data = INA219reading(); 
  
  ina219curr_samples[averaging_index] = ina219data.current_mA; 
  ina219volt_samples[averaging_index++] = ina219data.voltage_V; 
   
if (averaging_index >= num_samples){
    averaging_index = 0; 
  }


  present = millis(); 
  if (present >= due){
    String write_line = "time:";
    write_line += present; 
    
    // INA219
    float current = 0; 
    float voltage = 0; 
    for (int ii = 0; ii < num_samples; ii++){ // sum last X current samples
      current += ina219curr_samples[ii];
      voltage += ina219volt_samples[ii];
    } // end INA219 current sample sum
    
    current = current / float(num_samples); 
    voltage = voltage / float(num_samples);
    write_line += "\t"; 
		write_line += ", current:";
		write_line += current; 
    write_line += "\t"; 
		write_line += ", voltage:";
		write_line += voltage;
    
    File dataFile = SD.open("iv_curve.tsv", FILE_WRITE);
              // if the file is available, write to it:
              if (dataFile) {
                dataFile.println(write_line);
                dataFile.close();
                // print to the serial port too:
                
              }
              // if the file isn't open, pop up an error:
              else {
                Serial.println("error opening datalog.txt");
				Serial1.println("error opening datalog.txt");
              } // end if dataFile

	Serial.println(write_line);
	Serial1.println(write_line);

    due += interval; 
    
  } // end if (present >= due)
  
} // end function loop()


// Adafruit_INA219 ina219 ;

electrical_measurement INA219reading(){
  electrical_measurement data; //creates empty object 'data' of type electrical_measurement

  float shuntvoltage;
  float busvoltage ;
  float solar_panel_voltage ;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  //power_mW = ina219.getPower_mW();
  data.voltage_V = busvoltage + (shuntvoltage / 1000);
  data.current_mA = ina219.getCurrent_mA();
  
  return data;
  
} // end function INA219reading()



