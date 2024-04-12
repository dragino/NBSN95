#include "nb_coap.h"

static char buff[200]={0};
static char downlink_data[50]={0};
extern uint8_t dns_id_flags; 
/**
	* @brief  Configure to show the CoAP option of sender
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_COAP_config_run(const char* param)
{
	NBTask[_AT_COAP_CONFIG].try_num = 2;
	NBTask[_AT_COAP_CONFIG].set(param);
	
	while(NBTask[_AT_COAP_CONFIG].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_COAP_CONFIG]) == NB_CMD_SUCC )
		{
			NBTask[_AT_COAP_CONFIG].nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
		{
			NBTask[_AT_COAP_CONFIG].nb_cmd_status = NB_CMD_FAIL;
			HAL_Delay(500);
		}
	}
	return NBTask[_AT_COAP_CONFIG].nb_cmd_status;
}

NB_TaskStatus nb_COAP_config_set(const char* param)
{
	user_main_debug("NBTask[_AT_COAP_CONFIG].ATSendStr:%s",NBTask[_AT_COAP_CONFIG].ATSendStr);
	return NBTask[_AT_COAP_CONFIG].nb_cmd_status;
}

NB_TaskStatus nb_COAP_config_get(const char* param)
{
	return NBTask[_AT_COAP_CONFIG].nb_cmd_status;
}

/**
	* @brief  Create a CoAP context
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_COAP_open_run(const char* param)
{
	NBTask[_AT_COAP_OPEN].try_num = 4;
	NBTask[_AT_COAP_OPEN].set(param);
	
	while(NBTask[_AT_COAP_OPEN].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_COAP_OPEN]) == NB_CMD_SUCC )
		{
			NBTask[_AT_COAP_OPEN].nb_cmd_status = NB_OPEN_SUCC;
			break;
		}
		else
		{
			NBTask[_AT_COAP_OPEN].nb_cmd_status = NB_OPEN_FAIL;
			HAL_Delay(500);
		}
	}
	return NBTask[_AT_COAP_OPEN].nb_cmd_status;
}

NB_TaskStatus nb_COAP_open_set(const char* param)
{
	user_main_debug("NBTask[_AT_COAP_OPEN].ATSendStr:%s",NBTask[_AT_COAP_OPEN].ATSendStr);
	return NBTask[_AT_COAP_OPEN].nb_cmd_status;
}

NB_TaskStatus nb_COAP_open_get(const char* param)
{
	return NBTask[_AT_COAP_OPEN].nb_cmd_status;
}

/**
	* @brief  Configure CoAP Options
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_COAP_option_run(const char* param)
{
	NBTask[_AT_COAP_OPTION].try_num = 4;
	NBTask[_AT_COAP_OPTION].set(param);
	
	while(NBTask[_AT_COAP_OPTION].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_COAP_OPTION]) == NB_CMD_SUCC )
		{
			NBTask[_AT_COAP_OPTION].nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
		{
			NBTask[_AT_COAP_OPTION].nb_cmd_status = NB_CMD_FAIL;
			HAL_Delay(500);
		}
	}
	return NBTask[_AT_COAP_OPTION].nb_cmd_status;
}

NB_TaskStatus nb_COAP_option_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QCOAPOPTION"=");
	strcat(buff,(char*)user.uri);
	strcat(buff,"\n");
	
	NBTask[_AT_COAP_OPTION].ATSendStr  = buff;
	NBTask[_AT_COAP_OPTION].len_string = strlen(NBTask[_AT_COAP_OPTION].ATSendStr);
	user_main_debug("NBTask[_AT_COAP_OPTION].ATSendStr:%s",NBTask[_AT_COAP_OPTION].ATSendStr);
	return NBTask[_AT_COAP_OPTION].nb_cmd_status;
}
	
NB_TaskStatus nb_COAP_option_get(const char* param)
{
	return NBTask[_AT_COAP_OPTION].nb_cmd_status;
}
/**
	* @brief  COAP Send DATA Config
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_COAP_send_config_run(const char* param)
{
	NBTask[_AT_COAP_SEND_CONFIG].try_num = 2;
	NBTask[_AT_COAP_SEND_CONFIG].set(NULL);
	
	if(nb_at_send(&NBTask[_AT_COAP_SEND_CONFIG]) == NB_CMD_SUCC)
	{
		NBTask[_AT_COAP_SEND_CONFIG].nb_cmd_status = NB_CMD_SUCC;
	}
	else
		NBTask[_AT_COAP_SEND_CONFIG].nb_cmd_status = NB_CMD_FAIL;
	
	return NBTask[_AT_COAP_SEND_CONFIG].nb_cmd_status;
}

NB_TaskStatus nb_COAP_send_config_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QCOAPSEND"=1,3,");
	strcat(buff,(char*)user.add);
	strcat(buff,"\n");
	
	NBTask[_AT_COAP_SEND_CONFIG].ATSendStr  = buff;
	NBTask[_AT_COAP_SEND_CONFIG].len_string = strlen(NBTask[_AT_COAP_SEND_CONFIG].ATSendStr);
	user_main_debug("NBTask[_AT_COAP_SEND_CONFIG].ATSendStr:%s",NBTask[_AT_COAP_SEND_CONFIG].ATSendStr);
	return NBTask[_AT_COAP_SEND_CONFIG].nb_cmd_status;
}
/**
	* @brief  COAP Send DATA
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_COAP_send_run(const char* param)
{
	NBTask[_AT_COAP_SEND].try_num = 4;
	NBTask[_AT_COAP_SEND].set(NULL);

	if(nb_at_send(&NBTask[_AT_COAP_SEND]) == NB_CMD_SUCC)
	{
		NBTask[_AT_COAP_SEND].nb_cmd_status = NB_CMD_SUCC;
	}
	else
		NBTask[_AT_COAP_SEND].nb_cmd_status = NB_CMD_FAIL;

	return NBTask[_AT_COAP_SEND].nb_cmd_status;
}

NB_TaskStatus nb_COAP_send_set(const char* param)
{
	NBTask[_AT_COAP_SEND].ATSendStr  = sensor.data;
	NBTask[_AT_COAP_SEND].len_string = sensor.data_len;
	user_main_debug("NBTask[_AT_COAP_SEND].ATSendStr:%s",NBTask[_AT_COAP_SEND].ATSendStr);
	return NBTask[_AT_COAP_SEND].nb_cmd_status;
}
//////////////////////////////////
NB_TaskStatus nb_COAP_read_run(const char* param)
{
	nb_COAP_read_set(param);
	if(nb_COAP_read_get(param) == NB_READ_NODATA)
	{
		NBTask[_AT_COAP_READ].nb_cmd_status = NB_READ_NODATA;
	}
	else
	{		
		NBTask[_AT_COAP_READ].nb_cmd_status = NB_READ_DATA;
	}
	return NBTask[_AT_COAP_READ].nb_cmd_status;
}
	
NB_TaskStatus nb_COAP_read_set(const char* param)
{
	memset(downlink_data,0,20);
	return NBTask[_AT_COAP_READ].nb_cmd_status;
}
	
NB_TaskStatus nb_COAP_read_get(const char* param)
{	
	NBTask[_AT_COAP_READ].nb_cmd_status = NB_READ_NODATA;
	char* pos_start  = strstr((char*)nb.usart.data,QCOAPURC);
	if(pos_start != NULL)
	{
		if(countchar(pos_start,',')==5)
		{
			char* pos_end=strrchr((char*)pos_start,',');
			memcpy(downlink_data,pos_end+1,strlen(pos_end)-1);
			user_main_printf("Received downlink data:%s",downlink_data);
			rxPayLoadDeal(downlink_data);
		}
	}
	return NBTask[_AT_COAP_READ].nb_cmd_status;
}

/**
	* @brief  Delete the CoAP context
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_COAP_close_run(const char* param)
{
	NBTask[_AT_COAP_CLOSE].try_num = 2;
	NBTask[_AT_COAP_CLOSE].set(param);
	while(NBTask[_AT_COAP_CLOSE].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_COAP_CLOSE]) == NB_CMD_SUCC )
		{
			NBTask[_AT_COAP_CLOSE].nb_cmd_status = NB_CLOSE_SUCC;
			break;
		}
		else
		{
			NBTask[_AT_COAP_CLOSE].nb_cmd_status = NB_CLOSE_FAIL;
			HAL_Delay(500);
		}
	}
	return NBTask[_AT_COAP_CLOSE].nb_cmd_status;
}
	
NB_TaskStatus nb_COAP_close_set(const char* param)
{
	user_main_debug("NBTask[_AT_COAP_CLOSE].ATSendStr:%s",NBTask[_AT_COAP_CLOSE].ATSendStr);
	return NBTask[_AT_COAP_CLOSE].nb_cmd_status;
}

NB_TaskStatus nb_COAP_close_get(const char* param)
{
	return NBTask[_AT_COAP_CLOSE].nb_cmd_status;
}

/************************COAP BC95-GV***********************/
/**
	* @brief  COAP Configure server address
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_COAP_open_config95gv_run(const char* param)
{
	NBTask[_AT_COAP_OPEN_CONFIG_95GV].try_num = 2;
	NBTask[_AT_COAP_OPEN_CONFIG_95GV].set(NULL);
	
	if(nb_at_send(&NBTask[_AT_COAP_OPEN_CONFIG_95GV]) == NB_CMD_SUCC)
	{
		NBTask[_AT_COAP_OPEN_CONFIG_95GV].nb_cmd_status = NB_CMD_SUCC;
	}
	else
		NBTask[_AT_COAP_OPEN_CONFIG_95GV].nb_cmd_status = NB_CMD_FAIL;
	
	return NBTask[_AT_COAP_OPEN_CONFIG_95GV].nb_cmd_status;
}

NB_TaskStatus nb_COAP_open_config95gv_set(const char* param)
{
	char* pos=strchr((char*)user.add,',');
	char pos1[20]={0};
	memset(buff,0,sizeof(buff));
	memset(pos1,0,sizeof(pos1));
	memcpy(pos1,(char*)user.add,strlen((char*)user.add)-strlen(pos));
	strcat(buff,AT QCOAPOPEN"=0,");
	strcat(buff,"\"");
	strcat(buff,pos1);
	strcat(buff,"\"");
	strcat(buff,pos);
	strcat(buff,"\n");
	
	NBTask[_AT_COAP_OPEN_CONFIG_95GV].ATSendStr  = buff;
	NBTask[_AT_COAP_OPEN_CONFIG_95GV].len_string = strlen(NBTask[_AT_COAP_OPEN_CONFIG_95GV].ATSendStr);
	user_main_debug("NBTask[_AT_COAP_OPEN_CONFIG_95GV].ATSendStr:%s",NBTask[_AT_COAP_OPEN_CONFIG_95GV].ATSendStr);
	return NBTask[_AT_COAP_OPEN_CONFIG_95GV].nb_cmd_status;
}

/**
	* @brief  Configure CoAP Options
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_COAP_option95gv_run(const char* param)
{
	NBTask[_AT_COAP_OPTION_95GV].try_num = 4;
	NBTask[_AT_COAP_OPTION_95GV].set(param);
	
	while(NBTask[_AT_COAP_OPTION_95GV].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_COAP_OPTION_95GV]) == NB_CMD_SUCC )
		{
			NBTask[_AT_COAP_OPTION_95GV].nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
		{
			NBTask[_AT_COAP_OPTION_95GV].nb_cmd_status = NB_CMD_FAIL;
			HAL_Delay(500);
		}
	}
	return NBTask[_AT_COAP_OPTION_95GV].nb_cmd_status;
}

NB_TaskStatus nb_COAP_option95gv_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QCOAPOPTION"=0,0,");
	strcat(buff,(char*)user.uri);
	strcat(buff,"\n");
	
	NBTask[_AT_COAP_OPTION_95GV].ATSendStr  = buff;
	NBTask[_AT_COAP_OPTION_95GV].len_string = strlen(NBTask[_AT_COAP_OPTION_95GV].ATSendStr);
	user_main_debug("NBTask[_AT_COAP_OPTION_95GV].ATSendStr:%s",NBTask[_AT_COAP_OPTION_95GV].ATSendStr);
	return NBTask[_AT_COAP_OPTION_95GV].nb_cmd_status;
}
	
NB_TaskStatus nb_COAP_option95gv_get(const char* param)
{
	return NBTask[_AT_COAP_OPTION_95GV].nb_cmd_status;
}

/**
	* @brief  COAP Send DATA Config
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_COAP_send_config95gv_run(const char* param)
{
	NBTask[_AT_COAP_SEND_CONFIG_95GV].try_num = 2;
	NBTask[_AT_COAP_SEND_CONFIG_95GV].set(NULL);
	
	if(nb_at_send(&NBTask[_AT_COAP_SEND_CONFIG_95GV]) == NB_CMD_SUCC)
	{
		NBTask[_AT_COAP_SEND_CONFIG_95GV].nb_cmd_status = NB_CMD_SUCC;
	}
	else
		NBTask[_AT_COAP_SEND_CONFIG_95GV].nb_cmd_status = NB_CMD_FAIL;
	
	return NBTask[_AT_COAP_SEND_CONFIG_95GV].nb_cmd_status;
}

NB_TaskStatus nb_COAP_send_config95gv_set(const char* param)
{
	char* num;
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QCOAPSEND"=0,1,3,0,");
	num=Int2String(sensor.data_len,num);
	strcat(buff,(char*)num);
	strcat(buff,",\"");
	strcat(buff,(char*)sensor.data);
	strcat(buff,"\"");
	strcat(buff,"\n");
	
	NBTask[_AT_COAP_SEND_CONFIG_95GV].ATSendStr  = buff;
	NBTask[_AT_COAP_SEND_CONFIG_95GV].len_string = strlen(NBTask[_AT_COAP_SEND_CONFIG_95GV].ATSendStr);
	user_main_debug("NBTask[_AT_COAP_SEND_CONFIG_95GV].ATSendStr:%s",NBTask[_AT_COAP_SEND_CONFIG_95GV].ATSendStr);
	return NBTask[_AT_COAP_SEND_CONFIG_95GV].nb_cmd_status;
}

/**
	* @brief  Delete the CoAP context
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_COAP_closebc95gv_run(const char* param)
{
	NBTask[_AT_COAP_CLOSE_BC95GV].try_num = 2;
	NBTask[_AT_COAP_CLOSE_BC95GV].set(param);
	while(NBTask[_AT_COAP_CLOSE_BC95GV].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_COAP_CLOSE_BC95GV]) == NB_CMD_SUCC )
		{
			NBTask[_AT_COAP_CLOSE_BC95GV].nb_cmd_status = NB_CLOSE_SUCC;
			break;
		}
		else
		{
			NBTask[_AT_COAP_CLOSE_BC95GV].nb_cmd_status = NB_CLOSE_FAIL;
			HAL_Delay(500);
		}
	}
	return NBTask[_AT_COAP_CLOSE_BC95GV].nb_cmd_status;
}
	
NB_TaskStatus nb_COAP_closebc95gv_set(const char* param)
{
	user_main_debug("NBTask[_AT_COAP_CLOSE_BC95GV].ATSendStr:%s",NBTask[_AT_COAP_CLOSE_BC95GV].ATSendStr);
	return NBTask[_AT_COAP_CLOSE_BC95GV].nb_cmd_status;
}

NB_TaskStatus nb_COAP_closebc95gv_get(const char* param)
{
	return NBTask[_AT_COAP_CLOSE_BC95GV].nb_cmd_status;
}

/**
	* @brief  COAP URI:Scheduling tasks via URI 
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_COAP_uri_run(const char* param)
{
	user_main_debug("uri:%s",nb.usart.data);
	if(strstr((char*)nb.usart.data,QCOAPURC": \"rsp\",2,2.0") != NULL)
	{
		NBTask[_AT_COAP_URI].nb_cmd_status = NB_SEND_SUCC;	
	}
	else
		NBTask[_AT_COAP_URI].nb_cmd_status = NB_OTHER;	

////Judgment issued and received 
//	if(strstr((char*)nb.usart.data,QCOAPURC) != NULL)
//	{
////		nb_COAP_data_read_set(NULL);
//	}
//Ask if the process has failed 
	if(strstr((char*)nb.usart.data,QCOAPURC": \"rsp\",2,5") != NULL ||
		strstr((char*)nb.usart.data,QCOAPURC": \"rsp\",2,4") != NULL )
	{
		NBTask[_AT_COAP_URI].nb_cmd_status = NB_ERROR;
	}
	return NBTask[_AT_COAP_URI].nb_cmd_status;
}

/**
COAP URI BC95-GV*************************************************************
  */
