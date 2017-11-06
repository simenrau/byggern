#include "PWM.h"
#include "define.h"

#define  TC_TOP 1249

void PWM_init(void){
	ICR1 = TC_TOP;
	
	DDRB |= (1 << PB6);
	PWM_duty(50);
	
	TCCR1A |= (1 << WGM11) | (1 << COM1B1)	| (0 << COM1B0); //Kanskje A eller B istedet
	TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS12);
												
}

void PWM_duty(uint8_t duty_cycle){
	OCR1B = (uint16_t)((uint32_t)duty_cycle* TC_TOP / 1000);		//dividing by 10 for duty_cycle 75
	
}


void Joy_to_pw(char joypos){
	if (joypos> 255 || joypos < 0){
		return;
	}
	char pos = (char)round(45+(1.0*joypos/255)*60);
	printf("%d\n",pos);
	PWM_duty(pos);

}