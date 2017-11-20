#include "define2.h"
#include "UART2.h"

FILE *uart;

void USART_Init(unsigned int ubrr)
{
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	
	UCSR0B = (1 << RXEN0)|(1 << TXEN0);
	
	uart = fdevopen(&USART_Transmit, &USART_Receive);
}

void USART_Transmit(unsigned char data)
{
	while ( !(UCSR0A & (1 << UDRE0)) );
	
	UDR0 = data;
}

unsigned char USART_Receive(void)
{
	while ( !(UCSR0A & (1 << RXC0)) );
	
	return UDR0;
}
