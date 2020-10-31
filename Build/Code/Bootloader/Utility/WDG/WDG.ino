#include <avr/io.h>
#include <util/delay.h>
#include "watchdog.h"

int main(void) {
  DDRB = 0xFF;
  PORTB = 0x00;
  WATCHDOG_CONFIG(WATCHDOG_OFF);
  for(int i=0;i<10;i++){
    PORTB ^= 0xFF;
    _delay_ms(100);
  }
  PORTB = 0x00;
  WATCHDOG_CONFIG(WATCHDOG_8S);
  while (1) {
    PORTB = 0xFF;
    _delay_ms(1000);
    PORTB = 0x00;
    _delay_ms(1000);
  }
  return 0;
}
