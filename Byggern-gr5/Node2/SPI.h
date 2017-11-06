#ifndef SPI2_H
#define SPI2_H
#include "define.h"

#define SS PB7
#define SCK PB1
#define MOSI PB2
#define MISO PB3

void SPI_MasterInit(void);
void SPI_MasterTransmit(char cdata);
void SPI_SlaveInit(void);
uint8_t SPI_SlaveReceive(void);


#endif