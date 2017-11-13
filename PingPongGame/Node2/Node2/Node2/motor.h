#ifndef MOTOR_H
#define MOTOR_H

#include "define2.h"


#define MOTOR_PORT    PORTH
#define MOTOR_DDR     DDRH
#define MOTOR_PIN_RST PH6
#define MOTOR_PIN_OE  PH5
#define MOTOR_PIN_EN  PH4
#define MOTOR_PIN_SEL PH3
#define MOTOR_PIN_DIR PH1
#define ENCODER_DDR   DDRK
#define ENCODER_INPUT PINK
#define MAX520_ADDRESS 0b0101000 // Assumes AD0,AD1,AD2 are grounded

void    motor_init();
int16_t motor_read_encoder();
void    motor_velocity(int16_t velocity);


#endif
