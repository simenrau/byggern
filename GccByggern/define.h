#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>


//UART and SRAM
#define F_CPU 4915200 // 4,9152 MHz clock speed
#define FOSC 4915200 //Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1