NB_TaskStatus nb_COAP_uri95gv_run(const char* param)
{
	user_main_debug("uri:%s",nb.usart.data);
	
	if(strstr((char*)nb.usart.data,QCOAPOPEN": 0,0") != NULL)
	{
		NBTask[_AT_COAP_URI_95GV].nb_cmd_status = NB_QCOAPOPEN_SUCC;	
	}
	else if(strstr((char*)nb.usart.data,QCOAPCLOSE": 0,0") != NULL)
	{
		NBTask[_AT_COAP_URI_95GV].nb_cmd_status = NB_QCOAPCLOSE_SUCC;	
	}
	else if(strstr((char*)nb.usart.data,QCOAPURC": 0,2,20") != NULL)
	{
		NBTask[_AT_COAP_URI_95GV].nb_cmd_status = NB_SEND_SUCC;	
	}
	else
		NBTask[_AT_COAP_URI_95GV].nb_cmd_status = NB_OTHER;	

	if(strstr((char*)nb.usart.data,QCOAPURC": 0,2,5") != NULL ||
		strstr((char*)nb.usart.data,QCOAPURC": 0,2,4") != NULL ||
		strstr((char*)nb.usart.data,QCOAPURC": 0,1,4") != NULL||
		strstr((char*)nb.usart.data,QCOAPURC": 0,1,5") != NULL)
	{
		NBTask[_AT_COAP_URI_95GV].nb_cmd_status = NB_ERROR;
	}
	return NBTask[_AT_COAP_URI_95GV].nb_cmd_status;
}