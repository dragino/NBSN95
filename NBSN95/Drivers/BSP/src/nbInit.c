#include "nbInit.h"

static uint8_t net_acc_status_led = 0;
static char buff[200]={0};
static uint8_t	recieve_data[NB_RX_SIZE] = {0};	   	 			//Receive data

NB nb = {.net_flag=no_status,.recieve_flag=0,.usart.len=0,.usart.data=recieve_data,
				 .imei={0},.imsi={0},.singal=0};

NB_TaskStatus nb_at_send(struct NBTASK *NB_Task)
{
	nb.usart.len = 0;
	memset(nb.usart.data,0,NB_RX_SIZE);	
	HAL_UART_Transmit_DMA(&hlpuart1,(uint8_t*)NB_Task->ATSendStr,NB_Task->len_string);
	
	uint32_t time = HAL_GetTick();
	while(HAL_GetTick() - time < NB_Task->time_out)
	{
		user_main_info("...");
	}
	
	user_main_info("recieve data:%s",nb.usart.data);
	
	if(strstr((char*)nb.usart.data,NB_Task->ATRecStrOK) != NULL)
		NB_Task->nb_cmd_status =  NB_CMD_SUCC;
	else if(strstr((char*)nb.usart.data,NB_Task->ATRecStrError) != NULL)
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
		char *pch = strchr((char*)nb.usart.data,':');
		char *pch1 = strchr((char*)nb.usart.data,'O');
		strncpy((char*)nb.imei, pch +1,pch1 - pch -5);
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
		char *pch = strchr((char*)nb.usart.data,'\n');
		char *pch1 = strchr((char*)nb.usart.data,'O');
		strncpy((char*)nb.imsi, pch +1,pch1 - pch -5);		
	}
	return NBTask[_AT_IMSI].nb_cmd_status;
}

