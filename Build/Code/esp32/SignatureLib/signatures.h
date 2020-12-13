/* signatures.h
 * Created on 13 Nov 2020
 * Author : N Srikrishna, Aditya
 * Version: 0.6.0
*/
#ifndef ESP_SIGNATURE_H
#define ESP_SIGNATURE_H

#include <stdlib.h>
#include <arduino.h>

#define length(var) ((unsigned int)(sizeof(var)/sizeof(var[0])))    //Macros to get the length of an array

const unsigned long kb = 1024;

typedef struct {
  byte sig[3];
  String desc;
  unsigned long flashSize;
  int maxBootSize;
  int pageLen;
  bool sopti;
} signatureProperties;

struct device {
  int deviceMatchIndex = -1;
  byte sig[3];
};

const signatureProperties signatures[] = {
  //     signature          description   flash size   maxbootsize    pagelen    yesopti
  // Attiny84 family
  { { 0x1E, 0x92, 0x07 },    "ATtiny44",    4 * kb,          0,          64,         0},
  { { 0x1E, 0x91, 0x0B },    "ATtiny24",    2 * kb,          0,          32,         0},
  { { 0x1E, 0x93, 0x0C },    "ATtiny84",    8 * kb,          0,          64,         1},
  // Attiny85 family 
  { { 0x1E, 0x91, 0x08 },    "ATtiny25",    2 * kb,          0,          32,         0},
  { { 0x1E, 0x92, 0x06 },    "ATtiny45",    4 * kb,          0,          64,         0},
  { { 0x1E, 0x93, 0x0B },    "ATtiny85",    8 * kb,          0,          64,         0},
  // Atmega328 family
  { { 0x1E, 0x92, 0x0A },  "ATmega48PA",    4 * kb,          0,          64,         0},
  { { 0x1E, 0x93, 0x0F },  "ATmega88PA",    8 * kb,     1 * kb,          64,         1},
  { { 0x1E, 0x94, 0x0B }, "ATmega168PA",   16 * kb,     1 * kb,         128,         1},
  { { 0x1E, 0x94, 0x06 },  "ATmega168V",   16 * kb,     1 * kb,         128,         1},
  { { 0x1E, 0x95, 0x0F },  "ATmega328P",   32 * kb,     2 * kb,         128,         1},
  { { 0x1E, 0x95, 0x16 }, "ATmega328PB",   32 * kb,     2 * kb,         128,         1},
  // Atmega644 family
  { { 0x1E, 0x94, 0x0A },  "ATmega164P",   16 * kb,     1 * kb,         128,         0},
  { { 0x1E, 0x95, 0x08 },  "ATmega324P",   32 * kb,     2 * kb,         128,         0},
  { { 0x1E, 0x96, 0x0A },  "ATmega644P",   64 * kb,     4 * kb,         256,         0},
  // Atmega2560 family
  { { 0x1E, 0x96, 0x08 },   "ATmega640",   64 * kb,     4 * kb,         256,         0},
  { { 0x1E, 0x97, 0x03 },  "ATmega1280",  128 * kb,     4 * kb,         256,         1},
  { { 0x1E, 0x97, 0x04 },  "ATmega1281",  128 * kb,     4 * kb,         256,         0},
  { { 0x1E, 0x98, 0x01 },  "ATmega2560",  256 * kb,     4 * kb,         256,         1},
  { { 0x1E, 0x98, 0x02 },  "ATmega2561",  256 * kb,     4 * kb,         256,         0},
  // AT90USB family
  { { 0x1E, 0x93, 0x82 },   "At90USB82",    8 * kb,     2 * kb,         128,         0},
  { { 0x1E, 0x94, 0x82 },  "At90USB162",   16 * kb,     2 * kb,         128,         0},
  // Atmega32U2 family
  { { 0x1E, 0x93, 0x89 },   "ATmega8U2",    8 * kb,     2 * kb,         128,         0},
  { { 0x1E, 0x94, 0x89 },  "ATmega16U2",   16 * kb,     2 * kb,         128,         0},
  { { 0x1E, 0x95, 0x8A },  "ATmega32U2",   32 * kb,     2 * kb,         128,         1},
  // Atmega32U4 family
  { { 0x1E, 0x94, 0x88 },  "ATmega16U4",   16 * kb,     2 * kb,         128,         0},
  { { 0x1E, 0x95, 0x87 },  "ATmega32U4",   32 * kb,     2 * kb,         128,         1},
  // ATmega1284P family
  { { 0x1E, 0x97, 0x05 }, "ATmega1284P",  128 * kb,     4 * kb,         256,         1},
  { { 0x1E, 0x97, 0x06 },  "ATmega1284",  128 * kb,     4 * kb,         256,         1},
  // ATtiny4313 family
  { { 0x1E, 0x91, 0x0A }, "ATtiny2313A",    2 * kb,          0,          32,         0},
  { { 0x1E, 0x92, 0x0D },  "ATtiny4313",    4 * kb,          0,          64,         0},
  // ATtiny13 family
  { { 0x1E, 0x90, 0x07 },   "ATtiny13A",    1 * kb,          0,          32,         0},
  // Atmega8A family
  { { 0x1E, 0x93, 0x07 },    "ATmega8A",    8 * kb,      1 * kb,       1024,         1},
};

String verifySignature(byte b[]);
void printDetails();
String hexTOstring(byte a);
unsigned long getFlashSize();
int getPageSize();
int getBootloaderSize();

#endif
