/*
**************************************************************************************
                       avrdude.h for ESP32
                   ================================
  Library header file based on AVRDUDE for communication with AVR device.  
  
***************************************************************************************
*/
#ifndef __AVRDUDE_OTA_H__
#define __AVRDUDE_OTA_H__

#include <Arduino.h>
#include <stdint.h>

#include "signatures.h"
#include "stk500.h"

#define RXD2 16
#define TXD2 17

#define _MAJOR_VERSION_ 0
#define _MINOR_VERSION_ 5
#define _SUB_MINOR_VERSION_ 0

struct PROG {
  String fileName;
  uint8_t *progData;
  size_t len;
  uint16_t pageSize;
};

class Avrdude{
  private:
    String response;
    PROG details;
    /* Helper functions */
    void ResetExtDevice();                                    // Send RESET signal
    void Verbose(const char*);                                // Verbose output on the monitor/console
    void RW(const char*);                                     // Verbose printing for "Reading/Writing"
    void Done();                                              // Print avrdone. Thank you msg
    /*Essential functions*/
    void syncAVR();                                           // Synchronize the avrdude with the uC
    void readDevice();                                        // Read bootloader ver and other parameters
    void writeFlash();                                        // Read the prog bytes and writes to uC
    void verifyFlash();                                       // Verify the written flash for any errors
    void exitPgmMode();                                       // After verifying, exit the programming mode
  public:
    String begin(long, String, uint8_t*, size_t, uint16_t);         // initializer. Process starts here
};

/* Other Uitility functions */
void showHEX(const byte);                                     // Print the HEX number properly on monitor/console

#endif
