/*
 * GccApplication2.c
 *
 * Created: 10/17/2020 12:05:29 AM
 * Author : hp
 
 PROTOCOL :
 
 1) Handshake Signal -    ASCII CHARACTER TO BE SENT BY ESP - EOH - HEXADECIMAL VALUE - 0X01 - IDENTIFIER USED IN THIS CODE - startrans
    Safety Signal -       ASCII CHARACTER TO BE SENT BY ESP - TAB - HEXADECIMAL VALUE - 0x09 - IDENTIFIER USED IN THIS CODE - safevar
    MCU Acknowledge -     ASCII CHARACTER TO BE RECIEVED BY ESP - ACK - HEXADECIMAL VALUE - 0X06 - IDENTIFIER USED IN THIS CODE - acknow 
	
2)  Load Address Signal - ASCII CHARACTER TO BE SENT BY ESP - ENQ - HEXADECIMAL VALUE - 0X05 - IDENTIFIER USED IN THIS CODE - laddress
    Safety Signal -       ASCII CHARACTER TO BE SENT BY ESP - TAB - HEXADECIMAL VALUE - 0x09 - IDENTIFIER USED IN THIS CODE - safevar
    ( Send the addrH and addrH )
	MCU Acknowledge -     ASCII CHARACTER TO BE RECIEVED BY ESP - ACK - HEXADECIMAL VALUE - 0X06 - IDENTIFIER USED IN THIS CODE - acknow 

3) Program Mode Signal -  ASCII CHARACTER TO BE SENT BY ESP - STX - HEXADECIMAL VALUE - 0X02 - IDENTIFIER USED IN THIS CODE - progmode
   Safety Signal -       ASCII CHARACTER TO BE SENT BY ESP - TAB - HEXADECIMAL VALUE - 0x09 - IDENTIFIER USED IN THIS CODE - safevar
   ( Send the pageL and pageH then send ibytes)
   First MCU Acknowledge- ASCII CHARACTER TO BE RECEIEVD BY ESP - ETX - HEXADECIMAL VALUE - 0X03 - IDENTIFIER USED IN THIS CODE - enddata
   (Code flashed )
   MCU Acknowledge -      ASCII CHARACTER TO BE RECEIEVED BY ESP - ACK - HEXADECIMAL VALUE - 0X06 - IDENTIFIER USED IN THIS CODE - acknow 
   
 4) Exit Programming Mode - ASCII CHARACTER TO BE SENT BY ESP - EOT - HEXADECIMAL VALUE - 0X04 - IDENTIFIER USED IN THIS CODE - endtrans
    Safety Signal -       ASCII CHARACTER TO BE SENT BY ESP - TAB - HEXADECIMAL VALUE - 0x09 - IDENTIFIER USED IN THIS CODE - safevar
    MCU Acknowledge -       ASCII CHARACTER TO BE RECEIEVED BY ESP - ACK - HEXADECIMAL VALUE - 0X06 - IDENTIFIER USED IN THIS CODE - acknow 
  */ 

#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif
#include <avr/io.h>
#include <avr/boot.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/wdt.h>
#define startrans 0x01
#define endtrans  0x04
#define laddress  0x05
#define progmode  0x02
#define enddata   0x03
#define acknow    0x06
#define safevar   0x09
#define MAX 256

uint8_t prog[MAX];

void usart_init()
{
	UCSR0B = (1<<RXCIE0)|(1<<TXEN0)| (1<<RXEN0);   /* Enable transmission from USART and enable recieve interrupt */
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);              /* Set size of character to 8-bits*/
	UBRR0L =  0x08;                                /* Set baud rate to 1,15,200*/
}





void acknowledge_uart(char character)
{
	while(!(UCSR0A&(1<<UDRE0)));
	UDR0=character ;
}

uint8_t read_uart()
{
	uint8_t tempvar;
	while(!(UCSR0A & (1<<RXC0)));
	tempvar=UDR0;
	return tempvar;
}


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
	for(;;)
	{
		
		ch = read_uart();
		if( ch == startrans){
			ch=read_uart();
			if(ch==safevar)
			{acknowledge_uart(acknow);}
		}
		else if( ch == laddress){
			ch = read_uart();
			if(ch==safevar)
			{ uint8_t addrL = read_uart();
			  uint8_t addrH = read_uart();
	          addrfinal = (addrH << 8)| addrL ;
			 acknowledge_uart(acknow);
			}
		}
		else if( ch == progmode){
			ch = read_uart();
			if(ch==safevar)
			{
				uint8_t pageL = read_uart();
			    uint8_t pageH = read_uart();
			    uint16_t pageLen = (pageH << 8) | pageL;
			    uint16_t i;
			    for(i=0;i<pageLen;i++){
				prog[i] = read_uart();
			    }
			    acknowledge_uart(enddata); 
			    boot_program_page(addrfinal, prog);
			    acknowledge_uart(acknow);
			}
		}
		else if( ch == endtrans){
			ch=read_uart();
			if(ch==safevar)
			{
			  acknowledge_uart(acknow);
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
	usart_init();
	DDRB = 0xFF;
	for(uint8_t i=0;i<20;i++){
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



