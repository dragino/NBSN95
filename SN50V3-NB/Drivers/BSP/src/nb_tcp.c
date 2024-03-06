#include "nb_tcp.h"
#include "time.h"
#include <time.h>
extern char buff[2000];
static char downlink_data[20]={0};
extern float hum_value;
extern float tem_value;
extern float ds1820_value;
extern float ds1820_value2;
extern float ds1820_value3;
extern void pro_data(void);
/**
	* @brief  Open TCP port operation
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_TCP_open_run(const char* param)
{
	NBTask[_AT_TCP_OPEN].try_num = 4;
	NBTask[_AT_TCP_OPEN].set(param);
	
	if(nb_at_send(&NBTask[_AT_TCP_OPEN]) == NB_CMD_SUCC)
		NBTask[_AT_TCP_OPEN].nb_cmd_status = NB_CMD_SUCC;
	else
		NBTask[_AT_TCP_OPEN].nb_cmd_status = NB_CMD_FAIL;
	
	return NBTask[_AT_TCP_OPEN].nb_cmd_status;
}

NB_TaskStatus nb_TCP_open_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	char* pos = strchr((char*)user.add,',');
	
	strcat(buff,AT QIOPEN "=0,0,\"TCP\",\"");
	
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
	strcat(buff,",1\r\n");
	
	NBTask[_AT_TCP_OPEN].ATSendStr = buff;
	NBTask[_AT_TCP_OPEN].len_string = strlen(NBTask[_AT_TCP_OPEN].ATSendStr);
	
	user_main_debug("openPayload:%s",NBTask[_AT_TCP_OPEN].ATSendStr);		
	return NBTask[_AT_TCP_OPEN].nb_cmd_status;
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
	uint16_t str_end,str_beg=0;
	uint16_t batteryLevel_mV=getVoltage();	
	strcat(buff,AT QISEND "=");
	strcat(buff,"0,");
	if(sys.platform==5)	
	{
   pro_data();
	}
	if(sys.platform==0)		
	{
	sprintf(buff+strlen(buff), "%d", sensor.data_len);		
	strcat(buff,",\"");
	sprintf(buff+strlen(buff), "%s", sensor.data);	
	}
	strcat(buff,"\"\r\n");
	
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

NB_TaskStatus nb_TCP_read_get(const char* param)
{
	char *pch = strrchr((char*)nb.usart.data,','); 
	if(pch == NULL)
		NBTask[_AT_TCP_READ].nb_cmd_status = NB_READ_NODATA;
	else
	{
		memset(downlink_data,0,sizeof(downlink_data));
		char*	end    = pch;
		char* start  = strrchr((char*)nb.usart.data,'\"'); 

		memcpy(downlink_data,&nb.usart.data[end-((char*)nb.usart.data)+2],(start-end-2));	
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
	NBTask[_AT_TCP_CLOSE].try_num = 3;
	NBTask[_AT_TCP_CLOSE].set(NULL);

		while(NBTask[_AT_TCP_CLOSE].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_TCP_CLOSE]) == NB_CMD_SUCC )
		{
			NBTask[_AT_TCP_CLOSE].nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
			NBTask[_AT_TCP_CLOSE].nb_cmd_status = NB_CMD_FAIL;
	}		
	return NBTask[_AT_TCP_CLOSE].nb_cmd_status;
}
	
NB_TaskStatus nb_TCP_close_set(const char* param)
{
	user_main_debug("NBTask[_AT_TCP_CLOSE].ATSendStr:%s",NBTask[_AT_TCP_CLOSE].ATSendStr);
	
	return NBTask[_AT_TCP_CLOSE].nb_cmd_status;
}

NB_TaskStatus nb_TCP_uri_run(const char* param)
{
	user_main_debug("uri:%s",nb.usart.data);
//	if(strstr((char*)nb.usart.data,"QIOPEN:") != NULL && strstr((char*)nb.usart.data,"0,0") != NULL)
//	{
//		NBTask[_AT_TCP_URI].nb_cmd_status = NB_OPEN_SUCC;	
//	}
//Judgment issued and received 
	if(strstr((char*)nb.usart.data,"QIURC: \"recv\"") != NULL)
	{
		nb_TCP_read_run(NULL);
		NBTask[_AT_TCP_URI].nb_cmd_status = NB_STA_SUCC;
	}
	else
	{
		NBTask[_AT_TCP_URI].nb_cmd_status = NB_STA_SUCC;	
	}
	return NBTask[_AT_TCP_URI].nb_cmd_status;
}
