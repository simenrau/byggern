#include "define2.h"
#include "CAN2.h"
#include "MCP2515.h"

void CAN_init(void)
{
		MCP_init();


		MCP_write(MCP_CANCTRL, MODE_CONFIG);		
		
		MCP_write(MCP_CANINTE, 0x00);	
		MCP_write(MCP_CANINTF, 0x00);	
		MCP_write(MCP_EFLG, 0x00);		
		
		MCP_write(0x0C, 0x00);
		MCP_write(0x0D, 0x00);			
		
		MCP_write(MCP_RXB0CTRL, 0x60);
		MCP_write(MCP_RXB1CTRL, 0x60);

		MCP_write(MCP_CANCTRL,MODE_NORMAL);
}

void CAN_message_send(msg can_tx)
{
	MCP_write(MCP_TXB2SIDH, can_tx.id);
	MCP_write(MCP_TXB2SIDH, can_tx.id);

	MCP_write(MCP_TXB2DLC, (can_tx.length));
	for(int i = 0; i < can_tx.length; i++)
	{
		MCP_write(MCP_TXB2D0 + i, can_tx.data[i]);
	}
	MCP_rts(MCP_RTS_TX2);
}

void CAN_data_receive(msg *message)
{
	message->id = (MCP_read(MCP_RXB0SIDH));

	message->length = MCP_read(MCP_RXB0DLC);
		
	if (message->length > 8){
		message->length = 8;
	}
		
	for(int i = 0; i < message->length; i++)
	{
		message->data[i] = MCP_read(MCP_RXB0DM + i);
	}
	
	MCP_write(MCP_CANINTF,0x0);
}
