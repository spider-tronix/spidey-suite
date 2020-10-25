# OTA Programming Module
A module to add OTA Programming functionality to AVR Microcontrollers

## Abstract
OTA (Over the Air) Programming is a process which allows devices to upgrade their firmware or software wirelessly without any physical access. It uses wireless technology like Wi-Fi, Bluetooth, GPRS or 4G/3G rather than wired serial communication. OTA is used to reprogram the devices like microcontrollers, cell phones, computers, set-top box etc in order to add some feature, resolve some bugs, etc. The project aims to make a module which can be attached to any AVR microcontroller in order to add OTA Programming feature to it. The module will be just like any other peripheral sensor modules with few pins left open for the user to connect it to their circuit. The module will be cost effective and as minimum space consuming as possible. Also, a web portal will be developed where in the corresponding codes for the target μC for example – Arduino UNO/Mega/Nano can be downloaded and configured as per the user’s requirement. And a dedicated HTTP interface wherein the codes can be sent over the air.

# Status
#### Phase 1:
- Understanding complete memory architecture of a μC
- Understanding bootloader, how OTA works.
- How a program is transferred in the μC. 
- How to write a light weight custom bootloader which can be added on top of an existing bootloader so that the μC can still be programmed from Arduino IDE (if using Arduino boards). 
- If that’s not feasible (considering memory consumption by two bootloaders), how to write a standalone bootloader. 

#### Phase 2:
- Implementing the understanding done in the previous phase in the UNO and Mega boards. 
- Developing the portal
- Generalising the code in order to target more than 1 μC with the same code if possible (due to same architecture) 

# Team:
  - Aditya Kumar Singh [https://github.com/adityasingh3007] 
  - Miriam [https://github.com/Githubmsk]
  - Arunesh Saddish [https://github.com/arunesh006]
  - N Srikrishna [https://github.com/2001srikrishna]


## NOTE: Project under development phase.