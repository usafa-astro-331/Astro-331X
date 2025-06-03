Change:
- program radio channels (etc.) manually, instead of having Arduino program half on startup
- investigate (serial plotter? serial monitor?) effect of antenna orientation
- use big antennas?
- predict RSSI at distances
- measure

# Lab 3: communication

In this lab you will build FlatSAT's communication system including the onboard radio and ground station with dipole antenna. You will test communication and measure signal strength at multiple link distances to characterize the link budget and ensure FlatSAT has sufficient link margin to communicate from orbit. 

In this lab one laptop will serve as FlatSAT's ground station. The ground station computer will send commands to the spacecraft over an XBee serial link. 

One additional laptop or power supply is required to power FlatSAT. 

Predict communication system performance using the prelab report instructions: [https://www.overleaf.com/read/qksywqwkjcqb#3d1ec1](https://www.overleaf.com/read/qksywqwkjcqb#3d1ec1)

## equipment

- 2 laptops (one only provides power to FlatSAT)
- USB cables
- measuring tape

## hardware

FlatSAT
- components from previous labs
- XBee radio
	- on Explorer adapter board
- laptop for USB power

ground station
- laptop with serial terminal (PuTTY)
- XBee radio with RP-SMA RF connector
	- on USB Explorer adapter board
- 2x antenna (2 different lengths)
- LED and 220 Ω resistor

## software

- Arduino IDE
- Arduino SAMD drivers (already installed)
- PuTTY (serial terminal program)
- [lab_03_communication](lab_03_communication.ino)

## overview

In this lab the XBee–XBee link will act as a wireless serial connection. 

Ground-to-space communication:

```mermaid
%%{ init: { 'flowchart': { 'curve': 'stepAfter' } } }%%

graph LR;

    x1[serial terminal]--USB serial--> x2[ground XBee];
    x2-. "2.4 GHz" .-> x3[FlatSAT XBee]; 
    x3--wired serial--> x4[Arduino];
```

After receiving a command, FlatSAT will produce a response and return it to the ground station using the same link in reverse. 

Each XBee radio maintains a record of the signal strength of the last message it received. An RSSI query from the ground station involves the following steps:

- query message from ground station via XBee to FlatSAT
- (FlatSAT XBee keeps record of the query message’s RSSI since it is the latest message)
- Arduino asks FlatSAT XBee for RSSI
- FlatSAT XBee responds in hex format
- Arduino converts response to ASCII decimal format
- Arduino sends RSSI string via XBee to ground station

Both radios will need to be programmed with compatible parameters (channel, etc.) before they can communicate. 

## Pair radios

Pair your radios. Your radios must be configured with matching parameters. Complete the rest of this section twice--once for each radio. 

1. Connect the XBee breakout board to your computer via USB
2. Open PuTTTy serial terminal: ../computer_environment/PuTTYPortable/PuTTYPortable.exe
3. PuTTY settings ![putty settings](sources/PuTTYPortable.png)
	- Select 'XBee', hit load
	- Select 'Serial'
	- Change 'COM#' to the correct number
		- (look at device list in Arduino IDE to determine radio's COM number)
4. Open


To configure a radio you must enter AT command mode. When entering data into a serial terminal, **DO NOT TYPE BACKSPACE**. The backspace key counts as a key and does not delete the previous keystroke. If you make a mistake, wait and try again. 
See image below for an example. 

To enter command mode: 

- type nothing for 1 second

- type `+++` (do not press Enter)

- type nothing for 1 second

The local radio will reply `OK`. You are now in command mode. Command mode will timeout after 10 seconds of inactivity. 

1. enter command mode

2. enter relevant configuration commands (see table below). 
   
   - NOTE: **DO** press `Enter` after entering an AT command

|                                      | AT command | ground station | FlatSAT | note                                                                                                                                                                                                                                  |
| ------------------------------------ | ---------- | -------------- | ------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| network ID                           | ATID       | match          | match   | 4-digit number between 0000 and 9999---deconflict with other groups                                                                                                                                                                   |
| destination address (of other radio) | ATDH       | 0              | 0       |                                                                                                                                                                                                                                       |
|                                      | ATDL       | 0              | 1       |                                                                                                                                                                                                                                       |
| radio address                        | ATMY       | 1              | 0       | ATDL radio 1 = ATMY radio 2                                                                                                                                                                                                           |
| baud rate                            | ATBD       | 3              | 3       | 3 = 9600 bps<br/>see [documentation ](https://www.digi.com/resources/documentation/Digidocs/90001477/reference/r_cmd_bd_xtend.htm?TocPath=AT%20commands%7CSerial%20interfacing%20commands%7C_____1) for other data rates if necessary |

3. `ATWR` save current configuration to radio memory

4. `ATCN` to immediately exit command mode (or wait 10 seconds). 

![AT commands](./sources/AT_commands.png)

Repeat for the other radio. 


## setup

Disconnect spacecraft radio from USB and connect to FlatSAT
  - see connections in `xbee_pins.h`

Add LED and resistor
  - as listed in `LED_pins.cpp`
  - LEDs are directional—long leg of LED is positive (3.3 V)
  - LEDs will burn out without a resistor

<img src="../../fritzing_diagrams/03_communication_bb.svg" alt="breadboard"  />

### XBee breakout

The XBee radio communicates with Arduino using serial communication. Serial communication uses two wires: transmit and receive. 

![XBee_explorer](sources/XBee_explorer.png)


## Setup the ground station

- If your FlatSat radio has an RP-SMA connector, attach a small antenna to it.

![img](sources/clip_image002.jpg)

- open `Tera Term` and connect to the ground station XBee 

## Program FlatSAT

- Connect FlatSAT to a PC with Arduino IDE installed
- Connect ground station radio to the same PC
- Open `lab_03_communication.ino`
- From the tools menu, select the correct board (MKR Zero) and port
- Upload code to FlatSAT

## test link

FlatSAT and the ground station should now be connected and communicating. Restart FlatSAT (the small blue button on the Arduino). The terminal on the ground station PC should display a welcome message. If not, reopen your serial connection and restart FlatSAT. 

If the link doesn’t work, connect to FlatSAT with the serial monitor and look for error messages. 

(You should start to see messages from Arduino. If they end with “ Received: OK and 0 0” then you are good to go. If the messages continue to autoscroll without stopping, you will need to disconnect your USB cable from your laptop and close the serial monitor. Then you will need to re-connect the USB cable to your laptop and re-open the serial monitor. If you do not get the “OK” message in the serial monitor the second time, double-check that you connected all of the wires correctly.)

## Sending and Receiving Data

FlatSAT responds to the following commands. 

```
1 help
2 get RSSI
3 toggle LED
```

- Type 3 to toggle the LED. 
- Type 2 to get XBee’s current RSSI. 
- collect RSSI values at the following distances

| distance (m) | RSSI (dB) |
| ------------ | --------- |
| 0.5          |           |
| 1            |           |
| 2            |           |
| 5            |           |

At each distance, also toggle the LED. 

Next, repeat the RSSI measurement at 5 m with a different size antenna. Record this RSSI value and the antenna change. 

Finally, see how far FlatSAT can communicate. If you go outside, call your teammate on the telephone to establish out of band communication. Record your RSSI at distance and use https://gmap-pedometer.com/ to determine your separation. 

## Lab station cleanup

- Disconnect devices from all computers
- Remove the ESD wrist straps and replace them in the bag at your lab station.
- Replace all items at your lab station the way you found them. 
- Have your instructor check off your lab station before you depart.

## End of course cleanup

NOTE: If you change xbee baud rate or channel during this class, you must remember to return it to default configuration at the end of the class. 

To enter command mode:

- type nothing for 1 second

- type `+++` (do not press Enter)

- type nothing for 1 second

The local radio will reply `OK`. You are now in command mode. Command mode will timeout after 10 seconds of inactivity.

4. enter command mode

5. enter relevant configuration commands (see table below).
   
   - NOTE: **DO** press `Enter` after entering an AT command

|                                      | AT command | ground station | FlatSAT | note                                                                                                                                                                                                                                 |
| ------------------------------------ | ---------- | -------------- | ------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| network ID                           | ATID       | match          | match   | 4-digit number between 0000 and 9999---deconflict with other groups                                                                                                                                                                  |
| destination address (of other radio) | ATDH       | 0              | 0       |                                                                                                                                                                                                                                      |
|                                      | ATDL       | 0              | 1       |                                                                                                                                                                                                                                      |
| radio address                        | ATMY       | 1              | 0       | ATDL radio 1 = ATMY radio 2                                                                                                                                                                                                          |
| baud rate                            | ATBD       | 3              | 3       | 3 = 9600 bps<br>see [documentation](https://www.digi.com/resources/documentation/Digidocs/90001477/reference/r_cmd_bd_xtend.htm?TocPath=AT%20commands%7CSerial%20interfacing%20commands%7C_____1)  for other data rates if necessary |

6. `ATWR` save current configuration to radio memory

7. `ATCN` to immediately exit command mode (or wait 10 seconds).

Switch radios (can use the same explorer breakout board) and repeat steps 4 through 7.
