/*
**************************************************************************************
                       spideydude.h for ESP32
                   ================================
  Library header file for communication with spideyboot bootloader. Custom protocol
  inspired by stk500v1 is used. spideydude is inspired by avrdude.  
***************************************************************************************
*/
#ifndef __SPIDEY_DUDE_H__
#define __SPIDEY_DUDE_H__

#include <Arduino.h>

#include "spideyprotocol.h"

#define RXD2 16
#define TXD2 17

#define _MAJOR_VERSION_ 0
#define _MINOR_VERSION_ 8
#define _SUB_MINOR_VERSION_ 0

struct PROG {
  String fileName;
  uint8_t *progData;
  size_t len;
  uint16_t pageSize;
};

class Spideydude{
  private:
    String response;
    PROG details;
    void ResetExtDevice();
    void Verbose(const char*);
    void RW(const char*);
    void Done();
    void sync();
    void readDevice();
    void writeFlash();
    void verifyFlash();
    void endProg();
  public:
    String begin(String, uint8_t*, size_t, uint16_t);
};

void showHEX(const byte);   

#endif 
