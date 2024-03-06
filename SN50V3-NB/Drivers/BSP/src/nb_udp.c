#include "nb_udp.h"
#include "time.h"
#include <time.h>
extern char buff[2000];
static char downlink_data[50]={0};
extern uint8_t read_flag;
extern float hum_value;
extern float tem_value;
extern float ds1820_value;
extern float ds1820_value2;
extern float ds1820_value3;
extern void pro_data(void);
/**
	* @brief  Open UDP port operation
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_UDP_open_run(const char* param)
{
	NBTask[_AT_UDP_OPEN].try_num = 4;
	NBTask[_AT_UDP_OPEN].set(param);
	
	if(nb_at_send(&NBTask[_AT_UDP_OPEN]) == NB_CMD_SUCC)
		NBTask[_AT_UDP_OPEN].nb_cmd_status = NB_CMD_SUCC;
	else
		NBTask[_AT_UDP_OPEN].nb_cmd_status = NB_CMD_FAIL;
	
	return NBTask[_AT_UDP_OPEN].nb_cmd_status;
}

NB_TaskStatus nb_UDP_open_set(const char* param)
{	
	memset(buff,0,sizeof(buff));
	char* pos = strchr((char*)user.add,',');
	
	strcat(buff,AT QIOPEN "=0,0,\"UDP SERVICE\",\"");
	
			if(strlen((char*)user.add_ip)!=0)
	{	
	  char* pos = strchr((char*)user.add_ip,',');		
	  memcpy(buff+strlen(buff),user.add_ip,(pos-(char*)user.add_ip));	
	}
	else
	{
	   memcpy(buff+strlen(buff),user.add,(pos-(char*)user.add));	
	}
	strcat(buff,"\",1");
	strcat(buff,(char*)&user.add[(pos-(char*)user.add)]);
	strcat(buff,",1\r\n");
	
	NBTask[_AT_UDP_OPEN].ATSendStr = buff;
	NBTask[_AT_UDP_OPEN].len_string = strlen(NBTask[_AT_UDP_OPEN].ATSendStr);
	
	user_main_debug("openPayload:%s",NBTask[_AT_UDP_OPEN].ATSendStr);	
	return NBTask[_AT_UDP_OPEN].nb_cmd_status;	
}

/**
	* @brief  Send UDP data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_UDP_send_run(const char* param)
{
	NBTask[_AT_UDP_SEND].try_num = 4;
	NBTask[_AT_UDP_SEND].set(param);

	if(nb_at_send(&NBTask[_AT_UDP_SEND]) == NB_CMD_SUCC)
	{
		NBTask[_AT_UDP_SEND].nb_cmd_status = NB_CMD_SUCC;
	}
	else
		NBTask[_AT_UDP_SEND].nb_cmd_status = NB_CMD_FAIL;
	
	return NBTask[_AT_UDP_SEND].nb_cmd_status;
}

NB_TaskStatus nb_UDP_send_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QISEND "=");
	strcat(buff,"0");
	strcat(buff,",\"");
	if(strlen((char*)user.add_ip)!=0)
	{
	  char* pos = strchr((char*)user.add_ip,',');		
	  memcpy(buff+strlen(buff),user.add_ip,(pos-(char*)user.add_ip));
	  strcat(buff,"\"");
	  strcat(buff,(char*)&user.add_ip[(pos-(char*)user.add_ip)]);			
//		strcat(buff,(char*)user.add_ip);
	}
	else
	{
	  char* pos = strchr((char*)user.add,',');		
	  memcpy(buff+strlen(buff),user.add,(pos-(char*)user.add));
	  strcat(buff,"\"");
	  strcat(buff,(char*)&user.add[(pos-(char*)user.add)]);		
//		strcat(buff,(char*)user.add);
	}
	strcat(buff,",");
	if(sys.platform==5)	
	{
   pro_data();
	}
	if(sys.platform==0)	
	{		
	sprintf(buff+strlen(buff), "%d,\"", sensor.data_len);		
	sprintf(buff+strlen(buff), "%s", sensor.data);
	}		
	strcat(buff,"\"\r\n");	
	
	NBTask[_AT_UDP_SEND].ATSendStr = buff;
	NBTask[_AT_UDP_SEND].len_string = strlen(NBTask[_AT_UDP_SEND].ATSendStr);
	
	user_main_debug("NBTask[_AT_UDP_SEND].ATSendStr:%s",NBTask[_AT_UDP_SEND].ATSendStr);
	return NBTask[_AT_UDP_SEND].nb_cmd_status;
}

///**
//	* @brief  Read UDP data
//  * @param  Instruction parameter
//  * @retval None
//  */
NB_TaskStatus nb_UDP_read_run(const char* param)
{
		if(NBTask[_AT_UDP_READ].get(param) == NB_READ_DATA)
		{			
			rxPayLoadDeal(downlink_data);
		}
	return NBTask[_AT_UDP_READ].nb_cmd_status;
}

