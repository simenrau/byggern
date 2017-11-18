#include "define2.h"
#include "UART2.h"
#include "CAN2.h"
#include "MCP2515.h"
#include "SPI2.h"
#include "PWM.h"
#include "ADC_driver.h"
#include "TWI_master.h"
#include "motor.h"
#include "solenoid.h"

void test_CAN_nodes()
{
	
	while (1){
		msg *message = (msg*)malloc(sizeof(msg));
		
		uint8_t status = MCP_read(MCP_CANINTF);
		/*printf("Status: %x\n\n",status);
		printf("CANINTF: %x\n\n",MCP_read(MCP_CANINTF));
		printf("EFLG: %x\n\n",MCP_read(MCP_EFLG));*/
		if (status & 0x01)
		{
			CAN_data_receive(message);
			//printf("Register 0:\n");
		} else if (status & 0x02)
		{
			CAN_data_receive(message);
			printf("Register 1:\n");
		} else {continue;}

		/*printf("ID received: %02x \n",message->id);
		printf("Length: %d \n",message->length);
		
		for(int i = 0; i < message->length; i++)
		{
			printf("DATA[%d]: %d \n",i, message->data[i]);
		}*/
		
		motor_velocity(-message->data[0]+128);
		printf("x volt: %d\n",message->data[0]);
		printf("rigth slider: %d\n",message->data[6]);
		slider_to_pw(-message->data[6]);
		
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
		CAN_data_receive(message);
		
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

char ball_count(char life)
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
			printf("GAME OVER!\n");
			return 0;
		}
		
		_delay_ms(3000);
		while(ir_value < 10)
		{
			ir_value = ADC_read();
			printf("reset ball..\n");
		}
		_delay_ms(1000);
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
		CAN_data_receive(message);
		
		float Ki = 0.005;
		float Kp = 0.05; 
		
		if(I > 20000)
		{
			I = 0;
		}

		
		float y = motor_read_encoder();
		float ref = (-message->data[5] + 128)*35.2;
		float e = ref-y;
		I = e + I;
		float u = Kp*e; 
		motor_velocity((int)u);
		
		if (message->data[2] == 1)
		{
			solenoid_push();
		}
		else{
			solenoid_ret();
		}
		slider_to_pw(message->data[0]);
		
		
		//printf("I: %d\n",I);
		//printf("T: %d\n",T);
		//printf("U: %d	I: %d	e: %d	y: %d	T: %d\n",(int)u,(int)(I),(int)(e),(int)y,(int)T);
		//printf("Data: %d", message->data[0]);
		
		T = T+1;
		//max min -> [4250,-4250]

	}
}

void game()
{
	int T = 0;
	float I = 0;
	char lives_tot,lives;
	char i = 10;
	while(1){
		while(i == 10) // while 'not in game', chill 
		{
			msg *message = (msg*)malloc(sizeof(msg));
			CAN_data_receive(message);
			i = message->data[2];
			free(message);
			_delay_ms(1000);
			printf("waiting...\n");
		}
		
		msg *message = (msg*)malloc(sizeof(msg));
		CAN_data_receive(message);
		
		lives_tot = message->data[6];
		lives = lives_tot;
	
		free(message);
		
		while(i != 10){
		
			msg *message = (msg*)malloc(sizeof(msg));
			CAN_data_receive(message);

		
			float Ki = 0.005;
			float Kp = 0.05;
		
			if(I > 20000)
			{
				I = 0;
			}

			
			
			float y = motor_read_encoder();
			float ref = (-message->data[5] + 128)*35.2;
			float e = ref-y;
			I = e + I;
			float u = Kp*e;
			motor_velocity((int)u);
			
			if (message->data[2] == 1)
			{
				solenoid_push();
			}
			else{
				solenoid_ret();
			}
			slider_to_pw(message->data[0]);
			
			
			
			
			
			lives = ball_count(lives);
			printf("lives: %d",lives);
			msg can_message;
					
			can_message.id = 70;
			can_message.length = 8;
			can_message.data[0] = message->data[0];
			can_message.data[1] = message->data[1];
			can_message.data[2] = message->data[2];
			can_message.data[3] = message->data[3];
			can_message.data[4] = message->data[4];
			can_message.data[5] = message->data[5];
			can_message.data[6] = message->data[6];
			can_message.data[7] = lives;
			_delay_ms(5);
			
			//printf("0: %d	1: %d	2: %d	3: %d	4: %d	5: %d	6: %d	7: %d\n",can_message.data[0],can_message.data[1],can_message.data[2],can_message.data[3],can_message.data[4],can_message.data[5],can_message.data[6],can_message.data[7]);
					
			CAN_message_send(can_message);

			i = message->data[2];
			
			if(lives < 1){
				i =10;
			}
			free(message);
		}
	}
	
	
	
}

void can_transmit_receive(){
	int hearts_tot,life;
	int k = 0;
	while (1)
	{
		msg *message = (msg*)malloc(sizeof(msg));
		
		CAN_data_receive(message);
		
		if (message->data[2] == 1)
		{
			solenoid_push();
		}
		else{
			solenoid_ret();
		}

		
		motor_velocity(-message->data[0]+128);
		slider_to_pw(-message->data[5]);
		hearts_tot = message->data[6];
		printf("slider: %d\n",message->data[5]);

		
		if(k==0){
			life = hearts_tot;
		}
		
		
		if(life == 0)
		{
			break;
		}

		life = ball_count(life);
		msg can_message;
		
		can_message.id = 70;
		can_message.length = 8;
		can_message.data[7] = life;
		
		CAN_message_send(can_message);
		//printf("x_val: %d\n", can_message.data[0]);
		


		k++;
		if(k<100){
			k =1;
		}
		
		free(message);
	}
}


/*
void can_transmit_receive()
{
	int life = 100;
	while (1)
	{
		msg can_message;
		
		can_message.id = 70;
		can_message.length = 8;
		can_message.data[7] = life;
		
		life = ball_count(life);
		
		CAN_message_send(can_message);
		
		msg *message = (msg*)malloc(sizeof(msg));
		
		while(life == 100){
			printf("lives: %d	from can: %d",life,message->data[7]);
			_delay_ms(1000);
			CAN_data_receive(message);
			life = message->data[7];

		}
		
		CAN_data_receive(message);
		
		if (message->data[2] == 1)
		{
			solenoid_push();
		}
		else{
			solenoid_ret();
		}

		
		motor_velocity(-message->data[0]+128);
		slider_to_pw(-message->data[6]);
		
		printf("DATA: %d\n", message->data[0]);
		printf("DATA: %d\n", message->data[6]);
		
		free(message);		
				
	}
}
*/


int main(void)
{
	//INIT//
	USART_Init(MYUBRR);
	MCP_init();
	CAN_init();
	PWM_init();	
	ADC_init();
	motor_init();
	solenoid_init();
		
	//INTERRUPTS//
	sei();
	
	//PROGRAM//
	//can_transmit_receive();			//includes solenoid, motor, servo actions

	//test_motor_can();
	//test_CAN_nodes();
	
	game();
	
	return 0;
}