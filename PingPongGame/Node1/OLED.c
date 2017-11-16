#include "define.h"

void write_c(unsigned char out_com)
{
	volatile char *OLEDc = (char*)OLED_COMMAND_MEMORY_START;
	*OLEDc = out_com;
}

void write_d(unsigned char out_com)
{
	volatile char *OLEDd = (char*)OLED_DATA_MEMORY_START;
	*OLEDd = out_com;
}

void oled_init(void)
{
	bit_set(MCUCR, SRE);
	
	bit_set(SFIOR, XMM2);
	bit_clear(SFIOR, XMM1);
	bit_clear(SFIOR, XMM0);

	bit_set(MCUCR, SRW11);
	bit_clear(EMCUCR, SRW10);
}

void init_program()
{
	write_c(0xAE);        // display  off
	write_c(0xA1);        //segment  remap
	write_c(0xDA);        //common  pads  hardware:  alternative
	write_c(0x12);
	write_c(0xC8);        //common output scan direction:com63~com0
	write_c(0xA8);        //multiplex  ration  mode:63
	write_c(0x3F);
	write_c(0xD5);        //display divide ratio/osc. freq. mode
	write_c(0x80);
	write_c(0x81);        //contrast  control
	write_c(0x50);
	write_c(0xD9);        //set  pre-charge  period
	write_c(0x21);
	write_c(0x20);        //Set  Memory  Addressing  Mode
	write_c(0x02);
	write_c(0xBD);        //VCOM  deselect  level  mode
	write_c(0x30);
	write_c(0xAD);        //master  configuration
	write_c(0x00);
	write_c(0xA4);        //out  follows  RAM  content
	write_c(0xA6);        //set  normal  display
	write_c(0xAF);        // display  on
}

void oled_set_column(uint8_t column)
{
	write_c(0x00 + column%16);
	write_c(0x10 + column/16); // start address
	
}
void oled_set_page(uint8_t page)
{
	write_c(0xB0 + page);
}

void clear_oled()
{
	int i, j;
	
	for(j = 0; j < 8; j++)
	{
		oled_set_page(j);
		
		for(i = 0; i < 128; i++)
		{
			oled_set_column(i);
			write_d(0x00);
		}
	}
}

void OLED_print_arrow(uint8_t row, uint8_t col)
{
	write_d(0b00011000);
	write_d(0b00011000);
	write_d(0b01111110);
	write_d(0b00111100);
	write_d(0b00011000);
}



