#include "define2.h"
#include "UART2.h"
#include "CAN2.h"
#include "MCP2515.h"
#include "SPI2.h"

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
		
		printf("Length: %d \n",message->length);
		printf("ID received: %02x \n\n",message->id);
		_delay_ms(100);
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

	/*can_message.data[0] = x_volt;
	can_message.data[1] = y_volt;
	can_message.data[2] = joystick_pressed;*/
}

int main(void)
{
	MCP_init();
	CAN_init();
	USART_Init(MYUBRR);
	test_CAN_nodes();	
	/*while (1)
	{		
		printf(" %02x \n",MCP_read(MCP_CANCTRL));
	}	
	//test_CAN_loopback();*/
	
	
	
	return 0;
}