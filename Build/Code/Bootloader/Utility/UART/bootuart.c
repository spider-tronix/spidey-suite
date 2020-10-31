#include "bootuart.h"

void  __attribute__((noinline)) __attribute__((leaf)) writeUSART(uint8_t ch) {
    #if (SOFTUART == 0)
        #ifndef LINUART
            while (!(UART_SRA & _BV(UDRE0)));
        #else
            while (!(LINSIR & _BV(LTXOK)));
        #endif
    UART_UDR = ch;
    #endif
}

uint8_t __attribute__((noinline)) __attribute__((leaf)) readUSART(void) {
  uint8_t ch;
  #if (SOFTUART == 0)
    #ifndef LINUART
        while(!(UART_SRA & _BV(RXC0)));
    #else
        while(!(LINSIR & _BV(LRXOK)));
    #endif
    ch = UART_UDR;
    #endif
  return ch;
}
