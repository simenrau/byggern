#include "define.h"
#include "UART.h"
#include "SRAM.h"
#include "RAM.h"
#include "ADC.h"
#include "OLED.h"
#include "fonts.h"
#include "MCP2515.h"
#include "SPI.h"
#include "CAN.h"


void test_buttons()
{
	DDRB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2));
	 
	bool val_joystick;  //testing if joystick pushbutton returns its value
	while(1)
	{		
		bool button_left = (PINB & (1 << PB0));
		bool button_right = (PINB & (1 << PB1));
		
		bool joystick_pressed = val_joystick;		//Ettersom det i utgangspunktet returneres '0' når Joysticken trykkes, implementerer vi den simple koden nedenfor, slik at vi får '1' når Joysticken trykkes på.
		val_joystick = (PINB & (1 << PB2));
		
		if (joystick_pressed == 0)
		{
			joystick_pressed = 1;
		}
		else
		{
			joystick_pressed = 0;
		}
		
		
		printf("Joystick: %d\n", joystick_pressed);										//Pull-up resistor gjør at det fungerer
		printf("Left button: %d Right button: %d\n", button_left, button_right);
	}
	  
}

void test_adc()
{
		ram_init();
		
		printf("Testing ADC:\n");
		
		while (1)
	{
			uint8_t ch1 = read_adc(0);
			uint8_t ch2 = read_adc(1);
			uint8_t ch3 = read_adc(2);
			uint8_t ch4 = read_adc(3);
			printf("%d %d %d %d\n", ch1, ch2, ch3, ch4);
			_delay_ms(1000);
	}
	
}	

void test_joystick()
{
    USART_Init(MYUBRR);
    ram_init();
    uint8_t min_x = 255;
    uint8_t max_x = 0;
    uint8_t min_y = 255;
    uint8_t max_y = 0;
    uint8_t button_threshold = 128;
	
    while (1)
    {
        unsigned int x_volt = read_adc(ADC_CHANNEL_JOY_X);
        unsigned int y_volt = read_adc(ADC_CHANNEL_JOY_Y);
        unsigned int b_volt = read_adc(ADC_CHANNEL_JOY_B);
        uint8_t is_down = (b_volt < button_threshold);

		printf("%i, %i, %d\n", x_volt, y_volt, b_volt);
		_delay_ms(500);
		
		//Hvis verdiene er mellom 120 og 132 byte skal det returneres NEUTRAL
		if((x_volt >= 124 && x_volt <= 132) && (y_volt >= 122 && y_volt <= 130))
		{
			printf("NEUTRAL\n");
		}
		if (y_volt > 130)
		{
			printf("UP\n");
		}
		if (y_volt < 122)
		{
			printf("DOWN\n");
		}
		if (x_volt > 132)
		{
			printf("RIGHT\n");
		}
		if (x_volt < 124)
		
		{
			printf("LEFT\n");
		}
		
		
		printf("\n\n\n");
		
		
    }
	

}

void print_from_usart_to_oled()
{
	
	oled_init();
	init_program();
	clear_oled();
	oled_set_page(0);
	int i,j = 0;
	int count = 1;
	while(1)
	{
		char a = USART_Receive();
		
		for(i = 0; i < 5; i++)
			{
			oled_set_column(0 + i+(j*5));
			write_d(pgm_read_byte(&font5[a - ' '][i]));
			}
		if (j > 25)
			{
			oled_set_page(count);
			count = count+1;
			j=0;
			}

		j++;
	}
}

void print_oled()
{
	oled_init();
	init_program();
	clear_oled();
	
	oled_set_page(0);
	char c[27] = "Naa er det snart jul iaarrs";
	int i,j;
	for(j = 0;j<25;j++)
	{

		for(i = 0;i<5;i++)
			{
			oled_set_column(0 + i+(j*5));
			write_d(~pgm_read_byte(&font5[c[j] - ' '][i]));
			printf("%c%\n",c[i]);
			}
	}
	
}

