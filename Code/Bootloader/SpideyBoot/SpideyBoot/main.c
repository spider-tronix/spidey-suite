/*
 * GccApplication2.c
 *
 * Created: 10/17/2020 12:05:29 AM
 * Author : hp
 
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
   For (i=0; i< no of bytes in page ; i++)
   { 
	   MCU sends byte from flash page 
	   if byte is correct :
	   HEXADECIMAL VALUE TO BE SENT BY ESP - 0x09
	   if byte is incorrect:
	   ESP can send any value 
	   HEXADECIMAL VALUE TO BE RECIEVED BY ESP - 0x08
	   Exit Loop
   }
   At end if page write was successful with no error  - ASCII CHARACTER TO BE RECEIEVED BY ESP - ACK - HEXADECIMAL VALUE - 0X06 - IDENTIFIER USED IN THIS CODE - spidey_acknowledge
   
 4) Exit Programming Mode - ASCII CHARACTER TO BE SENT BY ESP - EOT - HEXADECIMAL VALUE - 0X04 - IDENTIFIER USED IN THIS CODE - spidey_end_tx
    Safety Signal -         ASCII CHARACTER TO BE SENT BY ESP - BS - HEXADECIMAL VALUE - 0x10 - IDENTIFIER USED IN THIS CODE - spidey_node_ack 
    MCU Acknowledge -       ASCII CHARACTER TO BE RECEIEVED BY ESP - ACK - HEXADECIMAL VALUE - 0X06 - IDENTIFIER USED IN THIS CODE - spidey_acknowledge 
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
#include <spideyprotocol.h>

#define MAX 256

uint8_t prog[MAX];

void usart_init()
{
	UCSR0B = (1<<RXCIE0)|(1<<TXEN0)| (1<<RXEN0);   /* Enable transmission from USART and enable recieve interrupt */
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);              /* Set size of character to 8-bits*/
	UBRR0L =  0x08;                                /* Set baud rate to 1,15,200*/
}





void write_uart(char character)
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
	uint8_t data;
	for(;;)
	{
		
		ch = read_uart();
		if( ch == spidey_start_tx){
			ch=read_uart();
			if(ch==spidey_node_ack)
			{write_uart(spidey_acknowledge);}
		}
		else if(ch == spidey_getsigbyte)           /* Code to read Signature Bytes */
		{   
			ch=read_uart();
			if(ch==spidey_node_ack)
			{int i;
			for(i=0;i<6;i+=2)                      /* Start for loop for 3 iterations to read the 3 signature bytes*/
			{
				write_uart(boot_signature_byte_get (i));    /*Read and send signature byte at locations 0 , 2 , 4 */
			}
			}
		}  
		else if( ch == spidey_load_address){
			ch = read_uart();
			if(ch==spidey_node_ack)
			{ uint8_t addrL = read_uart();
			  uint8_t addrH = read_uart();
	          addrfinal = (addrH << 8)| addrL ;
			 write_uart(spidey_acknowledge);
			}
		}
		else if( ch == spidey_start_progmode){
			ch = read_uart();
			if(ch==spidey_node_ack)
			{
				uint8_t pageL = read_uart();
			    uint8_t pageH = read_uart();
			    uint16_t pageLen = (pageH << 8) | pageL;
			    uint16_t i;
				uint8_t flag=0;
			    for(i=0;i<pageLen;i++){
				prog[i] = read_uart();
			    }
			    write_uart(spidey_data_recieved); 
			    boot_program_page(addrfinal, prog);
				for(i=0;i<pageLen;i++)               /* Code to check if page was written properly*/
				{
					data = pgm_read_byte(addrfinal+i);
					write_uart(data);
					flag=read_uart();       /*Send byte to ESP and wait for it confirm if its correct or not*/
					if(flag!=spidey_data_correct)             /* If ESP says byte is incorrect enter condition*/
					{   
						write_uart(spidey_error_in_flash);     /*Tell ESP there was ERROR in Flash write */
						break;                                       /* Exit the loop to check further pages*/
					}
				}
				if(flag==spidey_data_correct)
			    {write_uart(spidey_acknowledge);};
			}
		}
		else if( ch == spidey_end_tx){
			ch=read_uart();
			if(ch==spidey_node_ack)
			{
			  write_uart(spidey_acknowledge);
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