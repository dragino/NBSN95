#include "nbInit.h"

NB nb = {.net_flag=fail,.recieve_flag=0,.rxlen=0,.imei_flag=fail,.recieve_data={0},.imei={0}};

NBState nb_send(char* data,char* buff)
{
	uint16_t errorNum = 0;
	nb.recieve_flag = 1;
	nb.rxlen = 0;
	memset(data,0,strlen(data));
	HAL_UART_Transmit_DMA(&hlpuart1,(uint8_t*)buff,strlen(buff));
	
	while((READ_REG(hlpuart1.Instance->ISR)&0x00010000) == 0x00010000 || nb.rxlen == 0)
	{
		HAL_Delay(100);
		errorNum++;
		if(errorNum>10)
			return fail;
	}
	
	nb.recieve_flag = 0;	
	user_main_info("recieve data:%s",data);
	
	return success;
}

NBState nb_Init(void)
{
	if(nb_send(nb.recieve_data,AT "\n") == fail)
	{
		user_main_printf("BC95 is not responding");
		return fail;
	}
	else
		user_main_printf("BC95 has responded");
	
	HAL_Delay(500);
	
	if(nb_CGSN() == success)
	{
		char buff[20]={0};
		char *pch = strchr(nb.recieve_data,':');
		char *pch1 = strchr(nb.recieve_data,'O');
		strncpy(buff, pch +1,pch1 - pch -5);
		user_main_printf("The IMEI number is:%s.",buff);
	}
	
	if(nb_CIMI() == fail)
	{
		user_main_printf("No card detected");
//		return fail;
	}
	else
	{
		user_main_printf("The IMSI number is:%s.",nb.imei);
	}
	
	user_main_printf("BC95 initialized successfully\r\n");
	return success;
}

NBState nb_CIMI(void)
{	
	for(uint8_t i=0;i<5;i++)
	{
		nb_send(nb.recieve_data,AT CIMI"\n");
		if(strstr(nb.recieve_data,"ERROR") != NULL)
		{
			HAL_Delay(2000);
		}
		else
		{
			char *pch = strchr(nb.recieve_data,'\n');
			char *pch1 = strchr(nb.recieve_data,'O');
			strncpy(nb.imei, pch +1,pch1 - pch -5);
			nb.imei_flag = success;
			return success;
		}
	}
	nb.imei_flag = fail;
	nb_Reboot();
	return fail;
}

NBState nb_netAccess(void)
{	
	uint8_t errorNum=0;
	user_main_printf("Connecting to the network, please do not operate......");
	do
	{
		nb_send(nb.recieve_data,AT CSQ"\n");
		if(strstr(nb.recieve_data,"+CSQ:99,99") != NULL)
		{
			nb_send(nb.recieve_data,AT CFUN"=1\n");
			nb_send(nb.recieve_data,AT CGATT"=1\n");
			errorNum++;
			HAL_Delay(3000);
		}
		else 
		{
			char singalBuff[5]={0}; 	
			char *pch1 = strchr(nb.recieve_data,':');
			char *pch2 = strchr(nb.recieve_data,',');
			for(int i =0;i<(pch2 - pch1)-1;i++)
				sprintf(singalBuff+strlen(singalBuff), "%c", nb.recieve_data[7+i]);
			user_main_printf("Signal Strength:%s",singalBuff);
			
			return success;
		}
	}while(errorNum<10);
	
	user_main_printf("Failed to register to the network");
	
	nb_send(nb.recieve_data,AT CFUN"?\n");
	for(uint8_t i=0;i<strlen(nb.recieve_data);i++)
		if(nb.recieve_data[i] == 'O' || nb.recieve_data[i] == 'K')
			nb.recieve_data[i] = 0;
	user_main_printf("%s",nb.recieve_data);
	
	nb_send(nb.recieve_data,AT NABND"?\n");
	for(uint8_t i=0;i<strlen(nb.recieve_data);i++)
		if(nb.recieve_data[i] == 'O' || nb.recieve_data[i] == 'K')
			nb.recieve_data[i] = 0;
	user_main_printf("%s",nb.recieve_data);
	
	nb_send(nb.recieve_data,AT CFUN"=0\n");
	
	return fail;
}

NBState nb_Reboot(void)
{	
	return nb_send(nb.recieve_data,AT NRB"\n");
}

NBState nb_CGSN(void)
{
	return nb_send(nb.recieve_data,AT CGSN"=1\n");
}
