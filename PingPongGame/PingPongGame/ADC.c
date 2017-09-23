#include "define.h"

uint8_t read_adc(int channel)
{
	volatile uint8_t *ADC = (uint8_t*)0x1400;
	uint8_t mux_adress = channel;
	ADC[0] = mux_adress;
	_delay_us(500);
	uint8_t val = ADC[0];
	return val;
}