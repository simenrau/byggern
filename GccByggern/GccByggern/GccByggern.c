#define F_CPU 4915200 // 4,9152 MHz clock speed
#include <util/delay.h>


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>

#include "UART.h"
#include "SRAM.h"


int main(void)
{
	USART_Init(MYUBRR);
	
	MCUCR |= (1 << SRE);  // Initializes SRAM  
	
	SRAM_test();	
	
	PORTA |= (0 << PA0) | (0 << PA1) | (0 << PA2);
	PORTD |= (1 << PD6);
	while (1)
	{
		PORTD |= (1 << PD6);
		int pa1 = PA1;
		printf(pa1);
			
	}
}


//if ()
		//{
			//printf("X: %, Y: %");
		//}

