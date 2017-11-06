#include "define2.h"
#include "ADC_driver.h"

void ADC_init(void)
{
	DDRF &= ~(1 << PF2);
	
	ADCSRA |= (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
	ADMUX |= (1 << REFS0) | (1 << REFS1);
}

uint16_t ADC_read()
{
	uint16_t data = 0;
	
	ADMUX |= (1 << MUX1);
	
	ADCSRA |= (1 << ADSC);
	
	loop_until_bit_is_set(ADCSRA,ADIF);
	
	uint8_t data_low = ADCL;
	uint16_t data_high = ADCH;
	data = data_low + data_high*0b100000000;
		
	return data;
}
