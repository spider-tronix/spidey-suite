# spideyboot

spideyboot is a bootloader supporting the OTA update feature for Arduino and Atmel AVR. It depends on the [spideydude](https://github.com/spider-tronix/spidey-suite/tree/master/spideydude) code uploader. It has the following features:
- spideyboot is only 516 bytes hence more space to your code. 
- faster uploading of sketches. spideyboot operates at higher baud rates and has streamlined programming. 
- spideyboot implements "fastboot" that starts sketches immediately after power-on.
- compatible with ATmega8, ATmega168, and ATmega328p microcontrollers and many derivatives.*

(*) : Code under testing phase. It may have potential bugs with some of the microcontrollers mentioned above. However, fully tested with Atmega328p and its variants. 

## Building

The code needs to be built for every microcontroller separately. To do so, follow these steps:
- Download the [src](https://github.com/spider-tronix/spidey-suite/tree/master/spideyboot/) folder and open the folder with your desired code editor (ATMEL Studio preferred) 
- ```spideyboot.c``` is the main file. Open it and navigate to line 9 and set the appropriate ```F_CPU``` value
```
#define F_CPU 16000000L              // 16 MHz clock speed for ATMEGA328p. 
/* Set this value as per your microcontroller clock crystal */
```
- In the settings section (this varies as per the editor you use), select your desired microcontrollers name, and add ```.text=0xXXXX``` option to the compiler. 
    - ```0xXXXX```depends on the bootloader section size of your microcontroller.
    - spideyboot is ```516 bytes```. Hence, accordingly, .text address will vary. 
    - For instance, Atmega328p's total flash size is ```32768 bytes```. And as per the datasheet bootloader section exist in the last section of flash. 
    - Therefore, bootloader optimally should start from ```32768-516 = 32252 byte``` location. 
    As per the datasheet, the bootloader section can be either 256, 512, 1024, or 2048 words only. (1 word is 2 bytes). Hence, we need 512 words or 1024 bytes section. 
    - Therefore, the bootloader will start from ```32768-1024 = 31744``` byte location.
    - In word addressing, it will be ```15872``` word location. In hexadecimal, it will be ```0x3E00```
    - Finally, ```.text = 0x3E00 ``` for Atmega328p microcontroller. 
    - Similarly, for other microcontrollers, ```.text``` needs to be set very accurately. 
-  Also set flags such as ```Optimize for size (-Os)``` in compiler optimization, ```-Wl```, ```--relax``` in Linker settings. 
-  Finally, build the code. This will generate a ```.hex``` file. Note down the directory address where that HEX file is stored.

## Uploading the Bootloader
> For uploading the bootloader, a special programmer known as ```AVR ISP Programmer``` is required. 
> Also, ```avrdude``` is required. Avrdude can be downloaded from [here](http://download.savannah.gnu.org/releases/avrdude/). Follow [this](https://www.youtube.com/watch?v=B7KQ3W4RxcA) tutorial on setting up the environment so that avrdude can be called from the command prompt.
> Visit [this](https://www.ladyada.net/learn/avr/avrdude.html) page for avrdude  tutorial.

The command for updating flash is- 

```avrdude -v -c <programmer> -p <partno> -P <port> -U flash:w:"<Path to hex file>"```

For example, if the programmer is USBasp, the port is USB, part no is atmega328p, then the command will look like- 

```avrdude -v -c usbasp -p atmega328p -P usb -U flash:w:"<Path to hex file>"```

Where:
- ```<Path to hex file>``` is the full path where the HEX file resides in the system.
- NOTE: avrdude sometimes doesn't accept ```<Drive letter>:/``` path initial. So, check once before using.

After this, you might need to change the specific fuse bits value. For example, to select the 512 words bootloader section, in Atmega328p ```High Fuse``` byte value needs to be set to ```0xDC```. Hence, accordingly, fuse bytes needs to be changed.

##### Changing fuse bytes

The command for changing fuse bytes is-

``` avrdude -c <programmer> -p <partno> -P <port> -U <fusename>:w:<data>:m ```

where:
- ```<fusename>``` can be ```hfuse```, ```lfuse```, ```efuse```
- ```<data>``` is the 8-bit data. Preferably write in hexadeciaml form i.e ```0xDC```

Sample for setting high fuse to ```0xDC``` with usbasp programmer on atmega328p 

```avrdude -c usbasp -p m328p -P usb -U hfuse:w:0xDC:m```

## Testing

If the spideyboot bootloader code was successfully uploaded to flash at its intended location and all fuse bytes are correctly set, then after powering up, the built-in LED will blink three times, after which led will be off for 1-2 secs. And after that, the application code will start running.

For the very first time, there will be no application code. Hence the LED will glow continuously. And will remain like that. 

## Uploading application code

Write the application code and compile it with your target microcontroller. Get the Hex file and save it for future reference. 

> Visit [this](https://www.instructables.com/HOW-TO-GET-HEX-FILE-FROM-ARDUINO-/) tutorial on how to get hex file after compiling from Arduino IDE.

Now for uploading this hex file over the air (OTA), spideydude is needed. Know more about spideydude [here](https://github.com/spider-tronix/spidey-suite/tree/master/spideydude)

## Communication Protocol 

> The communication protocol while uploading the code on the flash memory is inspired by STK500 v1 protocol. More info on protocol [here](https://github.com/spider-tronix/spidey-suite/blob/master/spideyboot/PROTOCOL.md)

## Sample HEX Codes

A sample spideyboot bootloader hex file (compiled for AVR ATmega328p board, 16 MHz crystal) can be found [here](https://github.com/spider-tronix/spidey-suite/blob/master/spideyboot/HexCodes/)

## Bug Reports and Improvements
> Want to contribute? Great!

-  To report a ***bug*** in the software or to request *an enhancement*, create an [*issue*](https://github.com/spider-tronix/spidey-suite/issues) clearly stating the problem. Our team will look into the problem and will fix it as soon as possible.

- To suggest *changes/improvements*, create a [*pull-request*](https://github.com/spider-tronix/spidey-suite/pulls) with the modified files. Upon successfully validating the changes by us, the suggested changes will be added to the repository.

## Future Updates
 - Optimize further for size
 - Test on various other microcontrollers.

##### NOTE: spideyboot is inspired from Arduino's ```optiboot```