# Astro 331X

In this class we will use Arduinos to build a satellite analog called FlatSAT to explore spacecraft design lessons. FlatSAT is a mini version of the FalconSAT Avionics Test Bed (ATB). 

<img src="sources/fritzing/331X_bb.svg" alt="breadboard"  />



## Lab overview

Each lab group will build a FlatSAT using solderless breadboards. Arduino microcontrollers will run FlatSAT. You will communicate with FlatSAT using the Arduino IDE. 

Each lab will explore a new concept, and will add new components to FlatSAT.

Lab 0 is a metrology lab, but it doesn’t get a number because we don’t start building FlatSAT until the electrical system lab. 

1. electrical system
2. camera payload
3. radio communications
4. attitude determination and control



## Schedule

In general, each day will cover

- topical lecture
- pre-lab predictions
- lab execution
- data analysis


| Mon                                        | Tue              | Wed                | Thu                               | Fri            |
| ------------------------------------------ | ---------------- | ------------------ | --------------------------------- | -------------- |
| spacecraft overview, <br>electrical system | attitude sensors | attitude actuators | communication, <br>group projects | group projects |


Labs

Day 1
- voltage divider/potentiometer, measure solar cell voltage with analog-in pin and voltage divider
	- !! make lab 1
- can the solar panel power the spacecraft? 
	- 50 mA/250 mW
	- measure solar panel output with INA219 current sensor
	- [README](lab_01_electrical_power/README.md)
		- series/parallel--can it power?

Day 2
- [lab_03_communication/README.md](./lab_03_communication/README.md)

Day 3
- attitude determination
	- !! modify these lab activities from [lab_04a_attitude_determination/README.md](lab_04a_attitude_determination/README.md)
	- sun sensors
	- magnetometers

Day 4
- attitude actuation
- !! modify from [lab_04b_attitude_control/README.md](lab_04b_attitude_control/README.md) or  [lab_04c_attitude_control/README.md](lab_04c_attitude_control/README.md) 
	

Day 5
- remote sensing [lab_02_camera_payload/README.md](lab_02_camera_payload/README.md)
- 

