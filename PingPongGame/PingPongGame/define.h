#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <util/delay.h>


#ifndef F_CPU
#define F_CPU 4915200 // 4,9152 MHz clock speed
#endif

// stuff til oled
#define OLED_COMMAND_MEMORY_START 0x1000
#define OLED_DATA_MEMORY_START    0x1200
#define ADC_MEMORY_START          0x1400
#define SRAM_MEMORY_START         0x1800


#define FOSC 4915200 //Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

//USB multifunction board
#define ADC_CHANNEL_JOY_Y 0b00000100
#define ADC_CHANNEL_JOY_X 0b00000101
#define ADC_CHANNEL_JOY_B 0b00000110
#define ADC_CHANNEL_TOUCH 0b00000111

//RAM definitions
#define bit_set(reg, bit) (reg |= (1 << bit))
#define bit_clear(reg, bit) (reg &= ~(1 << bit))
#define bit_test(reg, bit) (reg & (1 << bit))