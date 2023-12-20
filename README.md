# Weekend-Pet-Feeder
This Repo presents an embedded systems project showcasing an automated weekend pet feeder. The system incorporates features such as water level sensing, scheduled food/water dispensing, user configuration, and low resource alerts, providing a reliable solution for pet owners during weekends.

## Table of Contents
1. [Introduction](#introduction)
2. [Hardware Architecture](#hardware-architecture)
3. [Software Capabilities](#software-capabilities)
4. [Code Overview](#code-overview)
5. [Detailed Code Analysis](#detailed-code-analysis)
6. [Design and Implementation](#design-and-implementation)
7. [Observations](#observations)
8. [Enhancements](#enhancements)
9. [Conclusion](#conclusion)

## Introduction
This repository presents an embedded systems project showcasing an automated weekend pet feeder. The system incorporates features such as water level sensing, scheduled food/water dispensing, user configuration, and low resource alerts, providing a reliable solution for pet owners during weekends.

## Hardware Architecture
The hardware comprises commercial off-the-shelf components and 3D printed parts. Safety measures, including food-safe containers and sealed prints, are implemented for precise control of food and water resources.

## Software Capabilities
The software demonstrates sophisticated features including Water Level Detection, Scheduled Feeding, User Interface, Input Validation, Persistent Data, and Precision Control. The code exhibits modular organization, hardware abstraction, and expert handling of the microcontroller's peripherals.

## Code Overview
### Main
The main routine handles hardware initialization, UART, PWM, EEPROM, and peripheral setup. The main loop polls the UART, ready to receive and parse user commands.

### User Command Handling
Key user-facing functionality is exposed through UART commands. Custom handlers process and execute each command.

### Sensing & Dispensing
MCU peripherals like ADC, PWM, Timers, and GPIO allow interfacing with attached hardware to enact system functions.

## Detailed Code Analysis
Explore functions like initHw(), initUart0(), initHm(), initEeprom(), parseFields(), isCommand(), initpwm(), main(), and others that ensure proper hardware initialization, UART communication, and overall functionality of the pet feeder system.

## Design and Implementation
### Feeder Structure and Components
The feeder's physical structure is built on a whitewood base with 3D-printed components. Safety considerations include food-safe materials and precautions to prevent choking hazards for pets.

### Water Delivery System
Precise control over water dispensing is achieved through a DC gearhead motor connected to a peristaltic pump.

### Food Dispensing Mechanism
A 3D-printed auger, driven by a motor, dispenses predetermined amounts of kibble, ensuring efficient and controlled dispensing.

### Electronics Integration
Various sensors and actuators are interfaced with the TM4C123GXL microcontroller board.

### User Interface and Control
Users interact with the feeder through UART commands, facilitating configuration and monitoring.

### Power Management
Efficient power management is achieved using the Hibernation Module, extending the operational lifespan of the device.

## Observations
The system architecture is well-considered from both technical and user perspectives, incorporating careful coding practices and full utilization of internal resources.

## Enhancements
Future enhancements include enclosure safety and durability testing, automated monitoring and correction of food/water amounts, wireless connectivity, and app integration.

## Conclusion
The code demonstrates efficient leveraging of TM4C123GXL peripherals and custom UART routines, creating a functional weekend pet feeder prototype with embedded systems aptitude. Further testing and refinements are recommended.
