/*
 * watchdog.h
 * Created: 10/10/2020 16:09:09 Hrs
 * Author: Aditya Kumar Singh
*/
#ifndef watchdog_h
#define watchdog_h

/* Watchdog Timer settings */
#define WATCHDOG_OFF    (0)
#define WATCHDOG_16MS   (_BV(WDE))
#define WATCHDOG_32MS   (_BV(WDP0) | _BV(WDE))
#define WATCHDOG_64MS   (_BV(WDP1) | _BV(WDE))
#define WATCHDOG_125MS  (_BV(WDP1) | _BV(WDP0) | _BV(WDE))
#define WATCHDOG_250MS  (_BV(WDP2) | _BV(WDE))
#define WATCHDOG_500MS  (_BV(WDP2) | _BV(WDP0) | _BV(WDE))
#define WATCHDOG_1S     (_BV(WDP2) | _BV(WDP1) | _BV(WDE))
#define WATCHDOG_2S     (_BV(WDP2) | _BV(WDP1) | _BV(WDP0) | _BV(WDE))
#ifdef  WDP3
#define WATCHDOG_4S     (_BV(WDP3) | _BV(WDE))
#define WATCHDOG_8S     (_BV(WDP3) | _BV(WDP0) | _BV(WDE))
#endif

#ifdef __cplusplus
extern "C" {
#endif
static inline  void WATCHDOG_RST();                       /* Function to reset the watchdog timer */
void __attribute__((noinline))  WATCHDOG_CONFIG(uint8_t); /* Function to configure the watchdog timer settings */
#ifdef __cplusplus
}
#endif

#endif
