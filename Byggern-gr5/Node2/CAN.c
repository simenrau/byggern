#include "define.h"
#include "CAN.h"
#include "MCP2515.h"

void CAN_init(void)
{
		/*SPI_MasterInit();
		SPI_SlaveInit();*/
		MCP_init();


		MCP_write(MCP_CANCTRL, MODE_CONFIG);		

		/*MCP_write(MCP_CNF1, 0x20); // Write config 1
		MCP_write(MCP_CNF2, 0x92); // Write config 2
		MCP_write(MCP_CNF3, 0xC2); // Write config 3*/
		
		MCP_write(MCP_CANINTE, 0x03);	//
		MCP_write(MCP_CANINTF, 0x00);	//
		MCP_write(MCP_EFLG, 0x00);		//
		
		MCP_write(0x0C, 0x00);			 // Disable all interrupts
		MCP_write(0x0D, 0x00);			// Clear TXRTSCTRL
		
		
		MCP_write(MCP_RXB0CTRL, 0x60);		// Receive buffer 0 control (turn mask/filters off, receive any message) 
		MCP_write(MCP_RXB1CTRL, 0x60);		// Receive buffer 1 control (turn mask/filters off, receive any message)

		//MCP_write(MCP_CANCTRL, 0x44);		// Enable can controller
		MCP_bit_mod(MCP_CANCTRL,MODE_MASK,MODE_NORMAL);

	
}

void CAN_message_send(msg can_tx)
{
	//MCP_write(MCP_TXB0CTRL, 0x03);				
	printf("\nCan ID sent: %02x \n", can_tx.id);

	//MCP_write(MCP_TXB0SIDH, can_tx.id);
	MCP_write(MCP_TXB0SIDH, can_tx.id);
	MCP_write(MCP_TXB0SIDH, can_tx.id);
	
	MCP_write(MCP_TXB0DLC, (can_tx.length));
	for(int i = 0; i < can_tx.length; i++)
	{
		MCP_write(MCP_TXB0D0 + i, can_tx.data[i]);
	}
	MCP_rts(MCP_RTS_TX0);
}

void CAN_data_receive(msg *message)
{
	//msg message;

	message->id = (MCP_read(MCP_RXB0SIDH));

	//message.id = MCP_read(MCP_RXB0SIDL);

	message->length = MCP_read(MCP_RXB0DLC);
	
	if (message->length > 8){
		message->length = 8;
	}
	
	for(int i = 0; i < message->length; i++) 
	{
		message->data[i] = MCP_read(MCP_RXB0DM + i);
	}
	
	MCP_bit_mod(MCP_CANINTF,0x01,0);

	//return message;
}




/*

void CAN_error(void);
void CAN_transmit_complete(void);*/

void CAN_int_vect(void)
{
	MCP_bit_mod(MCP_CANINTF, MCP_RXF0SIDL, MCP_RXF0SIDH);
}

