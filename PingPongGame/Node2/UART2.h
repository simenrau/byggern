#ifndef UART2_H
#define UART2_H
#include "define2.h"

void USART_Init(unsigned int ubrr);

void USART_Transmit(unsigned char data);

unsigned char USART_Receive(void);

extern FILE *uart;

#endif