NB_TaskStatus nb_UDP_read_get(const char* param)
{
	char *pch = strrchr((char*)nb.usart.data,','); 
	if(pch == NULL)
		NBTask[_AT_UDP_READ].nb_cmd_status = NB_READ_NODATA;
	else
	{
		memset(downlink_data,0,sizeof(downlink_data));
		char*	end    = pch;
		char* start  = strrchr((char*)nb.usart.data,'\"'); 

		memcpy(downlink_data,&nb.usart.data[end-((char*)nb.usart.data)+2],(start-end-2));	
		user_main_printf("Received downlink data:%s",downlink_data);
		
		NBTask[_AT_UDP_READ].nb_cmd_status = NB_READ_DATA;
	}
	return NBTask[_AT_UDP_READ].nb_cmd_status;
}
/**
	* @brief  Close UDP port
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_UDP_close_run(const char* param)
{
	NBTask[_AT_UDP_CLOSE].try_num = 3;
	NBTask[_AT_UDP_CLOSE].set(NULL);

			while(NBTask[_AT_UDP_CLOSE].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_UDP_CLOSE]) == NB_CMD_SUCC )
		{
			NBTask[_AT_UDP_CLOSE].nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
			NBTask[_AT_UDP_CLOSE].nb_cmd_status = NB_CMD_FAIL;
	}
		
	return NBTask[_AT_UDP_CLOSE].nb_cmd_status;
}

NB_TaskStatus nb_UDP_close_set(const char* param)
{
	user_main_debug("NBTask[_AT_UDP_CLOSE].ATSendStr:%s",NBTask[_AT_UDP_CLOSE].ATSendStr);
	
	return NBTask[_AT_UDP_CLOSE].nb_cmd_status;
}

/**
	* @brief  UDP URI:Scheduling tasks via URI 
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_UDP_uri_run(const char* param)
{
	user_main_debug("uri:%s",nb.usart.data);
	
	//Judgment issued and received 
	if(strstr((char*)nb.usart.data,"QIURC: \"recv\"") != NULL)
	{
		nb_UDP_read_run(NULL);
		NBTask[_AT_UDP_URI].nb_cmd_status = NB_STA_SUCC;
	}
	else
	{
		NBTask[_AT_UDP_URI].nb_cmd_status = NB_STA_SUCC;	
	}

if(read_flag==0)
{	
	if(strstr((char*)nb.usart.data,"SEND OK") != NULL)
	{
		NBTask[_AT_UDP_URI].nb_cmd_status = NB_SEND_SUCC;	
	}
	else if(strstr((char*)nb.usart.data,"SEND FAIL") != NULL)
	{
		NBTask[_AT_UDP_URI].nb_cmd_status = NB_SEND_FAIL;	
	}
	else
	{
		NBTask[_AT_UDP_URI].nb_cmd_status = NB_OTHER;	
	}	
}
	return NBTask[_AT_UDP_URI].nb_cmd_status;
}