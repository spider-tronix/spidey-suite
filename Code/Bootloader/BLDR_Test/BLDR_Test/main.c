/*
 * BLDR_Test.c
 *
 * Created: 29-09-2020 16:30:13
 * Author : Aditya Kumar Singh
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
	DDRB = 0xFF;
	PORTB = 0x00;
	for(int i=0;i<20;i++){
		PORTB ^= 0xFF;
		_delay_ms(100);
	}
	PORTB = 0x00;
	DDRB = 0x00;
	return 0;
}