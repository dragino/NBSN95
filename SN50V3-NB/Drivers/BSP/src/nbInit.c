#include "nbInit.h"
#include "gpio.h"
static uint8_t net_acc_status_led = 0;
static char buff[200]={0};
static uint8_t	recieve_data[NB_RX_SIZE] = {0};	   	 			//Receive data
extern uint8_t join_network_num;				  
extern uint8_t join_network_flag;	
extern uint8_t join_network_time;
extern uint8_t join_network_timer;
extern uint8_t error_num;
extern uint8_t  rxbuf;
uint8_t reupload_time = 0;
bool  succes_Status=false;
extern uint8_t nds_timer_flag2;
extern uint8_t is_time_to_send;
extern uint8_t sleep_status;
extern uint8_t nbmodel_int;
extern uint8_t  qband_flag;
static uint8_t mqtt_close_flag = 0;
static uint8_t resend_flag = 0;
static uint8_t no_response_flag = 0;
static uint8_t no_response_time = 0;
uint8_t read_flag = 0;
char record_log[512]={0};
bool DNS_RE_FLAG = false;
bool first_sample=0;
static uint8_t tcp_fail_flag = 0;
static uint8_t csq_fail_log = 0;
static uint8_t two_data_log = 0;
extern void OnTxTimerEvent( void );
extern void nb_intTimeoutEvent( void );
extern TimerEvent_t TxTimer;
extern TimerEvent_t nb_intTimeoutTimer;
extern void compare_time(uint16_t time);
extern TimerEvent_t timesampleTimer;
extern void OntimesampleEvent(void);

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
	NBTask[_AT].try_num = 3;
	while(NBTask[_AT].try_num--)
	{
		if(nb_at_send(&NBTask[_AT]) == NB_CMD_SUCC )
		{
			break;
		}
	}
	
	return NBTask[_AT].nb_cmd_status;
}
/**
	* @brief  Echo mode OFF
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_ate_run(const char* param)
{
	NBTask[_ATE].try_num = 3;
	while(NBTask[_ATE].try_num--)
	{
		if(nb_at_send(&NBTask[_ATE]) == NB_CMD_SUCC )
		{
			break;
		}
	}
	
	return NBTask[_ATE].nb_cmd_status;
}

/**
	* @brief  AT+QCFGEV : Disable the reporting of deep sleep event URC. 
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_qcfgev_run(const char* param)
{
	NBTask[_AT_QCFGEV].set(param);
	NBTask[_AT_QCFGEV].try_num = 3;
	while(NBTask[_AT_QCFGEV].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_QCFGEV])== NB_CMD_SUCC)
		{
			NBTask[_AT_QCFGEV].nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
			NBTask[_AT_QCFGEV].nb_cmd_status = NB_CMD_FAIL;
	}
	return NBTask[_AT_QCFGEV].nb_cmd_status;
}
NB_TaskStatus nb_qcfgev_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	
	strcat(buff,AT QCFGEV "=\"dsevent\",0");
	strcat(buff,NEWLINE);
	
	NBTask[_AT_QCFGEV].ATSendStr  = buff;
	NBTask[_AT_QCFGEV].len_string = strlen(NBTask[_AT_QCFGEV].ATSendStr);
	user_main_debug("NBTask[_AT_QCFGEV].ATSendStr:%s",NBTask[_AT_QCFGEV].ATSendStr);
	
	return NBTask[_AT_QCFGEV].nb_cmd_status;
}

/**
	* @brief  AT_CGSN :Get IMEI number
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_cgsn_get(const char* param)
{
	NBTask[_AT_IMEI].try_num = 3;
	while(NBTask[_AT_IMEI].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_IMEI])== NB_CMD_SUCC)
		{
			memset(nb.imei,0,sizeof((nb.imei)));
			char *pch = strchr((char*)nb.usart.data,':');
			char *pch1 = strchr((char*)nb.usart.data,'O');
			if(pch != NULL && pch1!=NULL)
				strncpy((char*)nb.imei, pch +2,pch1 - pch -6);
			break;
		}
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
	NBTask[_AT_IMSI].try_num = 3;
	while(NBTask[_AT_IMSI].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_IMSI])== NB_CMD_SUCC)
		{
			memset(nb.imsi,0,sizeof((nb.imsi)));
			char *pch = strchr((char*)nb.usart.data,'\n');
			char *pch1 = strchr((char*)nb.usart.data,'O');
			if(pch != NULL && pch1!=NULL)
				strncpy((char*)nb.imsi, pch +1,pch1 - pch -5);
			break;
		}
	}
	return NBTask[_AT_IMSI].nb_cmd_status;
}
/**
	* @brief  AT_CFUNSTA : close UE Functionality
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_cfunsta_run(const char* param)
{
	NBTask[_AT_CFUNSTA].try_num = 10;
	NBTask[_AT_CFUNSTA].set(param);
	
	while(NBTask[_AT_CFUNSTA].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_CFUNSTA]) == NB_CMD_SUCC )
		{
			break;
		}
	}
	
	return NBTask[_AT_CFUNSTA].nb_cmd_status;
}

NB_TaskStatus nb_cfunsta_get(const char* param)
{
	return NBTask[_AT_CFUNSTA].nb_cmd_status;
}

NB_TaskStatus nb_cfunsta_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT CFUNSTA "=1" NEWLINE);

	NBTask[_AT_CFUNSTA].ATSendStr  = buff;
	NBTask[_AT_CFUNSTA].len_string = strlen(NBTask[_AT_CFUNSTA].ATSendStr);
	user_main_debug("NBTask[_AT_CFUNSTA].ATSendStr:%s",NBTask[_AT_CFUNSTA].ATSendStr);

	return nb_at_send(&NBTask[_AT_CFUNSTA]);
}


/**
	* @brief  AT_CFUNOFF : close UE Functionality
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_cfunoff_run(const char* param)
{
	NBTask[_AT_CFUNOFF].try_num = 10;
	NBTask[_AT_CFUNOFF].set(param);
	
	while(NBTask[_AT_CFUNOFF].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_CFUNOFF]) == NB_CMD_SUCC )
		{
			break;
		}
	}
	
	return NBTask[_AT_CFUNOFF].nb_cmd_status;
}

NB_TaskStatus nb_cfunoff_get(const char* param)
{
	return NBTask[_AT_CFUNOFF].nb_cmd_status;
}

NB_TaskStatus nb_cfunoff_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT CFUNOFF "=0" NEWLINE);

	NBTask[_AT_CFUNOFF].ATSendStr  = buff;
	NBTask[_AT_CFUNOFF].len_string = strlen(NBTask[_AT_CFUNOFF].ATSendStr);
	user_main_debug("NBTask[_AT_CFUNOFF].ATSendStr:%s",NBTask[_AT_CFUNOFF].ATSendStr);

	return nb_at_send(&NBTask[_AT_CFUNOFF]);
}


/**
	* @brief  AT_CFUNEND : close UE Functionality
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_cfun_run(const char* param)
{
	NBTask[_AT_CFUNEND].try_num = 10;
	NBTask[_AT_CFUNEND].set(param);
	
	while(NBTask[_AT_CFUNEND].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_CFUNEND]) == NB_CMD_SUCC )
		{
			break;
		}
	}
	
	return NBTask[_AT_CFUNEND].nb_cmd_status;
}

NB_TaskStatus nb_cfun_get(const char* param)
{
	return NBTask[_AT_CFUNEND].nb_cmd_status;
}

NB_TaskStatus nb_cfun_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT CFUNEND "=0" NEWLINE);

	NBTask[_AT_CFUNEND].ATSendStr  = buff;
	NBTask[_AT_CFUNEND].len_string = strlen(NBTask[_AT_CFUNEND].ATSendStr);
	user_main_debug("NBTask[_AT_CFUNEND].ATSendStr:%s",NBTask[_AT_CFUNEND].ATSendStr);

	return nb_at_send(&NBTask[_AT_CFUNEND]);
}


NB_TaskStatus nb_qsclk_run(const char* param)
{
	NBTask[_AT_QSCLK].try_num = 10;
	NBTask[_AT_QSCLK].set(param);
	
	while(NBTask[_AT_QSCLK].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_QSCLK]) == NB_CMD_SUCC )
		{
			break;
		}
	}
	
	return NBTask[_AT_QSCLK].nb_cmd_status;
}

NB_TaskStatus nb_qsclk_get(const char* param)
{
	return NBTask[_AT_QSCLK].nb_cmd_status;
}

NB_TaskStatus nb_qsclk_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QSCLK "=1" NEWLINE);

	NBTask[_AT_QSCLK].ATSendStr  = buff;
	NBTask[_AT_QSCLK].len_string = strlen(NBTask[_AT_QSCLK].ATSendStr);
	user_main_debug("NBTask[_AT_QSCLK].ATSendStr:%s",NBTask[_AT_QSCLK].ATSendStr);

	return nb_at_send(&NBTask[_AT_QSCLK]);
}


NB_TaskStatus nb_qicfg_run(const char* param)
{
	NBTask[_AT_QICFG].try_num = 10;
	NBTask[_AT_QICFG].set(param);
	
	while(NBTask[_AT_QICFG].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_QICFG]) == NB_CMD_SUCC )
		{
			break;
		}
	}
	
	return NBTask[_AT_QICFG].nb_cmd_status;
}

NB_TaskStatus nb_qicfg_get(const char* param)
{
	return NBTask[_AT_QICFG].nb_cmd_status;
}

NB_TaskStatus nb_qicfg_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	if(sys.platform==0)
	strcat(buff,AT QICFG "=dataformat,1,1" NEWLINE);
  else
	strcat(buff,AT QICFG "=dataformat,0,1" NEWLINE);		
	NBTask[_AT_QICFG].ATSendStr  = buff;
	NBTask[_AT_QICFG].len_string = strlen(NBTask[_AT_QICFG].ATSendStr);
	user_main_debug("NBTask[_AT_QICFG].ATSendStr:%s",NBTask[_AT_QICFG].ATSendStr);

	return nb_at_send(&NBTask[_AT_QICFG]);
}

NB_TaskStatus nb_qsclkoff_run(const char* param)
{
	NBTask[_AT_QSCLKOFF].try_num = 10;
	NBTask[_AT_QSCLKOFF].set(param);
	
	while(NBTask[_AT_QSCLKOFF].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_QSCLKOFF]) == NB_CMD_SUCC )
		{
			break;
		}
	}
	
	return NBTask[_AT_QSCLKOFF].nb_cmd_status;
}

NB_TaskStatus nb_qsclkoff_get(const char* param)
{
	return NBTask[_AT_QSCLKOFF].nb_cmd_status;
}

NB_TaskStatus nb_qsclkoff_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QSCLKOFF "=0" NEWLINE);

	NBTask[_AT_QSCLKOFF].ATSendStr  = buff;
	NBTask[_AT_QSCLKOFF].len_string = strlen(NBTask[_AT_QSCLKOFF].ATSendStr);
	user_main_debug("NBTask[_AT_QSCLKOFF].ATSendStr:%s",NBTask[_AT_QSCLKOFF].ATSendStr);

	return nb_at_send(&NBTask[_AT_QSCLKOFF]);
}
/**
	* @brief  AT_NBAND : Get/Set frequency band
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_qband_run(const char* param)
{
	NBTask[_AT_QBAND].try_num = 3;
	NBTask[_AT_QBAND].set(NULL);

	while(NBTask[_AT_QBAND].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_QBAND])== NB_CMD_SUCC)
		{
			if(qband_flag==0)
      {
			NBTask[_AT_QBAND].nb_cmd_status = NBTask[_AT_QBAND].get(NULL);
			break;
			}
			qband_flag=0;
			NBTask[_AT_QBAND].set(NULL);
		}
	}
	
	return NBTask[_AT_QBAND].nb_cmd_status;
}

NB_TaskStatus nb_qband_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	if(qband_flag==1)
	strcat(buff,AT QBAND "=10,8,20,28,2,4,12,13,66,85,5" NEWLINE);		
	else
	strcat(buff,AT QBAND "?" NEWLINE);
	
	NBTask[_AT_QBAND].ATSendStr  = buff;
	NBTask[_AT_QBAND].len_string = strlen(NBTask[_AT_QBAND].ATSendStr);
	user_main_debug("NBTask[_AT_NBAND].ATSendStr:%s",NBTask[_AT_QBAND].ATSendStr);
	
	return NBTask[_AT_QBAND].nb_cmd_status;
}

NB_TaskStatus nb_qband_get(const char* param)
{

		printf("Currently set frequency band");
		char *pch = strchr((char*)nb.usart.data,':');
		if(pch!=NULL)
		{
			while(*pch != '\n')
				printf("%c",*pch++);
			printf("\n");
		}
		
		NBTask[_AT_QBAND].nb_cmd_status = NB_CMD_SUCC;
	return NBTask[_AT_QBAND].nb_cmd_status;
}

/**
	* @brief  AT_CGMM: Request Manufacturer Model
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_cgmm_run(const char* param)
{
	NBTask[_AT_CGMM].try_num = 3;
	while(NBTask[_AT_CGMM].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_CGMM])== NB_CMD_SUCC)
		{
			NBTask[_AT_CGMM].nb_cmd_status = nb_cgmm_get(param);
			break;
		}
		else
			NBTask[_AT_CGMM].nb_cmd_status = NB_CMD_FAIL;
	}
	return NBTask[_AT_CGMM].nb_cmd_status;
}

NB_TaskStatus nb_cgmm_get(const char* param)
{
	char *p = strstr((char*)nb.usart.data,"BC660K");
	if(p!=NULL)
	{	
	NBTask[_AT_CGMM].nb_cmd_status = NB_CMD_SUCC;	
	}
	if(p==NULL)
	{	
	NBTask[_AT_CGMM].nb_cmd_status = NB_CMD_FAIL;
	}
	return NBTask[_AT_CGMM].nb_cmd_status;
}


/**
	* @brief  AT_CCLK : Get Time
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_cclk_run(const char* param)
{
	sensor.time_stamp = 0;
	NBTask[_AT_CCLK].try_num = 3;
	while(NBTask[_AT_CCLK].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_CCLK])== NB_CMD_SUCC)
		{
			NBTask[_AT_CCLK].nb_cmd_status = nb_cclk_get(param);
			break;
		}
		else
			NBTask[_AT_CCLK].nb_cmd_status = NB_CMD_FAIL;
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
		memcpy(&date[2],&p[7],17);
		sensor.time_stamp = GetTick(date);
		if(first_sample==0)
		{
			uint16_t time_test=0;
			struct tm localtime;
			SysTimeLocalTime( sensor.time_stamp, &localtime );
			time_test=localtime.tm_min*60+localtime.tm_sec;
			TimerInit( &timesampleTimer,  OntimesampleEvent); 
			compare_time(time_test);
			first_sample=1;			
		}
		user_main_debug("time_stamp:%d",sensor.time_stamp);
	}
	return NBTask[_AT_CCLK].nb_cmd_status;
}

/**
	* @brief  AT_CCLK2 : Get Time
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_cclk2_run(const char* param)
{
	sensor.time_stamp = 0;
	NBTask[_AT_CCLK2].try_num = 3;
	while(NBTask[_AT_CCLK2].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_CCLK2])== NB_CMD_SUCC)
		{
			NBTask[_AT_CCLK2].nb_cmd_status = nb_cclk2_get(param);
			break;
		}
		else
			NBTask[_AT_CCLK2].nb_cmd_status = NB_CMD_FAIL;
	}
	return NBTask[_AT_CCLK2].nb_cmd_status;
}

NB_TaskStatus nb_cclk2_get(const char* param)
{
	char *p = strstr((char*)nb.usart.data,CCLK2);
	if( p==NULL)
		NBTask[_AT_CCLK2].nb_cmd_status = NB_NO_TIME;
	else
	{
		char date[20]={0};
		strcat(date,"20");
		memcpy(&date[2],&p[7],17);
		sensor.time_stamp = GetTick(date);
		if(first_sample==0)
		{
			uint16_t time_test=0;
			struct tm localtime;
			SysTimeLocalTime( sensor.time_stamp, &localtime );
			time_test=localtime.tm_min*60+localtime.tm_sec;
			TimerInit( &timesampleTimer,  OntimesampleEvent); 
			compare_time(time_test);
			first_sample=1;			
		}
		user_main_debug("time_stamp:%d",sensor.time_stamp);
	}
	return NBTask[_AT_CCLK2].nb_cmd_status;
}


/**
	* @brief  AT+CGDCONT : Define a PDP Context
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_cgdcont_run(const char* param)
{
	NBTask[_AT_CGDCONT].try_num = 2;
	NBTask[_AT_CGDCONT].set(param);
	while(NBTask[_AT_CGDCONT].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_CGDCONT]) == NB_CMD_SUCC )
		{
			break;
		}
	}
	return NBTask[_AT_CGDCONT].nb_cmd_status;
}

NB_TaskStatus nb_cgdcont_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	sprintf(buff,AT CGDCONT "=1,\"IPV4V6\",\"%s\"" NEWLINE,user.apn);
	
	NBTask[_AT_CGDCONT].ATSendStr = buff;
	NBTask[_AT_CGDCONT].len_string = strlen(NBTask[_AT_CGDCONT].ATSendStr);
	user_main_debug("NBTask[_AT_CGDCONT].ATSendStr:%s",NBTask[_AT_CGDCONT].ATSendStr);
	return NBTask[_AT_CGDCONT].nb_cmd_status;
}

NB_TaskStatus nb_cgdcont_get(const char* param)
{
	return NBTask[_AT_CGDCONT].nb_cmd_status;
}
/**
	* @brief  AT+CPSMS : Power Saving ModeSetting
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_cpsms_run(const char* param)
{
	NBTask[_AT_CPSMS].set(param);
	NBTask[_AT_CPSMS].try_num = 3;
	while(NBTask[_AT_CPSMS].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_CPSMS])== NB_CMD_SUCC)
		{
			NBTask[_AT_CPSMS].nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
			NBTask[_AT_CPSMS].nb_cmd_status = NB_CMD_FAIL;
	}
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
	strcat(buff,(char*)"\"01000001\"");
	strcat(buff,(char*)",\"00000000\"\r\n");
	
	NBTask[_AT_CPSMS].ATSendStr  = buff;
	NBTask[_AT_CPSMS].len_string = strlen(NBTask[_AT_CPSMS].ATSendStr);
	user_main_debug("NBTask[_AT_CPSMS].ATSendStr:%s",NBTask[_AT_CPSMS].ATSendStr);
	
	return NBTask[_AT_CPSMS].nb_cmd_status;
}

/**
	* @brief  AT+CSQ : Get signal strength
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_csq_get(const char* param)
{
	NBTask[_AT_CSQ].try_num = 2;
	while(NBTask[_AT_CSQ].try_num--)
	{
	if(nb_at_send(&NBTask[_AT_CSQ])== NB_CMD_SUCC)
	{
		char singalBuff[5]={0}; 	
		char *pch1 = strrchr((char*)nb.usart.data,':');
		char *pch2 = strchr((char*)nb.usart.data,',');
		for(int i =0;i<(pch2 - pch1)-1;i++)
			sprintf(singalBuff+strlen(singalBuff), "%c", nb.usart.data[7+i]);
		nb.singal = atoi(singalBuff);
			
		if(nb.singal == 99 || nb.singal == 0)
			NBTask[_AT_CSQ].nb_cmd_status = NB_CMD_OFF;

	}
  }

	return NBTask[_AT_CSQ].nb_cmd_status;
}
/**
	* @brief  AT+QDNSCFG : DNS Server 
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_qdnscfg_run(const char* param)
{
	NBTask[_AT_QDNSCFG].set(param);
	NBTask[_AT_QDNSCFG].try_num = 3;
	while(NBTask[_AT_QDNSCFG].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_QDNSCFG])== NB_CMD_SUCC)
		{
			NBTask[_AT_QDNSCFG].nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
			NBTask[_AT_QDNSCFG].nb_cmd_status = NB_CMD_FAIL;
	}
	return NBTask[_AT_QDNSCFG].nb_cmd_status;
}
NB_TaskStatus nb_qdnscfg_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	
	strcat(buff,AT QIDNSCFG "=0,");
	strcat(buff,(char*)user.dns_add);
	strcat(buff,NEWLINE);
	
	NBTask[_AT_QDNSCFG].ATSendStr  = buff;
	NBTask[_AT_QDNSCFG].len_string = strlen(NBTask[_AT_QDNSCFG].ATSendStr);
	user_main_debug("NBTask[_AT_QDNSCFG].ATSendStr:%s",NBTask[_AT_QDNSCFG].ATSendStr);
	
	return NBTask[_AT_QDNSCFG].nb_cmd_status;
}
/**
	* @brief  AT+QDNS : DNS resolve domain name 
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_qdns_run(const char* param)
{
	NBTask[_AT_QDNS].set(param);
	NBTask[_AT_QDNS].try_num = 4;
	while(NBTask[_AT_QDNS].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_QDNS])== NB_CMD_SUCC)
		{
			if(nb_at_send(&NBTask[_AT_QDNS])== NB_CMD_SUCC)
					NBTask[_AT_QDNS].nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
			NBTask[_AT_QDNS].nb_cmd_status = NB_CMD_FAIL;
	}
	return NBTask[_AT_QDNS].nb_cmd_status;
}
NB_TaskStatus nb_qdns_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	char *p=strrchr((char*)user.add,',');
	if(p!=NULL)
	{
		strcat(buff,AT QDNS "=0,");
		strcat(buff,"\"");
		memcpy(buff+strlen(buff),user.add,p-(char*)user.add);
	  strcat(buff,"\"");
		strcat(buff,NEWLINE);
	}
	
	NBTask[_AT_QDNS].ATSendStr  = buff;
	NBTask[_AT_QDNS].len_string = strlen(NBTask[_AT_QDNS].ATSendStr);
	user_main_debug("NBTask[_AT_QDNS].ATSendStr:%s",NBTask[_AT_QDNS].ATSendStr);
	
	return NBTask[_AT_QDNS].nb_cmd_status;
}
NB_TaskStatus nb_qdns_get(const char* param)
{
	char *p1=strrchr((char*)nb.usart.data,'P');	
	char *q1=strrchr((char*)user.add,',');
	if(p1==NULL || q1==NULL)
	{
		user_main_printf("Domain name resolution failed");
	  sprintf(record_log+strlen(record_log), "Domain name resolution failed\r\n");		
		NBTask[_AT_QDNS].nb_cmd_status = NB_CMD_FAIL;
	}
	else
	{
		p1 = strchr(p1,':');
		char *p2=strchr(p1,'\n');
		memset(user.add_ip,0,sizeof(user.add_ip));
		memcpy(user.add_ip+strlen((char*)user.add_ip),p1+2,p2-p1-3);
		memcpy(user.add_ip+strlen((char*)user.add_ip),q1,strlen(q1));
		user_main_printf("Domain IP:%s",user.add_ip);
	  sprintf(record_log+strlen(record_log), "Domain IP:%s\r\n",user.add_ip);	
		NBTask[_AT_QDNS].nb_cmd_status = NB_CMD_SUCC;
	}	

	return NBTask[_AT_QDNS].nb_cmd_status;
}
/**
	* @brief  AT+QRST : Restart
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_qrst_run(const char* param)
{
	NBTask[_AT_QRST].try_num = 3;
	while(NBTask[_AT_QRST].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_QRST]) == NB_CMD_SUCC )
		{
			NBTask[_AT_QRST].nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
			NBTask[_AT_QRST].nb_cmd_status = NB_CMD_FAIL;
	}
	return NBTask[_AT_QRST].nb_cmd_status;
}

NB_TaskStatus nb_qrst2_run(const char* param)
{
	NBTask[_AT_QRST2].try_num = 3;
	while(NBTask[_AT_QRST2].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_QRST2]) == NB_CMD_SUCC )
		{
			NBTask[_AT_QRST2].nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
			NBTask[_AT_QRST2].nb_cmd_status = NB_CMD_FAIL;
	}
	return NBTask[_AT_QRST2].nb_cmd_status;
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
case _AT:{
				if(NBTask[_AT].run(NULL) == NB_CMD_SUCC)
				{
					no_response_time=0;
					*task = _AT_QRST2;				
					user_main_printf("NBIOT has responded.");
				}
				else
				{
					at_state = _AT_ERROR;			
					user_main_printf("NBIOT did not respond.");			
				}
			}
			break;			
case _AT_QRST2:{
				if(NBTask[_AT_QRST2].run(NULL) == NB_CMD_SUCC)
				{
					*task = _ATE;
				}
				else
				{
					at_state = _AT_ERROR;			
					user_main_printf("No response when shutting down");	
				}
			}
				break;
case _ATE:{
				if(NBTask[_ATE].run(NULL) == NB_CMD_SUCC)
				{
					*task = _AT_QCFGEV;
					user_main_printf("Echo mode turned off successfully.");
				}
				else
				{
					at_state = _AT_ERROR;			
					user_main_printf("Echo mode turned off failed.");			
				}
			}
			break;	
case _AT_QCFGEV:{
				if(NBTask[_AT_QCFGEV].run(NULL) == NB_CMD_SUCC)
				{
					*task = _AT_CGMM;
					user_main_printf("Disable the reporting of deep sleep event URC.");
				}
				else
				{
					at_state = _AT_ERROR;			
					user_main_printf("Failed to turn off sleep event reporting.");			
				}
			}
			break;			
case _AT_CGMM:{
				if(NBTask[_AT_CGMM].run(NULL) == NB_CMD_SUCC)
				{
					*task = _AT_IMEI;
					user_main_printf("Model information:BC660K-GL.");
				}			
				else
				{
					at_state = _AT_ERROR;			
					user_main_printf("Request for manufacturer model failed.");			
				}
			}
			break;		
case _AT_IMEI:{
				if(NBTask[_AT_IMEI].get(NULL) == NB_CMD_SUCC)
				{
					*task = _AT_IMSI;
					user_main_printf("The IMEI number is:%s.",nb.imei);
					
					if(strstr((char*)user.deui,"NULL") != NULL)
					{
						memset(user.deui,0,sizeof(user.deui));
						memcpy(user.deui,nb.imei,15);	
            config_Set();	 						
					}
				}
				else 
				{
					at_state = _AT_ERROR;
					user_main_printf("Failed to read IMEI number.");
				}
			}
			break;			
case _AT_IMSI:{
				if(NBTask[_AT_IMSI].get(NULL) == NB_CMD_SUCC)
				{
					*task=_AT_QICFG;
					user_main_printf("The IMSI number is:%s.",nb.imsi);
				}
				else 
				{
					at_state = _AT_ERROR;
					user_main_printf("Failed to read IMSI number.");
				}
			}
			break;	
case _AT_QICFG:{
				if(NBTask[_AT_QICFG].run(NULL) == NB_CMD_SUCC)
				{
				     *task = _AT_QBAND;	
					user_main_printf("Set the data format for sending and receiving.");					
				}
				else
				{
					at_state = _AT_ERROR;
					user_main_printf("Failed to set the data format.");
				}
			}
			break;
			
case _AT_QBAND:{
				NB_TaskStatus nband_state = NBTask[_AT_QBAND].run(NULL);
				if(nband_state == NB_CMD_SUCC)
					*task=_AT_CGDCONT;
				else 
				{
					*task=_AT_IDLE;
					at_state = _AT_ERROR;
				}
			}
			break;		
case _AT_CGDCONT:{
				if(strstr((char*)user.apn,"NULL") == NULL)
				{
					if(NBTask[_AT_CGDCONT].run(NULL) != NB_CMD_SUCC)
					{
						at_state = _AT_ERROR;
						user_main_printf("Failed to set APN");
					}
					else
					{
						user_main_printf("Set APN successfully");
						*task=_AT_QSCLKOFF;
					}
				}
				else
				{
					*task=_AT_QSCLKOFF;
				}
			}
			break;			

case _AT_QSCLK:{
				if(NBTask[_AT_QSCLK].run(NULL) == NB_CMD_SUCC)
				{
				  *task = _AT_CFUNEND;				
				}
				else
				{
					at_state = _AT_ERROR;
				}
			}

			break;
case _AT_CFUNEND:{
				if(NBTask[_AT_CFUNEND].run(NULL) == NB_CMD_SUCC)
				{
				  *task = _AT_IDLE;
				}
				else
				{
					at_state = _AT_ERROR;
				}
				nb.uplink_flag = no_status;
				stored_datalog();
				user_main_printf("Enter sleep mode");	
			}
			break;

case _AT_CFUNOFF:{
				if(NBTask[_AT_CFUNOFF].run(NULL) == NB_CMD_SUCC)
				{
				  *task = _AT_QSCLK;
					if(sleep_status==0)
					{
				    TimerInit( &TxTimer, OnTxTimerEvent );
            TimerSetValue( &TxTimer,  sys.tdc*1000); 
            TimerStart( &TxTimer);						
					}						
					user_main_printf("Turn off the module receiving and sending RF function.");
					sprintf(record_log+strlen(record_log), "Turn off the module receiving and sending RF function.\r\n");
				}
				else
				{
					at_state = _AT_ERROR;
					*task = _AT_QSCLK;
				}
				csq_fail_log=0;
			}
			break;	

case _AT_QSCLKOFF:{
		     memset(record_log,0,sizeof(record_log));
				if(NBTask[_AT_QSCLKOFF].run(NULL) == NB_CMD_SUCC)
				{
					if(NBTask[_AT_QSCLKOFF].run(NULL) == NB_CMD_SUCC)
				                   *task = _AT_CFUNSTA;	
          user_main_printf("Exit sleep mode");						
					sprintf(record_log, "Exit sleep mode\r\n");
				}
				else
				{
					at_state = _AT_ERROR;
					user_main_printf("No response");	
					sprintf(record_log, "No response\r\n");
				}
			}
			break;
			
case _AT_CFUNSTA:{
				if(NBTask[_AT_CFUNSTA].run(NULL) == NB_CMD_SUCC)
				{
				                   *task = _AT_CSQ;				
				}
				else
				{
					at_state = _AT_ERROR;
					user_main_printf("No response");	
					sprintf(record_log, "No response\r\n");
				}
			}
			break;
			
case _AT_CSQ:{
				NBTask[_AT_CSQ].get((char*)NBTask[_AT_CSQ].cmd_num);
				user_main_printf("Signal Strength:%d",nb.singal);
	      if(csq_fail_log<5)
	      sprintf(record_log+strlen(record_log), "Signal Strength:%d\r\n",nb.singal);
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
					join_network_timer=0;
					join_network_time=0;
					is_time_to_send=0;
					csq_fail_log=0;
				}
				else
				{
					csq_fail_log++;
					*task = _AT_IDLE;	
				  join_network_flag=1;
					nb.net_flag = fail;
				}
			}
			break;			
case _AT_CPSMS:{
				if(NBTask[_AT_CPSMS].run(NULL) == NB_CMD_SUCC)
				{
					user_main_printf("PSM mode configured ");
				  sprintf(record_log+strlen(record_log), "PSM mode configured\r\n");
				}
				else 
				{
					user_main_printf("PSM mode configuration failed ");	
					sprintf(record_log+strlen(record_log), "PSM mode configuration failed\r\n");
				}					
				*task=_AT_CCLK2;
			}
			break;
case _AT_CCLK2:{
				if(NBTask[_AT_CCLK2].run(NULL) != NB_CMD_SUCC)
				{
					at_state = _AT_ERROR;
					user_main_printf("Failed to get time");
					sprintf(record_log+strlen(record_log), "Failed to get time\r\n");
				}		
				*task=_AT_QDNSCFG;
			}
			break;			
			
case _AT_QDNSCFG:{
				if(NBTask[_AT_QDNSCFG].run(NULL) == NB_CMD_SUCC)
				{
					user_main_printf("DNS configuration is successful");
					sprintf(record_log+strlen(record_log), "DNS configuration is successful\r\n");
				}
				else 
				{
					user_main_printf("DNS configuration failed");	
					sprintf(record_log+strlen(record_log), "DNS configuration failed\r\n");
				}
				HAL_Delay(5000);	
       if(sys.tlsmod==0)				
				*task=_AT_QDNS;
			 else if(sys.tlsmod==1 && sys.protocol == MQTT_PRO)	
				*task=_AT_QSSLCFG;		
       else
				*task=_AT_QDNS; 
			}
			break;

case _AT_QSSLCFG:
			if(NBTask[_AT_QSSLCFG].run(NULL) == NB_CMD_SUCC)
			{
				if(sys.cert==0)
				*task=_AT_QMTCFG_SSL;	
				user_main_printf("Manage server and client authentication.");						
			}
			else 
			{
					*task=_AT_QRST;
					at_state = _AT_ERROR;
				user_main_printf("Failed to configure authentication mode");
			}
			break;			

case _AT_QMTCFG_SSL:
			if(NBTask[_AT_QMTCFG_SSL].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_QDNS;		
				user_main_printf("Enable SSL and configure SSL context/connect index.");				
			}
			else 
			{
					*task=_AT_QRST;
				user_main_printf("Failed to enable SSL mode");
			}
			break;
			
case _AT_QDNS:{
				if((is_ipv4_addr((char*)user.add) == 1) ||(is_ipv4_addr((char*)user.add) == 2) || (nds_timer_flag2==1) || (is_ipv6_addr((char*)user.add) == 1)|| (is_ipv6_addr((char*)user.add) == 2) )
				{
          *task = _AT_UPLOAD_START;
					user_main_printf("No DNS resolution required");		
					sprintf(record_log+strlen(record_log), "No DNS resolution required\r\n");					
				}
				else  
				{
					NB_TaskStatus nbtask_state = NBTask[_AT_QDNS].run(NULL);
					if(nbtask_state == NB_CMD_SUCC)
					{
						user_main_printf("Resolving domain name...");		
					  sprintf(record_log+strlen(record_log), "Resolving domain name...\r\n");									
						*task = _AT_IDLE;
						nb.dns_flag = running;
						nb.uplink_flag = no_status;
						DNS_RE_FLAG=true;
					}					
				}
			}
			break;
case _AT_CCLK:{
				if(NBTask[_AT_CCLK].run(NULL) != NB_CMD_SUCC)
				{
					at_state = _AT_ERROR;
					user_main_printf("Failed to get time");
					sprintf(record_log+strlen(record_log), "Failed to get time\r\n");
				}		
				*task=_AT_UPLOAD_START;
			}
			break;
case _AT_UPLOAD_START:{
				nb.uplink_flag = send;
				nb.recieve_flag = NB_IDIE;

      if(sys.protocol == UDP_PRO)	{*task=_AT_UDP_OPEN;}
			else if(sys.protocol == MQTT_PRO)	{*task=_AT_MQTT_Config;}
			else if(sys.protocol == TCP_PRO)	{*task=_AT_TCP_OPEN;}
      sprintf(record_log+strlen(record_log), "*****Upload start:%d*****\r\n",sys.uplink_count);				
			user_main_printf("*****Upload start:%d*****",sys.uplink_count++);
			txPayLoadDeal(&sensor);
			memset((char*)nb.usart.data,0,sizeof(nb.usart.data));
			}
			break;

/***************************************************MQTT******************************************************************************/		
case _AT_MQTT_Config:
			if(strstr((char*)user.add,"NULL") != NULL || strstr((char*)user.pubtopic,"NULL") != NULL )
			{
				*task=_AT_UPLOAD_END;
				user_main_printf("MQTT parameter configuration error");
				sprintf(record_log+strlen(record_log), "MQTT parameter configuration error\r\n");
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
				sprintf(record_log+strlen(record_log),"MQTT configuration failed\r\n");
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
				sprintf(record_log+strlen(record_log), "Failed to open the MQTT client network\r\n");				
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
				sprintf(record_log+strlen(record_log), "Failed to connect to server\r\n");				
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
				sprintf(record_log+strlen(record_log), "Failed to subscribe to topic\r\n");				
			}
			break;
