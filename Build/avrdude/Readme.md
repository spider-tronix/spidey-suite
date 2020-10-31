# AVRDUDE Cheat Sheet

### To upload HEX file
Command: ```avrdude -v -cusbasp -patmega328p -Pusb -Uflash:w:"<Path to hex file>"```

Where:
- ```<Path to hex file>``` is the full path where the HEX file resides in the system.
- NOTE: avrdude sometimes doesnt accept ```<Drive letter>:/``` path initial. So, check once before using.

### To change Fuse
Command: ``` avrdude -c usbasp -p m328p -P usb -U <fusename>:w:<data>:m ```

Where:
- ```<fusename>``` can be ```hfuse```,```lfuse```,```efuse```
- ```<data>``` is the 8-bit data. Preferably write in hexadeciaml form i.e ```0xDE```