/**
	* @brief  AT_CFUN : get/set UE Functionality
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_cfun_get(const char* param)
{
	NBTask[_AT_CFUN].ATSendStr = (char*)param;
	NBTask[_AT_CFUN].len_string = strlen(NBTask[_AT_CFUN].ATSendStr) ;
	if(nb_at_send(&NBTask[_AT_CFUN])== NB_CMD_SUCC)
	{
		if(strstr((char*)nb.usart.data,CFUN":1") != NULL)
			NBTask[_AT_CFUN].nb_cmd_status = NB_CMD_SUCC;
		else if(strstr((char*)nb.usart.data,CFUN":0") != NULL)
		{
			NBTask[_AT_CFUN].nb_cmd_status = NB_CMD_OFF;
		}
	}
	return NBTask[_AT_CFUN].nb_cmd_status;
}

NB_TaskStatus nb_cfun_set(const char* param)
{
//	NBTask[_AT_CFUN].ATSendStr = AT CFUN "=1" NEWLINE;
	NBTask[_AT_CFUN].ATSendStr = (char*)param;
	NBTask[_AT_CFUN].len_string = strlen(NBTask[_AT_CFUN].ATSendStr) ;

	return nb_at_send(&NBTask[_AT_CFUN]);
}

/**
	* @brief  AT_NCONFIG :  Configure UE Behaviour
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_nconfig_run(const char* param)
{
	NBTask[_AT_NCONFIG].try_num = 4;
	while(NBTask[_AT_NCONFIG].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_NCONFIG])== NB_CMD_SUCC)
		{
			NBTask[_AT_NCONFIG].nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
			NBTask[_AT_NCONFIG].nb_cmd_status = NB_CMD_FAIL;
	}

	return NBTask[_AT_NCONFIG].nb_cmd_status;
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
		char *pch = strchr((char*)nb.usart.data,':');
		while(*pch != '\n')
			printf("%c",*pch++);
		printf("\n");
	}
	return NBTask[_AT_NBAND].nb_cmd_status;
}

/**
	* @brief  AT_CCLK : Get Time
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_cclk_run(const char* param)
{
	sensor.time_stamp = 0;
	if(nb_at_send(&NBTask[_AT_CCLK])== NB_CMD_SUCC)
	{
		NBTask[_AT_CCLK].nb_cmd_status = nb_cclk_get(param);
	}
	return NBTask[_AT_CCLK].nb_cmd_status;
}

NB_TaskStatus nb_cclk_get(const char* param)
{
	char *p = strstr((char*)nb.usart.data,CCLK);
	if( p==NULL)
		NBTask[_AT_CCLK].nb_cmd_status = NB_NO_TIME;
	else
	{
		char date[20]={0};
		strcat(date,"20");
		memcpy(&date[2],&p[6],17);
		sensor.time_stamp = GetTick(date);
		user_main_debug("time_stamp:%d",sensor.time_stamp);
	}
	return NBTask[_AT_CCLK].nb_cmd_status;
}

/**
	* @brief  AT+CPSMS : Power Saving ModeSetting
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_cpsms_run(const char* param)
{
	nb_cpsms_set(param);
	if(nb_at_send(&NBTask[_AT_CPSMS])== NB_CMD_SUCC)
	{
		NBTask[_AT_CPSMS].nb_cmd_status = NB_CMD_SUCC;
	}
	else
		NBTask[_AT_CPSMS].nb_cmd_status = NB_CMD_FAIL;
	
	return NBTask[_AT_CPSMS].nb_cmd_status;
}

NB_TaskStatus nb_cpsms_get(const char* param)
{
	return NBTask[_AT_CPSMS].nb_cmd_status;
}

NB_TaskStatus nb_cpsms_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT CPSMS"=1,,,");
	strcat(buff,(char*)"01000001");
	strcat(buff,(char*)",00000000\n");
	
	NBTask[_AT_CPSMS].ATSendStr  = buff;
	NBTask[_AT_CPSMS].len_string = strlen(NBTask[_AT_CPSMS].ATSendStr);
	user_main_debug("NBTask[_AT_CPSMS].ATSendStr:%s",NBTask[_AT_CPSMS].ATSendStr);
	
	return NBTask[_AT_CPSMS].nb_cmd_status;
}

/**
	* @brief  AT+CEREG : EPS Network Registration Status
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_cereg_run(const char* param)
{
	for(char i=0;i<2;i++)
	{
		nb_cereg_set(&i);
		if(nb_at_send(&NBTask[_AT_CEREG])== NB_CMD_SUCC)
		{
			nb_cereg_get(&i);
			NBTask[_AT_CEREG].nb_cmd_status = NB_CMD_SUCC;
		}
		else
			NBTask[_AT_CEREG].nb_cmd_status = NB_CMD_FAIL;
	}
	return NBTask[_AT_CEREG].nb_cmd_status;
}

NB_TaskStatus nb_cereg_get(const char* param)
{
	if(param[0] == 1)
	{
		char* p2 = strchr((char*)nb.usart.data,',');
		char* p1  = NULL;
		for(int i=0;i<6;i++)
		{			
			p1 = p2;
			p2 = strchr(p2,',');
			p2++;
		}
		char* p3 = strchr(p2,',');
		if(p2-p1>1)
		{
			char buff[4]={0};
			memcpy(buff,p2,p3-p2);
			user_main_printf("cause_type:%c",p1[0]);
			user_main_printf("reject_cause:%s",buff);
		}
	}
	return NBTask[_AT_CEREG].nb_cmd_status;
}

NB_TaskStatus nb_cereg_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	if(param[0] == 0)
		strcat(buff,AT CEREG"=5\n");
	else
		strcat(buff,AT CEREG"?\n");
	
	NBTask[_AT_CEREG].ATSendStr  = buff;
	NBTask[_AT_CEREG].len_string = strlen(NBTask[_AT_CEREG].ATSendStr);
	user_main_debug("NBTask[_AT_CEREG].ATSendStr:%s",NBTask[_AT_CEREG].ATSendStr);
	
	return NBTask[_AT_CEREG].nb_cmd_status;
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
		char *pch1 = strchr((char*)nb.usart.data,':');
		char *pch2 = strchr((char*)nb.usart.data,',');
		for(int i =0;i<(pch2 - pch1)-1;i++)
			sprintf(singalBuff+strlen(singalBuff), "%c", nb.usart.data[7+i]);
		nb.singal = atoi(singalBuff);
			
		if(nb.singal == 99 || nb.singal == 0)
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
	ATCmdNum at_state = _AT_IDLE;
	NB_TaskStatus uri_state = NB_IDIE;
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
				*task=_AT_NCONFIG;
			}
			else 
			{
				at_state = _AT_ERROR;
				NBTask[_AT_CFUN].set(AT CFUN "=1" NEWLINE);
				user_main_printf("Opening all features.");
			}
			break;
	
case _AT_NCONFIG:
			if(NBTask[_AT_NCONFIG].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_NBAND;
				user_main_printf("Set automatic network access successfully.");
			}
			else
			{
				*task=_AT_NBAND;
				user_main_printf("Failed to set up automatic network access.");
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
				*task=(net_acc_status_led == 0)?_AT_CPSMS:_AT_CCLK;				
				nb.net_flag = success;
				if(net_acc_status_led == 0)
				{
					led_on(3000);
					net_acc_status_led = 1;
				}
				led_on(500);
			}
			else
			{
				*task = _AT_CEREG;	
				at_state = _AT_ERROR;
				nb.net_flag = fail;
			}
			break;

case _AT_CEREG:
			NBTask[_AT_CEREG].run(NULL);
			*task=_AT_IDLE;
			break;
			
case _AT_CPSMS:
			if(NBTask[_AT_CPSMS].run(NULL) == NB_CMD_SUCC)
				user_main_printf("PSM mode configured ");
			else 
				user_main_printf("PSM mode configuration failed ");			
			*task=_AT_CCLK;
			break;
case _AT_CCLK:
			if(NBTask[_AT_CCLK].run(NULL) == NB_NO_TIME)
			{
				at_state = _AT_ERROR;
				user_main_printf("Failed to get time");
			}
			*task=_AT_UPLOAD_START;
			break;
case _AT_UPLOAD_START:
			if(sys.protocol == COAP_PRO)			{*task=_AT_COAP_CONFIG;}
			else if(sys.protocol == UDP_PRO)	{*task=_AT_UDP_SEND;}
			else if(sys.protocol == MQTT_PRO)	{*task=_AT_MQTT_Config;}
			else if(sys.protocol == TCP_PRO)	{*task=_AT_TCP_OPEN;}
			if(sys.exit_flag == 0)	My_AlarmInit(sys.tdc,0);
			user_main_printf("*****Upload start:%d*****",sys.uplink_count++);
			txPayLoadDeal(&sensor,sys.list);
			memset((char*)nb.usart.data,0,sizeof(nb.usart.data));
			nb.uplink_flag = send;
			nb.recieve_flag = NB_IDIE;
			break;
/***************************************************COAP******************************************************************************/
case _AT_COAP_CONFIG:
			if(strstr((char*)user.add,"NULL") != NULL || strstr((char*)user.uri,"NULL") != NULL)
			{
				*task=_AT_UPLOAD_END;
				user_main_printf("COAP parameter configuration error");
				break;
			}
			if(NBTask[_AT_COAP_CONFIG].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_COAP_OPEN;				
				user_main_info("_AT_COAP_Config successfully");
			}
			else
			{
				at_state = _AT_ERROR;
				*task = _AT_UPLOAD_END;
				user_main_printf("COAP configuration failed");
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
				*task=_AT_COAP_SEND_CONFIG;				
				user_main_info("Successfully configured CoAP resource path");
			}
			else
			{
				at_state = _AT_ERROR;
				*task = _AT_COAP_CLOSE;
				user_main_info("Failed to configure the CoAP resource path");
			}
			break;
