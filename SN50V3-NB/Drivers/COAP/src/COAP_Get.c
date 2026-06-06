#include "COAP_Get.h"

char* GetMsg(char* buff,const char* url)
{
	COAP coapGet = 
	{
		.head = (Version <<6) | 0x04,	/*Header Flags: 
																		01.. .... = Version: 1
																		..00 .... = Type: Confirmable (0)
																		.... 0100 = Token Length: 4*/
		.code = GET,									/* Code: GET (1) */
		.messageID = 0x0003,
		.token = 0x0A000001,
	};
	
	/*-------------------GET HEAD-------------------*/
	sprintf(buff+strlen(buff), "%.2x", coapGet.head);
	sprintf(buff+strlen(buff), "%.2x", coapGet.code);
	sprintf(buff+strlen(buff), "%.4x", coapGet.messageID);
	sprintf(buff+strlen(buff), "%.8x", coapGet.token);
	
	uint8_t optDelta=0;
	uint8_t optLength=0;
	uint8_t optName=0;
	/*-------------------PUT OPT NAME1 (Observe)-------------------*/
	optDelta = observe_optDelta - optDelta;
	optLength = 0;
	optName = optDelta <<4 | optLength;
	sprintf(buff+strlen(buff), "%.2x", optName);
	/*-------------------PUT OPT NAME2 (Resource route or path)-------------------*/
	uint8_t i = 1;
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
				optDelta = uri_Path_optDelta - observe_optDelta;
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
	/*-------------------PUT OPT NAME3 (Access resource parameters)-------------------*/
	optDelta = uri_Query_optDelta - uri_Path_optDelta;
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
	
	return buff;
}

