#include "define.h"
#include <util/delay.h>

void buzzer(int f, int time)
{
	/*int periodTime = (1/f)*1000000;
	int halfPeriod = periodTime/2;*/
	
	PORTE |= (1 << PE2);
	_delay_ms(time);
	PORTE &= ~(1 << PE2);
	_delay_ms(time);	
}

