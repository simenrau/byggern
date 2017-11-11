#include "define2.h"
#include "solenoid.h"

void solenoid_init()
{
	DDRF |= (1 << 1);
	PORTF &= ~(1 << 1);
	
}

void solenoid_shoot()
{
	PORTF |= (1 << 1);
	_delay_ms(solenoid_delay);
	PORTF &= ~(1 << 1);
}
