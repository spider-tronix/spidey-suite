/*
 * GccApplication2.c
 *
 * Created: 10/17/2020 12:05:29 AM
 * Author : Aditya And Arunesh 
 
 PROTOCOL :
 
 1) Handshake Signal -    ASCII CHARACTER TO BE SENT BY ESP - EOH - HEXADECIMAL VALUE - 0X01 - IDENTIFIER USED IN THIS CODE - spidey_start_tx
    Safety Signal -       ASCII CHARACTER TO BE SENT BY ESP - BS - HEXADECIMAL VALUE - 0x10 IDENTIFIER USED IN THIS CODE - spidey_node_ack
    MCU Acknowledge -     ASCII CHARACTER TO BE RECIEVED BY ESP - ACK - HEXADECIMAL VALUE - 0X06 - IDENTIFIER USED IN THIS CODE - spidey_acknowledge
	
 2) Get Signature Bytes - ASCII CHARATCTER TO BE SENT BY ESP - BEL - HEXADECIMAL VALUE - 0x07 - IDENTIFIER USED IN THIS CODE - spidey_getsigbyte
    Safety Signal -       ASCII CHARACTER TO BE SENT BY ESP - BS - HEXADECIMAL VALUE - 0x10 IDENTIFIER USED IN THIS CODE - spidey_node_ack
    MCU Acknowledge -     ASCII CHARACTER TO BE RECIEVED BY ESP - ACK - HEXADECIMAL VALUE - 0X06 - IDENTIFIER USED IN THIS CODE - spidey_acknowledge
    
	
2)  Load Address Signal - ASCII CHARACTER TO BE SENT BY ESP - ENQ - HEXADECIMAL VALUE - 0X05 - IDENTIFIER USED IN THIS CODE - spidey_load_address
    Safety Signal -       ASCII CHARACTER TO BE SENT BY ESP - BS - HEXADECIMAL VALUE - 0x10 - IDENTIFIER USED IN THIS CODE - spidey_node_ack
    ( Send the addrH and addrH )
	MCU Acknowledge -     ASCII CHARACTER TO BE RECIEVED BY ESP - ACK - HEXADECIMAL VALUE - 0X06 - IDENTIFIER USED IN THIS CODE - spidey_acknowledge
	
3) Program Mode Signal -  ASCII CHARACTER TO BE SENT BY ESP - STX - HEXADECIMAL VALUE - 0X02 - IDENTIFIER USED IN THIS CODE - spidey_start_progmode
   Safety Signal -       ASCII CHARACTER TO BE SENT BY ESP - BS - HEXADECIMAL VALUE - 0x10 - IDENTIFIER USED IN THIS CODE - spidey_node_ack
   ( Send the pageL and pageH then send ibytes)
   First MCU Acknowledge- ASCII CHARACTER TO BE RECEIEVD BY ESP - ETX - HEXADECIMAL VALUE - 0x03 - IDENTIFIER USED IN THIS CODE - spidey_data_recieved
   (Code flashed )
   At end of page write - ASCII CHARACTER TO BE RECEIEVED BY ESP - ACK - HEXADECIMAL VALUE - 0X06 - IDENTIFIER USED IN THIS CODE - spidey_acknowledge
   
 4)    Read Flash Page -       ASCII CHARACTER TO BE SENT BY ESP - TAB - HEXADECIMAL VALUE - 0X09 - IDENTIFIER USED IN THIS CODE - spidey_check_flash
       Safety Signal -         ASCII CHARACTER TO BE SENT BY ESP - BS - HEXADECIMAL VALUE - 0x10 - IDENTIFIER USED IN THIS CODE - spidey_node_ack
	   (Send pageL and page H )
	   (Bytes are sent repeatedly)
       MCU Acknowledge -       ASCII CHARACTER TO BE RECEIEVED BY ESP - ACK - HEXADECIMAL VALUE - 0X06 - IDENTIFIER USED IN THIS CODE - spidey_acknowledge
   
 5) Exit Programming Mode - ASCII CHARACTER TO BE SENT BY ESP - EOT - HEXADECIMAL VALUE - 0X04 - IDENTIFIER USED IN THIS CODE - spidey_end_tx
    Safety Signal -         ASCII CHARACTER TO BE SENT BY ESP - BS - HEXADECIMAL VALUE - 0x10 - IDENTIFIER USED IN THIS CODE - spidey_node_ack 
    MCU Acknowledge -       ASCII CHARACTER TO BE RECEIEVED BY ESP - ACK - HEXADECIMAL VALUE - 0X06 - IDENTIFIER USED IN THIS CODE - spidey_acknowledge 
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



#define MAX 256

uint8_t prog[MAX];



void boot_program_page(uint16_t page, uint8_t *buf){
	uint16_t i;
	uint8_t sreg;
	sreg = SREG;
	cli();
	eeprom_busy_wait();
	boot_page_erase(page);
	boot_spm_busy_wait();
	for (i = 0; i < SPM_PAGESIZE; i += 2){
		uint16_t w = *buf++;
		w += (*buf++) << 8;
		boot_page_fill(page + i, w);
	}
	boot_page_write(page);
	boot_spm_busy_wait();
	boot_rww_enable ();
	SREG = sreg;
}

void readcode()
{
	char ch;
	wdt_enable(WDTO_2S);
	uint16_t addrfinal=0;
	uint8_t data;
	for(;;)
	{
		
		ch = readUSART();
		if( ch == SPIDEY_START_TX){
			ch=readUSART();
			if(ch==SPIDEY_NODE_ACK)
			{writeUSART(SPIDEY_ACKNOWLEDGE);}
		}
		else if(ch == SPIDEY_GETSIGBYTES)           /* Code to read Signature Bytes */
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
		else if( ch == SPIDEY_LOAD_ADDRESS){
			ch = readUSART();
			if(ch==SPIDEY_NODE_ACK)
			{ uint8_t addrL = readUSART();
			  uint8_t addrH = readUSART();
	          addrfinal = (addrH << 8)| addrL ;
			 writeUSART(SPIDEY_ACKNOWLEDGE);
			}
		}
		else if( ch == SPIDEY_START_PROGMODE){
			ch = readUSART();
			if(ch==SPIDEY_NODE_ACK)
			{
				uint8_t pageL = readUSART();
			    uint8_t pageH = readUSART();
			    uint16_t pageLen = (pageH << 8) | pageL;
			    uint16_t i;
			    for(i=0;i<pageLen;i++){
				prog[i] = readUSART();
			    }
			    writeUSART(SPIDEY_DATA_RECIEVED); 
			    boot_program_page(addrfinal, prog);
			    writeUSART(SPIDEY_ACKNOWLEDGE);
			}
		}
		else if (ch==SPIDEY_CHECK_FLASH)
		{
			if(ch==SPIDEY_NODE_ACK)
			{   
				uint8_t pageL = readUSART();
				uint8_t pageH = readUSART();
				uint16_t pageLen = (pageH << 8) | pageL;
				uint16_t i;
				for(i=0;i<pageLen;i++)               /* Code to check if page was written properly*/
				{
					data = pgm_read_byte(addrfinal+i);
					writeUSART(data);
				}
				writeUSART(SPIDEY_ACKNOWLEDGE);
			}
		}
		else if( ch == SPIDEY_END_TX){
			ch=readUSART();
			if(ch==SPIDEY_NODE_ACK)
			{
			  writeUSART(SPIDEY_ACKNOWLEDGE);
			  wdt_enable(WDTO_15MS);
			  while(1); 
			}
		}
	}
}

void appStart(){
	asm("jmp 0x00000");
}

void bootLoader(){
    UART_Setup();
	DDRB = 0xFF;
	for(uint8_t i=0;i<5;i++){
		PORTB ^= 0xFF;
		_delay_ms(50);
	}
	PORTB = 0x00;
	DDRB = 0x00;
    readcode();
}

int main(void)
{
   	uint8_t ch = MCUSR;
   	MCUSR = 0;
   	wdt_disable();
   	if ((ch & _BV(EXTRF))){
	   	bootLoader();
   	}
   	else{
	   	appStart();
   	}
   	return 0;
}