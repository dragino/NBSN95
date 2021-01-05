#include "nb_mqtt.h"

static char buff[200]={0};
static char downlink_data[20]={0};

/**
	* @brief  Set MQTT configuration parameters
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_config_set(const char* param)
{
	NBTask[_AT_MQTT_Config].try_num = 4;
	while(NBTask[_AT_MQTT_Config].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_MQTT_Config])== NB_CMD_SUCC)
		{
			break;
		}
		else
			HAL_Delay(500);
	}
	
	return NBTask[_AT_MQTT_Config].nb_cmd_status;
}

/**
	* @brief  Open MQTT port
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_open_run(const char* param)
{	
	NBTask[_AT_MQTT_OPEN].try_num = 4;
	NBTask[_AT_MQTT_OPEN].set(param);
	user_main_debug("openPayload:%s",NBTask[_AT_MQTT_OPEN].ATSendStr);
	
	while(NBTask[_AT_MQTT_OPEN].try_num--)
	{
		nb_at_send(&NBTask[_AT_MQTT_OPEN]);
		if(NBTask[_AT_MQTT_OPEN].get(param) == NB_OPEN_SUCC)
				break;
	}
	
	return NBTask[_AT_MQTT_OPEN].nb_cmd_status;
}

NB_TaskStatus nb_MQTT_open_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	char* pos = strchr((char*)user.add,',');
	
	strcat(buff,AT QMTOPEN "=0,\"");
	memcpy(buff+strlen(buff),user.add,(pos-(char*)user.add));
	strcat(buff,"\"");
	strcat(buff,(char*)&user.add[(pos-(char*)user.add)]);
	strcat(buff,"\n");
	
	NBTask[_AT_MQTT_OPEN].ATSendStr = buff;
	NBTask[_AT_MQTT_OPEN].len_string = strlen(NBTask[_AT_MQTT_OPEN].ATSendStr);
	
	
	return NBTask[_AT_MQTT_OPEN].nb_cmd_status;	
}

NB_TaskStatus nb_MQTT_open_get(const char* param)
{
	uint32_t time = HAL_GetTick();
	while(HAL_GetTick() - time < 2000 && nb.recieve_flag != NB_RECIEVE)
	{
		user_main_info("...");
	}
	
	nb.recieve_flag = NB_IDIE;	
	user_main_info("recieve_data_server:%s",nb.recieve_data_server);
	
	if(strstr(nb.recieve_data_server,QMTOPEN": 0,0") != NULL || strstr(nb.recieve_data,QMTOPEN": 0,0") != NULL)
	{
		NBTask[_AT_MQTT_OPEN].nb_cmd_status = NB_OPEN_SUCC;	
	}
	else
		NBTask[_AT_MQTT_OPEN].nb_cmd_status = NB_OPEN_FAIL;
	
	return NBTask[_AT_MQTT_OPEN].nb_cmd_status;
}

/**
	* @brief  connect to the server
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_conn_run(const char* param)
{
	NBTask[_AT_MQTT_CONN].try_num = 4;
	NBTask[_AT_MQTT_CONN].set(param);
	
	while(NBTask[_AT_MQTT_CONN].try_num--)
	{
		nb_at_send(&NBTask[_AT_MQTT_CONN]);
		if(NBTask[_AT_MQTT_CONN].get(param) == NB_CONN_SUCC)
			break;
		else
			user_main_printf("Server ACK not received...");
	}
	
	return NBTask[_AT_MQTT_CONN].nb_cmd_status;
}

NB_TaskStatus nb_MQTT_conn_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QMTCONN"=0,\""); 
	strcat(buff,(char*)user.client);
	if(strstr((char*)user.uname,"NULL") ==NULL)
	{
		strcat(buff,"\",\"");
		strcat(buff,(char*)user.uname);
		if(strstr((char*)user.pwd,"NULL") ==NULL)
		{
			strcat(buff,"\",\"");
			strcat(buff,(char*)user.pwd);
		}
	}
	strcat(buff,"\"\n");
	
	NBTask[_AT_MQTT_CONN].ATSendStr = buff;
	NBTask[_AT_MQTT_CONN].len_string = strlen(NBTask[_AT_MQTT_OPEN].ATSendStr);
	user_main_debug("connPayload:%s",NBTask[_AT_MQTT_CONN].ATSendStr);
	return NBTask[_AT_MQTT_CONN].nb_cmd_status;
}

NB_TaskStatus nb_MQTT_conn_get(const char* param)
{	
	uint32_t time = HAL_GetTick();
	while(HAL_GetTick() - time < 2000 && nb.recieve_flag != NB_RECIEVE )
	{
		user_main_info("...");
	}
	
	nb.recieve_flag = NB_IDIE;	
	user_main_info("recieve_data_server:%s",nb.recieve_data_server);
	
	if(strstr(nb.recieve_data_server,QMTCONN": 0,0,0") != NULL || strstr(nb.recieve_data,QMTCONN": 0,0,0") != NULL)
	{
		NBTask[_AT_MQTT_CONN].nb_cmd_status = NB_CONN_SUCC;	
	}
	else
		NBTask[_AT_MQTT_CONN].nb_cmd_status = NB_CONN_FAIL;
	
	return NBTask[_AT_MQTT_CONN].nb_cmd_status;
}

/**
	* @brief  Subscribe to MQTT topics
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_sub_run(const char* param)
{
	NBTask[_AT_MQTT_SUB].try_num = 4;
	NBTask[_AT_MQTT_SUB].set(NULL);

	while(NBTask[_AT_MQTT_SUB].try_num--)
	{
		nb_at_send(&NBTask[_AT_MQTT_SUB]);
		if( NBTask[_AT_MQTT_SUB].get(param) == NB_SEND_SUCC)
			break;
		else
			user_main_printf("SUB ACK not received...");
	}

	return NBTask[_AT_MQTT_SUB].nb_cmd_status;
}

NB_TaskStatus nb_MQTT_sub_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QMTSUB"=0,1,\"");
	strcat(buff,(char*)user.subtopic);
	strcat(buff,(char*)"\",1\n");
	
	NBTask[_AT_MQTT_SUB].ATSendStr  = buff;
	NBTask[_AT_MQTT_SUB].len_string = strlen(NBTask[_AT_MQTT_SUB].ATSendStr);
	user_main_debug("NBTask[_AT_MQTT_SUB].ATSendStr:%s",NBTask[_AT_MQTT_SUB].ATSendStr);
	return NBTask[_AT_MQTT_SUB].nb_cmd_status;
}

NB_TaskStatus nb_MQTT_sub_get(const char* param)
{
	uint32_t time = HAL_GetTick();
	while(HAL_GetTick() - time < 2000 && nb.recieve_flag != NB_RECIEVE )
	{
		user_main_info("...");
	}

	nb.recieve_flag = NB_IDIE;	
	user_main_info("recieve_data_server:%s",nb.recieve_data_server);
	
	if(strstr(nb.recieve_data_server,QMTSUB": 0,1,0,1") != NULL || strstr(nb.recieve_data,QMTSUB": 0,1,0,1") != NULL )
	{
		NBTask[_AT_MQTT_SUB].nb_cmd_status = NB_SEND_SUCC;	
	}
	else
		NBTask[_AT_MQTT_SUB].nb_cmd_status = NB_SEND_FAIL;
	
	return NBTask[_AT_MQTT_SUB].nb_cmd_status;
}

/**
	* @brief  Send MQTT publish topic data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_pub_run(const char* param)
{
	NBTask[_AT_MQTT_PUB].try_num = 4;
	NBTask[_AT_MQTT_PUB].set(NULL);

	while(NBTask[_AT_MQTT_PUB].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_MQTT_PUB]) == NB_CMD_SUCC)
		{
			HAL_UART_Transmit_DMA(&hlpuart1,(uint8_t*)sensor.data,sensor.data_len);
			if( NBTask[_AT_MQTT_PUB].get(param) == NB_SEND_SUCC)
				break;
			else
				user_main_printf("Send ACK not received...");
		}
		else
		{
			HAL_Delay(500);
		}
	}
	
	return NBTask[_AT_MQTT_PUB].nb_cmd_status;
}

NB_TaskStatus nb_MQTT_pub_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QMTPUB"=0,0,0,0,\"");
	strcat(buff,(char*)user.pubtopic);
	strcat(buff,(char*)"\"\n");
	
	NBTask[_AT_MQTT_PUB].ATSendStr  = buff;
	NBTask[_AT_MQTT_PUB].len_string = strlen(NBTask[_AT_MQTT_PUB].ATSendStr);
	user_main_debug("NBTask[_AT_MQTT_PUB].ATSendStr:%s",NBTask[_AT_MQTT_PUB].ATSendStr);
	return NBTask[_AT_MQTT_PUB].nb_cmd_status;
}

NB_TaskStatus nb_MQTT_pub_get(const char* param)
{
	uint32_t time = HAL_GetTick();
	while(HAL_GetTick() - time < 2000 && nb.recieve_flag != NB_RECIEVE )
	{
		user_main_info("...");
	}

	nb.recieve_flag = NB_IDIE;	
	user_main_info("recieve_data_server:%s",nb.recieve_data_server);
	
	if(strstr(nb.recieve_data_server,QMTPUB": 0,0,0") != NULL || strstr(nb.recieve_data,QMTPUB": 0,0,0") != NULL )
	{
		NBTask[_AT_MQTT_PUB].nb_cmd_status = NB_SEND_SUCC;	
	}
	else
		NBTask[_AT_MQTT_PUB].nb_cmd_status = NB_SEND_FAIL;
	
	return NBTask[_AT_MQTT_PUB].nb_cmd_status;
}

/**
	* @brief  Read MQTT sublish topic data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_data_read_run(const char* param)
{
	if(nb_MQTT_data_read_get(param) == NB_READ_DATA)
	{
		nb_MQTT_data_read_set(param);
		NBTask[_AT_MQTT_READ].nb_cmd_status = NB_READ_DATA;
	}
	else
	{
		NBTask[_AT_MQTT_READ].nb_cmd_status = NB_READ_NODATA;
	}
	return NBTask[_AT_MQTT_READ].nb_cmd_status;
}

NB_TaskStatus nb_MQTT_data_read_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	memset(downlink_data,0,20);
	memcpy(buff,nb.recieve_data_server,strlen(nb.recieve_data_server));
	
	user_main_debug("rec_buff:%s",buff);
	
	char* pos_start  = strrchr(buff,'"');	 		user_main_debug("pos_start:%p",&pos_start);
	char* pos_end    = strchr(pos_start,'\n');user_main_debug("pos_end:%p",&pos_end);
	
	memcpy(downlink_data,&buff[pos_start-buff+2],(pos_end-pos_start)-1);	
	user_main_printf("Received downlink data:%s",downlink_data);
	rxPayLoadDeal(downlink_data);
	
	return NBTask[_AT_MQTT_READ].nb_cmd_status;
}

NB_TaskStatus nb_MQTT_data_read_get(const char* param)
{
	if(strstr(nb.recieve_data_server,QMTRECV) != NULL)
	{
		NBTask[_AT_MQTT_READ].nb_cmd_status = NB_READ_DATA;
	}
	else
	{
		NBTask[_AT_MQTT_READ].nb_cmd_status = NB_READ_NODATA;
	}
	
	return NBTask[_AT_MQTT_READ].nb_cmd_status;
}

/**
	* @brief  Close MQTT port
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_close_run(const char* param)
{
	NBTask[_AT_MQTT_CLOSE].try_num = 2;
	NBTask[_AT_MQTT_CLOSE].set(NULL);
	
	while(NBTask[_AT_MQTT_CLOSE].try_num--)
	{
		nb_at_send(&NBTask[_AT_MQTT_CLOSE]);
		if( NBTask[_AT_MQTT_CLOSE].get(param) == NB_CLOSE_SUCC)
			break;
		else
			user_main_printf("Close ACK not received...");
	}
	
	return NBTask[_AT_MQTT_CLOSE].nb_cmd_status;
}

NB_TaskStatus nb_MQTT_close_set(const char* param)
{
	user_main_debug("NBTask[_AT_MQTT_CLOSE].ATSendStr:%s",NBTask[_AT_MQTT_CLOSE].ATSendStr);

	return NBTask[_AT_MQTT_CLOSE].nb_cmd_status;
}

NB_TaskStatus nb_MQTT_close_get(const char* param)
{
	uint32_t time = HAL_GetTick();
	while(HAL_GetTick() - time < 2000 && nb.recieve_flag != NB_RECIEVE )
	{
		user_main_info("...");
	}

	nb.recieve_flag = NB_IDIE;	
	user_main_info("recieve_data_server:%s",nb.recieve_data_server);
	
	if(strstr(nb.recieve_data_server,QMTDISC": 0,0") != NULL || strstr(nb.recieve_data,QMTDISC": 0,0") != NULL)
	{
		NBTask[_AT_MQTT_CLOSE].nb_cmd_status = NB_CLOSE_SUCC;	
	}
	else
		NBTask[_AT_MQTT_CLOSE].nb_cmd_status = NB_CLOSE_FAIL;
	
	
	return NBTask[_AT_MQTT_CLOSE].nb_cmd_status;
}