case _AT_MQTT_PUB1:
			if(NBTask[_AT_MQTT_PUB1].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_IDLE;			
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_MQTT_CLOSE;
				user_main_printf("Failed to Set PUB");
				sprintf(record_log+strlen(record_log), "Failed to Set PUB\r\n");				
			}
				break;	
case _AT_MQTT_PUB2:
			if(NBTask[_AT_MQTT_PUB2].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_IDLE;			
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_MQTT_CLOSE;
				user_main_printf("Failed to Set PUB");
				sprintf(record_log+strlen(record_log), "Failed to Set PUB\r\n");				
			}
				break;	
case _AT_MQTT_PUB3:
			if(NBTask[_AT_MQTT_PUB3].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_IDLE;			
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_MQTT_CLOSE;
				user_main_printf("Failed to Set PUB");
				sprintf(record_log+strlen(record_log), "Failed to Set PUB\r\n");				
			}
				break;				
case _AT_MQTT_PUB5:
			if(NBTask[_AT_MQTT_PUB5].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_IDLE;			
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_MQTT_CLOSE;
				user_main_printf("Failed to Set PUB");
				sprintf(record_log+strlen(record_log), "Failed to Set PUB\r\n");				
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
				sprintf(record_log+strlen(record_log), "Failed to Set PUB\r\n");				
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
				sprintf(record_log+strlen(record_log), "Failed to upload data\r\n");				
			}
			break;
