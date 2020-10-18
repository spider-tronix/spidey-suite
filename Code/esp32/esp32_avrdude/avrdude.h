#ifndef __AVRDUDE_OTA_H__
#define __AVRDUDE_OTA_H__

#include <Arduino.h>
#include "stk500.h"

#define avrdudeVersion "0.1.0"

#define RXD2 16
#define TXD2 17

class Avrdude{
  private:
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
    void begin();                                             // initializer. Process starts here
};

/* Other Uitility functions */
void showHEX(const byte);                                 // Print the HEX number properly on monitor/console

#endif
