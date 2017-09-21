#include "define.h"
#include "UART.h"
#include "SRAM.h"
#include "RAM.h"
#include "ADC.h"

#define ADC_CHANNEL_JOY_X 0b00000100
#define ADC_CHANNEL_JOY_Y 0b00000101
#define ADC_CHANNEL_JOY_B 0b00000110
#define ADC_CHANNEL_TOUCH 0b00000111

void test_joystick(void);
int main(void)
{
	//SRAM_test();
	
	//test_adc();
	
	test_joystick();
}

//sei()

void test_adc()
{
		USART_Init(MYUBRR);
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
		_delay_us(1000);
    }
}

// Left button mid port B
// left slider mid back port B
// right button back left corner port D
// right slider left corner port D