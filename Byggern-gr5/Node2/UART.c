#include "define.h"
#include "UART.h"

FILE *uart;

void USART_Init(unsigned int ubrr)
{
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	
	UCSR0B = (1 << RXEN0)|(1 << TXEN0);
	
	uart = fdevopen(&USART_Transmit, &USART_Receive);
	
	//UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
}

void USART_Transmit(unsigned char data)
{
	/* Wait for empty transmit buffer */
	while ( !(UCSR0A & (1 << UDRE0)) );
	
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive(void)
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1 << RXC0)) );
	
	/* Get and return received data from buffer */
	return UDR0;
}