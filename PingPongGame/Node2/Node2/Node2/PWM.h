#ifndef PWM_H
#define PWM_H
#include "define2.h"

void PWM_init(void);
void PWM_duty(uint8_t duty_cycle);
void slider_to_pw(char joypos);


#endif