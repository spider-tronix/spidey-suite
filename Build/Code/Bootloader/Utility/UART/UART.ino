#if !defined(SOFTUART)
	#define SOFTUART 0
#endif
#if !defined(UART)
	#define UART 0
#endif
#if !defined(SINGLESPEED)
	#define SINGLESPEED 0
#endif

#include <avr/io.h>
#include "bootuart.h"

#ifndef BAUD_RATE
	#if F_CPU >= 8000000L
		#define BAUD_RATE   115200L  	// Highest rate Avrdude win32 can support
	#elif F_CPU >= 1000000L
		#define BAUD_RATE   9600L   	// 19200 also supported, but with significant error
	#elif F_CPU >= 128000L
		#define BAUD_RATE   4800L   	// Good for 128kHz internal RC
	#else
		#define BAUD_RATE 1200L     	// Good even at 32768Hz
	#endif
#endif

#if (SOFTUART == 0)
	#if SINGLESPEED
		/* Single speed option */
		#define BAUD_SETTING (( (F_CPU + BAUD_RATE * 8L) / ((BAUD_RATE * 16L))) - 1 )
		#define BAUD_ACTUAL (F_CPU/(16 * ((BAUD_SETTING)+1)))
	#else
		/* Normal U2X usage */
		#define BAUD_SETTING (( (F_CPU + BAUD_RATE * 4L) / ((BAUD_RATE * 8L))) - 1 )
		#define BAUD_ACTUAL (F_CPU/(8 * ((BAUD_SETTING)+1)))
	#endif
	#if BAUD_ACTUAL <= BAUD_RATE
		#define BAUD_ERROR (( 100*(BAUD_RATE - BAUD_ACTUAL) ) / BAUD_RATE)
		#if BAUD_ERROR >= 5
			#error BAUD_RATE off by greater than -5%
		#elif BAUD_ERROR >= 2  && !defined(PRODUCTION)
			#warning BAUD_RATE off by greater than -2%
		#endif
	#else
		#define BAUD_ERROR (( 100*(BAUD_ACTUAL - BAUD_RATE) ) / BAUD_RATE)
		#if BAUD_ERROR >= 5
			#error BAUD_RATE off by greater than 5%
		#elif BAUD_ERROR >= 2  && !defined(PRODUCTION)
			#warning BAUD_RATE off by greater than 2%
		#endif
	#endif
	#if BAUD_SETTING > 250
		#error Unachievable baud rate (too slow) BAUD_RATE 
	#endif // baud rate slow check
	#if (BAUD_SETTING - 1) < 3
		#if BAUD_ERROR != 0 // permit high bitrates (ie 1Mbps@16MHz) if error is zero
			#error Unachievable baud rate (too fast) BAUD_RATE 
		#endif
	#endif // baud rate fast check
#endif // SOFTUART

void UART_Setup(){
  #if (SOFTUART == 0)
  	#if defined(__AVR_ATmega8__) || defined (__AVR_ATmega8515__) ||	\
  		defined (__AVR_ATmega8535__) || defined (__AVR_ATmega16__) ||	\
  		defined (__AVR_ATmega32__)
  		#if (SINGLESPEED == 0)
  			UCSRA = _BV(U2X); //Double speed mode USART
  		#endif //singlespeed
  		UCSRB = _BV(RXEN) | _BV(TXEN);  // enable Rx & Tx
  		UCSRC = _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0);  // config USART; 8N1
  		UBRRL = (uint8_t)BAUD_SETTING;
  	#else // mega8/etc
  		#ifdef LINUART
  			//DDRB|=3;
  			LINCR = (1 << LSWRES); 
  			//LINBRRL = (((F_CPU * 10L / 32L / BAUD_RATE) + 5L) / 10L) - 1; 
  			LINBRRL=(uint8_t)BAUD_SETTING;
  			LINBTR = (1 << LDISR) | (8 << LBT0); 
  			LINCR = _BV(LENA) | _BV(LCMD2) | _BV(LCMD1) | _BV(LCMD0); 
  			LINDAT=0;
  		#else
  			#if (SINGLESPEED == 0)
  				UART_SRA = _BV(U2X0); //Double speed mode USART0
  			#endif
  			UART_SRB = _BV(RXEN0) | _BV(TXEN0);
  			UART_SRC = _BV(UCSZ00) | _BV(UCSZ01);
  			UART_SRL = (uint8_t)BAUD_SETTING;
  		#endif // LINUART
  	#endif // mega8/etc
  #endif // softUART
}

int main(void)
{ 
  UART_Setup();
  while(1){
   writeUSART(readUSART());
  }
  return 0;
}
