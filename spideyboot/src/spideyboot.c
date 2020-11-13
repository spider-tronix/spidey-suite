/*
 * SpideyBoot.c
 * Created: 10/17/2020 12:05:29 AM
 * Version: 0.5.0
 * Author : Aditya and Arunesh 
*/ 

#ifndef F_CPU
#define F_CPU 16000000L              // 16 MHz clock speed for ATMEGA328p
#endif
#if !defined(SOFTUART)
#define SOFTUART 0
#endif
#if !defined(UART)
#define UART 0
#endif
#if !defined(SINGLESPEED)
#define SINGLESPEED 0
#endif

/* Including built-in library files */
#include <inttypes.h>
#include <avr/io.h>
#include <avr/boot.h>
#include <avr/pgmspace.h>
/* Including custom library files */
#include "spideyprotocol.h"
#include "bootuart.h"
/* We are not using AVR's WDG lib file to avoid interrupt overhead. */
#include "watchdog.h"                   

/* Definition for flashing on board LED */
#if !defined(LED_START_FLASHES)
#define LED_START_FLASHES 3
#endif
#if !defined(LED_START_ON)
#  define LED_START_ON 0
#endif
/* Definition for UART communication */
#ifndef BAUD_RATE
#if F_CPU >= 8000000L
#define BAUD_RATE   115200L  	// Highest rate spideydude win32 can support
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
#if BAUD_ERROR != 0 // permit high bit rates (i.e 1Mbps@16MHz) if error is zero
#error Unachievable baud rate (too fast) BAUD_RATE
#endif
#endif // baud rate fast check
#endif // SOFTUART

#if LED_START_FLASHES > 0
static inline void Flash_LED(uint8_t);
#endif

