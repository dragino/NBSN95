#include "nb_mqtt.h"

static char buff[200]={0};
static char downlink_data[50]={0};
extern uint8_t dns_id_flags;
extern uint8_t 	bc35tobc95_flags;
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
			HAL_Delay(100);
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
	
	if(nb_at_send(&NBTask[_AT_MQTT_OPEN]) == NB_CMD_SUCC)
		NBTask[_AT_MQTT_OPEN].nb_cmd_status = NB_CMD_SUCC;
	else
		NBTask[_AT_MQTT_OPEN].nb_cmd_status = NB_CMD_FAIL;
	
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
	
	user_main_debug("openPayload:%s",NBTask[_AT_MQTT_OPEN].ATSendStr);
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
		
	if(nb_at_send(&NBTask[_AT_MQTT_CONN]) == NB_CMD_SUCC)
		NBTask[_AT_MQTT_CONN].nb_cmd_status = NB_CMD_SUCC;
	else
		NBTask[_AT_MQTT_CONN].nb_cmd_status = NB_CMD_FAIL;
	
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
	NBTask[_AT_MQTT_CONN].len_string = strlen(NBTask[_AT_MQTT_CONN].ATSendStr);
	user_main_debug("connPayload:%s",NBTask[_AT_MQTT_CONN].ATSendStr);
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

	if(nb_at_send(&NBTask[_AT_MQTT_SUB]) == NB_CMD_SUCC)
		NBTask[_AT_MQTT_SUB].nb_cmd_status = NB_CMD_SUCC;
	else
		NBTask[_AT_MQTT_SUB].nb_cmd_status = NB_CMD_FAIL;
	
	return NBTask[_AT_MQTT_SUB].nb_cmd_status;
}

NB_TaskStatus nb_MQTT_sub_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QMTSUB"=0,1,\"");
	strcat(buff,(char*)user.subtopic);
	strcat(buff,(char*)"\",2\n");
	
	NBTask[_AT_MQTT_SUB].ATSendStr  = buff;
	NBTask[_AT_MQTT_SUB].len_string = strlen(NBTask[_AT_MQTT_SUB].ATSendStr);
	user_main_debug("NBTask[_AT_MQTT_SUB].ATSendStr:%s",NBTask[_AT_MQTT_SUB].ATSendStr);
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

	if(nb_at_send(&NBTask[_AT_MQTT_PUB]) == NB_CMD_SUCC)
	{
		NBTask[_AT_MQTT_PUB].nb_cmd_status = NB_CMD_SUCC;
	}
	else
		NBTask[_AT_MQTT_PUB].nb_cmd_status = NB_CMD_FAIL;
	
	return NBTask[_AT_MQTT_PUB].nb_cmd_status;
}

NB_TaskStatus nb_MQTT_pub_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QMTPUB"=0,1,2,0,\"");
	strcat(buff,(char*)user.pubtopic);
	strcat(buff,(char*)"\"\n");
	
	NBTask[_AT_MQTT_PUB].ATSendStr  = buff;
	NBTask[_AT_MQTT_PUB].len_string = strlen(NBTask[_AT_MQTT_PUB].ATSendStr);
	user_main_debug("NBTask[_AT_MQTT_PUB].ATSendStr:%s",NBTask[_AT_MQTT_PUB].ATSendStr);
	return NBTask[_AT_MQTT_PUB].nb_cmd_status;
}

