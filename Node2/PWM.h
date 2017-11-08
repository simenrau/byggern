#ifndef PWM_H
#define PWM_H
#include "define.h"

void PWM_init(void);
void PWM_duty(uint8_t duty_cycle);
void Joy_to_pw(char joypos);


#endif