void oled_menu()
{
	ram_init();
	init_program();
	oled_init();
	clear_oled();
	
	const char menu[5][20] = {
		"--HOVEDMENY--",
		" Hjem        ",
		" Fotball     ",
		" Golf        ",
		" Annet       ",
	};
	
	const char fotball[3][20] = {
		"--FOTBALL--",
		" Arsenal   ",
		" Man.Utd.  ",
	};
			
	int count_menu = 4;
	int select_menu = 1;
	int sub_menu = 0;
	while(1){
	while(sub_menu == 1)
	{
		int delay = 2000; //delay in milliseconds
		oled_set_column(0);
		oled_set_page(0);
		int i,j,k;
		
		unsigned int x_volt = read_adc(ADC_CHANNEL_JOY_X);
		unsigned int y_volt = read_adc(ADC_CHANNEL_JOY_Y);
		//printf("%d %d\n", x_volt, y_volt);
	
		if (y_volt < 125 && select_menu < count_menu-2)
		{
			select_menu++;
		}
		if (y_volt > 135 && select_menu > 1)
		{
			select_menu--;
		}
		if(x_volt < 60)
			{
				sub_menu = 0;
				break;
			}
		for (k = 0; k < 3; k++)
		{
			for (j = 0; j < 11; j++)
			{
				for (i = 0; i < 5; i++)
				{
					if(select_menu==k)
					{
						oled_set_column(i+(j*5));
						write_d(~pgm_read_byte(&font5[fotball[k][j] - ' '][i]));
					}
					else
					{
						oled_set_column(i+(j*5));
						write_d(pgm_read_byte(&font5[fotball[k][j] - ' '][i]));
					}
				}
			}
			oled_set_page(k+1);
		}
		_delay_ms(delay);
	}
	
	while (sub_menu == 0)
	{
		int delay = 500; //delay in milliseconds
		oled_set_column(0);
		oled_set_page(0);
		int i,j,k;
		
		unsigned int x_volt = read_adc(ADC_CHANNEL_JOY_X);
		unsigned int y_volt = read_adc(ADC_CHANNEL_JOY_Y);
		//printf("%d %d\n", x_volt, y_volt);
		
		if (y_volt < 125 && select_menu < count_menu)
		{
			select_menu++;
		}
		if (y_volt > 130 && select_menu > 1)
		{
			select_menu--;
		}
		printf("%d\n",select_menu);
		
		if(x_volt > 135 && select_menu == 2)
		{
			sub_menu = 1;
			clear_oled();
			select_menu = 1;
			break;
		}
		for (k = 0; k < 5; k++)
		{
			for (j = 0; j < 13; j++)
			{
				for (i = 0; i < 5; i++)
				{
					if(select_menu==k)
					{
						oled_set_column(i+(j*5));
						write_d(~pgm_read_byte(&font5[menu[k][j] - ' '][i]));
					}
					else
					{
						oled_set_column(i+(j*5));
						write_d(pgm_read_byte(&font5[menu[k][j] - ' '][i]));
					}
				}
			}			
			oled_set_page(k+1);
		}
		_delay_ms(delay);
		
		
	}
	}

}

void test_SPI()
{
	
	SPI_MasterInit();
	
	/*while(1)
	{
		SPI_MasterTransmit(0b111101111);	
	}*/
	
	//SPI_SlaveInit();
	//SPI_SlaveReceive();
	MCP_init();
}
int main(void)
{
	USART_Init(MYUBRR);
	
	//SRAM_test();
	//test_adc();
	//test_buttons();
	//test_joystick();
	//print_to_oled();
	//print_oled();
	//OLED_print_arrow();
	//oled_menu();
	//test_SPI();
	
	
	MCP_init();
	CAN_init();
	for(char i =0;i<255;i++){
	printf("\n%d",MCP_read(i));
	}
	
}

