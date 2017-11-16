#include "define2.h"
#include "solenoid.h"

void solenoid_init()
{
	DDRB |= (1 << PB7);    //13
	PORTB |= (1 << PB7);
}

void solenoid_push()
{
	PORTB &= ~(1 << PB7);
	/*_delay_ms(50);
	PORTB |= (1 << PB7);*/
}
void solenoid_ret()
{
	PORTB |= (1 << PB7);
}

