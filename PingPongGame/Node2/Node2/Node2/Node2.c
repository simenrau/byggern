#include "define2.h"
#include "UART2.h"
#include "CAN2.h"
#include "MCP2515.h"
#include "SPI2.h"
#include "PWM.h"
#include "ADC_driver.h"
#include "TWI_master.h"
#include "motor.h"

void test_CAN_nodes()
{

	while (1){
		
		while (1)
		{
			char status = MCP_read_status();
			char status1 = status & 0b0000011;
			printf("Status: 0x%02x, Error flag:0x%02x \n",MCP_read(MCP_CANINTF),MCP_read(0x2D));
			if (status1 == 3){  // flagg 1 og 2 høy
				//CANINTF.RX0IF
				//CANINTFL.RX1IF
				printf("flagg 1 og 2\n");
				break;
			}
			else if (status1 == 2){  // flagg 2 høy
				//CANINTFL.RX1IF
				break;
			}
			else if (status1 == 1){  // flagg 1 høy
				//CANINTF.RX0IF
				break;
			}
		}
	
		msg *message = (msg*)malloc(sizeof(msg));
		CAN_data_receive(message);

	
		for(int i = 0; i < message->length; i++)
		{
			printf("DATA[%d]: %d \n",i, message->data[i]);
		}
		Joy_to_pw(message->data[6]);
		printf("%d",message->data[6]);
		
		printf("Length: %d \n",message->length);
		printf("ID received: %02x \n\n",message->id);
		
	}
}

void test_CAN_loopback()
{
	msg can_message;
	can_message.id = 0x30;
	can_message.length = 8;
	
	can_message.data[0] = 45;
	can_message.data[1] = 9;
	can_message.data[2] = 56;
	can_message.data[3] = 1;
	can_message.data[4] = 5;
	can_message.data[5] = 33;
	can_message.data[6] = 43;
	can_message.data[7] = 44;

	
	CAN_message_send(can_message);
	_delay_ms(10);
	printf("1\n");
	msg *message = (msg*)malloc(sizeof(msg));
	CAN_data_receive(message);
	printf("2\n");
	for(int i = 0; i < message->length; i++)
	{
		printf("DATA[%d]: %d \n",i, message->data[i]);
	}
	printf("ID received: %02x \n",message->id);
	printf("Length: %d \n",message->length);

/*
	can_message.data[0] = x_volt;
	can_message.data[1] = y_volt;
	can_message.data[2] = joystick_pressed;*/
}

void ir_driver(void)
{
	DDRK &= ~(1 << PK0);
	printf("AD1: %d \n",PINK & (1 << PINK0));
}

int ball_count(int life)
{
	int irval = 0;
	for (int i = 0;i<10;i++){
		irval = irval + ADC_read();
	}
	int ir_value = irval/10;
	if (ir_value < 10){
		life = life - 1;
		if (life == 0)
		{
			printf("GAME OVER!");
			return 0;
		}
		
		_delay_ms(1000);
		while(ir_value > 10){};
	}
	
	return life;
}

int main(void)
{
	MCP_init();
	CAN_init();
	PWM_init();	
	ADC_init();
	USART_Init(MYUBRR);
	
	motor_init();
	motor_enable();
	
	while(1){
		motor_set(100,1);
		printf("stuff %d \n",motor_read_encoder());
	}
	
	
	
	
	/*int life = 3;
	while (1)
	{
		life = ball_count(life);
				
		if(life == 0)
		{
			break;
		}
		printf("ADC: %d\nLife: %d\n",ADC, life);

	}*/
	
	//test_CAN_nodes();
	

	
	return 0;
}