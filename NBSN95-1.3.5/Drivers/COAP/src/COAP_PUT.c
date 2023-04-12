#include "COAP_PUT.h"

char* putMsg(char* buff,const char* url,const char* payload)
{
	COAP coapPut = 
	{
		.head = (Version <<6) | 0x00,	/*Header Flags: 
																		01.. .... = Version: 1
																		..00 .... = Type: Confirmable (0)
																		.... 0000 = Token Length: 0*/
		.code = PUT,									/* Code: PUT (3) */
		.messageID = 0x0001,
	};
	
	/*-------------------PUT HEAD-------------------*/
	sprintf(buff+strlen(buff), "%.2x", coapPut.head);
	sprintf(buff+strlen(buff), "%.2x", coapPut.code);
	sprintf(buff+strlen(buff), "%.4x", coapPut.messageID);
	/*-------------------PUT OPT NAME1 (Resource route or path)-------------------*/
	uint8_t i = 1;
	uint8_t optName = 0;
	uint8_t optDelta = 0;
	uint8_t optLength = 0;	
	char* end= strchr(url,'/');
	while(i)
	{				
		char* start = end;
		end++;
		end = strchr(end,'/');
		if(end == NULL)
		{
			end = strchr(url,'?');
			i = 0;
		}
		
		if(end != NULL)
		{
			if(i == 1)
				optDelta = uri_Path_optDelta;
			else 
				optDelta = 0;
			
			optLength = end - start -1;
			optName = optDelta<<4 | optLength;
			
			sprintf(buff+strlen(buff), "%.2x", optName);
			for(int j = 0;j<optLength;j++)
				sprintf(buff+strlen(buff), "%.2x", url[(start-url)+1+j]);
			
			if(i != 0)
				i++;
		}		
	}
	/*-------------------PUT OPT NAME2 (media type)-------------------*/
	optDelta = conten_tFormat_optDelta - uri_Path_optDelta;
	optLength = 0;
	optName = optDelta <<4 | optLength;
	sprintf(buff+strlen(buff), "%.2x", optName);
	/*-------------------PUT OPT NAME3 (Access resource parameters)-------------------*/
	optDelta = uri_Query_optDelta - conten_tFormat_optDelta;
	optLength = strlen(url) - (end - url) - 1;
	
	if(optLength > 13)
	{
		optName = optDelta <<4 | 0x0d;
		uint8_t optLengthExtented = optLength - 13;					
		sprintf(buff+strlen(buff), "%.2x", optName);
		sprintf(buff+strlen(buff), "%.2x", optLengthExtented);
	}
	else
	{
		optName = optDelta <<4 | optLength;
		sprintf(buff+strlen(buff), "%.2x", optName);
	}
	
	for(int j = 0;j<optLength;j++)
		sprintf(buff+strlen(buff), "%.2x", url[(end-url)+1+j]);
	/*-------------------PUT OPT NAME4 (BLOCK)-------------------*/
	optDelta = block_optDelta - uri_Query_optDelta;
	optLength = 1;
	optName = optDelta <<4 | optLength;
	sprintf(buff+strlen(buff), "%.2x", optName);
	sprintf(buff+strlen(buff), "%.2x", 2);
	/*-------------------END of OPT NAME-------------------*/
	sprintf(buff+strlen(buff), "%.2x", 0xff);
	/*-------------------PAYLOAD-------------------*/
	for(int j = 0;j<strlen(payload);j++)
//		buff[strlen(buff)] = payload[j];
		sprintf(buff+strlen(buff), "%.2x", payload[j]);
		
	return buff;
}
