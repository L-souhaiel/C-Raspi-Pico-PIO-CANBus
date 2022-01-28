# CAN bus interface on raspberry pi pico 

  An embedded CAN bus interface to send and receive Data with a baudrate of 125 kbt/s in C assembly developed on Raspberry Pi Pico based on the programmable I/O module and Pico state machines.
  
  
![Layout](https://github.com/TitiLouati/C-Raspi-Pico-PIO-CANBus/blob/main/CANBus-PIO-Pico/PicoCAN.jpeg)

# Background

## Controller Area Network 

The Controller Area Network a.o. CAN was originally developed for the automotive industry. Its most common application is therefore electronic networking in vehicles. However, over the last 20 years, other industries have noticed the advantages and reliability of the CAN bus and have adopted it for a variety of applications. For example, railway applications such as trams, metros, light rail and long-distance trains are equipped with CAN. The CAN bus can be found on numerous levels and in a wide variety of networks in these vehicles, for example to connect door units, in brake controls or in passenger counters. The CAN is also used in aircraft with aircraft with flight status sensors and navigation systems in the cockpit. In addition, CAN buses are found in numerous aerospace applications, from in-flight data analysis to control systems for aircraft engines such as fuel systems, pumps and linear drives.

## Programmable I/O

Programmable I/O (PIO) is a hardware component developed for the Raspberry Pi Pico. It enables the creation of new or additional types of hardware interfaces on the
Raspberry Pico-based device. Given the intended peripherals on an MCU and the need for four UART or SPI or other communication buses that the RP2040 does not
supported by the RP2040, programmable I/O can solve this problem.

# Example

Assuming that the Software is Already builded and executed on the Raspberry Pi pico and all the Dependecies are installed and settled ,the user can send and receive via CAN Bus with a baudrate of 125 kbt/s. 

To sent data the user should choose first in the code the Message identifier and the data to be send.the defaultsetteled Message Id is 0x446 and 8 Bytes Data will be send from 0x00 to 0x07, then the user should pusch the the press button to send data via CAN bus. 

When data are coming ,the will be Automatically pushed from the Input shift register into the FIFO and will be processed then on the USB console the user see the received Message ID and Data bytes then they will be stored in Array of structure containing CAN Bus Data. 

# Dependencies

## Software 

The Raspberry Pi Pico C-SDK should be installed with the ARM C compiler , these can be done by following these Link [Getting Started With Raspberry Pi Pico ](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf).  

## Hardware

The MCP2555 should be wired with the CAN_high and CAN_Low to handle transformation of binary message to Analogue message due to voltage differentiation. The complete Layout should be like below: 



![Layout](https://github.com/TitiLouati/C-Raspi-Pico-PIO-CANBus/blob/main/CANBus-PIO-Pico/schaltungPCB.png)


# Installation 

The Raspberry Pi Pico C-SDK should be installed from source. and this project should be cloned on the local host then : 

to only run this project on the Raspberry Pi pico , the CAN_Rx.uf2 should be copied from the build folder into the Raspberry Pi pico. 

to make some change like New Message_ID or the Data Byte that should be sendet that can be done from  CAN_Rx.c . than the user shoud build the project . to do 

that run :

```
cmake ..
```

than : 

```
make

```






