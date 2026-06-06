#include "upload.h"

NBState sockCreat(void)
{
	user_main_printf("Protocol in Used: LWM2M");
	uint8_t errorNum=0;
	uint8_t state_flag=0;
	
//	while(state_flag == 0)
//	{
//		while(nb_send(nb.recieve_data,AT NSOCR UDP)== fail)
//		{
//			HAL_Delay(1000);
//			errorNum++;
//			if(errorNum>3)
//			{
//				nb_Reboot();
//				return fail;				
//			}
//		}
//		if(strstr(nb.recieve_data,"ERROR") != NULL)
//		{
//				HAL_Delay(1000);
//				errorNum ++;
//				if(errorNum > 5)
//				{
//					user_main_printf("Failed to create socket.");
//					nb_Reboot();
//					return fail;
//				}
//		}
//		else 
//		{
//			user_main_debug("Socket creation succeeded.");
//			state_flag  = 1;
//		}			
//	}
//	
//	char *pch = strchr(nb.recieve_data,'O');
//	nb.socket = nb.recieve_data[(pch-nb.recieve_data)-5];
//	user_main_debug("nb.socket:%c\r\n",nb.socket);
//	
//	errorNum = 0;
//	state_flag=0;
//	while(state_flag == 0)
//	{
//		while(nb_send(nb.recieve_data,AT CSQ"\n")== fail)
//		{
//			HAL_Delay(1000);
//			errorNum++;
//			if(errorNum>3)
//			{
//				nb_Reboot();
//				return fail;				
//			}
//		}
//		if(strstr(nb.recieve_data,"+CSQ:99,99") != NULL)
//		{
//				HAL_Delay(1000);
//				errorNum ++;
//				if(errorNum > 10)
//				{
//					user_main_printf("No signal.");
//					nb_Reboot();
//					return fail;
//				}
//		}
//		else 
//		{
//			state_flag  = 1;
//		}			
//	}
//	
//	char singalBuff[5]={0}; 	
//	char *pch1 = strchr(nb.recieve_data,':');
//	char *pch2 = strchr(nb.recieve_data,',');
//	for(int i =0;i<(pch2 - pch1)-1;i++)
//		sprintf(singalBuff+strlen(singalBuff), "%c", nb.recieve_data[7+i]);
//	
//	uint8_t singal = atoi(singalBuff);
//	
//	char singalbuff[3]={0};
//	sprintf(singalbuff+strlen(singalbuff), "%.2x", singal);
//	sensor.data[4] = singalbuff[0];
//	sensor.data[5] = singalbuff[1];
//	
//	user_main_printf("Signal Strength:%s\r\n",singalBuff);
//	user_main_debug("sensor.data:%s\r\n",sensor.data);
	
	return success;
}

NBState sockClosd(void)
{

	
	return success;
}

NBState dataSendUDP(char* payload)
{

	return success;
}
