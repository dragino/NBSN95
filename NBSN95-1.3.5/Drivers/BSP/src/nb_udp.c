#include "nb_udp.h"

static char buff[200]={0};
static char downlink_data[50]={0};
extern uint8_t dns_id_flags;
uint8_t downlink_twice_flags=0;
/**
	* @brief  Open UDP port operation
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_UDP_open_run(const char* param)
{
	NBTask[_AT_UDP_OPEN].try_num = 4;
	NBTask[_AT_UDP_OPEN].set(param);
	
	while(NBTask[_AT_UDP_OPEN].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_UDP_OPEN]) == NB_CMD_SUCC )
		{
			NBTask[_AT_UDP_OPEN].nb_cmd_status = NBTask[_AT_UDP_OPEN].get(param);
			break;
		}
		else
			HAL_Delay(500);
	}
	
	return NBTask[_AT_UDP_OPEN].nb_cmd_status;
}

NB_TaskStatus nb_UDP_open_set(const char* param)
{	
	user_main_debug("NBTask[_AT_UDP_OPEN].ATSendStr:%s",NBTask[_AT_UDP_OPEN].ATSendStr);
	
	return NBTask[_AT_UDP_OPEN].nb_cmd_status;	
}

NB_TaskStatus nb_UDP_open_get(const char* param)
{
	char *pch = strchr((char*)nb.usart.data,'O');
	if(pch == NULL)
		NBTask[_AT_UDP_OPEN].nb_cmd_status = NB_CMD_FAIL;
	else
	{
		nb.socket = nb.usart.data[(pch-(char*)nb.usart.data)-5];
		user_main_debug("nb.socket:%c\r\n",nb.socket);
		NBTask[_AT_UDP_OPEN].nb_cmd_status = NB_OPEN_SUCC;
	}
	
	return NBTask[_AT_UDP_OPEN].nb_cmd_status;	
}

/**
	* @brief  Send UDP data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_UDP_send_run(const char* param)
{
	NBTask[_AT_UDP_SEND].try_num = 2;
	NBTask[_AT_UDP_SEND].set(param);
	while(NBTask[_AT_UDP_SEND].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_UDP_SEND]) == NB_CMD_SUCC )
		{
			break;
		}
		else
		{
			HAL_Delay(500);
		}
	}
	return NBTask[_AT_UDP_SEND].nb_cmd_status;
}

NB_TaskStatus nb_UDP_send_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT NSOSTF "=");
	if(nb.socket>='0' && nb.socket<='7')
		sprintf(buff+strlen(buff), "%c", nb.socket);
	else
		strcat(buff,"1");
	
	strcat(buff,",");
	if(strlen((char*)user.add_ip)!=0)
		strcat(buff,(char*)user.add_ip);
	else
		strcat(buff,(char*)user.add);
	strcat(buff,",");
	if(sys.cfm == '0')
		strcat(buff,"0x200,");
	else if(sys.cfm == '1')
		strcat(buff,"0x400,");
	else 
		strcat(buff,"0,");
	sprintf(buff+strlen(buff), "%d", sensor.data_len);
	strcat(buff,",");
	strcat(buff,sensor.data);
	strcat(buff,",100\n");
	
	NBTask[_AT_UDP_SEND].ATSendStr = buff;
	NBTask[_AT_UDP_SEND].len_string = strlen(NBTask[_AT_UDP_SEND].ATSendStr);
	
	user_main_debug("NBTask[_AT_UDP_SEND].ATSendStr:%s",NBTask[_AT_UDP_SEND].ATSendStr);
	return NBTask[_AT_UDP_SEND].nb_cmd_status;
}

/**
	* @brief  Read UDP data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_UDP_read_run(const char* param)
{
	NBTask[_AT_UDP_READ].set(param);
	if(nb_at_send(&NBTask[_AT_UDP_READ]) == NB_CMD_SUCC)
	{
		if(NBTask[_AT_UDP_READ].get(param) == NB_READ_DATA)
		{			
			rxPayLoadDeal(downlink_data);
		}
	}
	return NBTask[_AT_UDP_READ].nb_cmd_status;
}

NB_TaskStatus nb_UDP_read_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT NSORF "=");
	if(nb.socket>='0' && nb.socket<='7')
		sprintf(buff+strlen(buff), "%c", nb.socket);
	else
		strcat(buff,"1");
	strcat(buff,",50\n");

	
	NBTask[_AT_UDP_READ].ATSendStr = buff;
	NBTask[_AT_UDP_READ].len_string = strlen(NBTask[_AT_UDP_READ].ATSendStr);
	user_main_debug("NBTask[_AT_UDP_READ].ATSendStr:%s",NBTask[_AT_UDP_READ].ATSendStr);
	return NBTask[_AT_UDP_READ].nb_cmd_status;
}

NB_TaskStatus nb_UDP_read_get(const char* param)
{
	char *pch = strrchr((char*)nb.usart.data,','); 
	if(pch == NULL)
		NBTask[_AT_UDP_READ].nb_cmd_status = NB_READ_NODATA;
	else
	{
		memset(downlink_data,0,sizeof(downlink_data));
		char*	end    = NULL;
		char* start  = NULL;
		if(strlen((char*)user.add_ip)!=0)
			end = strstr((char*)nb.usart.data,(char*)user.add_ip);
		else
			end = strstr((char*)nb.usart.data,(char*)user.add);
		
		for(int i=0;i<4;i++)
		{			
			start = end;
			end = strchr(end,',');
			end++;
		}	
		memcpy(downlink_data,nb.usart.data +(start - (char*)nb.usart.data),(end-start)-1);
		user_main_printf("Received downlink data:%s",downlink_data);
		
		NBTask[_AT_UDP_READ].nb_cmd_status = NB_READ_DATA;
	}
	return NBTask[_AT_UDP_READ].nb_cmd_status;
}
/**
	* @brief  Close UDP port
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_UDP_close_run(const char* param)
{
	NBTask[_AT_UDP_CLOSE].try_num = 2;
	NBTask[_AT_UDP_CLOSE].set(param);
	while(NBTask[_AT_UDP_CLOSE].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_UDP_CLOSE]) == NB_CMD_SUCC )
		{
			NBTask[_AT_UDP_CLOSE].nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
			HAL_Delay(500);
	}
	return NBTask[_AT_UDP_CLOSE].nb_cmd_status;
}

NB_TaskStatus nb_UDP_close_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT NSOCL "=");
	if(nb.socket>='0' && nb.socket<='7')
		sprintf(buff+strlen(buff), "%c", nb.socket);
	else
		strcat(buff,"1");
	strcat(buff,"\n");
		
	NBTask[_AT_UDP_CLOSE].ATSendStr  = buff;
	NBTask[_AT_UDP_CLOSE].len_string = strlen(NBTask[_AT_UDP_CLOSE].ATSendStr);
	user_main_debug("NBTask[_AT_UDP_CLOSE].ATSendStr:%s",NBTask[_AT_UDP_CLOSE].ATSendStr);
	
	return NBTask[_AT_UDP_CLOSE].nb_cmd_status;
}

NB_TaskStatus nb_UDP_close_get(const char* param)
{
	return NBTask[_AT_UDP_CLOSE].nb_cmd_status;
}

/**
	* @brief  UDP URI:Scheduling tasks via URI 
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_UDP_uri_run(const char* param)
{
	user_main_debug("uri:%s",nb.usart.data);
		
	if(strstr((char*)nb.usart.data,NSOSTR) != NULL && strstr((char*)nb.usart.data,"100,1") != NULL)
	{
		NBTask[_AT_UDP_URI].nb_cmd_status = NB_SEND_SUCC;	
	}
	else
		NBTask[_AT_UDP_URI].nb_cmd_status = NB_OTHER;
//Judgment issued and received 
	if(strstr((char*)nb.usart.data,NSONMI) != NULL)
	{
		nb_UDP_read_run(NULL);
		if(downlink_twice_flags==1)
		{
		NBTask[_AT_UDP_URI].nb_cmd_status = NB_SEND_SUCC;
		}
		downlink_twice_flags++;
	}
	
	return NBTask[_AT_UDP_URI].nb_cmd_status;
}
