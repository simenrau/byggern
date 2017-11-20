#include "define2.h"
#include "solenoid.h"

void solenoid_init()
{
	DDRB |= (1 << PB7);
	PORTB |= (1 << PB7);
}

void solenoid_push()
{
	PORTB &= ~(1 << PB7);
}

void solenoid_ret()
{
	PORTB |= (1 << PB7);
}

