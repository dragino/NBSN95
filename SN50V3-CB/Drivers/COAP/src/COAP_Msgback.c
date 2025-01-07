#include "COAP_Msgback.h"

CODEMessageType ackMsg(uint8_t head)
{
	COAP coapMsgBack = 
	{
		.head = head & 0xF0,									
	};

	if(coapMsgBack.head == ((Version <<6) | (Acknowledgement<<4) | 0x00))	//0x60  c->s
		return ackMessage;
	else if(coapMsgBack.head == ((Version <<6) | 0x00))				//0x40   s->c
		return conMessage;
	
	return errorMessage;
}
