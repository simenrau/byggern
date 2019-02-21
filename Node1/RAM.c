#include "define.h"

void ram_init(void)
{
	bit_set(MCUCR, SRE);
	
	bit_set(SFIOR, XMM2);
	bit_clear(SFIOR, XMM1);
	bit_clear(SFIOR, XMM0);

	bit_set(MCUCR, SRW11);
	bit_clear(EMCUCR, SRW10);	
}