#include "lwm2m.h"

char* connectMsg(char* buff,const char* uri)
{
	char uri_buff[50] = { 0 };
	uint8_t optdesc_new = 0;
	/*-------------------Constrained Application Protocol, Confirmable, POST, MID:XX XX ,TOEKN:XX XX XX XX XX XX XX XX-------------------*/
	COAP coapPost = 
	{
		.head = (Version <<6) | 0x08,	/*Header Flags: 
										01.. .... = Version: 1
										..00 .... = Type: Confirmable (0)
										.... 1000 = Token Length: 8*/
		.code = POST,					/* Code: POST (2) */
		.messageID = rand(),			/* Message ID */
		.token_l = (rand()<<16)|rand(),	/* Token Low */
		.token_h = (rand()<<16)|rand(),	/* Token High */
	};
	
	sprintf(buff+strlen(buff), "%.2x", coapPost.head);
	sprintf(buff+strlen(buff), "%.2x", coapPost.code);
	sprintf(buff+strlen(buff), "%.4x", coapPost.messageID);
	sprintf(buff+strlen(buff), "%.4x", coapPost.token_l);
	sprintf(buff+strlen(buff), "%.4x", coapPost.token_h);
/*-------------------URI PATH-------------------*/
	char* psr = strstr(uri, "/");
	while(psr != NULL)
	{
		memset(uri_buff, 0, sizeof(uri_buff));
		for (uint8_t i = 0;; i++)
		{
			if ((*(psr + i + 1) != '/' ) && (*(psr + i + 1) != '?'))
				uri_buff[i] = *(psr + i + 1);
			else
				break;
		}
		coapPost.optdesc =((Uri_Path- optdesc_new) <<4) | strlen(uri_buff);	/*  Opt Desc: Type 11, Critical, Unsafe
																	1011 .... = Opt Delta: 11
																	.... xxxx = Opt Length: x*/																	
		sprintf(buff+strlen(buff), "%.2x", coapPost.optdesc);
		for(uint8_t i = 0;i< strlen(uri_buff); i++)				/*Uri-Path*/
			sprintf(buff+strlen(buff), "%.2x",   uri_buff[i]);
		psr = strstr((psr+1), "/");
		optdesc_new = Uri_Path;
	}
/*-------------------Content-Format-------------------*/
	coapPost.optdesc = ((Content_Format-optdesc_new) << 4) | 0x01;	/*  Opt Desc: Type 12, Elective, Safe
																	0001 .... = Opt Delta: 1
																	.... 0001 = Opt Length: 1*/
	sprintf(buff + strlen(buff), "%.2x", coapPost.optdesc);
	sprintf(buff + strlen(buff), "%.2x", Application_link_format);		/*Content-type: application/link-format*/
	optdesc_new = Content_Format;
/*-------------------Uri-Query-------------------*/
	psr = strstr(uri, "?");
	while (psr != NULL)
	{
		memset( uri_buff,0,sizeof(uri_buff) );
		for (uint8_t i = 0;; i++)
		{
			if ((*(psr + i + 1) != '&') && (*(psr + i + 1) != '\0'))
				uri_buff[i] = *(psr + i + 1);
			else
				break;
		}
		if (strlen(uri_buff) > 0x0d)
		{
			coapPost.optdesc = ((Uri_Query-optdesc_new) << 4) | 0x0d;
			sprintf(buff + strlen(buff), "%.2x", coapPost.optdesc);
			sprintf(buff + strlen(buff), "%.2x", strlen(uri_buff)-0x0d);
			for (uint8_t i = 0; i < strlen(uri_buff); i++)				
				sprintf(buff + strlen(buff), "%.2x", uri_buff[i]);
		}
		else
		{
			coapPost.optdesc = ((Uri_Query - optdesc_new) << 4) | strlen(uri_buff);	/*  Opt Desc: Type 11, Critical, Unsafe
																				1011 .... = Opt Delta: 11
																				.... xxxx = Opt Length: x*/
			sprintf(buff + strlen(buff), "%.2x", coapPost.optdesc);
			for (uint8_t i = 0; i < strlen(uri_buff); i++)				/*Uri-Path*/
				sprintf(buff + strlen(buff), "%.2x", uri_buff[i]);
		}			
		psr = strstr((psr + 1), "&");
		optdesc_new = Uri_Query;
	}
/*-------------------End of options marker-------------------*/
	sprintf(buff + strlen(buff), "%.2x", 0xff);
/*-------------------Payload: Payload Content - Format : application / link - format, Length : 37-------------------*/
	strcat(buff,"3c2f3e3b72743d226f6d612e6c776d326d222c203c2f312f303e2c203c2f333230302f303e");
	return buff;
}

