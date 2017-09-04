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

int main(void)
{
	DDRA |= (1 << PA0) | (1 << PA2);
	int i = 0;
	
    while(i < 5)
    {
        PORTA &= ~((1<<PA0)|(1<<PA2));
		_delay_ms(1000);
		PORTA |= (1<<PA0)|(1<<PA2);
		_delay_ms(1000);
		i = i+1;
    }
}