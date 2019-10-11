This example utilizes the xChips contained in the [XinaBox XK70 LoRaWAN IoT Starter Kit](https://xinabox.cc/products/xk70). Temperature, Humidity and Pressure is sent to the All Things Talk dashboard via a gateway hosted by The Things Network. The ability to actuate a relay is also incorporated in the example.

## LoRaWAN Brief Overview

In a nutshell, LoRaWAN is a media access control (MAC) protocol for wide area networks. It is designed to allow low-powered devices to communicate with Internet-connected applications over long range wireless connections. For more information, see [Background information about LoRaWAN](https://www.thethingsnetwork.org/docs/lorawan/).

## XK70

XK70 is a LoRaWAN IoT kit. The contents in the XK70 allow one to quickly get connected with LoRaWAN. The LMIC library orignally developed by IBM and ported to Arduino allows one to use the RF96 Low Power Long Range Transceiver Module (the core radio on RL02) as a LoRa node. This allows a user to transmit and recieve data to and from the nearest LoRa gateway, respectively. The inate limitations of LoRaWAN are payload size and transmission times. If your project requires larger payloads and a real time response, consider the [XinaBox XK19 IoT Starter Kit](https://xinabox.cc/products/xk19).

## All Things Talk (ATT) and The Things Network (TTN)

XinaBox has partnered with [ATT](https://www.allthingstalk.com/) to provide a rapid experience with LoRaWAN in IoT. An XK70 Kit can be selected as a device on [ATT maker](https://maker.allthingstalk.com/) with readily avaiable assets. All the user has to do is select the XK70 LoRaWAN Kit amongst the various devices and the LoRaWAN provider (in this case TTN). TTN provides the portal to interface with ATT. 

## XK70 Sensors and Actuators

To get started, the XK70 kit comes with a GPS module (SN01), a gas and weather sensor (SW02) and a relay actuator (OC03). 

### Applications

* Indoor air quality
* Weather forecast
* Tracking and Navigation
* Remote Control

## Microcontroller (MCU) and Power Supply (PS)

### MCU

XK70 is run by the SAMD21 with 32-bit ARM® Cortex® -M0+ processor (CC03). To put things into persepctive, on an atMega328P (CC01), the LMIC library itself uses 80% of program memory. The CC03 on the other hand is capable of executing LMIC, SN01, SW02 and CC03 libraries using only 15% memory. The kit also comes with a Raspberry Pi bridge (BR01) for using the Raspberry Pi to execute your program.

### PS

PB04 Dual AA Intelligent Battery Power Pack with onboard voltage and current measurements. These measurements can be obtained and sent to the ATT dashboard; ideal for remote applications. An alternative battery powered solution that comes with the kit is the PL02 3.7v LiPo Battery Power Supply. The IP02 (programming interface) can also be used as a power supply by simply connecting to a 5V USB source.

## XK70 Example

### The Things Network Setup

1. Create an account on [The Things Network](https://thethingsnetwork.org)
2. Navigate to [The Things Console](https://console.thethingsnetwork.org)
3. Select application and add a new application. Give the application a name and an optional description.

<img src="https://github.com/xinabox/arduino-LMIC/blob/master/extras/ttn1.png" alt="alt text" width="70%" height="auto" align="center">

4. Below the devices heading register a new device. Provide an ID for the device and generate the Device EUI.

<img src="https://github.com/xinabox/arduino-LMIC/blob/master/extras/ttn2.png" alt="alt text" width="70%" height="auto" align="center">

5. The keys will be generated similar to the image below. You will require these keys for OKdo.

<img src="https://github.com/xinabox/arduino-LMIC/blob/master/extras/ttn3.png" alt="alt text" width="70%" height="auto" align="center">

6. Navigate back to devices and open up the integrations tab. Add the AllThingsTalk Maker intergation.

<img src="https://github.com/xinabox/arduino-LMIC/blob/master/extras/ttn4.png" alt="alt text" width="70%" height="auto" align="center">

<img src="https://github.com/xinabox/arduino-LMIC/blob/master/extras/ttn5.png" alt="alt text" width="70%" height="auto" align="center">

7. Provide a unique ID and select the default access key and complete the integration. You have now created the integration between OKdo and The Things Network.

<img src="https://github.com/xinabox/arduino-LMIC/blob/master/extras/ttn6.png" alt="alt text" width="70%" height="auto" align="center">

### OKdo Setup

1. Create an account on [All Things Talk](https://maker.allthingstalk.com/).
2. Add a new ground.

<img src="https://github.com/xinabox/arduino-LMIC/blob/master/extras/okdo1.png" alt="alt text" width="70%" height="auto" align="center">

3. In the Devices menu, add a new device and select Xinabox XK70 LoRa IoT Kit.

<img src="https://github.com/xinabox/arduino-LMIC/blob/master/extras/att2.png" alt="alt text" width="70%" height="auto" align="center">

4. Add The Things Network integration.

<img src="https://github.com/xinabox/arduino-LMIC/blob/master/extras/att3.png" alt="alt text" width="70%" height="auto" align="center">

5. Provide a name for your device and paste your keys provided by The Things Network into the respective fields. ABP keys will only be generated when you connect to The Things Network. This can be reenetred afterwards by going to *settings > connectivity*.

<img src="https://github.com/xinabox/arduino-LMIC/blob/master/extras/att4.png" alt="alt text" width="70%" height="auto" align="center">

6. All assets will be generated automatically.

<img src="https://github.com/xinabox/arduino-LMIC/blob/master/extras/att7.png" alt="alt text" width="70%" height="auto" align="center">

7. In the settings enable use ABCL to convert custom binary data. We will be converting all binary data manually paste the attabcl.json contents in the field.

8.  Create a new pinboard to visualize your data and to easily control OC03. You can create a pinboard automatially by selecting the assets.

<img src="https://github.com/xinabox/arduino-LMIC/blob/master/extras/att11.png" alt="alt text" width="70%" height="auto" align="center">

That completes the OKdo setup.