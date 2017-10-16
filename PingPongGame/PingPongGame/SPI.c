#define "define.h"
#define "SPI.h"
#define SS PB4
#define MOSI PB5
#define MISO PB6
#define SCK PB7


int SPI_MasterInit(void)
{
	DDRB = (1 << MOSI) | (1 << SS) | (1 << SCK);
	
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
	
	return 0;
}


void SPI_MasterTransmit(char cdata)
{
	SPDR = cdata;
	
	while(!(SPSR & (1 << SPIF)));
}

void SPI_SlaveInit(void)
{
	/* Set MISO output, all others input */
	DDRB = (1 << MISO);
	
	/* Enable SPI */
	SPCR = (1<<SPE);
}


uint8_t SPI_SlaveReceive(void)
{
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)));
	/* Return data register */
	return SPDR;
}