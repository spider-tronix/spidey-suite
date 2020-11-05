/*
 * watchdog.c
 * Created: 10/10/2020 16:49:30 Hrs
 * Author: Aditya Kumar Singh
*/
#include <avr/io.h>
#include "watchdog.h"

/*
 * Function to reset the WATCHDOG timer
 */
static inline void WATCHDOG_RST() {
  __asm__ __volatile__ ("wdr\n");
}

/*
 * Function to configure the WATCHDOG timer with a particular
 * time setting
 */
void __attribute__((noinline)) WATCHDOG_CONFIG(uint8_t x) {
  #ifdef WDCE                                           // does it have a Watchdog Change Enable?
    #ifdef WDTCSR
      WDTCSR = _BV(WDCE) | _BV(WDE);
    #else
      WDTCR = _BV(WDCE) | _BV(WDE);
    #endif
  #else                                                 // then it must be one of those newfangled ones that use CCP
      CCP = 0xD8;                                       // so write this magic number to CCP
  #endif
  #ifdef WDTCSR
      WDTCSR = x;
  #else
      WDTCR = x;
  #endif
}
