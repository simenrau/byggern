/*
 * GccByggern.c
 *
 * Created: 04.09.2017 11:16:20
 *  Author: simenrau
 */ 

#define F_CPU 5000000UL // 5 MHz clock speed
#include <util/delay.h>


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include "UART.h"


int main(void)
{
	USART_Init(MYUBRR);
	
	DDRA |= (1 << PA0) | (1 << PA2);
	USART_Transmit(DDRA);
	//int f = 1000;
	int i = 0;
    while(i < 10)
    {
        //PORTA &= ~((1<<PA0)|(1<<PA2));
		//_delay_ms(f);
		//PORTA |= (1<<PA0)|(1<<PA2);
		//_delay_ms(f);
		
		//unsigned char c = USART_Receive();
		//USART_Transmit(c);
		
		printf("\n 9");
		i = i+1;
    }
	
}

