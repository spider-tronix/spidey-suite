/*
 * SpideyBoot.c
 * Created: 10/17/2020 12:05:29 AM
 * Author : Aditya And Arunesh 

*/ 

#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
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
#include <avr/io.h>
#include <avr/boot.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/wdt.h>
#include <spideyprotocol.h>
#include <bootuart.h>
#include <watchdog.h>

/* NECCESSARY DEFINITION FOR UART COMMUNICATION*/

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

/*FUNCTION TO INITIALISE UART COMMUNICATION*/

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



#define MAX 256  // Maximum number of bytes that can be written to page at a time */

uint8_t prog[MAX];  //Buffer Array to temporaraily hold values recieved from node through UART*/


/* Function to write values from buffer array to a page in flash
   Parameters to be passed to the function are page address and buffer array */

void boot_program_page(uint16_t page, uint8_t *buf){
	uint16_t i;
	uint8_t sreg;
	sreg = SREG;             /*Copy present state of SREG Register */
	cli();                   /* Disable Interrupt to avoid any accidental interference during writing */
	eeprom_busy_wait();      /* Wait for any ongoing eeprom operation to complete*/
	boot_page_erase(page);   /* Erase the page to be written to */     
	boot_spm_busy_wait();    /* Wait for any ongoing SPM operations to terminate */
	for (i = 0; i < SPM_PAGESIZE; i += 2){ /* i is incremented by 2 to skip 2 bytes(16bits or 1 word) because each time 1 word is written to page*/
		uint16_t w = *buf++;                             
		w += (*buf++) << 8;                /* 2 bytes(1 word) are stored in the 16 bit w variable*/
		boot_page_fill(page + i, w);       /* This word is flashed to buffer page */    
	}
	boot_page_write(page);                 /* Write the buffer page into the actual page in flash */
	boot_spm_busy_wait();                  /* Wait for SPM operation to complete*/
	boot_rww_enable ();                    /* Enable Read-while-write(RWW) operations*/
	SREG = sreg;                           /* Restore the state of SREG register as before */
}

/*Function to recieve the code via UART from Node and write it into a page in flash and read a page from flash and return it to Node */

