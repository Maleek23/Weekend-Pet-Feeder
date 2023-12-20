Embedded Systems for Weekend Pet Feeder by:
**Abdulmalik Ajisegiri  
December 5, 2023**

## Contents

1. [Introduction](#1-introduction)
2. [Hardware Architecture](#2-hardware-architecture)
3. [Software Capabilities](#3-software-capabilities)
4. [Code Overview](#4-code-overview)
   - 4.1 [Main](#41-main)
   - 4.2 [User Command Handling](#42-user-command-handling)
   - 4.3 [Sensing & Dispensing](#43-sensing--dispensing)
5. [Detailed Code Analysis](#5-detailed-code-analysis)
   - 5.1 [initHw()](#51-inithw)
   - 5.2 [initUart0()](#52-inituart0)
   - 5.3 [initHm()](#53-inithm)
   - 5.4 [initEeprom()](#54-initeeprom)
   - 5.5 [parseFields()](#55-parsefields)
   - 5.6 [isCommand()](#56-iscommand)
   - 5.7 [PWM Initialization (initpwm())](#57-pwm-initialization-initpwm)
   - 5.8 [Main Function (main())](#58-main-function-main)
   - 5.9 [RTC Initialization and Interrupts](#59-rtc-initialization-and-interrupts)
   - 5.10 [Hibernation Module Initialization](#510-hibernation-module-initialization)
   - 5.11 [EEPROM Functions](#511-eeprom-functions)
   - 5.12 [System Initialization](#512-system-initialization)
   - 5.13 [Fill Mode, Alert, and Schedule Commands](#513-fill-mode-alert-and-schedule-commands)
6. [Observations](#6-observations)
7. [Design and Implementation](#7-design-and-implementation)
   - 7.1 [Feeder Structure and Components](#71-feeder-structure-and-components)
   - 7.2 [Water Delivery System](#72-water-delivery-system)
   - 7.3 [Food Dispensing Mechanism](#73-food-dispensing-mechanism)
   - 7.4 [Electronics Integration](#74-electronics-integration)
   - 7.5 [Safety Considerations](#75-safety-considerations)
   - 7.6 [User Interface and Control](#76-user-interface-and-control)
   - 7.7 [Power Management](#77-power-management)
   - 7.8 [Conclusion on Design](#78-conclusion-on-design)
8. [Conclusion](#9-conclusion)

## 1. Introduction

This document introduces an embedded systems project featuring an automated weekend pet feeder. The system integrates advanced functionalities, including water level sensing, scheduled food/water dispensing, user configuration, and low resource alerts, providing a reliable solution for pet owners during weekends.

## 2. Hardware Architecture
<img width="379" alt="Screenshot 2023-12-20 at 5 23 47 PM" src="https://github.com/Maleek23/Weekend-Pet-Feeder/assets/97371652/9be058f5-bf0d-4260-870b-c4b287cbcb25">

The hardware combines commercial off-the-shelf components with 3D printed parts, such as a water bottle, food container, pet dish, delivery tube, auger, base, capacitive sensor, motors, MOSFETs, microcontroller, and peripherals (e.g., PIR, transducer, EEPROM). Safety measures, including food-safe containers and sealed prints, are implemented, ensuring precise control of food and water resources.

## 3. Software Capabilities
<img width="379" alt="Screenshot 2023-12-20 at 5 21 25 PM" src="https://github.com/Maleek23/Weekend-Pet-Feeder/assets/97371652/f095dffd-3cc1-47a0-9e2a-ec2c835660bc">


The software demonstrates sophisticated features, including:
- **Water Level Detection:** Utilizes a capacitive sensor circuit interfaced to an analog comparator and timer modules for real-time water volume measurement.


![8a1c7f](https://github.com/Maleek23/Weekend-Pet-Feeder/assets/97371652/3fdba1b9-d068-4851-86a1-3ae31586bfd9)  
- **Scheduled Feeding:** The TM4C123 MCU allows pre-programming and storing feed amounts and times in EEPROM to dispense food per a defined schedule.
- **User Interface:** A UART peripheral enables user configuration via commands for setting the current time, programming feeds, calibrating water volume, toggling motion-activated refills, and low resource alerts.
- **Input Validation:** Ensures user-supplied data is parsed into fields for validity before execution.
- **Persistent Data:** Feed schedules are saved in internal EEPROM to retain the programmed schedule.
- **Precision Control:** PWM and MOSFET controlled motors dispense measured amounts of food and water. The code exhibits modular organization, hardware abstraction, and expert handling of the MCU’s peripherals. Custom commands interface with components to enact system behaviors like scheduling feeds, sensing water, and alerting users.

## 4. Code Overview

### 4.1 Main

The main routine handles:

- **Hardware initialization**
- **UART, PWM, EEPROM, and peripheral setup**
- **Parsing user commands from the UART RX buffer**
- **Calling appropriate handlers for each command**
The main while loop polls the UART, ready to receive and parse user commands.

### 4.2 User Command Handling

Key user-facing functionality is exposed through UART commands. Custom handlers process and execute each command.

**Example Feed Scheduling Command Flow:**

1. User enters event details over UART
2. Input fields are parsed and validated
3. Extracted feed data gets written to the next available EEPROM slot
4. UART message confirms the written schedule
Similar flows handle time setting, water level alerts, reviewing schedules, etc. Invalid commands print error messages.

### 4.3 Sensing & Dispensing

MCU peripherals like ADC, PWM, Timers, and GPIO allow interfacing with attached hardware to enact system functions:

- ADC comparator triggers on capacitive sensor to measure water level
- PWM signals control motors for precise dispensing
- Timers enable timed events and sensor sampling

## 5. Detailed Code Analysis

### 5.1 initHw()

The initHw() function initializes various hardware components, including GPIO, analog comparator, PWM, and others. It ensures the proper configuration of the microcontroller peripherals required for the pet feeder system.

### 5.2 initUart0()

The initUart0() routine configures the UART0 peripheral for serial communication. It sets registers such as UART clock, baud rate, and data length, facilitating communication with external devices or a user interface.

### 5.3 initHm()

The initHm() function initializes the Hibernation Module, utilizing the real-time clock (RTC) in calendar mode for scheduling events and timer interrupts. This module enables efficient power management for scheduled tasks.

### 5.4 initEeprom()

The initEeprom() function initializes the EEPROM controller, ensuring reliable read/write operations for non-volatile storage of schedule data. It is a crucial part of the system for persistently storing user-defined feeding schedules.

### 5.5 parseFields()

The parseFields() function plays a key role in handling user input. It parses raw ASCII user input into discrete data fields for further processing. It identifies alphanumeric and numeric fields, allowing the system to extract command arguments accurately.

### 5.6 isCommand()

The isCommand() function checks if the parsed input corresponds to an expected command. It validates that the required minimum arguments are provided before executing the command.

### 5.7 PWM Initialization (initpwm())

The `initpwm()` function initializes the PWM module, configuring GPIO pins and PWM parameters. This enables precise control over the motors used for dispensing food and water. By setting appropriate PWM values, the system achieves accurate and adjustable dispensing of predetermined amounts of food and water.

### 5.8 Main Function (main())

The main function orchestrates the overall functionality of the system. It includes:

- **Hardware initialization:** Calls functions to set up various hardware components, ensuring they are ready for operation.
- **UART, PWM, EEPROM, and peripheral setup:** Configures communication interfaces, PWM for motor control, and initializes EEPROM for persistent data storage.
- **Parsing user commands from the UART RX buffer:** Monitors the UART for incoming user commands, parsing and validating them for further execution.
- **Calling appropriate handlers for each command:** Routes parsed commands to specific handlers, enabling the execution of corresponding functionalities.

The main loop continuously checks for user commands, making the system responsive to user inputs and ensuring the proper functioning of the pet feeder.

### 5.9 RTC Initialization and Interrupts

The `initHm()` function initializes the Hibernation Module, utilizing the real-time clock (RTC) in calendar mode. This setup facilitates scheduling events and timer interrupts, crucial for managing power efficiently. The RTC ensures precise timing for scheduled tasks, contributing to the system's reliability.

### 5.10 Hibernation Module Initialization

The Hibernation Module initialization involves configuring the RTC for calendar mode. This allows the system to enter low-power states during idle periods, conserving energy. By utilizing the RTC and implementing hibernation, the pet feeder can operate autonomously while consuming minimal power.

### 5.11 EEPROM Functions

The `initEeprom()` function ensures the proper initialization of the EEPROM controller. This is essential for reliable read and write operations, enabling the storage and retrieval of user-defined feeding schedules. Persistent data storage in EEPROM ensures that programmed schedules are retained even during power cycles.

### 5.12 System Initialization

The system initialization involves setting up various hardware components, communication interfaces, and peripherals. It ensures that the embedded system is in a known state before executing user commands. Proper system initialization contributes to the stability and functionality of the pet feeder.

### 5.13 Fill Mode, Alert, and Schedule Commands

These functions handle specific user commands:

- **Fill Mode:** Initiates the filling of the water dish when activated by the user. Utilizes PWM-controlled motors for precise water dispensing.
- **Alert Command:** Triggers low resource alerts, notifying the user visually and audibly when water levels are low.
- **Schedule Commands:** Enables the user to set and review feeding schedules. The schedules are stored in EEPROM for persistent use.

## 6. Observations

This section provides insights into the operation and performance of the embedded pet feeder system. It may include observations related to user interactions, system responsiveness, and any unexpected behaviors encountered during testing and usage.

## 7. Design and Implementation

### 7.1 Feeder Structure and Components

The design incorporates 3D-printed components, including the feeder body, auger, bottom mount, water bottle holder, and PCB brackets. The whitewood base provides a sturdy foundation for the system. Safety considerations, such as food-safe materials and sealed prints, ensure the feeder's reliability.

### 7.2 Water Delivery System

The water delivery system utilizes a 16oz plastic jar, silicone tubing, and a peristaltic pump. The system's design allows for precise water dispensing, and the capacitive water level sensor ensures accurate monitoring.

### 7.3 Food Dispensing Mechanism

The food dispensing mechanism consists of a DC gearhead motor, auger, and PLA printed components. PWM control enables the motor to dispense predetermined amounts of kibble, providing controlled and scheduled feeding.

### 7.4 Electronics Integration

The integration of various electronic components, including sensors (PIR, capacitive), actuators (motors, pump), and the microcontroller (TM4C123GXL), is crucial for the system's overall functionality. Careful integration ensures seamless communication and coordination among components.

### 7.5 Safety Considerations

Safety measures include the use of food-safe materials, sealed 3D prints, and the prevention of potential choking hazards. Adherence to UTA safety rules and guidelines during implementation is emphasized to guarantee user and pet safety.

### 7.6 User Interface and Control

The user interface relies on UART commands, providing a straightforward means for users to interact with the pet feeder. Commands cover various functionalities such as setting time, programming feeds, calibrating water volume, and toggling motion-activated refills.

### 7.7 Power Management

Efficient power management is achieved through the Hibernation Module, allowing the system to enter low-power states during idle periods. This enhances the pet feeder's autonomy while conserving energy.

### 7.8 Conclusion on Design

The design successfully integrates mechanical, electronic, and software components to create a functional pet feeder. Safety measures, precise dispensing mechanisms, and user-friendly interfaces contribute to the robustness and usability of the system.


## 9. Conclusion

In conclusion, the embedded weekend pet feeder project demonstrates a comprehensive integration of hardware and software components to create a reliable and user-friendly solution for pet owners. The design considerations, safety measures, and advanced features contribute to a proof-of-principle system that showcases the capabilities of embedded systems in addressing practical needs. The project provides a foundation for further exploration and enhancements in automated pet care systems.
