#include "nbInit.h"

static uint8_t net_acc_status_led = 0;

NB nb = {.net_flag=no_status,.recieve_flag=0,.rxlen=0,.recieve_data={0},.imei={0},.imsi={0},.singal=0};

NB_TaskStatus nb_at_send(struct NBTASK *NB_Task)
{
	nb.recieve_flag = NB_BUSY;
	nb.rxlen = 0;
	if(NB_Task->cmd_num>= _AT_COAP_OPEN && NB_Task->cmd_num <= _AT_TCP_CLOSE)
		strcat(nb.recieve_data_server,nb.recieve_data);
	
	memset(nb.recieve_data,0,sizeof(nb.recieve_data));
	
	HAL_UART_Transmit_DMA(&hlpuart1,(uint8_t*)NB_Task->ATSendStr,NB_Task->len_string);
	
	uint32_t time = HAL_GetTick();
	while(HAL_GetTick() - time < NB_Task->time_out && nb.recieve_flag != NB_CMD_SUCC)
	{
		user_main_info("...");
	}
	
	nb.recieve_flag = NB_IDIE;	
	user_main_info("recieve data:%s",nb.recieve_data);
	
	if(strstr(nb.recieve_data,NB_Task->ATRecStrOK) != NULL)
		NB_Task->nb_cmd_status =  NB_CMD_SUCC;
	else if(strstr(nb.recieve_data,NB_Task->ATRecStrError) != NULL)
		NB_Task->nb_cmd_status =  NB_CMD_FAIL;
	else
		NB_Task->nb_cmd_status =  NB_ERROR;
	
	return NB_Task->nb_cmd_status;
}

/**
	* @brief  Empty function
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_null_run(const char* param)
{
	return NB_CMD_FAIL;
}

/**
	* @brief  AT :Get module response
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_at_run(const char* param)
{
	return nb_at_send(&NBTask[_AT]);
}
/**
	* @brief  Echo mode OFF
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_ate_run(const char* param)
{
	return nb_at_send(&NBTask[_AT]);
}
/**
	* @brief  AT_CGSN :Get IMEI number
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_cgsn_get(const char* param)
{
	if(nb_at_send(&NBTask[_AT_IMEI])== NB_CMD_SUCC)
	{
		memset(nb.imei,0,sizeof((nb.imei)));
		char *pch = strchr(nb.recieve_data,':');
		char *pch1 = strchr(nb.recieve_data,'O');
		strncpy(nb.imei, pch +1,pch1 - pch -5);
	}
	return NBTask[_AT_IMEI].nb_cmd_status;
}

/**
	* @brief  AT_CIMI :Get IMSI number
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_cimi_get(const char* param)
{
	if(nb_at_send(&NBTask[_AT_IMSI])== NB_CMD_SUCC)
	{
		char *pch = strchr(nb.recieve_data,'\n');
		char *pch1 = strchr(nb.recieve_data,'O');
		strncpy(nb.imsi, pch +1,pch1 - pch -5);		
	}
	return NBTask[_AT_IMSI].nb_cmd_status;
}

/**
	* @brief  AT_CFUN : get UE Functionality
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_cfun_get(const char* param)
{
	NBTask[_AT_CFUN].ATSendStr = (char*)param;
	NBTask[_AT_CFUN].len_string = strlen(NBTask[_AT_CFUN].ATSendStr) ;
	if(nb_at_send(&NBTask[_AT_CFUN])== NB_CMD_SUCC)
	{
		if(strstr(nb.recieve_data,CFUN":1") != NULL)
			NBTask[_AT_CFUN].nb_cmd_status = NB_CMD_SUCC;
		else if(strstr(nb.recieve_data,CFUN":0") != NULL)
		{
			NBTask[_AT_CFUN].nb_cmd_status = NB_CMD_OFF;
		}
	}
	return NBTask[_AT_CFUN].nb_cmd_status;
}

/**
	* @brief  AT_CFUN : get UE Functionality
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_cfun_set(const char* param)
{
//	NBTask[_AT_CFUN].ATSendStr = AT CFUN "=1" NEWLINE;
	NBTask[_AT_CFUN].ATSendStr = (char*)param;
	NBTask[_AT_CFUN].len_string = strlen(NBTask[_AT_CFUN].ATSendStr) ;

	return nb_at_send(&NBTask[_AT_CFUN]);
}

/**
	* @brief  AT_NBAND : Get frequency band
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_nband_get(const char* param)
{
	if(nb_at_send(&NBTask[_AT_NBAND])== NB_CMD_SUCC)
	{
		printf("Currently set frequency band");
		char *pch = strchr(nb.recieve_data,':');
		while(*pch != '\n')
			printf("%c",*pch++);
	}
	return NBTask[_AT_NBAND].nb_cmd_status;
}


/**
	* @brief  AT+CSQ : Get signal strength
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_csq_get(const char* param)
{
	if(nb_at_send(&NBTask[_AT_CSQ])== NB_CMD_SUCC)
	{
		char singalBuff[5]={0}; 	
		char *pch1 = strchr(nb.recieve_data,':');
		char *pch2 = strchr(nb.recieve_data,',');
		for(int i =0;i<(pch2 - pch1)-1;i++)
			sprintf(singalBuff+strlen(singalBuff), "%c", nb.recieve_data[7+i]);
		nb.singal = atoi(singalBuff);
			
		if(nb.singal == 99)
			NBTask[_AT_CSQ].nb_cmd_status = NB_CMD_OFF;
	}
	return NBTask[_AT_CSQ].nb_cmd_status;
}

/**
	* @brief  AT+NRB : Send MQTT data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_nrb_run(const char* param)
{
	return nb_at_send(&NBTask[_AT_NRB]);
}

/**
	* @brief  NB task
  * @param  Task instruction code
  * @retval ATCmdNum
  */
