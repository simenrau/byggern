#include "define2.h"
#include "ADC_driver.h"

void ADC_init(void)
{
	ADMUX = (1 << REFS0);
	ADCSRB = (1 << MUX5);
	
	ADCSRA = (1 << ADEN);
	
	/*DDRF &= ~(1 << PF2);
	
	ADCSRA |= (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
	ADMUX |= (1 << REFS0) | (1 << REFS1);*/
}

uint16_t ADC_read()
{
	uint16_t data = 0;
	
	ADMUX |= (1 << MUX1);
	
	ADCSRA |= (1 << ADSC);
	
	
	while(!ADCSRA & (1 << ADIF)){};

	return ADC;
}
