#include "define.h"
#include "CAN.h"
#include "MCP2515.h"

#define LOOPBACK 0x40
#define CONFIG 0x80

void CAN_init(void)
{
	
		MCP_reset();

		MCP_write(MCP_CANCTRL, 0x80);		

		MCP_write(MCP_CNF1, 0x20); // Write config 1
		MCP_write(MCP_CNF2, 0x92); // Write config 2
		MCP_write(MCP_CNF3, 0xC2); // Write config 3
		
		MCP_write(MCP_CANINTE, 0x03);	//
		MCP_write(MCP_CANINTF, 0x00);	//
		MCP_write(MCP_EFLG, 0x00);		//
		
		MCP_write(0x0C, 0x00);			 // Disable all interrupts
		MCP_write(0x0D, 0x00);			// Clear TXRTSCTRL
		
		
		MCP_write(MCP_RXB0CTRL, 0x60);		// Receive buffer 0 control (turn mask/filters off, receive any message) 
		MCP_write(MCP_RXB1CTRL, 0x60);		// Receive buffer 1 control (turn mask/filters off, receive any message)


		MCP_write(MCP_RXM0SIDH, 0x00);		// Clear ID masks
		MCP_write(MCP_RXM0SIDL, 0x00);		// Clear ID masks
		MCP_write(MCP_RXM0EID8, 0x00);		// Clear ID masks
		MCP_write(MCP_RXM0EID0, 0x00);		// Clear ID masks

		MCP_write(MCP_CANCTRL, 0x44);		// Enable can controller
	}
}


/*
void CAN_message_send(void);
void CAN_error(void);
void CAN_transmit_complete(void);
void CAN_data_receive(void);
void CAN_int_vect(void);*/

