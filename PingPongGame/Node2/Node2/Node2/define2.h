#ifndef F_CPU
#define F_CPU 16000000 // 4,9152 MHz clock speed
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <util/delay.h>
#include <math.h>




#define FOSC 16000000 //Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1