void readcode()
{
	char ch;
	wdt_enable(WDTO_2S);   /* Enable watchdog timer for 2 Seconds within which the code is expected to be transferred after which reset occurs */
	uint16_t addrfinal=0;
	uint8_t data;
	for(;;)
	{
		/* If SPIDEY_START_TX (check its hex value in protocol documentation) is sent then handshake signals are transmitted between Node and MCU */
		ch = readUSART();
		if( ch == SPIDEY_START_TX){                 
			ch=readUSART();
			if(ch==SPIDEY_NODE_ACK)
			{writeUSART(SPIDEY_ACKNOWLEDGE);}
		}
        /* If SPIDEY_GETSIGBYTES is transmitted Code to read Signature Bytes is executed */
		else if(ch == SPIDEY_GETSIGBYTES)           
		{   
			ch=readUSART();
			if(ch==SPIDEY_NODE_ACK)
			{int i;
			for(i=0;i<6;i+=2)                      /* Start for loop for 3 iterations to read the 3 signature bytes*/
			{
				writeUSART(boot_signature_byte_get (i));    /*Read and send signature byte at locations 0 , 2 , 4 */
			}
			writeUSART(SPIDEY_ACKNOWLEDGE);
			}
		} 
		/*If SPIDEY_LOAD_ADDRESS is transmitted then code to recieve 16 bit address is executed*/ 
		else if( ch == SPIDEY_LOAD_ADDRESS){
			ch = readUSART();
			if(ch==SPIDEY_NODE_ACK)
			{ uint8_t addrL = readUSART();   /* Recieve high byte of address*/
			  uint8_t addrH = readUSART();   /* Recieve Low byte of address*/
	          addrfinal = (addrH << 8)| addrL ;  /*Put together high and low bytes */
			 writeUSART(SPIDEY_ACKNOWLEDGE);
			}
		}

		/*If SPIDEY_START_PROGMODE is transmitted then execute code to recieve page length followed by code to be written to flash*/
		else if( ch == SPIDEY_START_PROGMODE){
			ch = readUSART();
			if(ch==SPIDEY_NODE_ACK)
			{
				uint8_t pageL = readUSART();     /* Recieve High byte of page length*/
			    uint8_t pageH = readUSART();     /* Recieve Low byte of page Length*/
			    uint16_t pageLen = (pageH << 8) | pageL;  /* Put together high and low bytes*/
			    uint16_t i;
			    for(i=0;i<pageLen;i++){              /* Loop as many times as pagelength to recieve as many bytes of code*/
				prog[i] = readUSART();               /* Store Recieved code in buffer array*/
			    }
			    writeUSART(SPIDEY_DATA_RECIEVED);      /* Inform Node that all data as been recieved correctly*/
			    boot_program_page(addrfinal, prog);    /* Write contents of buffer array into page in flash whose address we earlier recieved*/
			    writeUSART(SPIDEY_ACKNOWLEDGE);
			}
		}
		/* If SPIDEY_CHECK_FLASH is sent code to recieve page length followed by reading a page from flash and sending it to Node is executed*/
		else if (ch==SPIDEY_CHECK_FLASH)
		{   
			ch = readUSART();
			if(ch==SPIDEY_NODE_ACK)
			{   
				uint8_t pageL = readUSART();           /* Recieve High byte of pagelength*/
				uint8_t pageH = readUSART();           /* Recieve Low byte of Page length*/
				uint16_t pageLen = (pageH << 8) | pageL; /*Put together high and Low Bytes*/
				uint16_t i;
				for(i=0;i<pageLen;i++)               /* Loop as many times as pagelength and send bytes continually to Node*/
				{
					data = pgm_read_byte(addrfinal+i);//Read bytes from the page in flash whose address was passed earlier when SPIDEY_LOAD_ADDRESS was sent
					writeUSART(data);                 /*Send the byte to Node*/
				}
				writeUSART(SPIDEY_ACKNOWLEDGE);
			}
		}
		/* If SPIDEY_END_TX is sent terminate the transmission*/
		else if( ch == SPIDEY_END_TX){
			ch=readUSART();
			if(ch==SPIDEY_NODE_ACK)
			{
			  writeUSART(SPIDEY_ACKNOWLEDGE);  /* Acknowledge to the Node */
			  wdt_enable(WDTO_15MS);           /* Set watchdog timer for 15 MS so that the MCU resets within 15MS without further delay*/
			  while(1);                        /* Wait here for the 15MS period */
			}
		}
	}
}

/*Function to start execution of application code from 0x00000*/
void appStart(){  
	asm("jmp 0x00000");
}
/* Function to modify application code by calling the bootloader code */
void bootLoader(){
    UART_Setup();     /* Initialise UART for communication with Node*/
	DDRB = 0xFF;      
	for(uint8_t i=0;i<5;i++){     /*Blink LED 5 times to signal that bootloader code is being accessed */
		PORTB ^= 0xFF;
		_delay_ms(123);          /* Delay is (F_CPU/(8196*16)) */
	}
	PORTB = 0x00;                
	DDRB = 0x00;
    readcode(); /* Call function that executes the modification of flash page */
}

int main(void)
{
   	uint8_t ch = MCUSR;    /* Read MCUSR register to determine Source of Reset */
   	MCUSR = 0;
   	wdt_disable();
   	if ((ch & _BV(EXTRF))){   /* If it was External Reset go to function that calls code in bootloader to modify application code */
	   	bootLoader();
   	}
   	else{                     /* If it was any other type of Reset go to function that goes to start of application code */
	   	appStart();
   	}
   	return 0;
}