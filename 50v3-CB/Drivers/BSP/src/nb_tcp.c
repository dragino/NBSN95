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
extern void pro_data(void);
extern char record_log[512];
extern char 	*ATSendStr;
extern int 	len_string;
extern uint8_t  try_num;
extern NB_TaskStatus  nb_cmd_status;
extern uint16_t pro_data_num;
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
	
	strcat(buff,AT QIOPEN "=1,0,\"TCP\",\"");
	
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
	strcat(buff,",0\r\n");
	
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
  pro_data();	
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QISEND "=");
	strcat(buff,"0,");
  sprintf(buff+strlen(buff),"%d",pro_data_num);
	strcat(buff,"\r\n");
	
	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	
	user_main_debug("NBTask[_AT_TCP_SEND].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}


/**
	* @brief  Send data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_TCP_data_run(const char* param)
{
	try_num = 4;
	NBTask[_AT_TCP_DATA].set(NULL);

	if(nb_at_send(&NBTask[_AT_TCP_DATA]) == NB_CMD_SUCC)
	{
		nb_cmd_status = NB_CMD_SUCC;
	}
	else
		nb_cmd_status = NB_CMD_FAIL;

	return nb_cmd_status;
}

NB_TaskStatus nb_TCP_data_set(const char* param)
{	
  pro_data();	
	ATSendStr  = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_TCP_DATA].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}

/**
	* @brief  Send TCP data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_TCP_sendex_run(const char* param)
{
	try_num = 4;
	NBTask[_AT_TCP_SENDEX].set(param);

	if(nb_at_send(&NBTask[_AT_TCP_SENDEX]) == NB_CMD_SUCC)
	{
		nb_cmd_status = NB_CMD_SUCC;
	}
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;
}

NB_TaskStatus nb_TCP_sendex_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QISENDEX "=");
	strcat(buff,"0,\"");	
	sprintf(buff+strlen(buff), "%s", sensor.data);	
	strcat(buff,"\"\r\n");	
	
	ATSendStr  = NULL;	
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_TCP_SENDEX].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}

/**
	* @brief  QIRD
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_TCP_qird_run(const char* param)
{
	try_num = 4;
	NBTask[_AT_TCP_QIRD].set(param);

	if(nb_at_send(&NBTask[_AT_TCP_QIRD]) == NB_CMD_SUCC)
	{
		nb_cmd_status = NB_CMD_SUCC;
	}
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;
}

NB_TaskStatus nb_TCP_qird_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QIRD "=0,1500");
	strcat(buff,"\r\n");	
	
	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_TCP_QIRD].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}

/**
	* @brief  Read TCP data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_TCP_read_run(const char* param)
{
		if(NBTask[_AT_TCP_READ].get(param) == NB_READ_DATA)
		{			
			rxPayLoadDeal(downlink_data);
		}
	return nb_cmd_status;
}

NB_TaskStatus nb_TCP_read_get(const char* param)
{
	if(sys.downlink_debug==1)
	{
	 user_main_printf("Debug downlink data:%s",nb.usart.data);
	}	
		memset(downlink_data,0,sizeof(downlink_data));
		char *pch,*pch3,*pch4; 
	  char* start;
	  char strings[200];
	  pch4 = strstr((char*)nb.usart.data,"QIRD"); 
	  pch3  = strrchr((char*)pch4,'K'); 
	  memcpy(strings,&nb.usart.data[pch4 - ((char*)nb.usart.data)],pch3-pch4);
		pch = strrchr((char*)strings,':'); 
	  start  = strrchr((char*)strings,'\n'); 
		memcpy(downlink_data,&strings[pch - ((char*)strings)+2],start-pch-5);
	if(downlink_data[0]=='0')
	{
		nb_cmd_status = NB_READ_NODATA;
		user_main_printf("No data in buffer");
		sprintf(record_log+strlen(record_log), "No data in buffer\r\n");	
	  return nb_cmd_status;
	}
	else
	{
		 char *pch2 = strchr((char*)pch,'\n'); 
	 if(pch2 == NULL)
		 nb_cmd_status = NB_READ_NODATA;
	 else
	 {
		 	start  = strrchr((char*)pch2,'O'); 
		 memset(downlink_data,0,sizeof(downlink_data));
		 memcpy(downlink_data,&nb.usart.data[pch2 - ((char*)nb.usart.data)+1],start-pch2-2);
		 user_main_printf("Received downlink data:%s",downlink_data);
		 sprintf(record_log+strlen(record_log), "Received downlink data:%s\r\n",downlink_data);			 
		 nb_cmd_status = NB_READ_DATA;
	 }
	  return nb_cmd_status;
  }
}

/**
	* @brief  Close TCP port
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_TCP_close_run(const char* param)
{
	ATSendStr  = NULL;
	ATSendStr  = AT QICLOSE"=0" NEWLINE;
	len_string = sizeof(AT QICLOSE"=0" NEWLINE) - 1;
	
	try_num = 2;
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
	
	//Judgment issued and received 
	
	if(strstr((char*)nb.usart.data,"+QIRD:") != NULL)
	{
		nb_TCP_read_run(NULL);
		nb_cmd_status = NB_RD_SUCC;
	}
	else if(strstr((char*)nb.usart.data,"SEND OK") != NULL)
	{
		nb_cmd_status = NB_SEND_SUCC;	
	}
	else if(strstr((char*)nb.usart.data,"SEND FAIL") != NULL)
	{
		nb_cmd_status = NB_SEND_FAIL;	
	}
	else
	{
		nb_cmd_status = NB_OTHER;	
	}	
	return nb_cmd_status;
}
