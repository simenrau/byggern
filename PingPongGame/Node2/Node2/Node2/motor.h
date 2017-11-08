#include "define2.h"


void motor_init();
void motor_enable();
void motor_disable();
void motor_set(uint8_t speed, char direction);
void motor_reset_encoder();
int16_t motor_read_encoder();
