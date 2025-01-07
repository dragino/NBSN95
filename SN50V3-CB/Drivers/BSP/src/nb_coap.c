#include "nb_coap.h"
#include "time.h"
#include <time.h>
extern char buff[2000];
extern char downlink_data[1000];
extern float hum_value;
extern float tem_value;
extern char 	*ATSendStr;
extern int 	len_string;
extern uint8_t  try_num;
extern NB_TaskStatus  nb_cmd_status;
extern void pro_data(void);
extern char record_log[512];
extern uint16_t pro_data_num;
/**
	* @brief  Configure to show the CoAP option of sender
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_COAP_config_run(const char* param)
{
	try_num = 2;
	NBTask[_AT_COAP_CONFIG].set(param);
	
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_COAP_CONFIG]) == NB_CMD_SUCC )
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

NB_TaskStatus nb_COAP_config_set(const char* param)
{
	ATSendStr  = NULL;
	ATSendStr = AT QCOAPCFG "=\"pdpcid\",0,1" NEWLINE;
	len_string = sizeof(AT QCOAPCFG "=\"pdpcid\",0,1" NEWLINE) - 1;
	
	user_main_debug("NBTask[_AT_COAP_CONFIG].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_config_get(const char* param)
{
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
		if(nb_at_send(&NBTask[_AT_COAP_OPEN]) == NB_CMD_SUCC )
		{
			nb_cmd_status = NB_OPEN_SUCC;
			break;
		}
		else
		{
			nb_cmd_status = NB_OPEN_FAIL;
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

NB_TaskStatus nb_COAP_open_get(const char* param)
{
	return nb_cmd_status;
}


NB_TaskStatus nb_COAP_header_run(const char* param)
{
	try_num = 2;
	NBTask[_AT_COAP_HEADER].set(param);
	
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_COAP_HEADER]) == NB_CMD_SUCC )
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

NB_TaskStatus nb_COAP_header_set(const char* param)
{
	ATSendStr  = NULL;
	ATSendStr = AT QCOAPHEADER "=0,1234,1" NEWLINE;
	len_string = sizeof(AT QCOAPHEADER "=0,1234,1" NEWLINE) - 1;
	
	user_main_debug("NBTask[_AT_COAP_HEADER].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_header_get(const char* param)
{
	return nb_cmd_status;
}

/**
	* @brief  Configure CoAP Options
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_COAP_option1_run(const char* param)
{
	try_num = 0;
	NBTask[_AT_COAP_OPTION1].set(param);
	
		if(nb_at_send(&NBTask[_AT_COAP_OPTION1]) == NB_CMD_SUCC )
		{
			nb_cmd_status = NB_CMD_SUCC;
		}
		else
		{
			nb_cmd_status = NB_CMD_FAIL;
		}
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_option1_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QCOAPOPTION"=0,0,0,");
	strcat(buff,(char*)user.uri1);
	strcat(buff,"\r\n");
	
	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_COAP_OPTION1].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}
	
NB_TaskStatus nb_COAP_option1_get(const char* param)
{
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_option2_run(const char* param)
{
	try_num = 0;
	NBTask[_AT_COAP_OPTION2].set(param);
	
		if(nb_at_send(&NBTask[_AT_COAP_OPTION2]) == NB_CMD_SUCC )
		{
			nb_cmd_status = NB_CMD_SUCC;
		}
		else
		{
			nb_cmd_status = NB_CMD_FAIL;
		}
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_option2_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QCOAPOPTION"=0,0,1,");
	strcat(buff,(char*)user.uri2);
	strcat(buff,"\r\n");
	
	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_COAP_OPTION2].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}
	
NB_TaskStatus nb_COAP_option2_get(const char* param)
{
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_option3_run(const char* param)
{
	try_num = 0;
	NBTask[_AT_COAP_OPTION3].set(param);
	
		if(nb_at_send(&NBTask[_AT_COAP_OPTION3]) == NB_CMD_SUCC )
		{
			nb_cmd_status = NB_CMD_SUCC;
		}
		else
		{
			nb_cmd_status = NB_CMD_FAIL;
		}
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_option3_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QCOAPOPTION"=0,0,2,");
	strcat(buff,(char*)user.uri3);
	strcat(buff,"\r\n");
	
	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_COAP_OPTION3].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}
	
NB_TaskStatus nb_COAP_option3_get(const char* param)
{
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_option4_run(const char* param)
{
	try_num = 0;
	NBTask[_AT_COAP_OPTION4].set(param);
	
		if(nb_at_send(&NBTask[_AT_COAP_OPTION4]) == NB_CMD_SUCC )
		{
			nb_cmd_status = NB_CMD_SUCC;
		}
		else
		{
			nb_cmd_status = NB_CMD_FAIL;
		}
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_option4_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QCOAPOPTION"=0,0,3,");
	strcat(buff,(char*)user.uri4);
	strcat(buff,"\r\n");
	
	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_COAP_OPTION4].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}
	
NB_TaskStatus nb_COAP_option4_get(const char* param)
{
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_option5_run(const char* param)
{
	try_num = 0;
	NBTask[_AT_COAP_OPTION5].set(param);
	
		if(nb_at_send(&NBTask[_AT_COAP_OPTION5]) == NB_CMD_SUCC )
		{
			nb_cmd_status = NB_CMD_SUCC;
		}
		else
		{
			nb_cmd_status = NB_CMD_FAIL;
		}
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_option5_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QCOAPOPTION"=0,0,4,");
	strcat(buff,(char*)user.uri5);
	strcat(buff,"\r\n");
	
	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_COAP_OPTION5].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}
	
NB_TaskStatus nb_COAP_option5_get(const char* param)
{
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_option6_run(const char* param)
{
	try_num = 0;
	NBTask[_AT_COAP_OPTION6].set(param);
	
		if(nb_at_send(&NBTask[_AT_COAP_OPTION6]) == NB_CMD_SUCC )
		{
			nb_cmd_status = NB_CMD_SUCC;
		}
		else
		{
			nb_cmd_status = NB_CMD_FAIL;
		}
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_option6_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QCOAPOPTION"=0,0,5,");
	strcat(buff,(char*)user.uri6);
	strcat(buff,"\r\n");

	ATSendStr  = NULL;	
	ATSendStr  = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_COAP_OPTION6].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}
	
NB_TaskStatus nb_COAP_option6_get(const char* param)
{
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_option7_run(const char* param)
{
	try_num = 0;
	NBTask[_AT_COAP_OPTION7].set(param);
	
		if(nb_at_send(&NBTask[_AT_COAP_OPTION7]) == NB_CMD_SUCC )
		{
			nb_cmd_status = NB_CMD_SUCC;
		}
		else
		{
			nb_cmd_status = NB_CMD_FAIL;
		}
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_option7_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QCOAPOPTION"=0,0,6,");
	strcat(buff,(char*)user.uri7);
	strcat(buff,"\r\n");

	ATSendStr  = NULL;	
	ATSendStr  = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_COAP_OPTION7].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}
	
NB_TaskStatus nb_COAP_option7_get(const char* param)
{
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_option8_run(const char* param)
{
	try_num = 0;
	NBTask[_AT_COAP_OPTION8].set(param);
	
		if(nb_at_send(&NBTask[_AT_COAP_OPTION8]) == NB_CMD_SUCC )
		{
			nb_cmd_status = NB_CMD_SUCC;
		}
		else
		{
			nb_cmd_status = NB_CMD_FAIL;
		}
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_option8_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QCOAPOPTION"=0,0,7,");
	strcat(buff,(char*)user.uri8);
	strcat(buff,"\r\n");

	ATSendStr  = NULL;	
	ATSendStr  = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_COAP_OPTION8].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}
	
NB_TaskStatus nb_COAP_option8_get(const char* param)
{
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
	NBTask[_AT_COAP_SEND_CONFIG].set(NULL);
	
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
  pro_data();	
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QCOAPSEND"=0,1,2,255");
	strcat(buff,"\r\n");
	
	ATSendStr  = NULL;
	ATSendStr  = buff;
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
  if(sys.platform==0)	
	{
	 ATSendStr  = NULL;
	 ATSendStr  = sensor.data;
	 len_string = sensor.data_len;
	}
  else
	{
   pro_data();	
	 ATSendStr  = NULL;
	 buff[strlen(buff)]=0x1A;
	 ATSendStr  = buff;
	 len_string = strlen(ATSendStr);		
	}
	
	user_main_debug("NBTask[_AT_COAP_SEND].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}
//////////////////////////////////
NB_TaskStatus nb_COAP_read_run(const char* param)
{
	if(nb_COAP_read_get(param) == NB_READ_DATA)
	{
		rxPayLoadDeal(downlink_data);
	}
	return nb_cmd_status;
}
	
NB_TaskStatus nb_COAP_read_get(const char* param)
{	
		memset(downlink_data,0,sizeof(downlink_data));	
	if(sys.downlink_debug==1)
	{
	 user_main_printf("Debug downlink data:%s",nb.usart.data);
	}	
		char *pch; 
	  char* start;	
	  start  = strstr((char*)nb.usart.data,QCOAPRECV);
		char *pch2 = strstr((char*)start,"\r\n"); 
	if(pch2 == NULL)
	{
		 nb_cmd_status = NB_READ_NODATA;
	}
	else
	 {
		 	start  = strrchr((char*)pch2,'\n'); 
		 memset(downlink_data,0,sizeof(downlink_data));
		 memcpy(downlink_data,&nb.usart.data[pch2 - ((char*)nb.usart.data)+2],start-pch2-2);
		 user_main_printf("Received downlink data:%s",downlink_data);
		 sprintf(record_log+strlen(record_log), "Received downlink data:%s\r\n",downlink_data);			 
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
		}
	}
	return nb_cmd_status;
}
	
NB_TaskStatus nb_COAP_close_set(const char* param)
{
	ATSendStr  = NULL;
	ATSendStr = AT QCOAPCLOSE "=0" NEWLINE;
	len_string = sizeof(AT QCOAPCLOSE "=0" NEWLINE) - 1;
	
	user_main_debug("NBTask[_AT_COAP_CLOSE].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}

NB_TaskStatus nb_COAP_close_get(const char* param)
{
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
	
	if(strstr((char*)nb.usart.data,QCOAPOPEN": 0,0") != NULL)
	{
		nb_cmd_status = NB_QCOAPOPEN_SUCC;	
	}
	else if(strstr((char*)nb.usart.data,"+QCOAPRECV") != NULL)
	{
		nb_COAP_read_run(NULL);
		nb_cmd_status = NB_RD_SUCC;
	}
	else
		nb_cmd_status = NB_OTHER;	
	
	return nb_cmd_status;
}