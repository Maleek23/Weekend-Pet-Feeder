# Lab Report: Embedded Systems for Weekend Pet Feeder

## Abdulmalik Ajisegiri
### December 5, 2023

### Contents
1. [Introduction](#introduction)
2. [Hardware Architecture](#hardware-architecture)
3. [Software Capabilities](#software-capabilities)
4. [Code Overview](#code-overview)
    1. [Main](#main)
    2. [User Command Handling](#user-command-handling)
    3. [Sensing & Dispensing](#sensing-and-dispensing)
5. [Detailed Code Analysis](#detailed-code-analysis)
    1. [initHw()](#inithw)
    2. [initUart0()](#inituart0)
    3. [initHm()](#inithm)
    4. [initEeprom()](#initeeprom)
    5. [parseFields()](#parsefields)
    6. [isCommand()](#iscommand)
    7. [PWM Initialization (initpwm())](#pwm-initialization-initpwm)
    8. [Main Function (main())](#main-function-main)
    9. [RTC Initialization and Interrupts](#rtc-initialization-and-interrupts)
    10. [Hibernation Module Initialization](#hibernation-module-initialization)
    11. [EEPROM Functions](#eeprom-functions)
    12. [System Initialization](#system-initialization)
    13. [Fill Mode, Alert, and Schedule Commands](#fill-mode-alert-and-schedule-commands)
6. [Observations](#observations)
7. [Design and Implementation](#design-and-implementation)
    1. [Feeder Structure and Components](#feeder-structure-and-components)
    2. [Water Delivery System](#water-delivery-system)
    3. [Food Dispensing Mechanism](#food-dispensing-mechanism)
    4. [Electronics Integration](#electronics-integration)
    5. [Safety Considerations](#safety-considerations)
    6. [User Interface and Control](#user-interface-and-control)
    7. [Power Management](#power-management)
    8. [Conclusion on Design](#conclusion-on-design)
8. [Enhancements](#enhancements)
9. [Conclusion](#conclusion)

### 1. Introduction<a name="introduction"></a>
This document presents an embedded systems project showcasing an automated weekend pet feeder. The system incorporates features such as water level sensing, scheduled food/water dispensing, user configuration, and low resource alerts, providing a reliable solution for pet owners during weekends.

### 2. Hardware Architecture<a name="hardware-architecture"></a>
The hardware comprises commercial off-the-shelf components and 3D printed parts, including a water bottle, food container, pet dish, delivery tube, auger, base, capacitive sensor, motors, MOSFETs, microcontroller, and peripherals (e.g., PIR, transducer, EEPROM). Safety measures, such as food-safe containers and sealed prints, are implemented. The choice of materials and integrated sensors allows for precise control of food and water resources.

### 3. Software Capabilities<a name="software-capabilities"></a>
The software demonstrates sophisticated features:

- **Water Level Detection:** Utilizes a capacitive sensor circuit interfaced to an analog comparator and timer modules for real-time water volume measurement.
- **Scheduled Feeding:** The TM4C123 MCU allows pre-programming and storing feed amounts and times in EEPROM to dispense food per a defined schedule.
- **User Interface:** A UART peripheral enables user configuration via commands for setting the current time, programming feeds, calibrating water volume, toggling motion-activated refills, and low resource alerts.
- **Input Validation:** Ensures user-supplied data is parsed into fields for validity before execution.
- **Persistent Data:** Feed schedules are saved in internal EEPROM to retain the programmed schedule.
- **Precision Control:** PWM and MOSFET controlled motors dispense measured amounts of food and water.

The code exhibits modular organization, hardware abstraction, and expert handling of the MCU’s peripherals. Custom commands interface with components to enact system behaviors like scheduling feeds, sensing water, and alerting users.

### 4. Code Overview<a name="code-overview"></a>

#### 4.1 Main<a name="main"></a>
The main routine handles:

- Hardware initialization
- UART, PWM, EEPROM, and peripheral setup
- Parsing user commands from the UART RX buffer
- Calling appropriate handlers for each command

The main while loop polls the UART, ready to receive and parse user commands.

#### 4.2 User Command Handling<a name="user-command-handling"></a>
Key user-facing functionality is exposed through UART commands. Custom handlers process and execute each command.

Example Feed Scheduling Command Flow:

1. User enters event details over UART
2. Input fields are parsed and validated
3. Extracted feed data gets written to the next available EEPROM slot
4. UART message confirms the written schedule

Similar flows handle time setting, water level alerts, reviewing schedules, etc. Invalid commands print error messages.

#### 4.3 Sensing & Dispensing<a name="sensing-and-dispensing"></a>
MCU peripherals like ADC, PWM, Timers, and GPIO allow interfacing with attached hardware to enact system functions:

- ADC comparator triggers on capacitive sensor to measure water level
- PWM signals control motors for precise dispensing
- Timers enable timed events and sensor sampling

### 5 Detailed Code Analysis<a name="detailed-code-analysis"></a>

#### 5.1 initHw()<a name="inithw"></a>
The initHw() function initializes various hardware components, including GPIO, analog comparator, PWM, and others. It ensures the proper configuration of the microcontroller peripherals required for the pet feeder system.

#### 5.2 initUart0()<a name="inituart0"></a>
The initUart0() routine configures the UART0 peripheral for serial communication. It sets registers such as UART clock, baud rate, and data length, facilitating communication with external devices or a user interface.

#### 5.3 initHm()<a name="inithm"></a>
The initHm() function initializes the Hibernation Module, utilizing the real-time clock (RTC) in calendar mode for scheduling events and timer interrupts. This module enables efficient power management for scheduled tasks.

#### 5.4 initEeprom()<a name="initeeprom"></a>
The initEeprom() function initializes the EEPROM controller, ensuring reliable read/write operations for non-volatile storage of schedule data. It is a crucial part of the system for persistently storing user-defined feeding schedules.

#### 5.5 parseFields()<a name="parsefields"></a>
The parseFields() function plays a key role in handling user input. It parses raw ASCII user input into discrete data fields for further processing. It identifies alphanumeric and numeric fields, allowing the system to extract command arguments accurately.

#### 5.6 isCommand()<a name="iscommand"></a>
The isCommand() function checks if the parsed input corresponds to an expected command. It validates that the required minimum arguments are provided before executing the command.
