#define SERIAL_PORT Serial
#define WIRE_PORT Wire  // Your desired Wire port.      Used when "USE_SPI" is not defined

// ----- ICM 20948 IMU
  #include <ICM_20948.h>  // Click here to get the library: http://librarymanager/All#SparkFun_ICM_20948_IMU
  ICM_20948_I2C myICM;
  // The value of the last bit of the I2C address.
  // On the SparkFun 9DoF IMU breakout the default is 1, and when the ADR jumper is closed the value becomes 0
  #define AD0_VAL 1
  #include "IMU_setup.h"
  #include "./sun_sensor_pins.h"


 // #include "set_speed.ino"
 // arduino IDE sees all *.ino files in the directory--it's automatically included


// ----- TB9051FTG Motor Carrier
  #include <TB9051FTGMotorCarrier.h>

// ------ quadrature encoder for motor position
  // Teensy is not compatible with QuadratureEncoder/Hardware Interrupt
  //   but comes with its own Encoder library
  #ifdef ARDUINO_TEENSY41
  #include <Encoder.h>
  #else  // Arduino MKR Zero
  #include <QuadratureEncoder.h>
  // NOTE: QuadratureEncoder requires <EnableInterrupt.h>
  #endif
  #include "motor_setup.h"
  // Instantiate TB9051FTGMotorCarrier
    static TB9051FTGMotorCarrier driver{ pwm1Pin, pwm2Pin };
  // set up variable for pulse width modulation of motor
    static float throttlePWM{ 0.0f };

unsigned long lastMilli = 0;
long currentEncoderCount = 0;
long lastEncoderCount = 0;
float speed_rpm = 0.0;
long timeElapsed = 0;

float speed_pwm;

// ----- SD card -----
  #include <SPI.h>
  #include <SD.h>
  #ifdef ARDUINO_TEENSY41
    const int chipSelect = BUILTIN_SDCARD;
  #else  // Arduino MKR Zero
    const int chipSelect = SDCARD_SS_PIN;
  #endif
  File dataFile;


// ----- time variables ----- for open loop motor speed control
// int print_time = 0;
// int print_delay = 500;
int current_time = 0;
int elapsed = 0;


void setup() {

  // spin reaction wheel to 500 RPM, wait 5 sec
throttlePWM = 0.5;
driver.setOutput(throttlePWM);
delay(5000); 


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


    SERIAL_PORT.print(F("Initialization of the sensor returned: "));
    SERIAL_PORT.println(myICM.statusString());
    if (myICM.status != ICM_20948_Stat_Ok) {
      SERIAL_PORT.println("Trying again...");
      delay(500);
    } else {
      initialized = true;
    }
  }

  // ----- TB9051FTG Motor Carrier
  driver.enable();

  // LED for state indication
  pinMode(A0, OUTPUT);

  Serial1.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial1.println("Card failed, or not present");
    // don't do anything more:
    while (1)
      ;
  }
  Serial1.println("card initialized.");

  dataFile = SD.open("05_sun.dat", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    String write_line = "";
    write_line += "units:\n";
    write_line += "time (ms)\n";
    write_line += "heading (rad)\n";
    write_line += "commanded wheel speed (RPM)\n";
    write_line += "measured wheel speed (RPM)\n";

    dataFile.print(write_line);

    Serial.print(write_line);

    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog");
  }


}  // end function setup

int speed;

// write accel data (to SD and/or serial) every `write_interval` ms
int last_wrote = 0;
int write_interval = 300;  // ms
float Heading, dHeading;

float w_old;
float w_new;
float w_500;
float a;
float sunpx;
float sunnx;
float sunpy;
float sunny;

int t;
int t0 = millis();  // set start time right before loop

void loop() {
  t = millis();
  String write_line = "t:";
  write_line += t;

  myICM.getAGMT();  // The values are only updated when you call 'getAGMT' 

  if (t - last_wrote >= write_interval) {
    // print wheel speed
    // Determine how much time has elapsed since last measurement
    timeElapsed = millis() - lastMilli;

////////////////////////////////////////////////////////////
    a = 0.01; // ADJUST THIS NUMBER AS NEEDED!!!
    w_500 = 0.5;
    
    sunpx_reading = analogRead(sunpx_pin);
    sunpy_reading = analogRead(sunpy_pin);
    sunnx_reading = analogRead(sunnx_pin);
    sunny_reading = analogRead(sunny_pin);

    t = millis() - t0;
    // Adjust w
    if (t - t0 < 10e3) {
      driver.setOutput(0.5);
    }

    if (t - t0 >= 10e3) {  
      // Find Sun Direction
      sunpx = analogRead(sunpx_pin);
      sunpy = analogRead(sunpy_pin);
      sunnx = analogRead(sunnx_pin);
      sunny = analogRead(sunny_pin);

      north =  sunpx - sunnx;
      east =  sunpy - sunny; 
      sun_direction = atan2(east*1.0, -north) + PI; 

      w_new = w_500 + a*sun_direction;
      //w_new = w_old + a*sun_direction;
      //w_old = w_new;

      if (w_new > 0.75){
        w_new = 0.75;
      }

      if (w_new < 0.25) {
        w_new = 0.25;
      }

      driver.setOutput(w_new);
     }

     

////////////////////////////////////////////////////////////

    // String write_line = "";
    String write_line = "t:";
    write_line += t;
    write_line += ", sun:"; 
    write_line += sun_direction;
    write_line += ", ω:";
    write_line += w_new * 1000; // speed in RPM
    
    
    // Print to serial monitor and file
    Serial.println(write_line);

    File dataFile = SD.open("05_sun.dat", FILE_WRITE);
    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(write_line);
      dataFile.close();

    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening file on SD card");
    }


    // reset variables to most recent value
    lastMilli = millis();
    lastEncoderCount = currentEncoderCount;
    

    last_wrote += write_interval;
  }

}  // end loop()