char* sendDataMsg(char* buff, const char* payload)
{
	COAP coapSend =
	{
		.head = (Version << 6) | 0x18,	/*Header Flags:
										01.. .... = Version: 1
										..01 .... = Type: Confirmable (1)
										.... 1000 = Token Length: 8*/
		.code = Content,					/* Code: DELETE (4) */
		.messageID = rand(),			/* Message ID */
		.token_l = (rand() << 16) | rand(),	/* Token Low */
		.token_h = (rand() << 16) | rand(),	/* Token High */
	};

	sprintf(buff + strlen(buff), "%.2x", coapSend.head);
	sprintf(buff + strlen(buff), "%.2x", coapSend.code);
	sprintf(buff + strlen(buff), "%.4x", coapSend.messageID);
	sprintf(buff + strlen(buff), "%.4x", coapSend.token_l);
	sprintf(buff + strlen(buff), "%.4x", coapSend.token_h);

	strcat(buff, "6102622d16ff");
	strcat(buff, "e61676");			//TLV header
	for (uint8_t i = 0; i < strlen(payload); i++)
		sprintf(buff + strlen(buff), "%.2x", payload[i]);

	strcat(buff, "e816770b53656e736f722054797065");
	
	return buff;
}

char* recieveMsg(char* buff, const char* recieve_data)
{
	uint8_t coap_head = ((recieve_data[0] - 0x30) << 4 | (recieve_data[1] - 0x30));
	uint8_t coap_code = ((recieve_data[2] - 0x30) << 4 | (recieve_data[3] - 0x30));
	switch (coap_head)
	{
		case 0x68:								//连接服务器收到ACK
			if (coap_code == Created)
			{
				//...
			}
			else if (coap_code == Deleta)		//主动与服务器断开连接收到ACK
			{
				//...
			}
			break;
		case 0x48:
			if (coap_code == GET)
			{
				strcat(buff, "68");
				sprintf(buff + strlen(buff), "%.2x", Content);
				memcpy(buff + strlen(buff), &recieve_data[4], 4);
				memcpy(buff + strlen(buff), &recieve_data[8], 16);
				if (strstr(recieve_data, "6054333230300130622D16"))		//  /3200/0
				{					
					strcat(buff, "60622d16ffe81676104170706c69636174696f6e2054797065e816770b53656e736f722054797065");
				}
				else if (strstr(recieve_data, "B4333230306128"))		//  /3200
				{
					strcat(buff, "c128ff3c2f333230303e2c203c2f333230302f303e2c203c2f333230302f302f353735303e2c203c2f333230302f302f353735313e");
				}
				//else if (strstr(recieve_data, "B1330130622D16"))		//  /3/0
				else
				{
					buff[2] = (NotFound >> 4) + 0x30;
					buff[3] = (NotFound & 0x0F)  + 0x30;
				}
			}
			break;
		default:

			break;
	}

	return buff;
}

char* deletetMsg(char* buff, const char* imei)
{
	COAP coapDelete =
	{
		.head = (Version << 6) | 0x08,	/*Header Flags:
										01.. .... = Version: 1
										..00 .... = Type: Confirmable (0)
										.... 1000 = Token Length: 8*/
		.code = DELETE,					/* Code: DELETE (4) */
		.messageID = rand(),			/* Message ID */
		.token_l = (rand() << 16) | rand(),	/* Token Low */
		.token_h = (rand() << 16) | rand(),	/* Token High */
	};

	sprintf(buff + strlen(buff), "%.2x", coapDelete.head);
	sprintf(buff + strlen(buff), "%.2x", coapDelete.code);
	sprintf(buff + strlen(buff), "%.4x", coapDelete.messageID);
	sprintf(buff + strlen(buff), "%.4x", coapDelete.token_l);
	sprintf(buff + strlen(buff), "%.4x", coapDelete.token_h);

	strcat(buff,"b272640d02");

	for (uint8_t i = 0; i < strlen(imei); i++)
		sprintf(buff + strlen(buff), "%.2x", imei[i]);

	return buff;
}