ATCmdNum NBTASK(uint8_t *task)
{
	UART2_DISABLE_RE();
	ATCmdNum at_state = _AT_IDLE;
	switch(*task)
	{
case _AT:
			if(NBTask[_AT].run(NULL) == NB_CMD_SUCC)
			{
				*task = _ATE;
				user_main_printf("NBIOT has responded.");
			}
			else
			{
				at_state = _AT_ERROR;			
				user_main_printf("NBIOT did not respond.");			
			}
			break;
			
case _ATE:
			if(NBTask[_ATE].run(NULL) == NB_CMD_SUCC)
			{
				*task = _AT_IMEI;
				user_main_printf("Echo mode turned off successfully.");
			}
			else
			{
				at_state = _AT_ERROR;			
				user_main_printf("Echo mode turned off failed.");			
			}
			break;

case _AT_IMEI:
			if(NBTask[_AT_IMEI].get((char*)NBTask[_AT_IMEI].cmd_num) == NB_CMD_SUCC)
			{
				*task = _AT_IMSI;
				user_main_printf("The IMEI number is:%s.",nb.imei);
				
				if(strstr((char*)user.deui,"NULL") != NULL)
				{
					memset(user.deui,0,sizeof(user.deui));
					memcpy(user.deui,nb.imei+3,12);	
				}
			}
			else 
			{
				at_state = _AT_ERROR;
				user_main_printf("Failed to read IMEI number.");
			}
			break;
			
case _AT_IMSI:
			if(NBTask[_AT_IMSI].get((char*)NBTask[_AT_IMSI].cmd_num) == NB_CMD_SUCC)
			{
				*task=_AT_CFUN;
				user_main_printf("The IMSI number is:%s.",nb.imsi);
			}
			else 
			{
				at_state = _AT_ERROR;
				user_main_printf("Failed to read IMSI number.");
			}
			break;
			
case _AT_CFUN:
			if(NBTask[_AT_CFUN].get(AT CFUN "?" NEWLINE) == NB_CMD_SUCC)
			{
				*task=_AT_NBAND;
			}
			else 
			{
				at_state = _AT_ERROR;
				NBTask[_AT_CFUN].set(AT CFUN "=1" NEWLINE);
				user_main_printf("Opening all features.");
			}
			break;
			
case _AT_NBAND:
			if(NBTask[_AT_NBAND].get((char*)NBTask[_AT_NBAND].cmd_num) == NB_CMD_SUCC)
				*task=_AT_CSQ;
			else 
				at_state = _AT_ERROR;
			break;
			
case _AT_CSQ:
			NBTask[_AT_CSQ].get((char*)NBTask[_AT_CSQ].cmd_num);
			user_main_printf("Signal Strength:%d",nb.singal);
			if(NBTask[_AT_CSQ].nb_cmd_status == NB_CMD_SUCC)
			{
				*task=_AT_UPLOAD_START;				
				nb.net_flag = success;
				if(net_acc_status_led == 0)
				{
					led_on(3000);
					net_acc_status_led = 1;
				}
				led_on(500);
			}
			else if(NBTask[_AT_CSQ].nb_cmd_status == NB_CMD_OFF)
			{
				*task = _AT_IDLE;	
				at_state = _AT_ERROR;
				nb.net_flag = fail;
			}
			else 
				at_state = _AT_ERROR;
			break;
			
case _AT_UPLOAD_START:
			if(sys.protocol == COAP_PRO)			{*task=_AT_COAP_CONFIG;}
			else if(sys.protocol == UDP_PRO)	{*task=_AT_UDP_OPEN;}
			else if(sys.protocol == MQTT_PRO)	{*task=_AT_MQTT_Config;}
			else if(sys.protocol == TCP_PRO)	{*task=_AT_TCP_OPEN;}
			My_AlarmInit(sys.tdc,0);
			user_main_printf("*****Upload start:%d*****\r\n",sys.uplink_count++);
			txPayLoadDeal(&sensor);
			memset(nb.recieve_data_server,0,sizeof(nb.recieve_data_server));
			break;
/***************************************************COAP******************************************************************************/
case _AT_COAP_CONFIG:
			if(strstr((char*)user.add,"NULL") != NULL || strstr((char*)user.uri,"NULL") != NULL)
			{
				*task=_AT_IDLE;
				user_main_printf("COAP parameter configuration error");
				break;
			}
			if(NBTask[_AT_COAP_CONFIG].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_COAP_OPEN;				
				user_main_info("_AT_COAP_Config successfully");
			}
			{
				at_state = _AT_ERROR;
			}
			break;
case _AT_COAP_OPEN:
			if(NBTask[_AT_COAP_OPEN].run(NULL) == NB_OPEN_SUCC)
			{
				*task=_AT_COAP_OPTION;				
				user_main_printf("Create a CoAP context successfully");
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_COAP_CLOSE;
				user_main_printf("Failed to Create a CoAP context");
			}
			break;	
case _AT_COAP_OPTION:
			if(NBTask[_AT_COAP_OPTION].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_COAP_SEND;				
				user_main_info("Successfully configured CoAP resource path");
			}
			else
			{
				at_state = _AT_ERROR;
				*task = _AT_COAP_CLOSE;
				user_main_info("Failed to configure the CoAP resource path");
			}
			break;
case _AT_COAP_SEND:
			if(NBTask[_AT_COAP_SEND].run(NULL) == NB_SEND_SUCC)
			{
				*task=_AT_COAP_CLOSE;
				user_main_printf("Upload data successfully");	
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_COAP_CLOSE;
				user_main_printf("Failed to upload data");
			}
			break;
case _AT_COAP_CLOSE:
			if(NBTask[_AT_COAP_CLOSE].run(NULL) == NB_CLOSE_SUCC)
			{
				*task=_AT_UPLOAD_END;
				user_main_printf("Successfully deleted CoAP context");
			}
			else 
			{
				at_state = _AT_ERROR;
				*task=_AT_UPLOAD_END;
				user_main_printf("Failed to delete CoAP context");
			}
			break;
/***************************************************MQTT******************************************************************************/		
case _AT_MQTT_Config:
			if(strstr((char*)user.add,"NULL") != NULL || strstr((char*)user.client,"NULL") != NULL || strstr((char*)user.pubtopic,"NULL") != NULL || strstr((char*)user.subtopic,"NULL") != NULL )
			{
				*task=_AT_IDLE;
				user_main_printf("MQTT parameter configuration error");
				break;
			}
			if(NBTask[_AT_MQTT_Config].set(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_MQTT_OPEN;				
				user_main_info("_AT_MQTT_Config successfully");
			}
			{
				at_state = _AT_ERROR;
			}
			break;

case _AT_MQTT_OPEN:
			if(NBTask[_AT_MQTT_OPEN].run(NULL) == NB_OPEN_SUCC)
			{
				*task=_AT_MQTT_CONN;				
				user_main_printf("Opened the MQTT client network successfully");
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_MQTT_CLOSE;
				user_main_printf("Failed to open the MQTT client network");
			}
			break;

case _AT_MQTT_CONN:
			if(NBTask[_AT_MQTT_CONN].run((char*)NBTask[_AT_MQTT_CONN].cmd_num) == NB_CONN_SUCC)
			{
				*task =_AT_MQTT_SUB;
				user_main_printf("Successfully connected to the server");
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_MQTT_CLOSE;
				user_main_printf("Failed to connect to server");
			}
			break;			
case _AT_MQTT_SUB:
			if(NBTask[_AT_MQTT_SUB].run(NULL) == NB_SEND_SUCC)
			{
				*task=_AT_MQTT_PUB;
				user_main_printf("Subscribe to topic successfully");	
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_MQTT_CLOSE;
				user_main_printf("Failed to subscribe to topic");
			}
			break;
case _AT_MQTT_PUB:
			if(NBTask[_AT_MQTT_PUB].run(NULL) == NB_SEND_SUCC)
			{
				*task=_AT_MQTT_READ;
				user_main_printf("Upload data successfully");	
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_MQTT_READ;
				user_main_printf("Failed to upload data");
			}
			break;

case _AT_MQTT_READ:
			HAL_Delay(sys.rxdl);
			if(NBTask[_AT_MQTT_READ].run(NULL) == NB_READ_DATA)
			{				
				*task = _AT_MQTT_CLOSE;
			}
			else 
			{
				user_main_printf("Did not receive the downlink data");
				*task = _AT_MQTT_CLOSE;
			}
			break;
			
case _AT_MQTT_CLOSE:
			if(NBTask[_AT_MQTT_CLOSE].run(NULL) == NB_CLOSE_SUCC)
			{
				*task=_AT_UPLOAD_END;
				user_main_printf("Close the port successfully");
			}
			else 
			{
				at_state = _AT_ERROR;
				*task=_AT_UPLOAD_END;
				user_main_printf("Failed to close the port");
			}
			break;
/**************************************************UDP***********************************************************************************/
case _AT_UDP_OPEN:				
			if(strstr((char*)user.add,"NULL") != NULL)
			{
				*task=_AT_IDLE;
				user_main_printf("UDP parameter configuration error");
				break;
			}
			if(NBTask[_AT_UDP_OPEN].run(NULL) == NB_OPEN_SUCC)
			{
				*task=_AT_UDP_SEND;				
				user_main_printf("Open UDP port successfully");
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_UDP_CLOSE;
				user_main_printf("Failed to open UDP port");
			}
			break;
case _AT_UDP_SEND:
			if(NBTask[_AT_UDP_SEND].run(NULL) == NB_SEND_SUCC)
			{
				*task=_AT_UDP_READ;				
				user_main_printf("Datagram is sent by RF");
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_UDP_CLOSE;
				user_main_printf("Message failed");
			}
			break;
case _AT_UDP_READ:
			HAL_Delay(sys.rxdl);
			if(NBTask[_AT_UDP_READ].run(NULL) == NB_READ_NODATA)
				*task = _AT_UDP_CLOSE;
			else
				*task = _AT_UDP_CLOSE;
			break;
case _AT_UDP_CLOSE:
			if(NBTask[_AT_UDP_CLOSE].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_UPLOAD_END;
				user_main_printf("Close the port successfully");
			}
			else 
			{
				at_state = _AT_ERROR;
				*task=_AT_UPLOAD_END;
				user_main_printf("Failed to close the port");
			}					
			break;
/***************************************************TCP******************************************************************************/		
case _AT_TCP_OPEN:
			if(strstr((char*)user.add,"NULL") != NULL)
			{
				*task=_AT_IDLE;
				user_main_printf("TCP parameter configuration error");
				break;
			}
			if(NBTask[_AT_TCP_OPEN].run(NULL) == NB_OPEN_SUCC)
			{
				*task=_AT_TCP_CONN;				
				user_main_printf("Open TCP port successfully");
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_TCP_CLOSE;
				user_main_printf("Failed to open TCP port");
			}
			break;
case _AT_TCP_CONN:
			if(NBTask[_AT_TCP_CONN].run(NULL) == NB_CONN_SUCC)
			{
				*task=_AT_TCP_SEND;				
				user_main_printf("Connect to the server");
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_TCP_CLOSE;
				user_main_printf("Not connected to server");
			}
			break;
case _AT_TCP_SEND:
			if(NBTask[_AT_TCP_SEND].run(NULL) == NB_SEND_SUCC)
			{
				*task=_AT_TCP_READ;				
				user_main_printf("Datagram has been confirmed to be received by the server");
			}
			else
			{
				at_state = _AT_ERROR;
				*task = _AT_TCP_CLOSE;
				user_main_printf("Message failed");
			}
			break;
case _AT_TCP_READ:
			HAL_Delay(sys.rxdl);
			if(NBTask[_AT_TCP_READ].run(NULL) == NB_READ_NODATA)
				*task = _AT_TCP_CLOSE;
			else
				*task = _AT_TCP_CLOSE;
			break;
case _AT_TCP_CLOSE:
			if(NBTask[_AT_TCP_CLOSE].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_UPLOAD_END;
				user_main_printf("Close the port successfully");
			}
			else 
			{
				at_state = _AT_ERROR;
				*task=_AT_UPLOAD_END;
				user_main_printf("Failed to close the port");
			}
			break;
/******************************************************************************************************************************************/			
case _AT_UPLOAD_END:
			user_main_printf("*****End of upload*****\r\n");	
			*task = _AT_IDLE;
			break;

case _AT_NRB:
			if(NBTask[_AT_NRB].run(NULL) == NB_CMD_SUCC)
				*task=_AT_IDLE;
			else 
				at_state = _AT_ERROR;
					
			nb.net_flag = no_status;
			net_acc_status_led = 0;
			My_AlarmInit(sys.tdc,0);
			break;	

case _AT_IDLE:

			break;			
		//....
default:
			user_main_info("No NB Task.");
			break;
	}

	UART2_ENABLE_RE();
	return at_state;
}
