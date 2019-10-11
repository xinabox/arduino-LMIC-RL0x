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

## Microcontoller (MCU) and Power Supply (PS)

### MCU

XK70 is run by the SAMD21 with 32-bit ARM® Cortex® -M0+ processor (CC03). To put things into persepctive, on an atMega328P (CC01), the LMIC library itself uses 80% of program memory. The CC03 on the other hand is capable of executing LMIC, SN01, SW02 and CC03 libraries using only 15% memory. The kit also comes with a Raspberry Pi bridge (BR01) for using the Raspberry Pi to execute your program.

### PS

PB04 Dual AA Intelligent Battery Power Pack with onboard voltage and current measurements. These measurements can be obtained and sent to the ATT dashboard; ideal for remote applications. An alternative battery powered solution that comes with the kit is the PL02 3.7v LiPo Battery Power Supply. The IP02 (programming interface) can also be used as a power supply by simply connecting to a 5V USB source.

## XK70 Example



