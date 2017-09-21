#include "define.h"
#define bit_set(reg, bit) (reg |= (1 << bit))
#define bit_clear(reg, bit) (reg &= ~(1 << bit))
#define bit_test(reg, bit) (reg & (1 << bit))

void ram_init(void)
{
	bit_set(MCUCR, SRE);
	
	bit_set(SFIOR, XMM2);
	bit_clear(SFIOR, XMM1);
	bit_clear(SFIOR, XMM0);

	bit_set(MCUCR, SRW11);
	bit_clear(EMCUCR, SRW10);	
}