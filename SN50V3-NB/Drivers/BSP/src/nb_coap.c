#include "nb_coap.h"
#include "time.h"
#include <time.h>
extern char buff[2000];
extern char downlink_data[1000];
extern uint8_t read_flag;

extern char 	*ATSendStr;
extern int 	len_string;
extern uint8_t  try_num;
extern NB_TaskStatus  nb_cmd_status;
extern void pro_data(void);
/**
	* @brief  Configure to show the CoAP option of sender
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_COAP_config_set(const char* param)
{
	try_num = 2;
	ATSendStr  = NULL;
	ATSendStr = AT QCOAPCFG "=\"dtls\",0,0" NEWLINE;
	len_string = sizeof(AT QCOAPCFG "=\"dtls\",0,0" NEWLINE) - 1;
	
	if(nb_at_send(&NBTask[_AT_COAP_CONFIG]) == NB_CMD_SUCC)
		nb_cmd_status = NB_CMD_SUCC;
	else
		nb_cmd_status = NB_CMD_FAIL;

	user_main_debug("NBTask[_AT_COAP_CONFIG].ATSendStr:%s",ATSendStr);		
	return nb_cmd_status;
}

/**
	* @brief  Create a CoAP context
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_COAP_open_run(const char* param)
{
	try_num = 4;
	NBTask[_AT_COAP_OPEN].set(param);	
	
	while(try_num--)
	{
	if(nb_at_send(&NBTask[_AT_COAP_OPEN]) == NB_CMD_SUCC)
		{
			nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
		{
		 nb_cmd_status = NB_CMD_FAIL;
			HAL_Delay(500);
		}
	}
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_open_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	char* pos = strchr((char*)user.add,',');
	
	strcat(buff,AT QCOAPOPEN "=0,\"");
	
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
	strcat(buff,"\r\n");
	
	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	
	user_main_debug("NBTask[_AT_COAP_OPEN].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;	
}


NB_TaskStatus nb_COAP_head_run(const char* param)
{
	try_num = 4;
	NBTask[_AT_COAP_QCOAPHEAD].set(param);	
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_COAP_QCOAPHEAD]) == NB_CMD_SUCC )
		{
			nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
		{
			nb_cmd_status = NB_CMD_FAIL;
		}
	}
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_head_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QCOAPHEAD"=0,0");
	strcat(buff,"\r\n");
	
	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);	
	user_main_debug("NBTask[_AT_COAP_QCOAPHEAD].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}
/**
	* @brief  Configure CoAP Options
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_COAP_option1_run(const char* param)
{
	try_num = 4;
	NBTask[_AT_COAP_OPTION1].set(param);	
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_COAP_OPTION1]) == NB_CMD_SUCC )
		{
			nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
		{
			nb_cmd_status = NB_CMD_FAIL;
			HAL_Delay(500);
		}
	}
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_option1_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QCOAPOPTION"=0,0,");
	strcat(buff,(char*)user.uri1);
	strcat(buff,"\r\n");
	
	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);	
	user_main_debug("NBTask[_AT_COAP_OPTION].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}
	
NB_TaskStatus nb_COAP_option2_run(const char* param)
{
	try_num = 4;
	NBTask[_AT_COAP_OPTION2].set(param);	
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_COAP_OPTION2]) == NB_CMD_SUCC )
		{
			nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
		{
			nb_cmd_status = NB_CMD_FAIL;
			HAL_Delay(500);
		}
	}
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_option2_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QCOAPOPTION"=0,0,");
	strcat(buff,(char*)user.uri2);
	strcat(buff,"\r\n");
	
	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);	
	user_main_debug("NBTask[_AT_COAP_OPTION].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_option3_run(const char* param)
{
	try_num = 4;
	NBTask[_AT_COAP_OPTION3].set(param);	
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_COAP_OPTION3]) == NB_CMD_SUCC )
		{
			nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
		{
			nb_cmd_status = NB_CMD_FAIL;
			HAL_Delay(500);
		}
	}
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_option3_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QCOAPOPTION"=0,0,");
	strcat(buff,(char*)user.uri3);
	strcat(buff,"\r\n");
	
	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);	
	user_main_debug("NBTask[_AT_COAP_OPTION].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_option4_run(const char* param)
{
	try_num = 4;
	NBTask[_AT_COAP_OPTION4].set(param);	
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_COAP_OPTION4]) == NB_CMD_SUCC )
		{
			nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
		{
			nb_cmd_status = NB_CMD_FAIL;
			HAL_Delay(500);
		}
	}
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_option4_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QCOAPOPTION"=0,0,");
	strcat(buff,(char*)user.uri4);
	strcat(buff,"\r\n");
	
	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);	
	user_main_debug("NBTask[_AT_COAP_OPTION].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}

/**
	* @brief  COAP Send DATA Config
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_COAP_send_config_run(const char* param)
{
	try_num = 2;
	NBTask[_AT_COAP_SEND_CONFIG].set(param);	
	
	if(nb_at_send(&NBTask[_AT_COAP_SEND_CONFIG]) == NB_CMD_SUCC)
	{
		nb_cmd_status = NB_CMD_SUCC;
	}
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_send_config_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QCOAPSEND"=0,1,2,0");
	strcat(buff,"\r\n");

	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_COAP_SEND_CONFIG].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}
/**
	* @brief  COAP Send DATA
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_COAP_send_run(const char* param)
{
	try_num = 4;
	NBTask[_AT_COAP_SEND].set(NULL);

	if(nb_at_send(&NBTask[_AT_COAP_SEND]) == NB_CMD_SUCC)
	{
		nb_cmd_status = NB_CMD_SUCC;
	}
	else
		nb_cmd_status = NB_CMD_FAIL;

	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_send_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	pro_data();			
	buff[strlen(buff)]=0x1A;	
	ATSendStr  = NULL;	
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_COAP_SEND].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_send_hex_run(const char* param)
{
	try_num = 2;
	NBTask[_AT_COAP_SEND_HEX].set(param);	
	
	if(nb_at_send(&NBTask[_AT_COAP_SEND_HEX]) == NB_CMD_SUCC)
	{
		nb_cmd_status = NB_CMD_SUCC;
	}
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_send_hex_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QCOAPSEND"=0,1,2,0,");
	sprintf(buff+strlen(buff), "%d,%s\r\n", sensor.data_len/2,sensor.data);

	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_COAP_SEND_HEX].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}
///**
//	* @brief  Read COAP data
//  * @param  Instruction parameter
//  * @retval None
//  */
NB_TaskStatus nb_COAP_read_run(const char* param)
{
		if(NBTask[_AT_COAP_READ].get(param) == NB_READ_DATA)
		{			
			rxPayLoadDeal(downlink_data);
		}
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_read_get(const char* param)
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
	* @brief  Delete the CoAP context
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_COAP_close_run(const char* param)
{
	try_num = 2;
	NBTask[_AT_COAP_CLOSE].set(param);
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_COAP_CLOSE]) == NB_CMD_SUCC )
		{
			nb_cmd_status = NB_CLOSE_SUCC;
			break;
		}
		else
		{
			nb_cmd_status = NB_CLOSE_FAIL;
			HAL_Delay(500);
		}
	}
	return nb_cmd_status;
}
	
NB_TaskStatus nb_COAP_close_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QCOAPCLOSE"=0");
	strcat(buff,"\r\n");

	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_COAP_CLOSE].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}

/**
	* @brief  COAP URI:Scheduling tasks via URI 
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_COAP_uri_run(const char* param)
{
	user_main_debug("uri:%s",nb.usart.data);
	//Judgment issued and received 
if(read_flag==1)
{		
	if(strstr((char*)nb.usart.data,"+QCOAPURC: 0,1,1") != NULL)
	{
		nb_COAP_read_run(NULL);
		nb_cmd_status = NB_STA_SUCC;
	}
	else
	{
		nb_cmd_status = NB_STA_SUCC;	
	}	
}	
else
{		
	if(strstr((char*)nb.usart.data,QCOAPOPEN": 0,0") != NULL)
	{
		nb_cmd_status = NB_QCOAPOPEN_SUCC;	
	}
	else
		nb_cmd_status = NB_OTHER;	
}	
	return nb_cmd_status;
}