#ifndef _BOOT_UART_H
    #define _BOOT_UART_H
#include <avr/io.h>

#if (SOFT_UART == 0)
  #if (UART == 0)
    #if defined(UDR0)
      #define UART_SRA UCSR0A
      #define UART_SRB UCSR0B
      #define UART_SRC UCSR0C
      #define UART_SRL UBRR0L
      #define UART_UDR UDR0
    #elif defined(UDR)
      #define UART_SRA UCSRA
      #define UART_SRB UCSRB
      #define UART_SRC UCSRC
      #define UART_SRL UBRRL
      #define UART_UDR UDR
    #elif defined(LINDAT)
      #define LIN_UART 1
      #define UART_SRA UCSRA
      #define UART_SRB UCSRB
      #define UART_SRC UCSRC
      #define UART_SRL UBRRL
      #define UART_UDR LINDAT
    #else
      #error UART == 0, but no UART0 on device
    #endif
  #elif (UART == 1)
    #if !defined(UDR1)
      #error UART == 1, but no UART1 on device
    #endif
    #define UART_SRA UCSR1A
    #define UART_SRB UCSR1B
    #define UART_SRC UCSR1C
    #define UART_SRL UBRR1L
    #define UART_UDR UDR1
  #elif (UART == 2)
    #if !defined(UDR2)
      #error UART == 2, but no UART2 on device
    #endif
    #define UART_SRA UCSR2A
    #define UART_SRB UCSR2B
    #define UART_SRC UCSR2C
    #define UART_SRL UBRR2L
    #define UART_UDR UDR2
  #elif (UART == 3)
    #if !defined(UDR3)
      #error UART == 3, but no UART3 on device
    #endif
    #define UART_SRA UCSR3A
    #define UART_SRB UCSR3B
    #define UART_SRC UCSR3C
    #define UART_SRL UBRR3L
    #define UART_UDR UDR3
  #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
	void  __attribute__((noinline)) __attribute__((leaf)) writeUSART(uint8_t);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
	uint8_t __attribute__((noinline)) __attribute__((leaf)) readUSART(void);
#ifdef __cplusplus
}
#endif

#endif
