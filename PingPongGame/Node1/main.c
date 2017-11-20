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
#include "hearts.h"


void game()
{
	//MENU CONSTANTS
	const char menu[4][13] = { // menu 1
		"--HOVEDMENY--",
		" Choose Team ",
		" Gamemodes   ",
	};
	const char team[5][18] = {  // menu 2
		"     --TEAM--     ",
		" German           ",
		" Allied           ",
		" Soviet           ",
		" Sweden (cowards) ",
	};
	const char games[4][13] = {  // menu 3
		"--GAMEMODES--",
		" Normal      ",
		" Hard-mode   ",
		" Dark souls  ",
	};
	const char team_print[4][20] = {  // print team
		"Team chosen: German ",
		"Team chosen: Allied ",
		"Team chosen: Soviet ",
		"Team chosen: Sweden ",
	};
	const char game_over[1][22] = {  // game over
		"     G A M E  O V E R ",
	};
	const char team_gaming[4][15] = {  // menu 4
		" Team: Germany ",
		" Team: Allied  ",
		" Team: Soviet  ",
		" Team: Sweden  ",
	};
	const char hearts[2][1] = {  // hearts
		" ", //empty heart -- space ---
		"!", //full heart  --- ! ---
	};
	const char gameOn[1][17] = {  // menu  -- game on--,
		"-- G A M I N G --",
	};
	const char score[1][9] = {  
			"S C O R E",
	};
	const char numbers[10][1] = {  
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
	};

	
	//CONSTANTS
	int select_menu = 1;		// place in menu
	int sub_menu = 1;			//which menu
	int delay = 200;		
	int team_chosen = 0;		//default germany
	int gamemode_chosen;	
	char lives = 0;
	char lives_tot = 0;
	int i,j,k;
	long int timer; 
	int count = 0;		// number of digits in the score
	
	
	
	// SETTING NOED 2 TO WAITING STATE
	msg can_message;
	
	can_message.id = 0x70;
	can_message.length = 7;				
	can_message.data[2] = 10;
	
	CAN_message_send(can_message);
	
	//START PROGRAM
	while(1){
		
		select_menu = 1;  // setting the menu to the first choice every time
		
		// MAIN MENU
		while (sub_menu == 1){ 

			unsigned int x_volt = read_adc(ADC_CHANNEL_JOY_X);
			unsigned int y_volt = read_adc(ADC_CHANNEL_JOY_Y);
			
			if (y_volt < 125 && select_menu < 2){
				select_menu++;
			}
			if (y_volt > 130 && select_menu > 1){
				select_menu--;
			}
			
			if(x_volt > 135){
				if(select_menu == 1){ 	//go to teams menu
					sub_menu = 2;

					break;
				}
				if(select_menu == 2){ 	 // go to gamemodes menu
					sub_menu = 3;

					break;
				}
			}
			
			clear_oled();
			oled_set_column(0);
			oled_set_page(0);
			
			for (k = 0; k < 3; k++){
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
		
		// TEAM MENU
		while(sub_menu == 2){	
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
			
			//READ MULTIFUNCTION CARD VALUES
			unsigned int x_volt = read_adc(ADC_CHANNEL_JOY_X);
			unsigned int y_volt = read_adc(ADC_CHANNEL_JOY_Y);
			DDRB &= ~(1 << PB2);
			bool val_joystick;
			bool joystick_pressed = val_joystick;
			val_joystick = (PINB & (1 << PB2));
						
						
			if (joystick_pressed == 0){
				joystick_pressed = 1;
			}
			else{
				joystick_pressed = 0;
			}

			
			if (y_volt < 125 && select_menu < 4){	//5 lines
				select_menu++;
			}
			if (y_volt > 135 && select_menu > 1){
				select_menu--;
			}
			if(x_volt < 60){
				sub_menu = 1;
				break;
			}
			if(joystick_pressed==1){
				if (select_menu == 1){
					team_chosen = 0;
				}
				if (select_menu == 2){	
					team_chosen = 1;
				}
				if (select_menu == 3){	
					team_chosen = 2;
				}
				if (select_menu == 4){	
					team_chosen = 3;
				}
				sub_menu = 4;	// Go to print chosen team
				break;
			}
			

		}
		
		//GAMEMODES MENU
		while (sub_menu == 3){ 	
			
			clear_oled();
			oled_set_column(0);
			oled_set_page(0);
						
			for (k = 0; k < 4; k++){
				for (j = 0; j < 13; j++){
					for (i = 0; i < 5; i++)	{
						if(select_menu==k)		{
							oled_set_column(i+(j*5));
							write_d(~pgm_read_byte(&font5[games[k][j] - ' '][i]));
						}
						else{
							oled_set_column(i+(j*5));
							write_d(pgm_read_byte(&font5[games[k][j] - ' '][i]));
						}
					}
				}
				oled_set_page(k+1);
			}

			_delay_ms(delay);
			
			//READ MULTIFUNCTION CARD VALUES
			DDRB &= ~(1 << PB2);
			bool val_joystick;
			bool joystick_pressed = val_joystick;
			val_joystick = (PINB & (1 << PB2));
			
			if (joystick_pressed == 0){
				joystick_pressed = 1;
			}
			else{
				joystick_pressed = 0;
			}
			unsigned int x_volt = read_adc(ADC_CHANNEL_JOY_X);
			unsigned int y_volt = read_adc(ADC_CHANNEL_JOY_Y);

			
			if (y_volt < 125 && select_menu < 3){
				select_menu++;
			}
			if (y_volt > 135 && select_menu > 1){
				select_menu--;
			}
			if(x_volt < 60){
				sub_menu = 1;			//Go to main manu
				break;
			}
			if(joystick_pressed == 1){
				if (select_menu == 1){	
					lives =  5;
					sub_menu = 6;
				}
				if (select_menu == 2){	
					lives = 3;
					sub_menu =6;
				}
				if (select_menu == 3){
					lives = 1;
					sub_menu =6;
				}
				break;		// Go to gaming
			}
			
			_delay_ms(delay);			
		}
		
		//PRINT TEAM CHOSEN
		while (sub_menu == 4){	

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
			sub_menu = 1;			// go to main menu
			_delay_ms(delay*5);
		}
		
		// PRINT GAME OVER
		while(sub_menu==5){   
			clear_oled();
			oled_set_column(0);
			oled_set_page(3);
			
			for (j = 0; j < 22; j++){
				for (i = 0; i < 5; i++)	{
					oled_set_column(i+(j*5));
					write_d(pgm_read_byte(&font5[game_over[0][j] - ' '][i]));	
				}
			}
			_delay_ms(delay*10);
			sub_menu = 7;			//Go to score
		}
		//GAMING
		while(sub_menu==6){		
			
			timer = 0;
			lives_tot = lives;
	
			clear_oled();
			oled_set_page(0);
			for (j = 0; j < 17; j++){
				for (i = 0; i < 5; i++){
					oled_set_column(i+(j*5));
					write_d(pgm_read_byte(&font5[gameOn[0][j] - ' '][i]));		
				}
			}
			while(lives>0){
				
				msg *message = (msg*)malloc(sizeof(msg));
				
				CAN_data_receive(message);
				lives = message->data[7];
				
				free(message);
				
				clear_page(4);
				oled_set_column(0);
				oled_set_page(4);
					
				for(k = 0; k < lives_tot; k++){
					for (i = 0; i < 11; i++){
						if(lives <= k ){
							oled_set_column(i+(k*11));
							write_d(pgm_read_byte(&heart[hearts[0][0] - ' '][i]));
						}
						else{
							oled_set_column(i+(k*11));
							write_d(pgm_read_byte(&heart[hearts[1][0] - ' '][i]));
						}
					}
				}
				_delay_ms(1);			
				
				DDRB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2));
				
				//READ MULTIFUNCTION CARD VALUES
				unsigned int x_volt = read_adc(ADC_CHANNEL_JOY_X);
				unsigned int y_volt = read_adc(ADC_CHANNEL_JOY_Y);
				unsigned int SR_volt = read_adc(ADC_CHANNEL_SLIDER_R);
				unsigned int SL_volt = read_adc(ADC_CHANNEL_SLIDER_L);
				bool val_joystick;  
				bool button_left = (PINB & (1 << PB0));
				bool button_right = (PINB & (1 << PB1));
				bool joystick_pressed = val_joystick;		
				val_joystick = (PINB & (1 << PB2));
				
				if (joystick_pressed == 0){
					joystick_pressed = 1;
				}
				else{
					joystick_pressed = 0;
				}
				
				//CREATE MESSAGE
				msg can_message;
				
				can_message.id = 0x70;
				can_message.length = 7;
				
				can_message.data[0] = x_volt;
				can_message.data[1] = y_volt;
				can_message.data[2] = joystick_pressed;
				can_message.data[3] = button_left;
				can_message.data[4] = button_right;
				can_message.data[5] = SR_volt;
				can_message.data[6] = lives_tot;


				
				//SEND MESSAGE
				CAN_message_send(can_message);
				timer = timer + 1;

			}
			sub_menu = 5;  //Go to game over
			
			// SET NOE 2 TO WAITING
			msg can_message;
				
			can_message.id = 0x70;
			can_message.length = 7;				
			can_message.data[2] = 10;
				
			CAN_message_send(can_message);
		}
		//DISPLAY SCORE
		while(sub_menu == 7) 
		{
			int val;
			int count = 0;
			double n = (double)timer;
			
			while(n >= 1){
				n /= 10;
				++count;
			}

			char arr[count];
			
			//SPILTING THE DIGITS
			for(i = 0; i < count; i++){
				val = timer % 10;
				arr[i] = val;
				timer = timer - val;
				timer = timer / 10;
			}
			
			clear_oled();
			oled_set_column(0);
			oled_set_page(0);
			for (k = 0; k < 9; k++){
				for (i = 0; i < 5; i++)	{
					oled_set_column(30+i+(k*5));
					write_d(pgm_read_byte(&font5[score[0][k] - ' '][i]));
				}
			}

			oled_set_page(2);
			for (k = count; k > 0; k--){
				for (i = 0; i < 5; i++)	{
					oled_set_column(30+i+((count-k)*5));
					write_d(pgm_read_byte(&font5[numbers[arr[k-1]][0] - ' '][i]));
					_delay_ms(50);
				}
			}		
			_delay_ms(delay*20);
			sub_menu = 1;			//Go to main manu
		}

	}

}



void main(void)
{
	//INIT
	USART_Init(MYUBRR);
	ram_init();
	MCP_init();
	CAN_init();
	oled_init();
	init_program();
	
	//GAING 
	game();
	
}