/*FUNCTION TO INITIALISE UART COMMUNICATION*/
void UART_Setup() {
	#if (SOFTUART == 0)
	#if defined(__AVR_ATmega8__) || defined (__AVR_ATmega8515__) ||	\
	defined (__AVR_ATmega8535__) || defined (__AVR_ATmega16__) ||	\
	defined (__AVR_ATmega32__)
	#if (SINGLESPEED == 0)
	UCSRA = _BV(U2X); //Double speed mode USART
	#endif //single speed
	UCSRB = _BV(RXEN) | _BV(TXEN);  // enable RX & TX
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

#define MAX 256     // Maximum number of bytes that can be written to page at a time */
uint8_t prog[MAX];  //Buffer Array to temporarily hold values received from node through UART*/

/* Function to write values from buffer array to a page in flash
   Parameters to be passed to the function are page address and buffer array */
void boot_program_page(uint16_t page, uint8_t *buf) {
	uint16_t i;
	uint8_t sreg;
	sreg = SREG;                            /* Copy present state of SREG Register */
	__asm__ __volatile__ ("cli" ::);        /* Disable Interrupt to avoid any accidental interference during writing */
	eeprom_busy_wait();                     /* Wait for any ongoing eeprom operation to complete*/
	boot_page_erase(page);                  /* Erase the page which is going to be re-programmed */     
	boot_spm_busy_wait();                   /* Wait for any ongoing SPM operations to terminate */
	for (i = 0; i < SPM_PAGESIZE; i += 2){  /* i is incremented by 2 to skip 2 bytes(16bits or 1 word) because each time 1 word is written to page*/
		uint16_t w = *buf++;                             
		w += (*buf++) << 8;                 /* 2 bytes(1 word) are stored in the 16 bit w variable*/
		boot_page_fill(page + i, w);        /* This word is flashed to buffer page */    
	}
	boot_page_write(page);                  /* Write the buffer page into the actual page in flash */
	boot_spm_busy_wait();                   /* Wait for SPM operation to complete*/
	boot_rww_enable ();                     /* Enable Read-while-write(RWW) operations*/
	SREG = sreg;                            /* Restore the state of SREG register as before */
	return;
}

/*Function to receive the code via UART and */
void readcode() {
	char ch;
	WATCHDOG_CONFIG(WATCHDOG_2S);                        /* Enable watchdog timer for 2 Seconds within which the code is expected to be transferred after which reset occurs */
	uint16_t addrfinal = 0;
	/* Infinite for loop. The loop will only terminate when watchdog overflow occurs */ 
	for(;;) {
		ch = readUSART();
		/* If SPIDEY_START_TX (check its hex value in protocol documentation) is sent then handshake signals are transmitted */
		if(ch == SPIDEY_START_TX){                 
			if(readUSART() == SPIDEY_NODE_ACK) {
				writeUSART(SPIDEY_ACKNOWLEDGE);                 /*  Acknowledge back */
			}
		}
        /* If SPIDEY_GETSIGBYTES is transmitted, Code to read Signature Bytes is executed */
		else if(ch == SPIDEY_GETSIGBYTES) {   
			if(readUSART() == SPIDEY_NODE_ACK) {
				/* Start for loop for 3 iterations to read the 3 signature bytes*/
				for(int i=0;i<6;i+=2) {
					writeUSART(boot_signature_byte_get(i));    /* Read and send signature byte at locations 0 , 2 , 4 */
				}
				writeUSART(SPIDEY_ACKNOWLEDGE);                /*  Acknowledge back */
			}
		} 
		/* If SPIDEY_LOAD_ADDRESS is transmitted then code to receive 16 bit address is executed */ 
		else if(ch == SPIDEY_LOAD_ADDRESS) {
			if(readUSART() == SPIDEY_NODE_ACK) { 
				uint8_t addrL = readUSART();		/* Receive HIGH byte of address */
				uint8_t addrH = readUSART();		/* Receive LOW byte of address */
				addrfinal = (addrH << 8) | addrL ;   /* Put together high and low bytes */
				writeUSART(SPIDEY_ACKNOWLEDGE);
			}
		}
		/*If SPIDEY_START_PROGMODE is transmitted then execute code to receive page length followed by code to be written to flash*/
		else if(ch == SPIDEY_START_PROGMODE) {
			if(readUSART() == SPIDEY_NODE_ACK) {
				uint8_t pageL = readUSART();				/* Receive HIGH byte of page length */
			    uint8_t pageH = readUSART();				/* Receive LOW byte of page Length */
			    uint16_t pageLen = (pageH << 8) | pageL;    /* Put together high and low bytes */
			    /* Loop as many times as page length to receive as many bytes of code */
			    for(uint16_t i=0;i<pageLen;i++) {              
					prog[i] = readUSART();               /* Store Received code in buffer array */
			    }
			    writeUSART(SPIDEY_DATA_RECIEVED);      /* Inform Node that all data as been received correctly */
			    boot_program_page(addrfinal, prog);    /* Write contents of buffer array into page in flash whose address we earlier received */
			    writeUSART(SPIDEY_ACKNOWLEDGE);
			}
		}
		/* If SPIDEY_CHECK_FLASH is sent code to receive page length followed by reading a page from flash and sending it to Node is executed */
		else if(ch == SPIDEY_CHECK_FLASH) {   
			if(readUSART() == SPIDEY_NODE_ACK) {   
				uint8_t pageL = readUSART();				/* Receive HIGH byte of page length */
				uint8_t pageH = readUSART();				/* Receive LOW byte of page Length */
				uint16_t pageLen = (pageH << 8) | pageL;    /* Put together high and low bytes */
				/* Loop as many times as page length and send bytes continuously */
				for(uint16_t i=0;i<pageLen;i++) 
				{
					/* Read bytes from the page in flash whose address was passed earlier when SPIDEY_LOAD_ADDRESS was sent
					   and then send it to UART
				    */
					writeUSART(pgm_read_byte(addrfinal+i));
				}
				writeUSART(SPIDEY_ACKNOWLEDGE);
			}
		}
		/* If SPIDEY_END_TX is sent terminate the transmission */
		else if(ch == SPIDEY_END_TX) {
			writeUSART(SPIDEY_ACKNOWLEDGE);
			if(readUSART() == SPIDEY_NODE_ACK) {
				WATCHDOG_CONFIG(WATCHDOG_16MS);      /* Set watchdog timer for 16ms so that the MCU resets within without further delay */
				while(1);                            /* Wait here for the 16ms period */
			}
		}
	}
	return;
}

/* Function to start execution of application code from 0x00000 */
void appStart() {  
	asm("jmp 0x00000");
	return;
}

/* Function to modify application code by calling the bootloader code */
void bootLoader() {
    UART_Setup();                 /* Initialize UART */
	#if LED_START_FLASHES > 0
	// Set up Timer 1 for timeout counter
	#if defined(__AVR_ATtiny261__)||defined(__AVR_ATtiny461__)||defined(__AVR_ATtiny861__)
	TCCR1B = 0x0E; //div 8196 - we could divide by less since it's a 10-bit counter, but why?
	#elif defined(__AVR_ATtiny25__)||defined(__AVR_ATtiny45__)||defined(__AVR_ATtiny85__)
	TCCR1 = 0x0E; //div 8196 - it's an 8-bit timer.
	#elif defined(__AVR_ATtiny43__)
	#error "LED flash for Tiny43 not yet supported"
	#else
	TCCR1B = _BV(CS12) | _BV(CS10); // div 1024
	#endif
	#endif
	#if (LED_START_FLASHES > 0)
	/* Set LED pin as output */
	DDRB |= 0xFF;
	#endif
	#if LED_START_FLASHES > 0
	/* Flash on board LED to signal entering of bootloader */
	Flash_LED(LED_START_FLASHES * 2);
	#else
	#if LED_START_ON
	/* Turn on LED to indicate starting bootloader (less code!) */
	PORTB |= 0xFF;
	#endif
	#endif
    readcode();                   /* Call function that executes the modification of flash page */
}

int main(void) {
   	uint8_t ch = MCUSR;       /* Read MCUSR register to determine Source of Reset */
   	MCUSR = 0;
   	WATCHDOG_CONFIG(0);       /* Disable watchdog timer */
   	if ((ch & _BV(EXTRF))){   /* If it was External Reset, execute bootloader */
	   	bootLoader();
   	}
   	else{                     /* If it was any other type of Reset, execute application code */
	   	appStart();
   	}
   	return 0;
}

#if LED_START_FLASHES > 0
void Flash_LED(uint8_t count) {
  do {
  	#if defined(__AVR_ATtiny261__)||defined(__AVR_ATtiny461__)||defined(__AVR_ATtiny861__) || defined(__AVR_ATtiny25__)||defined(__AVR_ATtiny45__)||defined(__AVR_ATtiny85__)
  		TCNT1 = -(F_CPU/(8196*16));
    	TIFR = _BV(TOV1);
    	while(!(TIFR & _BV(TOV1)));
	#elif defined(__AVR_ATtiny43__)
  		#error "LED flash for Tiny43 not yet supported"
	#else
  		TCNT1 = -(F_CPU/(1024*16));
    	TIFR1 = _BV(TOV1);
    	while(!(TIFR1 & _BV(TOV1)));
	#endif   
		PORTB ^= 0xFF;
	} while (--count);
}
#endif