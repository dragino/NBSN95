#include "upload.h"

NBState sockCreat(void)
{
	if(sys.protocol == COAP_PRO)
		user_main_printf("Protocol in Used: Coap");
	else if(sys.protocol == UDP_PRO)
		user_main_printf("Protocol in Used: UDP");
	uint8_t errorNum=0;
	uint8_t state_flag=0;
	
	while(state_flag == 0)
	{
		while(nb_send(nb.recieve_data,AT NSOCR UDP)== fail)
		{
			HAL_Delay(1000);
			errorNum++;
			if(errorNum>3)
			{
				nb_Reboot();
				return fail;				
			}
		}
		if(strstr(nb.recieve_data,"ERROR") != NULL)
		{
				HAL_Delay(1000);
				errorNum ++;
				if(errorNum > 5)
				{
					user_main_printf("Failed to create socket.");
					nb_Reboot();
					return fail;
				}
		}
		else 
		{
			user_main_debug("Socket creation succeeded.");
			state_flag  = 1;
		}			
	}
	
	char *pch = strchr(nb.recieve_data,'O');
	nb.socket = nb.recieve_data[(pch-nb.recieve_data)-5];
	user_main_debug("nb.socket:%c\r\n",nb.socket);
	
	errorNum = 0;
	state_flag=0;
	while(state_flag == 0)
	{
		while(nb_send(nb.recieve_data,AT CSQ"\n")== fail)
		{
			HAL_Delay(1000);
			errorNum++;
			if(errorNum>3)
			{
				nb_Reboot();
				return fail;				
			}
		}
		if(strstr(nb.recieve_data,"+CSQ:99,99") != NULL)
		{
				HAL_Delay(1000);
				errorNum ++;
				if(errorNum > 10)
				{
					user_main_printf("No signal.");
					nb_Reboot();
					return fail;
				}
		}
		else 
		{
			state_flag  = 1;
		}			
	}
	
	char singalBuff[5]={0}; 	
	char *pch1 = strchr(nb.recieve_data,':');
	char *pch2 = strchr(nb.recieve_data,',');
	for(int i =0;i<(pch2 - pch1)-1;i++)
		sprintf(singalBuff+strlen(singalBuff), "%c", nb.recieve_data[7+i]);
	
	uint8_t singal = atoi(singalBuff);
	
	char singalbuff[3]={0};
	sprintf(singalbuff+strlen(singalbuff), "%.2x", singal);
	sensor.data[4] = singalbuff[0];
	sensor.data[5] = singalbuff[1];
	
	user_main_printf("Signal Strength:%s\r\n",singalBuff);
	user_main_debug("sensor.data:%s\r\n",sensor.data);
	
	return success;
}

NBState dataSendPut(char* payload)
{
//	uint8_t errorNum=0;
//	uint8_t unread=0;
	
	char add[25]={0};
	memcpy(add,user.add,strlen((char*)user.add));
	
	for(uint8_t i=0;i<strlen(add);i++)
	{
		if(add[i] == 0x3A)
			add[i] = 0x2C;
	}
	
	putMsg(payload,(char*)user.uri,sensor.data);																							
	uint16_t payloadLen = strlen(payload)/2;
	memset(payload,0,strlen(payload));																
																																		
	strcat(payload,AT NSOST "=");																			
	sprintf(payload+strlen(payload), "%c", nb.socket);
	strcat(payload, ",");
	strcat(payload,add);																					
	strcat(payload, ",");																							
	sprintf(payload+strlen(payload), "%d", payloadLen);								
	strcat(payload, ",");																							
	putMsg(payload,(char*)user.uri,sensor.data);	
	strcat(payload, ",1\n");
	user_main_debug("put-payload:%s",payload);
	
	nb_send(nb.recieve_data,payload);
			
	return success;
}

NBState sockClosd(void)
{
	char payload[60]={0};
	char add[25]={0};
	memcpy(add,user.add,strlen((char*)user.add));	
	for(uint8_t i=0;i<strlen(add);i++)
	{
		if(add[i] == 0x3A)
			add[i] = 0x2C;
	}
	
	strcat(payload,AT NSOSTF "=");																			
	sprintf(payload+strlen(payload), "%c", nb.socket);					
	strcat(payload, ",");
	strcat(payload,add);
	strcat(payload, ",0x200,1,FF");																															
	strcat(payload, "\n");																							
	nb_send(nb.recieve_data,payload);				
	
	memset(payload,0,sizeof (payload));
	strcat(payload,"AT+NSOCL=1");
	payload[strlen(payload)-1] = nb.socket;
	strcat(payload,"\n");
	nb_send(nb.recieve_data,payload);	
	
	return success;
}

NBState dataSendUDP(char* payload)
{
	uint8_t errorNum=3;
	//uint8_t unread=0;
	
	char add[25]={0};
	memcpy(add,user.add,strlen((char*)user.add));
	
	for(uint8_t i=0;i<strlen(add);i++)
	{
		if(add[i] == 0x3A)
			add[i] = 0x2C;
	}
																								
	memset(payload,0,strlen(payload));
																																		
	strcat(payload,AT NSOST "=");																			
	sprintf(payload+strlen(payload), "%c", nb.socket);
	strcat(payload, ",");
	strcat(payload,add);																					
	strcat(payload, ",");																							
	sprintf(payload+strlen(payload), "%d", strlen(sensor.data));								
	strcat(payload, ",");																							
	for(int j = 0;j<strlen(sensor.data);j++)
		sprintf(payload+strlen(payload), "%.2x", sensor.data[j]);
	strcat(payload, ",1\n");
	user_main_debug("payload:%s",payload);
	
	if(sys.cfm == '0')
		nb_send(nb.recieve_data,payload);
	else if(sys.cfm == '1')
	{
		char readCFM[50]={0};
		strcat(readCFM,AT NSORF "=");																			
		sprintf(readCFM+strlen(readCFM), "%c", nb.socket);
		strcat(readCFM, ",50\n");	
		user_main_debug("readCFM:%s",readCFM);
		do
		{
			nb_send(nb.recieve_data,payload);
			HAL_Delay(2000);
			nb_send(nb.recieve_data,readCFM);
			if(strstr(nb.recieve_data,",") != NULL)
				break;
		}while(errorNum--);
	}
	
	if(errorNum == 0)
		user_main_printf("Server ACK not received");
	
	return success;
}
