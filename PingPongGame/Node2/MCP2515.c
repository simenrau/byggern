#include "define2.h"
#include "SPI2.h"
#include "MCP2515.h"

int MCP_init(void)
{
	SPI_MasterInit();
	MCP_reset();
	_delay_us(10);
	return 0;
}

int MCP_reset(void)
{
	PORTB &= ~(1 << SS);
	
	SPI_MasterTransmit(MCP_RESET);
	
	PORTB |= (1 << SS);
	
	return 0;
}

char MCP_read(char address)
{
	PORTB &= ~(1 << SS); 
		
	SPI_MasterTransmit(MCP_READ);
	SPI_MasterTransmit(address);
	
	char data = SPI_SlaveReceive();
	
	PORTB |= (1 << SS);
	
	return data;

}

void MCP_write(char address, char data)
{
	PORTB &= ~(1 << SS); 
	
	SPI_MasterTransmit(MCP_WRITE);
	SPI_MasterTransmit(address);
	SPI_MasterTransmit(data);
	
	PORTB |= (1 << SS);
}

void MCP_rts(char rts)
{
	PORTB &= ~(1 << SS);
	
	SPI_MasterTransmit(rts);
	
	PORTB |= (1 << SS);
	
}

char MCP_read_status(void)
{
	PORTB &= ~(1 << SS);
	
	SPI_MasterTransmit(MCP_READ_STATUS);
	SPI_SlaveReceive();
	char status = SPI_SlaveReceive();
	
	PORTB |= (1 << SS);
	
	return status;
}

void MCP_bit_mod(char address, char mask, char data)
{
	PORTB &= ~(1 << SS);
	
	SPI_MasterTransmit(MCP_BITMOD);
	SPI_MasterTransmit(address);
	SPI_MasterTransmit(mask);
	SPI_MasterTransmit(data);
	
	PORTB |= ~(1 << SS);
}