case _AT_COAP_SEND_CONFIG:
			if(NBTask[_AT_COAP_SEND_CONFIG].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_COAP_SEND;
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_COAP_CLOSE;
				user_main_printf("Failed to upload data");
				break;
			}
			
case _AT_COAP_SEND:
			if(NBTask[_AT_COAP_SEND].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_IDLE;
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
				*task=_AT_UPLOAD_SUCC;
				user_main_printf("Successfully deleted CoAP context");
			}
			else 
			{
				at_state = _AT_ERROR;
				*task=_AT_UPLOAD_FAIL;
				user_main_printf("Failed to delete CoAP context");
			}
			break;
case _AT_COAP_URI:
			uri_state = NBTask[_AT_COAP_URI].run(NULL);
			if(uri_state == NB_SEND_SUCC)
			{
				*task = _AT_COAP_CLOSE;
				user_main_printf("Upload data successfully");	
			}
			else
			{
				at_state = _AT_ERROR;
				*task = _AT_COAP_CLOSE;
			}
			break;
/***************************************************MQTT******************************************************************************/		
case _AT_MQTT_Config:
			if(strstr((char*)user.add,"NULL") != NULL || strstr((char*)user.client,"NULL") != NULL || strstr((char*)user.pubtopic,"NULL") != NULL || strstr((char*)user.subtopic,"NULL") != NULL )
			{
				*task=_AT_UPLOAD_END;
				user_main_printf("MQTT parameter configuration error");
				break;
			}
			if(NBTask[_AT_MQTT_Config].set(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_MQTT_OPEN;			
				user_main_info("_AT_MQTT_Config successfully");
			}
			else
			{
				at_state = _AT_ERROR;
				*task = _AT_UPLOAD_END;
				user_main_printf("MQTT configuration failed");
			}
			break;

case _AT_MQTT_OPEN:
			if(NBTask[_AT_MQTT_OPEN].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_IDLE;
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_MQTT_CLOSE;
				user_main_printf("Failed to open the MQTT client network");
			}
			break;

case _AT_MQTT_CONN:
			if(NBTask[_AT_MQTT_CONN].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_IDLE;				
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_MQTT_CLOSE;
				user_main_printf("Failed to connect to server");
			}
			break;
			
case _AT_MQTT_SUB:
			if(NBTask[_AT_MQTT_SUB].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_IDLE;
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_MQTT_CLOSE;
				user_main_printf("Failed to subscribe to topic");
			}
			break;
			
case _AT_MQTT_PUB:
			if(NBTask[_AT_MQTT_PUB].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_MQTT_SEND;				
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_MQTT_CLOSE;
				user_main_printf("Failed to Set PUB");
				break;
			}
			
case _AT_MQTT_SEND:
			if(NBTask[_AT_MQTT_SEND].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_IDLE;
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_MQTT_CLOSE;
				user_main_printf("Failed to upload data");
			}
			break;
case _AT_MQTT_READ:
			HAL_Delay(sys.rxdl);
			*task = _AT_MQTT_CLOSE;
			break;
			
case _AT_MQTT_CLOSE:
			if(NBTask[_AT_MQTT_CLOSE].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_MQTT_URI;				
			}
			else
			{
				at_state = _AT_ERROR;
				*task=_AT_UPLOAD_FAIL;
				user_main_printf("Failed to close the port");
			}
			break;
			
case _AT_MQTT_URI:
			uri_state = NBTask[_AT_MQTT_URI].run(NULL);
			switch((uint8_t)uri_state)
			{
				case NB_OPEN_SUCC:
							*task=_AT_MQTT_CONN;
							user_main_printf("Opened the MQTT client network successfully");
						break;
				case NB_CONN_SUCC:
							*task=_AT_MQTT_PUB;
							user_main_printf("Successfully connected to the server");
						break;
				case NB_SUB_SUCC:
							*task=_AT_MQTT_READ;
							user_main_printf("Subscribe to topic successfully");	
						break;
				case NB_PUB_SUCC:
							*task=_AT_MQTT_SUB;
							user_main_printf("Upload data successfully");	
						break;
				case NB_CLOSE_SUCC:
							*task = _AT_UPLOAD_SUCC;
							user_main_printf("Close the port successfully");
						break;
				case NB_ERROR:
							at_state = _AT_ERROR;
							*task = _AT_UPLOAD_FAIL;
						break;
				default:
							*task = _AT_IDLE;
						break;
			}
			
			break;
/**************************************************UDP***********************************************************************************/
case _AT_UDP_OPEN:		
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
			if(strstr((char*)user.add,"NULL") != NULL)
			{
				*task=_AT_UPLOAD_END;
				user_main_printf("UDP parameter configuration error");
				break;
			}
			if(nb.socket>='0' && nb.socket<='7')
			{
				if(NBTask[_AT_UDP_SEND].run(NULL) != NB_CMD_SUCC)
				{					
					*task = _AT_UDP_OPEN;
					break;							
				}
			}
			else
			{
				*task = _AT_UDP_OPEN;
				break;
			}
case _AT_UDP_READ:
			HAL_Delay(sys.rxdl);
			*task = _AT_IDLE;
			break;
case _AT_UDP_CLOSE:
			if(NBTask[_AT_UDP_CLOSE].run(NULL) == NB_CMD_SUCC)
			{
				user_main_printf("Close the port successfully");
			}
			else
			{
				at_state = _AT_ERROR;				
				user_main_printf("Failed to close the port");
			}
			*task=_AT_UPLOAD_FAIL;
			break;
case _AT_UDP_URI:
			uri_state = NBTask[_AT_UDP_URI].run(NULL);
			if(uri_state == NB_SEND_SUCC)
			{
				*task = _AT_UPLOAD_SUCC;
				user_main_printf("Datagram is sent by RF");
			}
			else
			{
				at_state = _AT_ERROR;
				*task = _AT_UDP_CLOSE;	
			}
			break;
/***************************************************TCP******************************************************************************/		
case _AT_TCP_OPEN:
			if(strstr((char*)user.add,"NULL") != NULL)
			{
				*task=_AT_UPLOAD_END;
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
			if(NBTask[_AT_TCP_SEND].run(NULL) != NB_CMD_SUCC)
			{
				at_state = _AT_ERROR;
				*task = _AT_TCP_CLOSE;
				break;				
			}
			
case _AT_TCP_READ:
			HAL_Delay(sys.rxdl);
			*task = _AT_IDLE;
			break;
case _AT_TCP_CLOSE:
			if(NBTask[_AT_TCP_CLOSE].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_UPLOAD_SUCC;
				user_main_printf("Close the port successfully");
			}
			else 
			{
				at_state = _AT_ERROR;
				*task=_AT_UPLOAD_FAIL;
				user_main_printf("Failed to close the port");
			}				
			break;
case _AT_TCP_URI:
			uri_state = NBTask[_AT_TCP_URI].run(NULL);
			if(uri_state == NB_SEND_SUCC)
			{
				*task = _AT_TCP_CLOSE;
				user_main_printf("Datagram has been confirmed to be received by the server");
			}
			else
			{
				at_state = _AT_ERROR;
				*task=_AT_UPLOAD_FAIL;
				user_main_printf("Message failed");
			}
			break;
/******************************************************************************************************************************************/			
case _AT_UPLOAD_END:
			user_main_printf("*****End of upload*****\r\n");	
			*task = _AT_IDLE;
			memset((char*)nb.usart.data,0,sizeof(nb.usart.data));
			nb.uplink_flag = no_status;
			break;

case _AT_UPLOAD_SUCC:
			if(sys.list_flag ==1)	
				upLink_flash_clear(sys.list);
			*task = _AT_UPLOAD_END;
			user_main_printf("Send complete");
			sys.list_flag =0;
			break;

case _AT_UPLOAD_FAIL:
			user_main_printf("Failed to send");			
			sys.list = upLink_fail_write(sys.list);
			user_main_printf("Saving data...");
			*task = _AT_UPLOAD_END;
			sys.list_flag =1;
			break;

case _AT_NRB:
			if(NBTask[_AT_NRB].run(NULL) == NB_CMD_SUCC)
				*task=_AT_IDLE;
			else
				at_state = _AT_ERROR;
			nb.net_flag = no_status;
			net_acc_status_led = 0;
			nb.uplink_flag = no_status;			
			My_AlarmInit(sys.tdc,0);
			break;

case _AT_URI:
			if(sys.protocol == COAP_PRO)			*task = _AT_COAP_URI;
			else if(sys.protocol == UDP_PRO)	*task = _AT_UDP_URI;
			else if(sys.protocol == MQTT_PRO)	*task = _AT_MQTT_URI;
			else if(sys.protocol == TCP_PRO)	*task = _AT_TCP_URI;

			break;
case _AT_FLAG_INIT:
			nb.uplink_flag = no_status;
			nb.net_flag = no_status;
			net_acc_status_led = 0;
			nb.uplink_flag = no_status;
			*task = _AT;
			break;

case _AT_IDLE:

			break;			
		//....
default:
			user_main_info("No NB Task.");
			*task=_AT_IDLE;
			break;
	}
	return at_state;
}
