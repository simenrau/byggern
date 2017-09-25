#include "define.h"
#include "UART.h"
#include "SRAM.h"
#include "RAM.h"
#include "ADC.h"
#include "OLED.h"
#include "fonts.h"

void test_buttons()
{
	 DDRB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2));
	 
	
	while(1)
	{		
		bool button_left = (PINB & (1 << PB0));
		bool button_right = (PINB & (1 << PB1));
		bool joystick_pressed = (PINB & (1 << PB2));
		
		printf("Joystick: %d\n", joystick_pressed);										//Pull-up resistor gjør at det fungerer (verdi 0 når joystick holdes nede)
		//printf("Left button: %d Right button: %d\n", button_left, button_right);
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

		//printf("%i, %i, %d\n", x_volt, y_volt, b_volt);
		_delay_ms(500);
		
		//Hvis verdiene er mellom 120 og 132 byte skal det returneres NEUTRAL
		if((x_volt >= 120 && x_volt <= 132) && (y_volt >= 120 && y_volt <= 130))
		{
			printf("NEUTRAL\n");
		}
		if (y_volt > 130)
		{
			printf("UP\n");
		}
		if (y_volt < 120)
		{
			printf("DOWN\n");
		}
		if (x_volt > 132)
		{
			printf("RIGHT\n");
		}
		if (x_volt < 120)
		
		{
			printf("LEFT\n");
		}
		printf("\n\n\n");
    }

}


int main(void)
{
	USART_Init(MYUBRR);
	
	//SRAM_test();
	//test_adc();
	//test_buttons();
	
	//test_joystick();

	
	init_program();
	//oled_set_column();
	//oled_set_page();
	while(1)
	{
		
	write_c(0x00);
	write_c(0x10);
	write_c(0xB0);
	
	write_d(0xF);
	}
}


//sei()
