/*
 * GccByggern.c
 *
 * Created: 04.09.2017 09:14:04
 *  Author: simenrau
 */ 


#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>


int main(void)
{
		
	DDRA = 0x00;
	DDRB = 0xFF;
	
	while(1)
	{
		PORTB = 100;
	}	
	return 0;
} 

