#ifndef SPI_H
#define SPI_H
#include "define.h"

#define SS PB4
#define MOSI PB5
#define MISO PB6
#define SCK PB7


void SPI_MasterInit(void);
void SPI_MasterTransmit(char cdata);
void SPI_SlaveInit(void);
uint8_t SPI_SlaveReceive(void);


#endif