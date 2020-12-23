# spideyAVRdude

spideyAVRdude is an ESP32 based over the air (OTA) code uploader for AVR microcontrollers with a bootloader (USART/UART based). It provides the following features:
  - Can be used with any Atmel AVR or Arduino device having a bootloader. 
  - Easy to use code upload interface. 
  - Can connect to an already existing WiFi network or can create its own WiFi station.
  - Works on Intel's **HEX** file system. 
  - Easy to integrate code for converting your existing system into OTA updates compatible, without taking down the serial programming feature. 

##### NOTE:
- Only **FLASH** programming is supported.
- Bootloaders supporting STK500v1 protocol are supported.

## Uploading the Code on ESP32

> Arduino IDE will be used to upload the code to ESP32 easily. 
> Make sure you have Arduino IDE and follow [this](https://lastminuteengineers.com/esp32-arduino-ide-tutorial/) tutorial first to set up few things.
> Make sure you follow till "Blink" code upload step.

After setting up the Arduino IDE for ESP32 programming, now follow these steps:

- Download the [src]() folder and open the *src.ino* file. 
- Navigate to line no 34 and select the appropriate mode. **Default is AP mode**.
```
#define OTA_WIFI_MODE 0
```
| Code | Mode | Description|
| ------ | ------ |------ |
| 0 | Access Point (AP) | In this mode, the ESP32 will create its own WiFi AP. |
| 1 | Station |In this mode, ESP32 will connect to one of the already existing WiFi networks. User needs to provide the network credentials|
- If opted for **Station Mode (1)**, navigate to line 42 and provide the existing WiFi SSID and its Password. Otherwise, skip this step. 
```sh
const char* ssid = "NETWORK_SSID_HERE";
const char* password = "NETWORK_PASSWORD_HERE";
```
- Now, navigate to line 57 and select the clock code based on the clock value on which your target microcontroller operates.
```
#define F_CLK 0
```
| Clock | Code |
| ----- | ----- |
| Greater or equal to 8 MHz | 0 |
| Greater or equal to 1 MHz  | 1 |
| Greater or equal to 128 kHz | 2 |

**Make sure you choose the correct code value for F_CPU because based on this value only, the BAUD_RATE will be set to communicate with your AVR device. Any random value might lead to failure during the code flashing process**

- That's it, now the code can be uploaded to your ESP32 board. 
- After uploading the code, open the Serial Monitor and note down the IP Address printed on the screen. 
- ![Serial Monitor Image](https://github.com/spider-tronix/spidey-suite/)

## Circuit Connection 

A custom PCB module is in the making. Until then, ESP32 Dev Board (ESP32 based node MCU) can be used. The circuit connection is as follows: 
| ESP32 Dev Board | Microcontroller |
| ------ | ------ |
| GND | GND |
| GPIO21 | RESET |
| TX2 | RX |
| RX2 | TX |
| Vin | Vcc |

**Note:**

- If your microcontroller has Vcc more than 5V, then do not connect it to Vin. Arrange a 5V supply. Make sure to join the GND of the external supply to both - ESP and your microcontroller. 
- Microcontroller's digital pin will not be sufficient to provide enough current to the ESP32 therefore, do not give a connection from the digital I/O pin even though the digital pin is of 5V level. 
- **TX2** and **RX2** of ESP32 are being used for the communication. Take care of this otherwise system won't work if you connect it to the default TX and RX pins of ESP32.


## Deploying OTA Update

To update your microcontroller, you need to have the compiled **HEX file** of your source code. 

To know how to get a HEX file from a source code - 
- in Arduino IDE, click [here](https://www.instructables.com/HOW-TO-GET-HEX-FILE-FROM-ARDUINO-/)
- in Atmel Studio, click [here](https://avrstudio5.wordpress.com/2012/05/17/creating-hex-lss-map-files-when-building/)
 
Now, follow these steps:

- Make the circuit connection. 
- Power up the system. Once done, wait for a few seconds
- Now, in your device from which you will be uploading code,
   - if ESP is in **Station Mode**, connect your device to the same WiFi network.
   - if ESP is in **Access Point Mode**, you will notice a new WiFi signal named ```Avrdude```. Connect to it by entering ```spider.123``` as password. 
- Open a browser and then hit URL - ```http://avrdude.local```
- A page like this should come - 
    - ![Main page]() 
If the first time page doesn't show up and a DNS error comes up, then try manually once by entering the IP Address which you noted down when code was uploaded to ESP32. This time the page should come. 
- Then, click on ***Choose File...***. A window like this should pop up. Navigate to the directory where the HEX file is stored. Select it and then click **Open**.
  - ![File Uploading Window]() 
- Finally, click on ***Upload*** button and wait for few seconds. The system will update your microcontroller with the code you have provided. 
  - ![Upload button selected]() 
- Once the process finishes, a ***Console Log*** modal box should come up as shown -
   - ![File Uploading Window]() 
   - In this log, you can see whether the process successfully flashed your chip or not.
   - If any error percentage is shown, it's recommended that the user should try flashing again. Error is only detected, not rectified.
   - If any other error comes up, check for the circuit connection, especially Rx, Tx pins, and the Reset pin connectivity. 

- ###### NOTE: The flashing process is tested and verified up to 32kb of Flash-only. It's possible that beyond that, few bugs might be there. 

## Bug Reports and Improvements
> Want to contribute? Great!

-  To report a ***bug*** in the software or to request *an enhancement*, create an [*issue*](https://github.com/spider-tronix/spidey-suite/issues) clearly stating the problem. Our team will look into the problem and will fix it as soon as possible.

- To suggest *changes/improvements*, create a [*pull-request*](https://github.com/spider-tronix/spidey-suite/pulls) with the modified files. Upon successfully validating the changes by us, the suggested changes will be added to the repository.

## Future Updates
 - Support EEPROM programming. 
 - Support lfash update of more than 32kb.

### Product under testing phase