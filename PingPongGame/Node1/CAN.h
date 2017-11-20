#ifndef CAN_H
#define CAN_H
#include "define.h"

typedef struct  
{
	uint16_t id;
	uint8_t length;
	uint8_t data[8];	
} msg;



void CAN_init(void);
void CAN_message_send(msg can_tx);
void CAN_data_receive(msg *message);
#endif