case _AT_MQTT_READ:
			HAL_Delay(sys.rxdl);
      mqtt_close_flag=1;
	    succes_Status=true;
      reupload_time=0;
			*task = _AT_MQTT_CLOSE;
			break;
			
case _AT_MQTT_CLOSE:
			if(NBTask[_AT_MQTT_CLOSE].run(NULL) == NB_CMD_SUCC && mqtt_close_flag==1)
			{
				*task=_AT_MQTT_URI;			
        mqtt_close_flag=0;				
			}
			else
			{
				at_state = _AT_ERROR;
				*task=_AT_UPLOAD_FAIL;
				user_main_printf("Failed to close the port");
				sprintf(record_log+strlen(record_log), "Failed to close the port\r\n");				
			}
			break;
			
case _AT_MQTT_URI:
			uri_state = NBTask[_AT_MQTT_URI].run(NULL);
			switch((uint8_t)uri_state)
			{
				case NB_OPEN_SUCC:
							*task=_AT_MQTT_CONN;
							user_main_printf("Opened the MQTT client network successfully");
				      sprintf(record_log+strlen(record_log), "Opened the MQTT client network successfully\r\n");				
						break;
				case NB_CONN_SUCC:
					   if(sys.platform==0)
							*task=_AT_MQTT_PUB;
						 else if(sys.platform==1)
							*task=_AT_MQTT_PUB1; 
						 else if(sys.platform==2)
							*task=_AT_MQTT_PUB2; 
						 else if(sys.platform==3)
							*task=_AT_MQTT_PUB3; 						
						 else if(sys.platform==5)
							*task=_AT_MQTT_PUB5; 
							user_main_printf("Successfully connected to the server");
				      sprintf(record_log+strlen(record_log), "Successfully connected to the server\r\n");						 
						break;
				case NB_SUB_SUCC:
							*task=_AT_MQTT_READ;
							user_main_printf("Subscribe to topic successfully");	
							sprintf(record_log+strlen(record_log), "Subscribe to topic successfully\r\n");
						break;
				case NB_PUB_SUCC:
							*task=_AT_MQTT_SUB;
							user_main_printf("Upload data successfully");	
							sprintf(record_log+strlen(record_log), "Upload data successfully\r\n");
						break;
				case NB_CLOSE_SUCC:
							*task = _AT_UPLOAD_SUCC;
							user_main_printf("Close the port successfully");
							sprintf(record_log+strlen(record_log), "Close the port successfully\r\n");
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
			if(strstr((char*)user.add,"NULL") != NULL)
			{
				*task=_AT_UPLOAD_END;
				user_main_printf("UDP parameter configuration error");
				sprintf(record_log+strlen(record_log), "UDP parameter configuration error\r\n");				
				break;
			}		
			if(NBTask[_AT_UDP_OPEN].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_UDP_SEND;
				user_main_printf("Open a Socket Service successfully");
				sprintf(record_log+strlen(record_log), "Open a Socket Service successfully\r\n");				
			}
			else 
			{
				at_state = _AT_ERROR;
			 *task=_AT_UDP_CLOSE;
				user_main_printf("Failed to open a Socket Service");
				sprintf(record_log+strlen(record_log),"Failed to open a Socket Service\r\n");				
			}
			break;

case _AT_UDP_SEND:	
			if(NBTask[_AT_UDP_SEND].run(NULL) == NB_CMD_SUCC)
			{		
				if(resend_flag==3)
				*task = _AT_UDP_CLOSE;					
				else
				*task = _AT_IDLE;												
			}
			else
			{
				at_state = _AT_ERROR;
			 *task=_AT_UDP_CLOSE;
				user_main_printf("Failed to send");
				sprintf(record_log+strlen(record_log), "Failed to send\r\n");				
			}
				break;	
case _AT_UDP_READ:
			HAL_Delay(sys.rxdl);
      read_flag=1;
			*task = _AT_IDLE;
	    succes_Status=true;
      reupload_time=0;
			break;
			
case _AT_UDP_CLOSE:
			if(NBTask[_AT_UDP_CLOSE].run(NULL) == NB_CMD_SUCC)
			{
				read_flag=0;
				resend_flag=0;
				if(succes_Status==true)
				*task = _AT_UPLOAD_SUCC;
				else
				*task=_AT_UPLOAD_FAIL;
			user_main_printf("Close the port successfully");
				sprintf(record_log+strlen(record_log), "Close the port successfully\r\n");				
			}
			else
			{
				read_flag=0;
				resend_flag=0;
				*task=_AT_UPLOAD_FAIL;
				at_state = _AT_ERROR;				
				user_main_printf("Failed to close the port");
				sprintf(record_log+strlen(record_log),"Failed to close the port\r\n");				
			}
			break;
case _AT_UDP_URI:
			uri_state = NBTask[_AT_UDP_URI].run(NULL);
			if(uri_state == NB_STA_SUCC)
			{
				resend_flag=0;
				read_flag=0;
				*task = _AT_UDP_CLOSE;
				user_main_printf("Datagram is sent by RF");
				sprintf(record_log+strlen(record_log), "Datagram is sent by RF\r\n");				
			}
			else if(uri_state == NB_SEND_SUCC)			
			{
				*task = _AT_UDP_READ;		
				user_main_printf("Upload data successfully");	
				sprintf(record_log+strlen(record_log), "Upload data successfully\r\n");				
			}
			else if(uri_state == NB_SEND_FAIL)			
			{
				resend_flag++;
				*task = _AT_UDP_SEND;		
				user_main_printf("Failed to upload data,resend data");
				sprintf(record_log+strlen(record_log), "Failed to upload data,resend data\r\n");				
			}
			else
			{
				*task = _AT_IDLE;	
			}
			break;
/***************************************************TCP******************************************************************************/		
case _AT_TCP_OPEN:
			if(strstr((char*)user.add,"NULL") != NULL)
			{
				*task=_AT_UPLOAD_END;
				user_main_printf("TCP parameter configuration error");
				sprintf(record_log+strlen(record_log), "TCP parameter configuration error\r\n");				
				break;
			}
			if(NBTask[_AT_TCP_OPEN].run(NULL) == NB_CMD_SUCC)
			{
//				*task=_AT_IDLE;		
				  *task=_AT_TCP_SEND;	
				user_main_printf("Open a Socket Service successfully");			
				sprintf(record_log+strlen(record_log), "Open a Socket Service successfully\r\n");				
				
			}
			else 
			{
				at_state = _AT_ERROR;
				tcp_fail_flag=1;
				*task = _AT_TCP_CLOSE;
				user_main_printf("Failed to open a Socket Service");
				sprintf(record_log+strlen(record_log), "Failed to open a Socket Service\r\n");				
			}
			break;
case _AT_TCP_SEND:
			if(NBTask[_AT_TCP_SEND].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_TCP_READ;	
				user_main_printf("Upload data successfully");		
				sprintf(record_log+strlen(record_log), "Upload data successfully\r\n");				
			}
			else	
			{
				at_state = _AT_ERROR;
				tcp_fail_flag=1;
				*task = _AT_TCP_CLOSE;
				user_main_printf("Failed to upload data");	
				sprintf(record_log+strlen(record_log), "Failed to upload data\r\n");				
			}
				break;					
case _AT_TCP_READ:
			HAL_Delay(sys.rxdl);
			*task = _AT_IDLE;
	    succes_Status=true;
      reupload_time=0;
			break;
case _AT_TCP_CLOSE:
			if(NBTask[_AT_TCP_CLOSE].run(NULL) == NB_CMD_SUCC)
			{
				if(tcp_fail_flag==1)
				*task=_AT_UPLOAD_FAIL;
				else
				*task=_AT_UPLOAD_SUCC;
				 tcp_fail_flag=0;
				user_main_printf("Close the port successfully");
				sprintf(record_log+strlen(record_log), "Close the port successfully\r\n");
			}
			else 
			{
				at_state = _AT_ERROR;
				*task=_AT_UPLOAD_FAIL;
				user_main_printf("Failed to close the port");
				sprintf(record_log+strlen(record_log), "Failed to close the port\r\n");				
			}				
			break;
case _AT_TCP_URI:
			uri_state = NBTask[_AT_TCP_URI].run(NULL);
//			if(uri_state == NB_OPEN_SUCC)
//			{
//				*task = _AT_TCP_SEND;
//				user_main_printf("Open a Socket Service successfully");				
//			}
			if(uri_state == NB_STA_SUCC && succes_Status==true)
			{
				*task = _AT_TCP_CLOSE;
			}
			else if(succes_Status==true)
			{
				at_state = _AT_ERROR;
				*task=_AT_UPLOAD_FAIL;
				user_main_printf("Message failed");
				sprintf(record_log+strlen(record_log), "Message failed\r\n");				
			}
			break;
/******************************************************************************************************************************************/			
case _AT_UPLOAD_END:
			user_main_printf("*****End of upload*****\r\n");	
			sprintf(record_log+strlen(record_log), "*****End of upload*****\r\n");
      error_num = 0;
			memset((char*)nb.usart.data,0,sizeof(nb.usart.data));
			if(succes_Status==false && reupload_time<3)
			{
				if(reupload_time==1)
			 {
			 stored_datalog();
			 memset(record_log,0,sizeof(record_log));
			 two_data_log=1;
			 }
				nb.uplink_flag = send;
				nb.recieve_flag = NB_IDIE;
        if(sys.protocol == UDP_PRO)	{*task=_AT_UDP_OPEN;}
		  	else if(sys.protocol == MQTT_PRO)	{*task=_AT_MQTT_Config;}
			  else if(sys.protocol == TCP_PRO)	{*task=_AT_TCP_OPEN;}
			  reupload_time++;
			}	
		 if(succes_Status==true ||reupload_time==3)
		{
			if(sys.exit_flag == 0)	{TimerInit( &TxTimer, OnTxTimerEvent );
                               TimerSetValue(&TxTimer,sys.tdc*1000); 
                               TimerStart( &TxTimer);	}
			reupload_time=0;											 
			*task = _AT_QSCLK;
		}
			succes_Status=false;	
			break;

case _AT_UPLOAD_SUCC:
			*task = _AT_UPLOAD_END;
			user_main_printf("Send complete");
				sprintf(record_log+strlen(record_log), "Send complete\r\n");
			break;

case _AT_UPLOAD_FAIL:
			user_main_printf("Failed to send");	
				sprintf(record_log+strlen(record_log), "Failed to send\r\n");
			*task = _AT_UPLOAD_END;
			break;

case _AT_QRST:{
				if(NBTask[_AT_QRST].run(NULL) != NB_CMD_SUCC)
				{
					at_state = _AT_ERROR;
					user_main_printf("No response when shutting down");
					sprintf(record_log+strlen(record_log), "No response when shutting down\r\n");					
				  RESET_GPIO_Init();
				  HAL_Delay(100);
				  RESET_GPIO_DeInit();
					HAL_Delay(2000);
					no_response_flag=1;
					no_response_time++;
				}
				if(no_response_flag==0||no_response_time==6)
				{
					no_response_time=0;
					no_response_flag=0;
		      *task=_AT_IDLE;		
          TimerInit( &TxTimer, OnTxTimerEvent );
          TimerSetValue( &TxTimer,  sys.tdc*1000); 
          TimerStart( &TxTimer);	
				}
				else
				{
				 *task=_AT;
					no_response_flag=0;
				}
				nb.net_flag = no_status;
				net_acc_status_led = 0;
				nb.uplink_flag = no_status;		
				
				break;
			}
case _AT_URI:{
				if(sys.protocol == UDP_PRO)	*task = _AT_UDP_URI;
				else if(sys.protocol == MQTT_PRO)	*task = _AT_MQTT_URI;
				else if(sys.protocol == TCP_PRO)	*task = _AT_TCP_URI;
			}
			break;
case _AT_FLAG_INIT:{
				nb.uplink_flag = no_status;
				nb.net_flag = no_status;
				net_acc_status_led = 0;
	      nbmodel_int=1;
	      TimerInit( &nb_intTimeoutTimer, nb_intTimeoutEvent );
			  TimerSetValue( &nb_intTimeoutTimer, 8000);
			  TimerStart( &nb_intTimeoutTimer );
	      user_main_printf("NB module is initializing...");
	      *task=_AT_IDLE;
			  MX_USART2_UART_Init();
				HAL_UART_Receive_IT(&huart2,(uint8_t*)&rxbuf,RXSIZE);	
			}
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

void stored_datalog(void)
{
  if(two_data_log!=1)
	{
	uint32_t parameters_log[128]={0};
	for(uint16_t i=0,j=0;i<strlen((char*)record_log);i=i+4,j++)
		 parameters_log[j]=record_log[i+0]<<24 | record_log[i+1]<<16 | record_log[i+2]<<8 | record_log[i+3];
		 FLASH_erase(FLASH_USER_START_DATALOG+sys.log_seq * FLASH_PAGE_SIZE*4,((FLASH_USER_START_DATALOG+(sys.log_seq+1) * FLASH_PAGE_SIZE*4	) - (FLASH_USER_START_DATALOG+sys.log_seq * FLASH_PAGE_SIZE*4	)) / FLASH_PAGE_SIZE);
	   FLASH_program(FLASH_USER_START_DATALOG+sys.log_seq * FLASH_PAGE_SIZE*4,parameters_log, sizeof(parameters_log)/4);	
     sys.log_seq++;
     if(sys.log_seq==20)
        sys.log_seq=0;
		config_Set();
	 }
	two_data_log=0;
}					