/**
	* @brief  Send data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_send_run(const char* param)
{
	NBTask[_AT_MQTT_SEND].try_num = 4;
	NBTask[_AT_MQTT_SEND].set(NULL);

	if(nb_at_send(&NBTask[_AT_MQTT_SEND]) == NB_CMD_SUCC)
	{
		NBTask[_AT_MQTT_SEND].nb_cmd_status = NB_CMD_SUCC;
	}
	else
		NBTask[_AT_MQTT_SEND].nb_cmd_status = NB_CMD_FAIL;
	
	return NBTask[_AT_MQTT_SEND].nb_cmd_status;
}

NB_TaskStatus nb_MQTT_send_set(const char* param)
{	
	NBTask[_AT_MQTT_SEND].ATSendStr  = sensor.data;
	NBTask[_AT_MQTT_SEND].len_string = sensor.data_len;
	user_main_debug("NBTask[_AT_MQTT_SEND].ATSendStr:%s",NBTask[_AT_MQTT_SEND].ATSendStr);
	return NBTask[_AT_MQTT_SEND].nb_cmd_status;
}

/**
	* @brief  Read MQTT sublish topic data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_data_read_run(const char* param)
{
	return NBTask[_AT_MQTT_READ].nb_cmd_status;
}

NB_TaskStatus nb_MQTT_data_read_set(const char* param)
{
	memset(downlink_data,0,20);		
	char* pos_start  = strrchr((char*)nb.usart.data,'"');	user_main_debug("pos_start:%p",&pos_start);
	char* pos_end    = strchr(pos_start,'\n');			user_main_debug("pos_end:%p",&pos_end);		
	if(bc35tobc95_flags==0)
	memcpy(downlink_data,&nb.usart.data[pos_start-((char*)nb.usart.data)+2],(pos_end-pos_start)-1);	
	else if(bc35tobc95_flags==1)
	memcpy(downlink_data,&nb.usart.data[pos_start-((char*)nb.usart.data)+4],(pos_end-pos_start)-1);	
	user_main_printf("Received downlink data:%s",downlink_data);
	rxPayLoadDeal(downlink_data);
	
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
	
	if(nb_at_send(&NBTask[_AT_MQTT_CLOSE]) == NB_CMD_SUCC)
	{
		NBTask[_AT_MQTT_CLOSE].nb_cmd_status = NB_CMD_SUCC;
	}
	else
		NBTask[_AT_MQTT_CLOSE].nb_cmd_status = NB_CMD_FAIL;
	
	return NBTask[_AT_MQTT_CLOSE].nb_cmd_status;
}

NB_TaskStatus nb_MQTT_close_set(const char* param)
{
	user_main_debug("NBTask[_AT_MQTT_CLOSE].ATSendStr:%s",NBTask[_AT_MQTT_CLOSE].ATSendStr);

	return NBTask[_AT_MQTT_CLOSE].nb_cmd_status;
}

/**
	* @brief  MQTT URI:Scheduling tasks via URI 
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_uri_run(const char* param)
{
	user_main_debug("uri:%s",nb.usart.data);
	if(strstr((char*)nb.usart.data,QMTOPEN": 0,0") != NULL)
	{
		NBTask[_AT_MQTT_URI].nb_cmd_status = NB_OPEN_SUCC;	
	}
	else if(strstr((char*)nb.usart.data,QMTCONN": 0,0,0") != NULL)
	{
		NBTask[_AT_MQTT_URI].nb_cmd_status = NB_CONN_SUCC;	
	}
	else if(strstr((char*)nb.usart.data,QMTSUB": 0,1,0,2") != NULL)
	{
		NBTask[_AT_MQTT_URI].nb_cmd_status = NB_SUB_SUCC;	
	}
	else if(strstr((char*)nb.usart.data,QMTPUB": 0,1,0") != NULL)
	{
		NBTask[_AT_MQTT_URI].nb_cmd_status = NB_PUB_SUCC;
	}
	else if(strstr((char*)nb.usart.data,QMTDISC": 0,0") != NULL)
	{
		NBTask[_AT_MQTT_URI].nb_cmd_status = NB_CLOSE_SUCC;
	}
	else
		NBTask[_AT_MQTT_URI].nb_cmd_status = NB_OTHER;
	
//Judgment issued and received 
	if(strstr((char*)nb.usart.data,QMTRECV) != NULL)
	{
		nb_MQTT_data_read_set(NULL);
	}
//Ask if the process has failed 
	if(strstr((char*)nb.usart.data,QMTSTAT) != NULL)
	{
		NBTask[_AT_MQTT_URI].nb_cmd_status = NB_ERROR;
		if(dns_id_flags==1)
		{
		 if((strstr((char*)nb.usart.data,QMTCONN": 0,2") != NULL)&&(strstr((char*)nb.usart.data,QMTSTAT": 0,3") != NULL))
	    {
	      NBTask[_AT_MQTT_URI].nb_cmd_status = NB_STA_SUCC;	
	    }
	  }
	}
	return NBTask[_AT_MQTT_URI].nb_cmd_status;
}
