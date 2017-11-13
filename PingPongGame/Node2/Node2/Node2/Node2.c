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
		msg *message = (msg*)malloc(sizeof(msg));
		
		uint8_t status = MCP_read(MCP_CANINTF);
		printf("Status: %x\n\n",status);
		printf("CANINTF: %x\n\n",MCP_read(MCP_CANINTF));
		printf("EFLG: %x\n\n",MCP_read(MCP_EFLG));
		if (status & 0x01)
		{
			CAN_data_receive(message,0);
			printf("Register 0:\n");
		} else if (status & 0x02)
		{
			CAN_data_receive(message,1);
			printf("Register 1:\n");
		} else {continue;}

		printf("ID received: %02x \n",message->id);
		printf("Length: %d \n",message->length);
		
		for(int i = 0; i < message->length; i++)
		{
			printf("DATA[%d]: %d \n",i, message->data[i]);
		}
		
		motor_velocity(message->data[0]-128);
		printf("%d\n",message->data[0]);
		
		free(message);
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

	while (1)
	{
		//CAN_message_send(can_message);
		//delay_ms(10);
		//printf("1\n");
		uint8_t status = MCP_read(MCP_CANINTF);
		printf("CANINTF: %x\n\n",status);
		
		msg *message = (msg*)malloc(sizeof(msg));
		CAN_data_receive(message,0);
		
		status = MCP_read(MCP_CANINTF);
		printf("CANINTF: %x\n\n",status);
		
		printf("2\n");
		for(int i = 0; i < message->length; i++)
		{
			printf("DATA[%d]: %d \n",i, message->data[i]);
		}
		printf("ID received: %02x \n",message->id);
		printf("Length: %d \n",message->length);
		free(message);
		_delay_ms(1000);
	}
	
	

	/*can_message.data[0] = x_volt;
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

void test_motor()
{
	printf("Testing motor...\n");
	motor_init();

	int16_t x = 70;
	while (1)
	{
		printf("Sending %d\n", x);
		motor_velocity(x);
		int16_t encoder = motor_read_encoder();
		printf("Encoder: %d\n", encoder);
		_delay_ms(500);
	}
}
	
void test_motor_can()
{
	int T = 0;
	float I = 0;
	while (1){
				
		msg *message = (msg*)malloc(sizeof(msg));
		CAN_data_receive(message,0);

		

		//motor_velocity(message->data[0] - 128);
		
		//--PRINTING VALUES----
		/*printf("motor speed: %d",message->data[0]);
		printf("Length: %d \n",message->length);
		printf("ID received: %02x \n\n",message->id);
		for(int i = 0; i < message->length; i++)
		{
			printf("DATA[%d]: %d \n",i, message->data[i]);
		}*/
		
		//REGULATOR
		
		float Ki = 0.004;
		float Kp = 0.03; 

		
		float y = motor_read_encoder();
		float ref = (message->data[0] - 128)*33.2;
		float e = ref-y;
		I = e + I;
		float u = Kp*e  + Ki*I; 
		motor_velocity((int)u);
		
		printf("T: %d\n",T);
		//printf("U: %d	I*Ki: %d	e*Kp: %d	y: %d	T: %d\n",(int)u,(int)(I*Ki),(int)(Kp*e),(int)y,(int)T);
		
		T = T+1;
		//max min -> [4250,-4250]

	}
}


void motor_PD()
{
	
}


int main(void)
{
	//INIT
	USART_Init(MYUBRR);
	MCP_init();
	CAN_init();
	//PWM_init();	
	ADC_init();
	motor_init();
	

	
	
	sei();
	test_motor_can();
	
	//test_CAN_loopback();
	
	//test_CAN_nodes();

	
	
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
	
	
	return 0;
}