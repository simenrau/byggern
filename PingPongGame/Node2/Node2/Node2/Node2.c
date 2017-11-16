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
		
		_delay_ms(5000);
		while(ir_value < 10)
		{
			ir_value = ADC_read();
			printf("%d\n",ir_value);
		}
		_delay_ms(2000);
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
		
		//motor_velocity(message->data[0]);
		
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
		
		//printf("T: %d\n",T);
		//printf("U: %d	I*Ki: %d	e*Kp: %d	y: %d	T: %d\n",(int)u,(int)(I*Ki),(int)(Kp*e),(int)y,(int)T);
		printf("Data: %d", message->data[0]);
		
		T = T+1;
		//max min -> [4250,-4250]

	}
}

void new_oled_menu()
{
	const char menu[4][20] = { // menu 1, 13 plasser
		"--HOVEDMENY--",
		" Choose Team ",
		" Gamemodes   ",
		" High Score  ",
	};
	const char team[5][20] = {  // menu 2, 18 plasser
		"     --TEAM--     ",
		" German           ",
		" Allied           ",
		" Soviet           ",
		" Sweden (cowards) ",
	};
	const char games[4][20] = {  // menu 3, 13 plasser
		"--GAMEMODES--",
		" Normal      ",
		" Hard-mode   ",
		" Dark souls  ",
	};
	const char team_print[4][22] = {  // print team, 20 plasser
		"Team chosen: German ",
		"Team chosen: Allied ",
		"Team chosen: Soviet ",
		"Team chosen: Sweden ",
	};
	
	int select_menu = 1;
	int sub_menu = 1;
	int delay = 500;
	int team_chosen = 0;
	int gamemode_chosen = 0;
	int lives = 0;
	int i,j,k,lives_tot;
	
	while(1){
		
		select_menu = 1;  // setting the menu to the first choice every time
		
		while (sub_menu == 1){ // Main menu

			unsigned int x_volt = read_adc(ADC_CHANNEL_JOY_X);
			unsigned int y_volt = read_adc(ADC_CHANNEL_JOY_Y);
			
			if (y_volt < 125 && select_menu < 4){
				select_menu++;
			}
			if (y_volt > 130 && select_menu > 1){
				select_menu--;
			}
			printf("%d\n",select_menu);
			
			if(x_volt > 135){
				if(sub_menu == 1){ 	//go to teams
					sub_menu = 2;
					break;
				}
				if(sub_menu == 2){ 	 // go to gamemodes
					sub_menu = 3;
					break;
				}
				if(sub_menu == 3){   // display highscore
					sub_menu = 4;
					break;
				}
			}
			
			clear_oled();
			oled_set_column(0);
			oled_set_page(0);
			
			for (k = 0; k < 4; k++){
				for (j = 0; j < 13; j++){
					for (i = 0; i < 5; i++){
						if(select_menu==k){
							oled_set_column(i+(j*5));
							write_d(~pgm_read_byte(&font5[menu[k][j] - ' '][i]));
						}
						else{
							oled_set_column(i+(j*5));
							write_d(pgm_read_byte(&font5[menu[k][j] - ' '][i]));
						}
					}
				}
				oled_set_page(k+1);
			}
			_delay_ms(delay);
		}
		while(sub_menu == 2){	 // Team menu

			unsigned int x_volt = read_adc(ADC_CHANNEL_JOY_X);
			unsigned int y_volt = read_adc(ADC_CHANNEL_JOY_Y);

			
			if (y_volt < 125 && select_menu < 5){	//5 lines
				select_menu++;
			}
			if (y_volt > 135 && select_menu > 1){
				select_menu--;
			}
			if(x_volt < 60){
				sub_menu = 1;
				break;
			}
			if(x_volt > 150){
				if (select_menu == 1){	//German
					team_chosen = 0;
				}
				if (select_menu == 2){	//allied
					team_chosen = 1;
				}
				if (select_menu == 3){	//soviet
					team_chosen = 2;
				}
				if (select_menu == 4){	//sweden
					team_chosen = 3;
				}
				sub_menu = 4;	//print chosen team
				break;
			}
			
			clear_oled();
			oled_set_column(0);
			oled_set_page(0);
			
			for (k = 0; k < 5; k++){
				for (j = 0; j < 18; j++){
					for (i = 0; i < 5; i++){
						if(select_menu==k){
							oled_set_column(i+(j*5));
							write_d(~pgm_read_byte(&font5[team[k][j] - ' '][i]));
						}
						else{
							oled_set_column(i+(j*5));
							write_d(pgm_read_byte(&font5[team[k][j] - ' '][i]));
						}
					}
				}
				oled_set_page(k+1);
			}
			_delay_ms(delay);
		}
		

		while (sub_menu == 3){ 	//Gamemodes menu

			unsigned int x_volt = read_adc(ADC_CHANNEL_JOY_X);
			unsigned int y_volt = read_adc(ADC_CHANNEL_JOY_Y);

			
			if (y_volt < 125 && select_menu < 4){
				select_menu++;
			}
			if (y_volt > 135 && select_menu > 1){
				select_menu--;
			}
			if(x_volt < 60){
				sub_menu = 1;
				break;
			}
			if(x_volt > 150){
				if (select_menu == 1){	//normal
					gamemode_chosen = 0;
					lives = 10;
					sub_menu =6;
					break;
				}
				if (select_menu == 2){	//hard-mode
					gamemode_chosen = 1;
					lives = 3;
					sub_menu =6;
					break;
				}
				if (select_menu == 3){	//dark souls
					gamemode_chosen = 2;
					lives = 1;
					sub_menu =6;
					break;
				}

			}
			
			clear_oled();
			oled_set_column(0);
			oled_set_page(0);
			
			for (k = 0; k < 4; k++){
				for (j = 0; j < 13; j++){
					for (i = 0; i < 5; i++)	{
						if(select_menu==k)		{
							oled_set_column(i+(j*5));
							write_d(~pgm_read_byte(&font5[team[k][j] - ' '][i]));
						}
						else{
							oled_set_column(i+(j*5));
							write_d(pgm_read_byte(&font5[team[k][j] - ' '][i]));
						}
					}
				}
				oled_set_page(k+1);
			}
			_delay_ms(delay);
		}
		
		
		while (sub_menu == 4){	//print team chosen

			clear_oled();
			oled_set_column(0);
			oled_set_page(0);
			
			for (k = 0; k < 1; k++){
				for (j = 0; j < 20; j++){
					for (i = 0; i < 5; i++){
						oled_set_column(i+(j*5));
						write_d(pgm_read_byte(&font5[team_print[team_chosen][j] - ' '][i]));
						
					}
				}
				oled_set_page(k+1);
			}
			sub_menu = 1;
			_delay_ms(delay*5);
		}
		while(sub_menu==5){
			
			//print highscore
			sub_menu = 1;
		}
		while(sub_menu==6){
			sub_menu = 1;		//  <----
			break;				//går direkte ut
			lives_tot = lives
			// --starte en timer kanksje--
			
			while(lives>0){
				
				game_running_print(lives,gamemode_chosen,team_chosen,lives_tot);
				lives  = can_joystick(lives);
				
				//program som sender info til node 2 og får igjen om det blir mistset liv
				//lagrer nytt antall liv
			}
			sub_menu = 5;  // highscore
		}
	}

}
void game_running_print(int lives, int gamemode,int team_chosen,int lives_tot){

	int i,j,k;
	
	const char team_gaming[5][20] = {  // menu 2, 15 plasser
		" Team: Germany ",
		" Team: Allied  ",
		" Team: Soviet  ",
		" Team: Sweden  ",
	};
	const char hearts[2][20] = {  // hearts
		" ", //empty heart -- space ---
		"!", //full heart  --- ! ---
	};
	const char gameOn[1][20] = {  // menu  -- game on--, 10 plasser
		"--GAMING--",
	};
	

	clear_oled();
	oled_set_column(0);
	oled_set_page(0);
	for (j = 0; j < 10; j++){
		for (i = 0; i < 5; i++){
			oled_set_column(i+(j*5));
			write_d(pgm_read_byte(&font5[gameOn[0][j] - ' '][i]));
			
		}
	}
	oled_set_page(1);
	
	for (j = 0; j < 15; j++){
		for (i = 0; i < 5; i++){
			oled_set_column(i+(j*5));
			write_d(pgm_read_byte(&font5[team_gaming[team_chosen][j] - ' '][i]));
			
		}
	}
	oled_set_page(2);
	
	for(k = 0;k < lives;k++){
		for (i = 0; i < 11; i++){
			if(lives_tot < k ){		//if hearts now is less than hearts startet with, print empty heart.
				oled_set_column(i+(k*11));
				write_d(pgm_read_byte(&font5[hearts[0][0] - ' '][i]));
			}
			else{
				oled_set_column(i+(k*11));
				write_d(pgm_read_byte(&font5[hearts[1][0] - ' '][i]));
			}
		}
	}
	
}
void can_transmit_receive()
{
	int life = 10;
	while (1)
	{
		life = ball_count(life);
		
		if(life == 0)
		{
			break;
		}
		printf("ADC: %d\nLife: %d\n",ADC, life);
		msg can_message;
		
		can_message.id = 70;
		can_message.length = 8;
		can_message.data[7] = life;
		
		CAN_message_send(can_message);
		//printf("x_val: %d\n", can_message.data[0]);
		
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
		slider_to_pw(-message->data[6]);
		
	/*	printf("DATA: %d\n", message->data[0]);
		printf("DATA: %d\n", message->data[6]);*/
		
		free(message);
	}
}


int main(void)
{
	//INIT
	USART_Init(MYUBRR);
	MCP_init();
	CAN_init();
	PWM_init();	
	ADC_init();
	motor_init();
	solenoid_init();
		
	//Interupts
	sei();
	
	//Programs
	//test_motor_can();
	//test_CAN_nodes();
	//can_transmit_receive();
	new_oled_menu();
		
	
	
	
	return 0;
}