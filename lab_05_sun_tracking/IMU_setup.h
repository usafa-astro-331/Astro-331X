// IMU sensor connects with I2C via a QWIIC cable

    // Black = GND.
    // Red = 3.3V.
    // Blue = SDA.
    // Yellow = SCL.


// INA219 pin = Arduino pin

// SDA = SDA
// SCL = SCL
// if not using QWIIC connection, see datasheet for power and ground connections

// float x_max = 19.12; 
// float x_min = -11.7; 
// float y_max = -32.18;
// float y_min = -58.2; 

// magnetometer range values used for calibration
float x_max = 10.95; 
float x_min = -5.85; 
float y_max = 121.2;
float y_min = 107.7; 

float x_range = (x_max - x_min)/2; 
float x_bias = x_max-x_range; 
float y_range = (y_max - y_min)/2; 
float y_bias = y_max-y_range; 

float magx, magy, heading; 