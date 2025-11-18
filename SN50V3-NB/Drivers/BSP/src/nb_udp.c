#include "nb_udp.h"
#include "time.h"
#include <time.h>

extern char buff[2000];
extern char downlink_data[1000];
extern uint8_t read_flag;

extern uint8_t downlink_check_event;
extern uint8_t received_dwonlink_flags;
extern float hum_value;
extern float tem_value;
extern char 	*ATSendStr;
extern int 	len_string;
extern uint8_t  try_num;
extern NB_TaskStatus  nb_cmd_status;
extern uint8_t at_downlink_flag;

extern void pro_data(void);
extern void downilnk_check_data(void);
extern void downilnk_ack_data(void);
/**
	* @brief  Open UDP port operation
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_UDP_open_run(const char* param)
{
	try_num = 4;
	NBTask[_AT_UDP_OPEN].set(param);
	
	if(nb_at_send(&NBTask[_AT_UDP_OPEN]) == NB_CMD_SUCC)
		nb_cmd_status = NB_CMD_SUCC;
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;
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
	
	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	
	user_main_debug("openPayload:%s",ATSendStr);	
	return nb_cmd_status;	
}

/**
	* @brief  Send UDP data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_UDP_send_run(const char* param)
{
	try_num = 4;
	NBTask[_AT_UDP_SEND].set(param);

	if(nb_at_send(&NBTask[_AT_UDP_SEND]) == NB_CMD_SUCC)
	{
		nb_cmd_status = NB_CMD_SUCC;
	}
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;
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
		if(downlink_check_event==1)
			downilnk_check_data();
		else if(received_dwonlink_flags==1)
			downilnk_ack_data();				
		else	
      pro_data();
	}
	
	if(sys.platform==0)	
	{		
	sprintf(buff+strlen(buff), "%d,\"", sensor.data_len);		
	sprintf(buff+strlen(buff), "%s", sensor.data);
	}		
	strcat(buff,"\"\r\n");	
	
	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	
	user_main_debug("NBTask[_AT_UDP_SEND].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
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
			if(downlink_check_event==0)
			  rxPayLoadDeal(downlink_data);
		}
	return nb_cmd_status;
}

NB_TaskStatus nb_UDP_read_get(const char* param)
{
	uint8_t downlink_code=0;
	char*	end;
	char* start;
	char *pch;	
	char *pch2;
	memset(downlink_data,0,sizeof(downlink_data));	
	if(sys.downlink_debug==1)
	{
	 user_main_printf("Debug downlink data:%s",nb.usart.data);
	}	
	if(strstr((char*)nb.usart.data,"{\"Config\":\"[") != NULL)
	{
	  downlink_code=1;
	}		
	
	if(sys.platform==5 && strstr((char*)nb.usart.data,"Event:Status") != NULL)
	{
	  downlink_check_event=1;
	}	
	

		
		if(downlink_code==1)
		{
	      pch = strchr((char*)nb.usart.data,'{'); 	
		    end    = pch;
		    start  = strrchr((char*)nb.usart.data,'\"'); 
		    memcpy(downlink_data,&nb.usart.data[end-((char*)nb.usart.data)+11],(start-end)-13);	
			  at_downlink_flag=1;	
		    user_main_printf("Received downlink data:%s",downlink_data);
		    nb_cmd_status = NB_READ_DATA;		
		}
		else if(sys.downlink_1t==0)
		{
	     pch = strrchr((char*)nb.usart.data,','); 			
	     if(pch == NULL)
		     nb_cmd_status = NB_READ_NODATA;
	     else
	    {
		    end    = pch;
		    start  = strrchr((char*)nb.usart.data,'\"'); 
		    memcpy(downlink_data,&nb.usart.data[end-((char*)nb.usart.data)+2],(start-end-2));	
		    user_main_printf("Received downlink data:%s",downlink_data);
		    nb_cmd_status = NB_READ_DATA;
	    }
    }
			else if(sys.downlink_1t==1)
		{
			if(sys.platform==0)
			{
        char tem[100]={0};	
	      pch2 = strstr((char*)nb.usart.data,"recv\""); 
	      char* start2  = strchr(pch2,'\n'); 	
	      memcpy(tem,&nb.usart.data[pch2-((char*)nb.usart.data)+2],(start2-pch2-2));		
	      char* pch  = strrchr((char*)tem,','); 		
	      if(pch == NULL)
		      nb_cmd_status = NB_READ_NODATA;
	      else
	     {
	      	memset(downlink_data,0,sizeof(downlink_data));
	  	    char*	end    = pch;
	  	    char* start  = strrchr((char*)tem,'\"'); 

		      memcpy(downlink_data,&tem[end-((char*)tem)+2],(start-end-2));	
		      user_main_printf("Received downlink data:%s",downlink_data);
		
		      nb_cmd_status = NB_READ_DATA;
	     }	
	    }
			else
		 {
		      pch  = strrchr((char*)nb.usart.data,'{'); 		
	        if(pch == NULL)
		       nb_cmd_status = NB_READ_NODATA;
	        else
	       {
		       end    = pch;
		       start  = strrchr((char*)nb.usart.data,'}'); 
		       memcpy(downlink_data,&nb.usart.data[end-((char*)nb.usart.data)+1],(start-end-1));	
		       user_main_printf("Received downlink data:%s",downlink_data);
		       nb_cmd_status = NB_READ_DATA;
	       }			
		 }
	 }
	return nb_cmd_status;
}

/**
	* @brief  Close UDP port
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_UDP_close_run(const char* param)
{
	ATSendStr  = NULL;
	ATSendStr = AT QICLOSE"=0" NEWLINE;
	len_string = sizeof(AT QICLOSE"=0" NEWLINE) - 1;
	
	try_num = 3;
	NBTask[_AT_UDP_CLOSE].set(NULL);

			while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_UDP_CLOSE]) == NB_CMD_SUCC )
		{
			nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
			nb_cmd_status = NB_CMD_FAIL;
	}
		
	return nb_cmd_status;
}

NB_TaskStatus nb_UDP_close_set(const char* param)
{
	user_main_debug("NBTask[_AT_UDP_CLOSE].ATSendStr:%s",ATSendStr);
	
	return nb_cmd_status;
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
if(read_flag==1)
{		
	if(strstr((char*)nb.usart.data,"QIURC: \"recv\"") != NULL)
	{
		nb_UDP_read_run(NULL);
		nb_cmd_status = NB_STA_SUCC;
	}
	else
	{
		nb_cmd_status = NB_STA_SUCC;	
	}
}
else
{	
	if(strstr((char*)nb.usart.data,"SEND OK") != NULL)
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
}
	return nb_cmd_status;
}
