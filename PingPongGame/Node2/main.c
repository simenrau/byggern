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

char ball_count(char life)
{
	// CHECKING IF THE AVRAGE FROM THE LAST 10 VALUES 
	int irval = 0;
	for (int i = 0;i<10;i++){
		irval = irval + ADC_read();
	}
	int ir_value = irval/10;
	
	//IF THE BALL BREAKES THE LIGTH, LOOSE A LIFE.
	if (ir_value < 10){
		life = life - 1;
		if (life == 0)
		{
			return 0;
		}
		
		_delay_ms(1500);
		while(ir_value < 10)
		{
			ir_value = ADC_read();
		}
		_delay_ms(1000);
	}
	
	return life;
}

void game()
{
	char lives_tot,lives;
	char i = 10;
	
	while(1){
		
		//WATING FOR NODE 1 TO SEND
		while(i == 10)			
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
		
		
		//START GAMING
		while(i != 10){
		
			msg *message = (msg*)malloc(sizeof(msg));
			CAN_data_receive(message);
			
			// P REGULATOR
			float Kp = 0.05;
			float y = motor_read_encoder();
			float ref = (-message->data[5] + 128)*35.2;
			float e = ref-y;
			float u = Kp*e;
			
			//SEND U TO THE MOTOR  
			motor_velocity((int)u);
			
			
			//SOLENOID
			if (message->data[2] == 1)
			{
				solenoid_push();
			}
			else{
				solenoid_ret();
			}
			
			//SERVO
			slider_to_pw(message->data[0]);
			
			//CHECKING IR	
			lives = ball_count(lives);
			
			
			// CREATE MESSAGE
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
			
			// SEND	
			CAN_message_send(can_message);

			i = message->data[2];
			
			if(lives < 1){
				i =10;
			}
			free(message);
		}
	}
		
}

void main(void)
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
	
	//THE GAME//
	game();
}