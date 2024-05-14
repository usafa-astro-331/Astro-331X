# Lab 1b: battery capacity

In this lab you will measure the capacity of FlatSAT's battery to determine if it will store enough energy to satisfy mission requirements. You will measure the voltage and current supplied by a lithium ion cell to generate a discharge curve. Discharging the battery may take several hours, so you will record data overnight. 

## hardware

- FlatSAT
  - existing components
  - micro SD card
  - solar array

## equipment

- 
- laptop
- Micro USB cable
- USB micro SD card reader
- wires

## software

- Arduino IDE
- `lab_01_electrical_power.ino`
- Arduino libraries (install using Arduino library manager)
  - `Adafruit INA219`

## setup

Start with the same setup you used for the solar array lab. Connect components according to this schematic. Replace the solar array with a lithium ion cell and the potentiometer with a power resistor. 

![](sources/electrical_schematic.svg)

Ensure your SD card is in your Arduino MKR Zero.

## upload FlatSAT code and test setup

Open `lab_01b_battery_capacity.ino` in the Arduino IDE. 

Connect Arduino to computer via USB, select the correct board, and upload your code. 

Open serial monitor (tools -> serial monitor) and ensure you are collecting data periodically.

Slowly turn the potentiometer and watch the current and voltage change in the serial plotter. (Deselect ‘value 1’, which is time.)

## data collection scheme

FlatSAT saves current and voltage to `iv_curve.tsv` every 200 ms. On each powerup it writes a legend and then begins logging data. Subsequent powerups will again write a legend and add more data to the file. The legend message can be used to separate test runs. It will be obvious which run is series vs parallel. 

## collect solar array performance data outside

Disconnect power to the Arduino so you're not saving excess data. 

Take FlatSAT and the luxmeter outside and find a test site in direct sunlight. 

Point the luxmeter toward the sun and record the sun's power in lux. You will have to adjust the range to its maximum setting. The measured value may jump around, try to take an average value. Record the average for later use calculating efficiency. 

Point FlatSAT at the sun and power it on. *Very* slowly (slower than you’re thinking), turn the potentiometer from one end to the other. FlatSAT will record current and voltage at 200 ms intervals. 

Remove the potentiometer to record a measurement of $V_{oc}$. Short the potentiometer pins with a wire to record a measurement of $I_{sc}$. 

Disconnect power to the Arduino, rewire the solar array in parallel, and repeat these steps to collect the parallel I-V data. 

## data collection and analysis

- transfer `iv_curve.tsv` to your computer for later analysis
- perform data analysis from `lab 1 analysis.pdf`

## cleanup

Leave all FlatSAT components and wires connected for future labs. 

Turn off everything and straighten your work area. 
