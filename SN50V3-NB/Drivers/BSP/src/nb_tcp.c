#include "nb_tcp.h"
#include "time.h"
#include <time.h>
extern char buff[2000];
extern char downlink_data[1000];
extern float hum_value;
extern float tem_value;
extern float ds1820_value;
extern float ds1820_value2;
extern float ds1820_value3;
extern char 	*ATSendStr;
extern int 	len_string;
extern uint8_t  try_num;
extern NB_TaskStatus  nb_cmd_status;
extern void pro_data(void);
/**
	* @brief  Open TCP port operation
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_TCP_open_run(const char* param)
{
	try_num = 4;
	NBTask[_AT_TCP_OPEN].set(param);
	
	if(nb_at_send(&NBTask[_AT_TCP_OPEN]) == NB_CMD_SUCC)
		nb_cmd_status = NB_CMD_SUCC;
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;
}

NB_TaskStatus nb_TCP_open_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	char* pos = strchr((char*)user.add,',');
	
	strcat(buff,AT QIOPEN "=0,0,\"TCP\",\"");
	
			if(strlen((char*)user.add_ip)!=0)
	{	
	  char* pos = strchr((char*)user.add_ip,',');		
	  memcpy(buff+strlen(buff),user.add_ip,(pos-(char*)user.add_ip));	
	}
	else
	{
	   memcpy(buff+strlen(buff),user.add,(pos-(char*)user.add));	
	}
	strcat(buff,"\"");
	strcat(buff,(char*)&user.add[(pos-(char*)user.add)]);
	strcat(buff,",0");
	strcat(buff,",1\r\n");

	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	
	user_main_debug("openPayload:%s",ATSendStr);		
	return nb_cmd_status;
}

/**
	* @brief  Send TCP data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_TCP_send_run(const char* param)
{
	try_num = 2;
	NBTask[_AT_TCP_SEND].set(param);
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_TCP_SEND]) == NB_CMD_SUCC )
		{
			break;
		}
		else
		{
			HAL_Delay(500);
		}
	}
	return nb_cmd_status;
}

NB_TaskStatus nb_TCP_send_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	uint16_t str_end,str_beg=0;
	uint16_t batteryLevel_mV=getVoltage();	
	strcat(buff,AT QISEND "=");
	strcat(buff,"0,");
	if(sys.platform==5)	
	{
   pro_data();
	}
	if(sys.platform==0)		
	{
	sprintf(buff+strlen(buff), "%d", sensor.data_len);		
	strcat(buff,",\"");
	sprintf(buff+strlen(buff), "%s", sensor.data);	
	}
	strcat(buff,"\"\r\n");
	
	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	
	user_main_debug("NBTask[_AT_TCP_SEND].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}

/**
	* @brief  Read TCP data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_TCP_read_run(const char* param)
{
	NBTask[_AT_TCP_READ].set(param);
	if(nb_at_send(&NBTask[_AT_TCP_READ]) == NB_CMD_SUCC)
	{
		if(NBTask[_AT_TCP_READ].get(param) == NB_READ_DATA)
		{			
			rxPayLoadDeal(downlink_data);
		}
	}
	
	return nb_cmd_status;
}

NB_TaskStatus nb_TCP_read_get(const char* param)
{
	char *pch = strrchr((char*)nb.usart.data,','); 
	if(pch == NULL)
		nb_cmd_status = NB_READ_NODATA;
	else
	{
		memset(downlink_data,0,sizeof(downlink_data));
		char*	end    = pch;
		char* start  = strrchr((char*)nb.usart.data,'\"'); 

		memcpy(downlink_data,&nb.usart.data[end-((char*)nb.usart.data)+2],(start-end-2));	
		user_main_printf("Received downlink data:%s",downlink_data);
		
		nb_cmd_status = NB_READ_DATA;
	}
	return nb_cmd_status;	
}

/**
	* @brief  Close TCP port
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_TCP_close_run(const char* param)
{
	ATSendStr  = NULL;
	ATSendStr = AT QICLOSE"=0" NEWLINE;
	len_string = sizeof(AT QICLOSE"=0" NEWLINE) - 1;
	
	try_num = 3;
	NBTask[_AT_TCP_CLOSE].set(NULL);

		while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_TCP_CLOSE]) == NB_CMD_SUCC )
		{
			nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
			nb_cmd_status = NB_CMD_FAIL;
	}		
	return nb_cmd_status;
}
	
NB_TaskStatus nb_TCP_close_set(const char* param)
{
	user_main_debug("NBTask[_AT_TCP_CLOSE].ATSendStr:%s",ATSendStr);
	
	return nb_cmd_status;
}

NB_TaskStatus nb_TCP_uri_run(const char* param)
{
	user_main_debug("uri:%s",nb.usart.data);
//	if(strstr((char*)nb.usart.data,"QIOPEN:") != NULL && strstr((char*)nb.usart.data,"0,0") != NULL)
//	{
//		NBTask[_AT_TCP_URI].nb_cmd_status = NB_OPEN_SUCC;	
//	}
//Judgment issued and received 
	if(strstr((char*)nb.usart.data,"QIURC: \"recv\"") != NULL)
	{
		nb_TCP_read_run(NULL);
		nb_cmd_status = NB_STA_SUCC;
	}
	else
	{
		nb_cmd_status = NB_STA_SUCC;	
	}
	return nb_cmd_status;
}
