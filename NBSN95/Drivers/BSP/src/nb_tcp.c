#include "nb_tcp.h"

static char buff[200]={0};
static char downlink_data[20]={0};

/**
	* @brief  Open TCP port operation
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_TCP_open_run(const char* param)
{
	NBTask[_AT_TCP_OPEN].try_num = 4;
	NBTask[_AT_TCP_OPEN].set(param);
	
	while(NBTask[_AT_TCP_OPEN].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_TCP_OPEN]) == NB_CMD_SUCC )
		{
			NBTask[_AT_TCP_OPEN].nb_cmd_status = NBTask[_AT_TCP_OPEN].get(param);
			break;
		}
		else
			HAL_Delay(500);
	}
	
	return NBTask[_AT_TCP_OPEN].nb_cmd_status;
}
NB_TaskStatus nb_TCP_open_set(const char* param)
{
	user_main_debug("NBTask[_AT_TCP_OPEN].ATSendStr:%s",NBTask[_AT_TCP_OPEN].ATSendStr);
	return NBTask[_AT_TCP_OPEN].nb_cmd_status;
}

NB_TaskStatus nb_TCP_open_get(const char* param)
{
	char *pch = strchr((char*)nb.usart.data,'O');
	if(pch == NULL)
		NBTask[_AT_TCP_OPEN].nb_cmd_status = NB_OPEN_FAIL;
	else
	{
		nb.socket = nb.usart.data[(pch - ((char*)nb.usart.data))-5];
		user_main_debug("nb.socket:%c\r\n",nb.socket);
		NBTask[_AT_TCP_OPEN].nb_cmd_status = NB_OPEN_SUCC;
	}
	return NBTask[_AT_TCP_OPEN].nb_cmd_status;
}

/**
	* @brief  Connect to TCP server
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_TCP_conn_run(const char* param)
{
	NBTask[_AT_TCP_CONN].try_num = 4;
	NBTask[_AT_TCP_CONN].set(param);
	while(NBTask[_AT_TCP_CONN].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_TCP_CONN]) == NB_CMD_SUCC )
		{
			NBTask[_AT_TCP_CONN].nb_cmd_status = NB_CONN_SUCC;
			break;
		}
		else
		{
			NBTask[_AT_TCP_CONN].nb_cmd_status = NB_CONN_FAIL;
			HAL_Delay(500);
		}
	}
	
	return NBTask[_AT_TCP_CONN].nb_cmd_status;
}

NB_TaskStatus nb_TCP_conn_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT NSOCO "=");
	sprintf(buff+strlen(buff), "%c", nb.socket);
	strcat(buff,",");
	strcat(buff,(char*)user.add);
	strcat(buff,"\n");
	
	NBTask[_AT_TCP_CONN].ATSendStr = buff;
	NBTask[_AT_TCP_CONN].len_string = strlen(NBTask[_AT_TCP_CONN].ATSendStr);
	
	user_main_debug("NBTask[_AT_TCP_CONN].ATSendStr:%s",NBTask[_AT_TCP_CONN].ATSendStr);
	
	return NBTask[_AT_TCP_CONN].nb_cmd_status;
}
NB_TaskStatus nb_TCP_conn_get(const char* param)
{
	return NBTask[_AT_TCP_CONN].nb_cmd_status;
}
/**
	* @brief  Send TCP data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_TCP_send_run(const char* param)
{
	NBTask[_AT_TCP_SEND].try_num = 2;
	NBTask[_AT_TCP_SEND].set(param);
	while(NBTask[_AT_TCP_SEND].try_num--)
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
	return NBTask[_AT_TCP_SEND].nb_cmd_status;
}

NB_TaskStatus nb_TCP_send_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT NSOSD "=");
	sprintf(buff+strlen(buff), "%c", nb.socket);
	strcat(buff,",");
	sprintf(buff+strlen(buff), "%d", sensor.data_len);
	strcat(buff,",");
	strcat(buff,sensor.data);
	strcat(buff,",0x100,100\n");
	
	NBTask[_AT_TCP_SEND].ATSendStr = buff;
	NBTask[_AT_TCP_SEND].len_string = strlen(NBTask[_AT_TCP_SEND].ATSendStr);
	
	user_main_debug("NBTask[_AT_TCP_SEND].ATSendStr:%s",NBTask[_AT_TCP_SEND].ATSendStr);
	return NBTask[_AT_TCP_SEND].nb_cmd_status;
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
	
	return NBTask[_AT_TCP_READ].nb_cmd_status;
}

NB_TaskStatus nb_TCP_read_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT NSORF "=");
	if(nb.socket>='0' && nb.socket<='7')
		sprintf(buff+strlen(buff), "%c", nb.socket);
	else
		strcat(buff,"1");
	strcat(buff,",50\n");
	
	NBTask[_AT_TCP_READ].ATSendStr = buff;
	NBTask[_AT_TCP_READ].len_string = strlen(NBTask[_AT_TCP_READ].ATSendStr);
	user_main_debug("NBTask[_AT_TCP_READ].ATSendStr:%s",NBTask[_AT_TCP_READ].ATSendStr);
	
	return NBTask[_AT_TCP_READ].nb_cmd_status;
}
	
NB_TaskStatus nb_TCP_read_get(const char* param)
{
	char *pch = strchr((char*)nb.usart.data,','); 
	if(pch == NULL)
	{
		NBTask[_AT_TCP_READ].nb_cmd_status = NB_READ_NODATA;
	}
	else
	{
		memset(downlink_data,0,sizeof(downlink_data));		
		char* end = strstr((char*)nb.usart.data,(char*)user.add);
		char* start  = NULL;
		for(int i=0;i<4;i++)
		{
			start = end;
			end = strchr(end,',');
			end++;
		}
		memcpy(downlink_data,nb.usart.data +(start - (char*)nb.usart.data),(end-start)-1);		
		user_main_printf("Received downlink data:%s",downlink_data);
		
		NBTask[_AT_TCP_READ].nb_cmd_status = NB_READ_DATA;
	}
	return NBTask[_AT_TCP_READ].nb_cmd_status;
}

/**
	* @brief  Close TCP port
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_TCP_close_run(const char* param)
{
	NBTask[_AT_TCP_CLOSE].try_num = 2;
	NBTask[_AT_TCP_CLOSE].set(param);
	while(NBTask[_AT_TCP_CLOSE].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_TCP_CLOSE]) == NB_CMD_SUCC )
		{
			NBTask[_AT_TCP_CLOSE].nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
			HAL_Delay(500);
	}
	return NBTask[_AT_TCP_CLOSE].nb_cmd_status;
}
	
NB_TaskStatus nb_TCP_close_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT NSOCL "=");
	if(nb.socket>='0' && nb.socket<='7')
		sprintf(buff+strlen(buff), "%c", nb.socket);
	else
		strcat(buff,"1");
	strcat(buff,"\n");	
	
	NBTask[_AT_TCP_CLOSE].ATSendStr  = buff;
	NBTask[_AT_TCP_CLOSE].len_string = strlen(NBTask[_AT_TCP_CLOSE].ATSendStr);
	user_main_debug("NBTask[_AT_TCP_CLOSE].ATSendStr:%s",NBTask[_AT_TCP_CLOSE].ATSendStr);
	
	return NBTask[_AT_TCP_CLOSE].nb_cmd_status;
}

NB_TaskStatus nb_TCP_close_get(const char* param)
{
	return NBTask[_AT_TCP_CLOSE].nb_cmd_status;
}

NB_TaskStatus nb_TCP_uri_run(const char* param)
{
	user_main_debug("uri:%s",nb.usart.data);
	if(strstr((char*)nb.usart.data,NSOSTR) != NULL && strstr((char*)nb.usart.data,",100,1") != NULL)
	{
		NBTask[_AT_TCP_URI].nb_cmd_status = NB_SEND_SUCC;	
	}
	else
		NBTask[_AT_UDP_URI].nb_cmd_status = NB_OTHER;
//Judgment issued and received 
	if(strstr((char*)nb.usart.data,NSONMI) != NULL)
	{
		nb_TCP_read_run(NULL);
	}
//Ask if the process has failed
	if(strstr((char*)nb.usart.data,NSOCLI) != NULL)
	{
		NBTask[_AT_TCP_URI].nb_cmd_status = NB_ERROR;
	}
	return NBTask[_AT_TCP_URI].nb_cmd_status;
}
