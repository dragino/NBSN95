#include "nbInit.h"
#include "gpio.h"
#include "hw_rtc.h"

char 	*ATSendStr;
int 	len_string;
uint8_t  try_num;
NB_TaskStatus  nb_cmd_status;
int32_t cal_time_difference=0;
bool clock_cal_time_flag=0;
static uint8_t net_acc_status_led = 0;
static char buff[200]={0};
static uint8_t	recieve_data[NB_RX_SIZE] = {0};	   	 			//Receive data
extern uint8_t join_network_num;				  
extern uint8_t join_network_flag;	
extern uint8_t join_network_time;
extern uint8_t join_network_timer;
extern uint8_t error_num;
extern uint8_t  rxbuf;
extern bool at_sleep_flag;
uint8_t reupload_time = 0;
bool  succes_Status=false;
extern uint8_t is_time_to_send;
extern uint8_t sleep_status;
extern uint8_t ipv46;
extern uint8_t nbecho_flag;
extern uint8_t gnss_tdc_flag;
extern uint16_t gnss_timer;
extern uint8_t gps_flag;
extern bool act_network_flag;
extern uint8_t dns_reset_num;
extern uint8_t dns_log;
extern bool gpstime_flag;
uint8_t udp_close_flag = 0;
uint8_t gnss_time_num = 0;
extern uint8_t nb_no_rev;
bool no_singal_flag = 0;
char record_log[512]={0};
bool DNS_RE_FLAG = false;
bool first_sample=0;
uint8_t tcp_close_flag = 0;
static uint8_t csq_fail_log = 0;
static bool tls_flag = 0;
extern void OnTxTimerEvent( void );
extern void nb_intTimeoutEvent( void );
extern void GNSSTimerEvent( void );
extern TimerEvent_t TxTimer;
extern TimerEvent_t nb_intTimeoutTimer;
extern TimerEvent_t GNSSTimer;
extern void compare_time(uint16_t time);
extern TimerEvent_t timesampleTimer;
extern void OntimesampleEvent(void);
extern TimerEvent_t CalibrationtimeTimer;

extern bool Calibrat_flag;
extern bool nb_start;
NB nb = {.net_flag=no_status,.recieve_flag=0,.usart.len=0,.usart.data=recieve_data,
				 .imei={0},.imsi={0},.singal=0};

NB_TaskStatus nb_at_send(const struct NBTASK *NB_Task)
{
	nb.usart.len = 0;
	memset(nb.usart.data,0,NB_RX_SIZE);	
	HAL_UART_Transmit_DMA(&hlpuart1,(uint8_t*)ATSendStr,len_string);
	uint32_t time = HAL_GetTick();
if(sys.dns_time==1&&(NB_Task->cmd_num == _AT_UDP_OPEN||NB_Task->cmd_num == _AT_TCP_OPEN))
{
    while(HAL_GetTick() - time < NB_Task->time_out+3000)
    {
        user_main_info("...");
    }
}
else{
    while(HAL_GetTick() - time < NB_Task->time_out)
    {
        user_main_info("...");
    }
}    	
	user_main_info("recieve data:%s",nb.usart.data);
	
	if(strstr((char*)nb.usart.data,NB_Task->ATRecStrOK) != NULL)
		nb_cmd_status =  NB_CMD_SUCC;
	else if(strstr((char*)nb.usart.data,NB_Task->ATRecStrError) != NULL)
		nb_cmd_status =  NB_CMD_FAIL;
	else
		nb_cmd_status =  NB_ERROR;
	
	return nb_cmd_status;
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
	ATSendStr  = NULL;
	ATSendStr  = AT NEWLINE;
	len_string = sizeof(AT  NEWLINE) - 1;
	
	try_num = 3;
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT]) == NB_CMD_SUCC )
		{
			break;
		}
	}
	
	return nb_cmd_status;
}
/**
	* @brief  Echo mode OFF
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_ate_run(const char* param)
{
	ATSendStr  = NULL;
	ATSendStr  = ATE"0" NEWLINE;
	len_string = sizeof(ATE"0" NEWLINE) - 1;	
	
	try_num = 6;
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_ATE]) == NB_CMD_SUCC )
		{
			break;
		}
	}
	
	return nb_cmd_status;
}

/**
	* @brief  AT_CGSN :Get IMEI number
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_cgsn_get(const char* param)
{
	ATSendStr  = NULL;
	ATSendStr  = AT CGSN NEWLINE;
	len_string = sizeof(AT CGSN NEWLINE) - 1;
	
	try_num = 3;
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_IMEI])== NB_CMD_SUCC)
		{
			memset(nb.imei,0,sizeof((nb.imei)));
			char *pch = strchr((char*)nb.usart.data,'\n');
			char *pch1 = strchr((char*)nb.usart.data,'O');
			if(pch != NULL && pch1!=NULL)
				strncpy((char*)nb.imei, pch +1,pch1 - pch -5);
			break;
		}
	}
	
	return nb_cmd_status;
}

/**
	* @brief  AT_CIMI :Get IMSI number
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_cimi_get(const char* param)
{
	ATSendStr  = NULL;
	ATSendStr  = AT CIMI NEWLINE;
	len_string = sizeof(AT CIMI NEWLINE) - 1;
	
	try_num = 3;
	while(try_num--)
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
	return nb_cmd_status;
}

NB_TaskStatus nb_qicfg_run(const char* param)
{
	try_num = 10;
	NBTask[_AT_QICFG].set(param);
	
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_QICFG]) == NB_CMD_SUCC )
		{
			break;
		}
	}
	
	return nb_cmd_status;
}

NB_TaskStatus nb_qicfg_get(const char* param)
{
	return nb_cmd_status;
}

NB_TaskStatus nb_qicfg_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	if(sys.platform==0)
	strcat(buff,AT QICFG "=\"dataformat\",1,0" NEWLINE);
  else
	strcat(buff,AT QICFG "=\"dataformat\",0,0" NEWLINE);
	
	ATSendStr  = NULL;	
	ATSendStr  = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_QICFG].ATSendStr:%s",ATSendStr);

	return nb_at_send(&NBTask[_AT_QICFG]);
}

/**
	* @brief  AT_CGMM: Request Manufacturer Model
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_cgmm_run(const char* param)
{
	ATSendStr  = NULL;
	ATSendStr  = AT CGMM NEWLINE;
	len_string = sizeof(AT CGMM  NEWLINE) - 1;
	
	try_num = 3;
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_CGMM])== NB_CMD_SUCC)
		{
			nb_cmd_status = nb_cgmm_get(param);
			break;
		}
		else
			nb_cmd_status = NB_CMD_FAIL;
	}
	return nb_cmd_status;
}

NB_TaskStatus nb_cgmm_get(const char* param)
{
	char *p = strstr((char*)nb.usart.data,"BG95-M1");
	char *p1 = strstr((char*)nb.usart.data,"BG95-M2");
	char *p2 = strstr((char*)nb.usart.data,"BG95-M3");
	if(p!=NULL)
	nb_cmd_status = NB_M1_SUCC;	
	if(p1!=NULL)
	nb_cmd_status = NB_M2_SUCC;	
	if(p2!=NULL)
	nb_cmd_status = NB_M3_SUCC;	
	if(p==NULL && p1==NULL && p2==NULL)
	{	
	nb_cmd_status = NB_CMD_FAIL;
	}
	return nb_cmd_status;
}

NB_TaskStatus nb_qband_run(const char* param)
{
	try_num = 2;
	NBTask[_AT_QBAND].set(param);
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_QBAND]) == NB_CMD_SUCC )
		{
			break;
		}
	}
	return nb_cmd_status;
}

NB_TaskStatus nb_qband_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	sprintf(buff,AT QCFG "=\"band\",0xF,%s,1" NEWLINE,user.qband);
	
	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_QBAND].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}

NB_TaskStatus nb_qband_get(const char* param)
{
	return nb_cmd_status;
}

NB_TaskStatus nb_iotm_run(const char* param)
{
	try_num = 2;
	NBTask[_AT_IOTM].set(param);
	while(try_num--)
	{
	if(nb_at_send(&NBTask[_AT_IOTM]) == NB_CMD_SUCC )
		{
			break;
		}
	}	
	return nb_cmd_status;
}

NB_TaskStatus nb_iotm_get(const char* param)
{
	return nb_cmd_status;
}

NB_TaskStatus nb_iotm_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	sprintf(buff,AT QCFG "=\"iotopmode\",%d,1" NEWLINE,sys.iotmode);

	ATSendStr  = NULL;	
	ATSendStr  = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_IOTM].ATSendStr:%s",ATSendStr);

	return nb_cmd_status;
}
/**
	* @brief  AT_CCLK : Get Time
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_cclk_run(const char* param)
{
	ATSendStr  = NULL;
	ATSendStr  = AT CCLK "?" NEWLINE;
	len_string = sizeof(AT CCLK "?" NEWLINE) - 1;
	
	sensor.time_stamp = 0;
	try_num = 3;
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_CCLK])== NB_CMD_SUCC)
		{
			nb_cmd_status = nb_cclk_get(param);
			break;
		}
		else
			nb_cmd_status = NB_CMD_FAIL;
	}
	return nb_cmd_status;
}

NB_TaskStatus nb_cclk_get(const char* param)
{
	char *p = strstr((char*)nb.usart.data,CCLK);
	if( p==NULL)
		nb_cmd_status = NB_NO_TIME;
	else
	{
		char date[20]={0};
		strcat(date,"20");
		memcpy(&date[2],&p[8],17);
		sensor.time_stamp = GetTick(date);
		if(Calibrat_flag==1)
		{		
			uint32_t rtc_time=0;
			SysTime_t sysTimeCurrent = { 0 };
			sysTimeCurrent=SysTimeGet();
			rtc_time=sysTimeCurrent.Seconds;		

			cal_time_difference=rtc_time-sensor.time_stamp;
			SysTime_t sysTime = { 0 };
			sysTime.Seconds=sensor.time_stamp;
			SysTimeSet( sysTime );
			
			clock_cal_time_flag=1;				
			Calibrat_flag=0;
		}	
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
	return nb_cmd_status;
}

/**
	* @brief  AT_CCLK2 : Get Time
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_cclk2_run(const char* param)
{
	ATSendStr  = NULL;
	ATSendStr  = AT CCLK2 "?" NEWLINE;
	len_string = sizeof(AT CCLK2 "?" NEWLINE) - 1;
	
	sensor.time_stamp = 0;
	try_num = 3;
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_CCLK2])== NB_CMD_SUCC)
		{
			nb_cmd_status = nb_cclk2_get(param);
			break;
		}
		else
			nb_cmd_status = NB_CMD_FAIL;
	}
	return nb_cmd_status;
}

NB_TaskStatus nb_cclk2_get(const char* param)
{
	char *p = strstr((char*)nb.usart.data,CCLK2);
	if( p==NULL)
		nb_cmd_status = NB_NO_TIME;
	else
	{
		char date[20]={0};
		strcat(date,"20");
		memcpy(&date[2],&p[8],17);
		sensor.time_stamp = GetTick(date);
		if(gps_flag==1&&gpstime_flag==1)	
		{
     sensor.gps_timestamp= sensor.time_stamp;
		 gpstime_flag=0;
		}
		
		if(Calibrat_flag==1)
		{		
			uint32_t rtc_time=0;
			SysTime_t sysTimeCurrent = { 0 };
			sysTimeCurrent=SysTimeGet();
			rtc_time=sysTimeCurrent.Seconds;		

			cal_time_difference=rtc_time-sensor.time_stamp;
			
			SysTime_t sysTime = { 0 };
			sysTime.Seconds=sensor.time_stamp;
			SysTimeSet( sysTime );
			
			clock_cal_time_flag=1;
			Calibrat_flag=0;
		}
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
	return nb_cmd_status;
}


/**
	* @brief  AT+CGDCONT : Define a PDP Context
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_cgdcont_run(const char* param)
{
	try_num = 2;
	NBTask[_AT_CGDCONT].set(param);
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_CGDCONT]) == NB_CMD_SUCC )
		{
			break;
		}
	}
	return nb_cmd_status;
}

NB_TaskStatus nb_cgdcont_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	sprintf(buff,AT CGDCONT "=1,\"IPV4V6\",\"%s\"" NEWLINE,user.apn);
	
	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_CGDCONT].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}

NB_TaskStatus nb_cgdcont_get(const char* param)
{
	return nb_cmd_status;
}


/**
	* @brief AT_QGPS : turn on gps
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_qgps_run(const char* param)
{
	ATSendStr  = NULL;
	ATSendStr  = AT QGPS NEWLINE;
	len_string = sizeof(AT QGPS NEWLINE) - 1;
	
	try_num = 5;
	NBTask[_AT_QGPS].set(param);
	while(try_num--)
	{
	if(nb_at_send(&NBTask[_AT_QGPS]) == NB_CMD_SUCC )
		{
			break;
		}
	}	
	return nb_cmd_status;
}

NB_TaskStatus nb_qgps_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QGPS "=1" NEWLINE);

	ATSendStr  = NULL;
	ATSendStr  = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_QGPS].ATSendStr:%s",ATSendStr);

	return nb_cmd_status;
}


/**
	* @brief  AT+CSQ : Get signal strength
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_csq_get(const char* param)
{
	ATSendStr  = NULL;
	ATSendStr  = AT CSQ NEWLINE;
	len_string = sizeof(AT CSQ NEWLINE) - 1;
	
	try_num = 2;
	while(try_num--)
	{
	if(nb_at_send(&NBTask[_AT_CSQ])== NB_CMD_SUCC)
	{
		char singalBuff[5]={0}; 	
		char *pch1 = strrchr((char*)nb.usart.data,':');
		char *pch2 = strchr((char*)nb.usart.data,',');
		for(int i =0;i<(pch2 - pch1)-1;i++)
			sprintf(singalBuff+strlen(singalBuff), "%c", nb.usart.data[7+i]);
		nb.singal = atoi(singalBuff);
			
		if(nb.singal == 99 || nb.singal == 0 || nb.singal == 100)
			nb_cmd_status = NB_CMD_OFF;

	}
  }
	user_main_debug("NBTask[_AT_CSQ].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}

NB_TaskStatus nb_qnwinfo_run(const char* param)
{
	ATSendStr  = NULL;
	ATSendStr  = AT QNWINFO NEWLINE;
	len_string = sizeof(AT QNWINFO NEWLINE) - 1;
	
	try_num = 3;
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_QNWINFO])== NB_CMD_SUCC)
		{
			nb_cmd_status = nb_qnwinfo_get(param);
			break;
		}
		else
			nb_cmd_status = NB_CMD_FAIL;
	}
	return nb_cmd_status;
}

NB_TaskStatus nb_qnwinfo_get(const char* param)
{
	char Buff[80]={0}; 	
	char *p = strstr((char*)nb.usart.data,QNWINFO);
	if( p==NULL)
		nb_cmd_status = NB_CMD_FAIL;
	else
	{
		char *pch1 = strchr((char*)nb.usart.data,':');
		char *pch2 = strrchr((char*)nb.usart.data,'O');
		memcpy(Buff,&nb.usart.data[pch1 - ((char*)nb.usart.data)+2],pch2-pch1-6);
	}
	user_main_printf("Network Information:%s",Buff);
	return nb_cmd_status;
}


/**
	* @brief  AT+QICSGP : Configure Parameters of a TCP/IP Context
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_qicsgp_run(const char* param)
{
	try_num = 3;
	NBTask[_AT_QICSGP].set(param);
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_QICSGP]) == NB_CMD_SUCC )
		{
			break;
		}
	}
	return nb_cmd_status;
}

NB_TaskStatus nb_qicsgp_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	if(strstr((char*)user.apn,"NULL") ==NULL)
	sprintf(buff,AT QICSGP "=1,%d,\"%s\",\"\",\"\",1" NEWLINE,ipv46,user.apn);
	else
	sprintf(buff,AT QICSGP "=1,%d,\"\",\"\",\"\",1" NEWLINE,ipv46);

  ATSendStr  = NULL;	
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_QICSGP].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}

NB_TaskStatus nb_qicsgp_get(const char* param)
{
	return nb_cmd_status;
}

/**
	* @brief  QIDEACT : close UE Functionality
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_qideact_run(const char* param)
{
	try_num = 4;
	NBTask[_AT_QIDEACT].set(param);
	
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_QIDEACT]) == NB_CMD_SUCC )
		{
			break;
		}
	}
	
	return nb_cmd_status;
}

NB_TaskStatus nb_qideact_get(const char* param)
{
	return nb_cmd_status;
}

NB_TaskStatus nb_qideact_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QIDEACT "=1" NEWLINE);

	ATSendStr  = NULL;
	ATSendStr  = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_QIDEACT].ATSendStr:%s",ATSendStr);

	return nb_cmd_status;
}

/**
	* @brief  AT_QIACT : close UE Functionality
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_qiact_run(const char* param)
{
	try_num = 2;
	NBTask[_AT_QIACT].set(param);
	while(try_num--)
	{
	if(nb_at_send(&NBTask[_AT_QIACT]) == NB_CMD_SUCC )
		{
			break;
		}
	}	
	return nb_cmd_status;
}

NB_TaskStatus nb_qiact_get(const char* param)
{
	return nb_cmd_status;
}

NB_TaskStatus nb_qiact_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QIACT "=1" NEWLINE);

	ATSendStr  = NULL;	
	ATSendStr  = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_QIACT].ATSendStr:%s",ATSendStr);

	return nb_cmd_status;
}

/**
	* @brief  AT+QDNSCFG : DNS Server 
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_qdnscfg_run(const char* param)
{
 if(ipv46==2)	
 {
	 nb_cmd_status = NB_CMD_SUCC;
 }
 else
 {
	NBTask[_AT_QDNSCFG].set(param);
	try_num = 3;
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_QDNSCFG])== NB_CMD_SUCC)
		{
			nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
			nb_cmd_status = NB_CMD_FAIL;
	}
 }
	return nb_cmd_status;
}
NB_TaskStatus nb_qdnscfg_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	
	strcat(buff,AT QIDNSCFG "=1,");
	strcat(buff,(char*)user.dns_add);
	strcat(buff,NEWLINE);
	
	ATSendStr  = NULL;
	ATSendStr  = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_QDNSCFG].ATSendStr:%s",ATSendStr);
	
	return nb_cmd_status;
}
/**
	* @brief  AT+QDNS : DNS resolve domain name 
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_qdns_run(const char* param)
{
	NBTask[_AT_QDNS].set(param);
	try_num = 4;
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_QDNS])== NB_CMD_SUCC)
		{

					nb_cmd_status = NB_CMD_SUCC;
			break;
		}
		else
			nb_cmd_status = NB_CMD_FAIL;
	}
	return nb_cmd_status;
}
NB_TaskStatus nb_qdns_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	char *p=strrchr((char*)user.add,',');
	if(p!=NULL)
	{
		strcat(buff,AT QDNS "=1,");
		strcat(buff,"\"");
		memcpy(buff+strlen(buff),user.add,p-(char*)user.add);
	  strcat(buff,"\"");
		strcat(buff,NEWLINE);
	}

	ATSendStr  = NULL;
	ATSendStr  = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_QDNS].ATSendStr:%s",ATSendStr);
	
	return nb_cmd_status;
}
NB_TaskStatus nb_qdns_get(const char* param)
{
    char *p1=strrchr((char*)nb.usart.data,'n');    
    char *q1=strrchr((char*)user.add,',');
	  uint8_t q2,q3;
	
     if(countchar((char*)nb.usart.data,'.')>=3)
			  q2=1;
     else
				q2=0;  
     if(countchar((char*)nb.usart.data,':')>3)
        q3=1;
     else
				q3=0; 
    if(p1==NULL || q1==NULL || (q2==0&&q3==0))
    {
        user_main_printf("Domain name resolution failed");
			  if(dns_log==1)
				{
        sprintf(record_log+strlen(record_log), "Domain name resolution failed\r\n");   
				}					
        nb_cmd_status = NB_CMD_FAIL;
    }
    else
    {
        p1 = strchr(p1,',');
        char *p2=strchr(p1,'\n');
        memset(user.add_ip,0,sizeof(user.add_ip));
        memcpy(user.add_ip+strlen((char*)user.add_ip),p1+2,p2-p1-4);
        memcpy(user.add_ip+strlen((char*)user.add_ip),q1,strlen(q1));
        user_main_printf("Domain IP:%s",user.add_ip);
        sprintf(record_log+strlen(record_log), "Domain IP:%s\r\n",user.add_ip);    
        nb_cmd_status = NB_CMD_SUCC;
    }    
    dns_log=0;
    return nb_cmd_status;
}

/**
	* @brief  AT_QGPSLOC: Get gps
  * @param  Instruction parameter
  * @retval NB_TaskStatus
  */
NB_TaskStatus nb_qgpsloc_run(const char* param)
{
	ATSendStr  = NULL;
	ATSendStr  = AT QGPSLOC"=2,0" NEWLINE;
	len_string = sizeof(AT QGPSLOC"=2,0" NEWLINE) - 1;
	
	try_num = 3;
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_QGPSLOC])== NB_CMD_SUCC)
		{
			nb_cmd_status = nb_qgpsloc_get(param);
			break;
		}
		else
			nb_cmd_status = NB_CMD_FAIL;
	}
	return nb_cmd_status;
}

NB_TaskStatus nb_qgpsloc_get(const char* param)
{
	uint8_t f_lat[15]={0};
  uint8_t f_lon[15]={0};
	char *p = strstr((char*)nb.usart.data,QGPSLOC);
	if( p==NULL)
		nb_cmd_status = NB_NO_GPS;
	else
	{
		char *pch1 = strchr((char*)nb.usart.data,',');
		char *pch2 = strchr(pch1+1,',');
		char *pch3 = strchr(pch2+1,',');
		memcpy(f_lat+strlen((char*)f_lat),pch1+1,pch2-pch1-1);
		memcpy(f_lon+strlen((char*)f_lon),pch2+1,pch3-pch2-1);
		sensor.latitude=atof((char *)f_lat);
		sensor.longitude=atof((char *)f_lon);
		user_main_printf("latitude:%f,longitude:%f",sensor.latitude,sensor.longitude);
	  sprintf(record_log+strlen(record_log), "latitude:%f,longitude:%f",sensor.latitude,sensor.longitude);			
	}
	return nb_cmd_status;
}

NB_TaskStatus nb_qgpssend_run(const char* param)
{
	try_num = 4;
	NBTask[_AT_QGPSEND].set(param);
	
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_QGPSEND]) == NB_CMD_SUCC )
		{
			break;
		}
	}
	
	return nb_cmd_status;
}

NB_TaskStatus nb_qgpssend_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QGPSEND  NEWLINE);

	ATSendStr  = NULL;	
	ATSendStr  = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_QGPSEND].ATSendStr:%s",ATSendStr);

	return nb_cmd_status;
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
					*task = _ATE;				
					user_main_printf("NBIOT has responded.");
				}
				else
				{
					at_state = _AT_ERROR;			
					user_main_printf("NBIOT did not respond.");	
			    while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_13)==1)
			   {
		       bg95module_on();
			     HAL_Delay(1000);	
			   }					
				}			
			}
			break;			
case _ATE:{
				memset(record_log,0,sizeof(record_log));
	      if(gnss_tdc_flag==1||sys.exit_flag==1 || sys.exit_flag_pa4 ||sys.exit_flag_pa0)
			  {
			     while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_13)==1)
	        {
		        bg95module_on();
			      HAL_Delay(1000);	
	        }
					if(gnss_tdc_flag==1)
	          HAL_Delay(5000);	
				}
				if(NBTask[_ATE].run(NULL) == NB_CMD_SUCC)
				{
					if(nbecho_flag==1)
					{
					  *task = _AT_CSQ;
					  nbecho_flag=0;
					}
					else if(gnss_tdc_flag==1)
					{
						*task = _AT_CCLK2;						
					}
					else
					*task = _AT_CGMM;
				 user_main_printf("Echo mode turned off successfully.");
					sprintf(record_log, "Echo mode turned off successfully.\r\n");					
				}
				else
				{
					at_state = _AT_ERROR;			
					user_main_printf("Echo mode turned off failed.");		
					sprintf(record_log, "Echo mode turned off failed.\r\n");							
				}
			}
			break;				
case _AT_CGMM:{
				if(NBTask[_AT_CGMM].run(NULL) == NB_M1_SUCC)
				{
					*task = _AT_IMEI;
					user_main_printf("Model information:BG95-M1.");
				}	
				else if(NBTask[_AT_CGMM].run(NULL) == NB_M2_SUCC)
				{
					*task = _AT_IMEI;
					user_main_printf("Model information:BG95-M2.");
				}	
				else if(NBTask[_AT_CGMM].run(NULL) == NB_M3_SUCC)
				{
					*task = _AT_IMEI;
					user_main_printf("Model information:BG95-M3.");
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
					*task=_AT_QBAND;
					user_main_printf("The IMSI number is:%s.",nb.imsi);
					memset(user.ccid,0,sizeof(user.ccid));
					memcpy(user.ccid,nb.imsi,15);						
				}
				else 
				{
					at_state = _AT_ERROR;
					user_main_printf("Failed to read IMSI number.");
				}
			}
			break;	
case _AT_QBAND:{
				if(NBTask[_AT_QBAND].run(NULL) == NB_CMD_SUCC)
				{
					user_main_printf("Frequency band configuration successful.");							
				}
				else
				{
					user_main_printf("Failed to set the Frequency band.");	
				}
				*task = _AT_IOTM;	
			}
			break;						
case _AT_IOTM:{
				if(NBTask[_AT_IOTM].run(NULL) == NB_CMD_SUCC)
				{
					user_main_printf("Configure Network Category to be Searched for under LTE RAT.");							
				}
				else
				{
					user_main_printf("Failed to set the Network Category.");	
				}
				*task = _AT_QICFG;	
			}
			break;	
case _AT_QICFG:{
				if(NBTask[_AT_QICFG].run(NULL) == NB_CMD_SUCC)
				{
					user_main_printf("Set the data format for sending and receiving.");	
					sprintf(record_log+strlen(record_log), "Set the data format for sending and receiving.\r\n");							
				}
				else
				{
					at_state = _AT_ERROR;
					user_main_printf("Failed to set the data format.");
				 sprintf(record_log+strlen(record_log), "Failed to set the data format.\r\n");		
				}
				*task = _AT_CGDCONT;	
			}
			break;
		
case _AT_CGDCONT:{
				if(strstr((char*)user.apn,"NULL") == NULL)
				{
					if(NBTask[_AT_CGDCONT].run(NULL) != NB_CMD_SUCC)
					{
						at_state = _AT_ERROR;
						user_main_printf("Failed to set APN");
				 sprintf(record_log+strlen(record_log), "Failed to set APN\r\n");							
					}
					else
					{
						user_main_printf("Set APN successfully");
					  sprintf(record_log+strlen(record_log), "Set APN successfully\r\n");		
						*task=_AT_CSQ;
					}
				}
				else
				{
					*task=_AT_CSQ;
				}
			}
			break;			

case _AT_QSCLK:{
				while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_13)==0)
				{
				bg95module_off();
				HAL_Delay(1000);
				user_main_printf("Closing NB module...");
					sprintf(record_log+strlen(record_log), "Closing NB module...\r\n");						
				}
			  if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_13)==1)
			  {
			    user_main_printf("NB module power-off successful.");
					sprintf(record_log+strlen(record_log), "NB module power-off successful.\r\n");					
			  }
				*task = _AT_IDLE;
	      nb.uplink_flag = no_status;
				stored_datalog();
			}

			break;

case _AT_CFUNOFF:{
				  *task = _AT_QSCLK;
					if(sleep_status==0)
					{
				    TimerInit( &TxTimer, OnTxTimerEvent );
						if(no_singal_flag==1 && sys.join_interval!=0)
              TimerSetValue( &TxTimer,  sys.tdc*1000*sys.join_interval); 
						else
						  TimerSetValue( &TxTimer,  sys.tdc*1000);
            TimerStart( &TxTimer);						
					}
					if(no_singal_flag==1)
					{
				  sprintf(record_log+strlen(record_log), "Signal Strength:%d *%d\r\n",nb.singal,csq_fail_log);
          no_singal_flag=0;			
					}						
					user_main_printf("Turn off the module receiving and sending RF function.");
					sprintf(record_log+strlen(record_log), "Turn off the module receiving and sending RF function.\r\n");
				csq_fail_log=0;
			}
			break;	
			
case _AT_QGPS:{
					HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);	
	        nb.uplink_flag = running;
					TimerInit( &GNSSTimer, GNSSTimerEvent );
          TimerSetValue(&GNSSTimer,10000); 
	        gnss_time_num=gnss_timer;
					if(NBTask[_AT_QGPS].run(NULL) == NB_CMD_SUCC)
					{
						*task = _AT_IDLE;
						user_main_printf("Successfully turn on GNSS,Fix Timeout:%ds",gnss_timer);
					sprintf(record_log+strlen(record_log), "Successfully turn on GNSS,Fix Timeout:%ds\r\n",gnss_timer);
            TimerStart( &GNSSTimer);
						user_main_printf("NB module is obtaining location information...");
					  sprintf(record_log+strlen(record_log), "NB module is obtaining location information...\r\n");						
					}
					else
					{
						user_main_printf("GNSS failed to turn on or GNSS is running");
					sprintf(record_log+strlen(record_log), "GNSS failed to turn on or GNSS is running\r\n");						
						*task=_AT_QGPSLOC;
					}
			}
			break;	
			
case _AT_CSQ:{
				NBTask[_AT_CSQ].get((char*)NBTask[_AT_CSQ].cmd_num);
				user_main_printf("Signal Strength:%d",nb.singal);
				if(nb_cmd_status == NB_CMD_SUCC)
				{
					if(csq_fail_log>0)
				 sprintf(record_log+strlen(record_log), "Signal Strength:99 *%d\r\n",csq_fail_log);
				 sprintf(record_log+strlen(record_log), "Signal Strength:%d\r\n",nb.singal);
					*task=(net_acc_status_led == 0)?_AT_CCLK2:_AT_CCLK;				
					nb.net_flag = success;
					sys.join_interval=0;
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
case _AT_CCLK2:{
				if(NBTask[_AT_CCLK2].run(NULL) != NB_CMD_SUCC)
				{
					at_state = _AT_ERROR;
					user_main_printf("Failed to get time");
					sprintf(record_log+strlen(record_log), "Failed to get time\r\n");				
				}		
				  *task=_AT_QNWINFO;
				if(gnss_tdc_flag==1)
					*task=_AT_QGPS;	
			}
			break;			
case _AT_QNWINFO:{
				if(NBTask[_AT_QNWINFO].run(NULL) != NB_CMD_SUCC)
				{
					user_main_printf("Unable to query network information");
					sprintf(record_log+strlen(record_log), "Unable to query network information\r\n");				
				}		
				  *task=_AT_QICSGP;
			}
			break;			

case _AT_QICSGP:{
					if(NBTask[_AT_QICSGP].run(NULL) != NB_CMD_SUCC)
					{
						at_state = _AT_ERROR;
						user_main_printf("Failed to configure parameters for TCP/IP context");
						sprintf(record_log+strlen(record_log), "Failed to configure parameters for TCP/IP context\r\n");		
					}
					else
					{
						user_main_printf("Successfully configured parameters for TCP/IP context");
						sprintf(record_log+strlen(record_log), "Successfully configured parameters for TCP/IP context\r\n");		
					}
					*task=_AT_QIDEACT;
			}
			break;	
case _AT_QIDEACT:{
				if(NBTask[_AT_QIDEACT].run(NULL) != NB_CMD_SUCC)
				{
					at_state = _AT_ERROR;
					user_main_printf("Failed to deactivate PDP context");
						sprintf(record_log+strlen(record_log), "Failed to deactivate PDP context\r\n");						
				}		
				*task=_AT_QIACT;
				user_main_printf("Successfully deactivated PDP context");
						sprintf(record_log+strlen(record_log), "Successfully deactivated PDP context\r\n");					
			}
				stored_datalog();
				memset(record_log,0,sizeof(record_log));
			break;				
case _AT_QIACT:{
				if(NBTask[_AT_QIACT].run(NULL) != NB_CMD_SUCC)
				{
					at_state = _AT_ERROR;
					user_main_printf("Failed to activate PDP context");
						sprintf(record_log+strlen(record_log), "Failed to activate PDP context\r\n");					
				}					
				*task=_AT_QDNSCFG;
				user_main_printf("Successfully activated PDP context");
						sprintf(record_log+strlen(record_log), "Successfully activated PDP context\r\n");								
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
				HAL_Delay(1000);	
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
				if(sys.cert==0)
				{
						if(tls_flag==1)
				     *task=_AT_UPLOAD_START;	
				    else
				     *task=_AT_QDNS;	
					tls_flag=0;		
				}
				else
				*task=_AT_CACERT;	
				user_main_printf("Enable SSL and configure SSL context/connect index.");				
			}
			else 
			{
					*task=_AT_QRST;
					tls_flag=0;	
				user_main_printf("Failed to enable SSL mode");
			}			
			break;

case _AT_CACERT:
			if(NBTask[_AT_CACERT].run(NULL) == NB_CMD_SUCC)
			{
				if(sys.cert==1)
				{
						if(tls_flag==1)
				     *task=_AT_UPLOAD_START;	
				    else
				     *task=_AT_QDNS;	
					tls_flag=0;				
				}
				else
				*task=_AT_CLICERT;	
				user_main_printf("Configure the path of CA certificate for SSL.");				
			}
			else 
			{
					*task=_AT_QRST;
						tls_flag=0;	
				user_main_printf("Failed to configure CA certificate");
			}		
			break;
case _AT_CLICERT:
			if(NBTask[_AT_CLICERT].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_CLIKEY;	
				user_main_printf("Configure the path of client certificate for SSL.");				
			}
			else 
			{
					*task=_AT_QRST;
				user_main_printf("Failed to configure client certificate");
			}		
			break;
case _AT_CLIKEY:
			if(NBTask[_AT_CLIKEY].run(NULL) == NB_CMD_SUCC)
			{
				if(tls_flag==1)
				*task=_AT_UPLOAD_START;	
				else
				*task=_AT_QDNS;	
				user_main_printf("Configure the path of client private key for SSL.");				
			}
			else 
			{
					*task=_AT_QRST;
				user_main_printf("Failed to configure client private key");
			}		
			tls_flag=0;	
			break;			
case _AT_QDNS:{
				if((is_ipv4_addr((char*)user.add) == 1) ||(is_ipv4_addr((char*)user.add) == 2) || (is_ipv6_addr((char*)user.add) == 1)|| (is_ipv6_addr((char*)user.add) == 2) || (sys.dns_time == 1))
				{
				  if(gps_flag==1)	
			       *task = _AT_QGPS;
			    else
			       *task =_AT_UPLOAD_START;	
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
					user_main_printf("Failed to get time");
					sprintf(record_log+strlen(record_log), "Failed to get time\r\n");					
				}		
				if(sys.tlsmod==1 && sys.protocol == MQTT_PRO)
			 {				 
				*task=_AT_QSSLCFG;	
				 tls_flag=1;
			 }
			 else
			  *task =_AT_UPLOAD_START;	
			}
			break;
			
case _AT_QGPSLOC:
			if(NBTask[_AT_QGPSLOC].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_QGPSEND;		
				user_main_printf("Successfully obtained location information");	
					sprintf(record_log+strlen(record_log), "Successfully obtained location information\r\n");						
			}
			else 
			{			
				gnss_time_num=gnss_time_num-10;
			  if(gnss_time_num==0)
			 {
				*task=_AT_QGPSEND;	
				user_main_printf("Failed to obtain location information");
					sprintf(record_log+strlen(record_log), "Failed to obtain location information\r\n");		
			 }
			 else    
			 {				 
				 TimerStart( &GNSSTimer);
				 *task=_AT_IDLE;
				 user_main_printf("Searching for location...");
			 }
		  }			
			break;	
			
case _AT_QGPSEND:
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
			if(NBTask[_AT_QGPSEND].run(NULL) == NB_CMD_SUCC)
			{
				 if(gnss_tdc_flag==1)
				{
				 *task = _AT_QSCLK;
				 gnss_tdc_flag=0;
				}
				else
				*task=_AT_UPLOAD_START;	
				user_main_printf("Successfully turn off GNSS");		
					sprintf(record_log+strlen(record_log), "Successfully turn off GNSS\r\n");					
			}
			else 
			{
				 if(gnss_tdc_flag==1)
				{
				 *task = _AT_QSCLK;
				 gnss_tdc_flag=0;
				}
				else
				*task=_AT_UPLOAD_START;	
				user_main_printf("Failed to turn off GNSS");
					sprintf(record_log+strlen(record_log), "Failed to turn off GNSS\r\n");						
			}
			break;		
			
case _AT_UPLOAD_START:{
				stored_datalog();
				memset(record_log,0,sizeof(record_log));
				nb.uplink_flag = send;
				nb.recieve_flag = NB_IDIE;
	    if(nb_start==1)
			{
				nb_start=0;
				TimerSetValue(&TxTimer,sys.tdc*1000); 
        TimerStart( &TxTimer);
			}	
			if(sys.protocol == COAP_PRO)	    {*task=_AT_COAP_CONFIG;}				
            else if(sys.protocol == UDP_PRO)	{*task=_AT_UDP_OPEN;}
			else if(sys.protocol == MQTT_PRO)	{*task=_AT_MQTT_Config;}
			else if(sys.protocol == TCP_PRO)	{*task=_AT_TCP_OPEN;}
      sprintf(record_log, "*****Upload start:%d*****\r\n",sys.uplink_count);				
			user_main_printf("*****Upload start:%d*****",sys.uplink_count++);
			txPayLoadDeal(&sensor);
			memset((char*)nb.usart.data,0,sizeof(nb.usart.data));
			}
			break;
/***************************************************COAP******************************************************************************/
case _AT_COAP_CONFIG:
			if(strstr((char*)user.add,"NULL") != NULL || strstr((char*)user.uri1,"NULL") != NULL)
			{
				*task=_AT_UPLOAD_END;
				user_main_printf("COAP parameter configuration error");
				sprintf(record_log+strlen(record_log), "COAP parameter configuration error\r\n");				
				break;
			}
			if(NBTask[_AT_COAP_CONFIG].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_COAP_OPEN;				
				user_main_printf("COAP configuration successfully");
				sprintf(record_log+strlen(record_log), "COAP configuration successfully\r\n");		
			}
			else
			{
				at_state = _AT_ERROR;
				*task = _AT_UPLOAD_END;
				user_main_printf("COAP configuration failed");
				sprintf(record_log+strlen(record_log), "COAP configuration failed\r\n");		
			}
			break;
case _AT_COAP_OPEN:
			if(NBTask[_AT_COAP_OPEN].run(NULL) == NB_OPEN_SUCC)
			{
				*task=_AT_IDLE;
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_COAP_CLOSE;
				user_main_printf("Failed to Create a CoAP session");
				sprintf(record_log+strlen(record_log), "Failed to Create a CoAP session\r\n");					
			}
			break;
case _AT_COAP_HEADER:
			if(NBTask[_AT_COAP_HEADER].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_COAP_OPTION1;
				user_main_printf("Set the CoAP message ID to 1234 and automatically generate a token");
				sprintf(record_log+strlen(record_log), "Set the CoAP message ID to 1234 and automatically generate a token\r\n");							
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_COAP_CLOSE;
				user_main_printf("Failed to Set a CoAP message ID");
				sprintf(record_log+strlen(record_log), "Failed to Set a CoAP message ID\r\n");					
			}
			break;
case _AT_COAP_OPTION1:
			if(NBTask[_AT_COAP_OPTION1].run(NULL) == NB_CMD_SUCC)
			{
				if(strstr((char*)user.uri2,"NULL") != NULL)	
				*task=_AT_COAP_SEND_CONFIG;						
        else					
				*task=_AT_COAP_OPTION2;				
				user_main_printf("Successfully configured CoAP option index 1");
				sprintf(record_log+strlen(record_log), "Successfully configured CoAP option index 1\r\n");					
			}
			else
			{
				at_state = _AT_ERROR;
				*task = _AT_COAP_CLOSE;
				user_main_printf("Failed to configure the CoAP option index 1");
				sprintf(record_log+strlen(record_log), "Failed to configure the CoAP option index 1\r\n");							
			}
			break;
case _AT_COAP_OPTION2:
			if(NBTask[_AT_COAP_OPTION2].run(NULL) == NB_CMD_SUCC)
			{
				if(strstr((char*)user.uri3,"NULL") != NULL)	
				*task=_AT_COAP_SEND_CONFIG;						
        else					
				*task=_AT_COAP_OPTION3;				
				user_main_printf("Successfully configured CoAP option index 2");
				sprintf(record_log+strlen(record_log), "Successfully configured CoAP option index 2\r\n");						
			}
			else
			{
				at_state = _AT_ERROR;
				*task = _AT_COAP_CLOSE;
				user_main_printf("Failed to configure the CoAP option index 2");
				sprintf(record_log+strlen(record_log), "Failed to configure the CoAP option index 2\r\n");						
			}
			break;
case _AT_COAP_OPTION3:
			if(NBTask[_AT_COAP_OPTION3].run(NULL) == NB_CMD_SUCC)
			{
				if(strstr((char*)user.uri4,"NULL") != NULL)	
				*task=_AT_COAP_SEND_CONFIG;						
        else					
				*task=_AT_COAP_OPTION4;				
				user_main_printf("Successfully configured CoAP option index 3");
				sprintf(record_log+strlen(record_log), "Successfully configured CoAP option index 3\r\n");						
			}
			else
			{
				at_state = _AT_ERROR;
				*task = _AT_COAP_CLOSE;
				user_main_printf("Failed to configure the CoAP option index 3");
				sprintf(record_log+strlen(record_log), "Failed to configure the CoAP option index 3\r\n");						
			}
			break;
case _AT_COAP_OPTION4:
			if(NBTask[_AT_COAP_OPTION4].run(NULL) == NB_CMD_SUCC)
			{
				if(strstr((char*)user.uri5,"NULL") != NULL)	
				*task=_AT_COAP_SEND_CONFIG;						
        else					
				*task=_AT_COAP_OPTION5;				
				user_main_printf("Successfully configured CoAP option index 4");
				sprintf(record_log+strlen(record_log), "Successfully configured CoAP option index 4\r\n");						
			}
			else
			{
				at_state = _AT_ERROR;
				*task = _AT_COAP_CLOSE;
				user_main_printf("Failed to configure the CoAP option index 4");
				sprintf(record_log+strlen(record_log), "Failed to configure the CoAP option index 4\r\n");						
			}
			break;
case _AT_COAP_OPTION5:
			if(NBTask[_AT_COAP_OPTION5].run(NULL) == NB_CMD_SUCC)
			{
				if(strstr((char*)user.uri6,"NULL") != NULL)	
				*task=_AT_COAP_SEND_CONFIG;						
        else					
				*task=_AT_COAP_OPTION6;				
				user_main_printf("Successfully configured CoAP option index 5");
				sprintf(record_log+strlen(record_log), "Successfully configured CoAP option index 5\r\n");						
			}
			else
			{
				at_state = _AT_ERROR;
				*task = _AT_COAP_CLOSE;
				user_main_printf("Failed to configure the CoAP option index 5");
				sprintf(record_log+strlen(record_log), "Failed to configure the CoAP option index 5\r\n");						
			}
			break;
case _AT_COAP_OPTION6:
			stored_datalog();
			memset(record_log,0,sizeof(record_log));
			if(NBTask[_AT_COAP_OPTION6].run(NULL) == NB_CMD_SUCC)
			{
				if(strstr((char*)user.uri7,"NULL") != NULL)	
				*task=_AT_COAP_SEND_CONFIG;						
        else					
				*task=_AT_COAP_OPTION7;				
				user_main_printf("Successfully configured CoAP option index 6");
				sprintf(record_log+strlen(record_log), "Successfully configured CoAP option index 6\r\n");						
			}
			else
			{
				at_state = _AT_ERROR;
				*task = _AT_COAP_CLOSE;
				user_main_printf("Failed to configure the CoAP option index 6");
				sprintf(record_log+strlen(record_log), "Failed to configure the CoAP option index 6\r\n");						
			}
			break;
case _AT_COAP_OPTION7:
			if(NBTask[_AT_COAP_OPTION7].run(NULL) == NB_CMD_SUCC)
			{
				if(strstr((char*)user.uri8,"NULL") != NULL)	
				*task=_AT_COAP_SEND_CONFIG;						
        else					
				*task=_AT_COAP_OPTION8;				
				user_main_printf("Successfully configured CoAP option index 7");
				sprintf(record_log+strlen(record_log), "Successfully configured CoAP option index 7\r\n");						
			}
			else
			{
				at_state = _AT_ERROR;
				*task = _AT_COAP_CLOSE;
				user_main_printf("Failed to configure the CoAP option index 7");
				sprintf(record_log+strlen(record_log), "Failed to configure the CoAP option index 7\r\n");						
			}
			break;
case _AT_COAP_OPTION8:
			if(NBTask[_AT_COAP_OPTION8].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_COAP_SEND_CONFIG;									
				user_main_printf("Successfully configured CoAP option index 8");
				sprintf(record_log+strlen(record_log), "Successfully configured CoAP option index 8\r\n");						
			}
			else
			{
				at_state = _AT_ERROR;
				*task = _AT_COAP_CLOSE;
				user_main_printf("Failed to configure the CoAP option index 8");
				sprintf(record_log+strlen(record_log), "Failed to configure the CoAP option index 8\r\n");						
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
				sprintf(record_log+strlen(record_log), "Failed to upload data\r\n");						
				break;
			}
			
case _AT_COAP_SEND:
			if(NBTask[_AT_COAP_SEND].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_COAP_READ;
				user_main_printf("Upload data successfully");	
				sprintf(record_log+strlen(record_log), "Upload data successfully\r\n");						
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_COAP_CLOSE;
				user_main_printf("Failed to upload data");
				sprintf(record_log+strlen(record_log), "Failed to upload data\r\n");						
			}
			break;		
case _AT_COAP_READ:
			HAL_Delay(sys.rxdl);
	    succes_Status=true;
      reupload_time=0;
			*task = _AT_COAP_CLOSE;
			break;
case _AT_COAP_CLOSE:
			if(NBTask[_AT_COAP_CLOSE].run(NULL) == NB_CLOSE_SUCC)
			{
				if(succes_Status==true)
				*task=_AT_UPLOAD_SUCC;
				else
				*task=_AT_UPLOAD_FAIL;	
				user_main_printf("Closed the CoAP session successfully");
				sprintf(record_log+strlen(record_log), "Closed the CoAP session successfully\r\n");						
			}
			else 
			{
				at_state = _AT_ERROR;
				*task=_AT_UPLOAD_FAIL;
				user_main_printf("Failed to close CoAP session");
				sprintf(record_log+strlen(record_log), "Failed to close CoAP session\r\n");						
			}
			break;
case _AT_COAP_URI:
			uri_state = NBTask[_AT_COAP_URI].run(NULL);
       if(uri_state == NB_QCOAPOPEN_SUCC)
			{
				*task=_AT_COAP_HEADER;				
				user_main_printf("Create a CoAP session and connect to the CoAP server");
				sprintf(record_log+strlen(record_log), "Create a CoAP session and connect to the CoAP server\r\n");						
			}
			else
			{
				*task = _AT_IDLE;
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
				HAL_Delay(sys.rxdl);
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
				*task=_AT_MQTT_SEND;			
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
				*task=_AT_MQTT_SEND;			
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
				*task=_AT_MQTT_SEND;			
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
				*task=_AT_MQTT_SEND;			
			}
			else 
			{
				at_state = _AT_ERROR;
				*task = _AT_MQTT_CLOSE;
				user_main_printf("Failed to Set PUB");
				sprintf(record_log+strlen(record_log), "Failed to Set PUB\r\n");					
			}
				break;				
case _AT_MQTT_PUBEX:
			if(NBTask[_AT_MQTT_PUBEX].run(NULL) == NB_CMD_SUCC)
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
	    succes_Status=true;
      reupload_time=0;
			*task = _AT_MQTT_DISC;
			break;
			
case _AT_MQTT_DISC:
			if(NBTask[_AT_MQTT_DISC].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_MQTT_URI;						
			}
			else
			{
				at_state = _AT_ERROR;
				*task=_AT_MQTT_CLOSE;
				user_main_printf("Failed to disconnect client from MQTT server");
				sprintf(record_log+strlen(record_log), "Failed to disconnect client from MQTT server\r\n");					
			}
			break;
			
case _AT_MQTT_CLOSE:
			if(NBTask[_AT_MQTT_CLOSE].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_UPLOAD_FAIL;	
				user_main_printf("The TCP connection is closed successfully");
				sprintf(record_log+strlen(record_log), "The TCP connection is closed successfully\r\n");					
			}
			else
			{
				at_state = _AT_ERROR;
				*task=_AT_UPLOAD_FAIL;
				user_main_printf("Failed to close TCP connection");
				sprintf(record_log+strlen(record_log), "Failed to close TCP connection\r\n");					
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
							*task=_AT_MQTT_PUBEX;
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
				case NB_DISC_SUCC:
							*task = _AT_UPLOAD_SUCC;
							user_main_printf("Successfully disconnected the client from the MQTT server");
							sprintf(record_log+strlen(record_log), "Successfully disconnected the client from the MQTT server\r\n");
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
				udp_close_flag=1;
				at_state = _AT_ERROR;
			 *task=_AT_UDP_CLOSE;
				user_main_printf("Failed to open a Socket Service");
				sprintf(record_log+strlen(record_log),"Failed to open a Socket Service\r\n");				
			}
			break;

case _AT_UDP_SEND:	
			if(sys.platform==0)	
			{
			  if(NBTask[_AT_UDP_SENDEX].run(NULL) == NB_CMD_SUCC)
			  {
				  *task = _AT_IDLE;
			  }
			  else 
			  {
					udp_close_flag=1;
				  at_state = _AT_ERROR;
			    *task=_AT_UDP_CLOSE;
				  user_main_printf("Failed to upload data");
				sprintf(record_log+strlen(record_log),"Failed to upload data\r\n");						
			  }				
			}
			else
			{
			  if(NBTask[_AT_UDP_SEND].run(NULL) == NB_CMD_SUCC)
			  {
				  *task = _AT_UDP_DATA;
			  }
			  else 
			  {
					udp_close_flag=1;
				  at_state = _AT_ERROR;
			    *task=_AT_UDP_CLOSE;
				  user_main_printf("Failed to upload data");
				sprintf(record_log+strlen(record_log),"Failed to upload data\r\n");						
			  }	
			}
				break;		
case _AT_UDP_DATA:
			if(NBTask[_AT_UDP_DATA].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_IDLE;
			}
			else 
			{
				udp_close_flag=1;
				at_state = _AT_ERROR;
				*task = _AT_UDP_CLOSE;
				user_main_printf("Failed to upload data");
				sprintf(record_log+strlen(record_log),"Failed to upload data\r\n");				
			}
			break;
case _AT_UDP_READ:
			HAL_Delay(sys.rxdl);
			*task = _AT_UDP_QIRD;
	    succes_Status=true;
      reupload_time=0;
			break;

case _AT_UDP_QIRD:
			if(NBTask[_AT_UDP_QIRD].run(NULL) == NB_CMD_SUCC)
			{
				*task = _AT_IDLE;
				user_main_printf("Retrieve the Received downlink Data");
				sprintf(record_log+strlen(record_log),"Retrieve the Received downlink Data\r\n");					
			}
			else
			{
				udp_close_flag=1;
				at_state = _AT_ERROR;		
        *task = _AT_UDP_CLOSE;				
				user_main_printf("Retrieve received data failed");
				sprintf(record_log+strlen(record_log),"Retrieve received data failed\r\n");					
			}
			break;			
case _AT_UDP_CLOSE:
			if(NBTask[_AT_UDP_CLOSE].run(NULL) == NB_CMD_SUCC)
			{
				if(udp_close_flag==1)
				  *task=_AT_UPLOAD_FAIL;
				else
				  *task = _AT_UPLOAD_SUCC;	
       udp_close_flag=0;				
			 user_main_printf("Close the port successfully");
				sprintf(record_log+strlen(record_log), "Close the port successfully\r\n");				
			}
			else
			{
				udp_close_flag=0;
				at_state = _AT_ERROR;		
        *task=_AT_UPLOAD_FAIL;				
				user_main_printf("Failed to close the port");
				sprintf(record_log+strlen(record_log),"Failed to close the port\r\n");				
			}
			break;
case _AT_UDP_URI:
			uri_state = NBTask[_AT_UDP_URI].run(NULL);

			if(uri_state == NB_SEND_SUCC)			
			{
				*task = _AT_UDP_READ;		
				user_main_printf("Upload data successfully");	
				sprintf(record_log+strlen(record_log), "Upload data successfully\r\n");				
			}
				else if(uri_state == NB_RD_SUCC)			
			{
				*task = _AT_UDP_CLOSE;
        user_main_printf("Datagram is sent by RF");				
				sprintf(record_log+strlen(record_log), "Datagram is sent by RF\r\n");				
			}
			else if(uri_state == NB_SEND_FAIL)			
			{
				udp_close_flag=1;
				*task = _AT_UDP_CLOSE;		
				user_main_printf("Failed to upload data");
				sprintf(record_log+strlen(record_log), "Failed to upload data\r\n");				
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
				  *task=_AT_TCP_SEND;	
				user_main_printf("Open a Socket Service successfully");						
				sprintf(record_log+strlen(record_log), "Open a Socket Service successfully\r\n");				
				
			}
			else 
			{
				tcp_close_flag=1;
				at_state = _AT_ERROR;
				*task = _AT_TCP_CLOSE;
				user_main_printf("Failed to open a Socket Service");
				sprintf(record_log+strlen(record_log), "Failed to open a Socket Service\r\n");				
			}
			break;
case _AT_TCP_SEND:
			if(sys.platform==0)	
			{
			  if(NBTask[_AT_TCP_SENDEX].run(NULL) == NB_CMD_SUCC)
			  {
				  *task = _AT_IDLE;
			  }
			  else 
			  {
					tcp_close_flag=1;
				  at_state = _AT_ERROR;
			    *task=_AT_TCP_CLOSE;
				  user_main_printf("Failed to upload data");
				sprintf(record_log+strlen(record_log), "Failed to upload data\r\n");	
			  }
			}
			else
			{
			  if(NBTask[_AT_TCP_SEND].run(NULL) == NB_CMD_SUCC)
			  {
				  *task=_AT_TCP_DATA;				
			  }
			  else	
			  {
					 tcp_close_flag=1;
				   at_state = _AT_ERROR;
				   *task = _AT_TCP_CLOSE;
				   user_main_printf("Failed to upload data");		
				sprintf(record_log+strlen(record_log), "Failed to upload data\r\n");						
			}
		}
				break;	
case _AT_TCP_DATA:
			if(NBTask[_AT_TCP_DATA].run(NULL) == NB_CMD_SUCC)
			{
				*task=_AT_IDLE;
			}
			else 
			{
				tcp_close_flag=1;
				at_state = _AT_ERROR;
				*task = _AT_TCP_CLOSE;
				user_main_printf("Failed to upload data");
				sprintf(record_log+strlen(record_log), "Failed to upload data\r\n");					
			}
			break;		
case _AT_TCP_READ:
			HAL_Delay(sys.rxdl);
			*task = _AT_TCP_QIRD;
	    succes_Status=true;
      reupload_time=0;
			break;

case _AT_TCP_QIRD:
			if(NBTask[_AT_TCP_QIRD].run(NULL) == NB_CMD_SUCC)
			{
				*task = _AT_IDLE;
				user_main_printf("Retrieve the Received downlink Data");
				sprintf(record_log+strlen(record_log), "Retrieve the Received downlink Data\r\n");					
			}
			else
			{
				tcp_close_flag=1;
				at_state = _AT_ERROR;		
        *task = _AT_TCP_CLOSE;				
				user_main_printf("Retrieve received data failed");
				sprintf(record_log+strlen(record_log), "Retrieve received data failed\r\n");					
			}
			break;	
			
case _AT_TCP_CLOSE:
			if(NBTask[_AT_TCP_CLOSE].run(NULL) == NB_CMD_SUCC)
			{
				if(tcp_close_flag==1)
				*task=_AT_UPLOAD_FAIL;
				else
				*task=_AT_UPLOAD_SUCC;
				 tcp_close_flag=0;
				user_main_printf("Close the port successfully");
				sprintf(record_log+strlen(record_log), "Close the port successfully\r\n");
			}
			else 
			{
				tcp_close_flag=0;
				at_state = _AT_ERROR;
				*task=_AT_UPLOAD_FAIL;
				user_main_printf("Failed to close the port");
				sprintf(record_log+strlen(record_log), "Failed to close the port\r\n");				
			}				
			break;
case _AT_TCP_URI:
			uri_state = NBTask[_AT_TCP_URI].run(NULL);

			if(uri_state == NB_SEND_SUCC)			
			{
				*task = _AT_TCP_READ;	
				user_main_printf("Upload data successfully");	
				sprintf(record_log+strlen(record_log), "Upload data successfully\r\n");							
			}
				else if(uri_state == NB_RD_SUCC)			
			{
				*task = _AT_TCP_CLOSE;
        user_main_printf("Datagram is sent by RF");		
				sprintf(record_log+strlen(record_log), "Datagram is sent by RF\r\n");							
			}
			else if(uri_state == NB_SEND_FAIL)			
			{
				tcp_close_flag=1;
				*task = _AT_TCP_CLOSE;		
				user_main_printf("Failed to upload data");
				sprintf(record_log+strlen(record_log), "Failed to upload data\r\n");							
			}
			else
			{
				*task = _AT_IDLE;	
			}
			break;
/******************************************************************************************************************************************/			
case _AT_UPLOAD_END:
			user_main_printf("*****End of upload*****\r\n");	
			sprintf(record_log+strlen(record_log), "*****End of upload*****\r\n");
			stored_datalog();
			memset(record_log,0,sizeof(record_log));
      error_num = 0;
			memset((char*)nb.usart.data,0,sizeof(nb.usart.data));
			if(succes_Status==false && reupload_time<3)
			{
				nb.uplink_flag = send;
				nb.recieve_flag = NB_IDIE;
        if(sys.protocol == COAP_PRO)	{*task=_AT_COAP_CONFIG;}				
        	else if(sys.protocol == UDP_PRO)	{*task=_AT_UDP_OPEN;}
		  	else if(sys.protocol == MQTT_PRO)	{*task=_AT_MQTT_Config;}
			  else if(sys.protocol == TCP_PRO)	{*task=_AT_TCP_OPEN;}
			  reupload_time++;
			}	
		 if(succes_Status==true ||reupload_time==3)
		{

			reupload_time=0;	
      sensor.exit_state = 0;		
      sensor.exit_state_pa4 = 0;	
      sensor.exit_state_pa0 = 0;																 
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
				user_main_printf("Restart the module...");
				sprintf(record_log+strlen(record_log), "Restart the module...\r\n");
				user_main_printf("No response when shutting down");
				sprintf(record_log+strlen(record_log), "No response when shutting down\r\n");
				while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_13)==0)
				{
				bg95module_off();
				HAL_Delay(1000);
				user_main_printf("Closing NB module...");
					sprintf(record_log+strlen(record_log), "Closing NB module...\r\n");					
				}
			  if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_13)==1)
			  {
			    user_main_printf("NB module power-off successful.");
					sprintf(record_log+strlen(record_log), "NB module power-off successful.\r\n");					
			  }
				stored_datalog();
				memset(record_log,0,sizeof(record_log));
		   *task=_AT_IDLE;
				nb.net_flag = no_status;
				net_acc_status_led = 0;
				nb.uplink_flag = no_status;		
        if(act_network_flag==0||dns_reset_num>2)	
				{					
        TimerInit( &TxTimer, OnTxTimerEvent );
        TimerSetValue( &TxTimer,  sys.tdc*1000); 
        TimerStart( &TxTimer);	
				act_network_flag=0;
				}
				else
				{
					is_time_to_send=1;
					act_network_flag=0;
				}
				
				break;
			}
case _AT_URI:{
				if(sys.protocol == COAP_PRO)	*task = _AT_COAP_URI;
				else if(sys.protocol == UDP_PRO)	*task = _AT_UDP_URI;
				else if(sys.protocol == MQTT_PRO)	*task = _AT_MQTT_URI;
				else if(sys.protocol == TCP_PRO)	*task = _AT_TCP_URI;
			}
			break;
case _AT_FLAG_INIT:{
				nb.uplink_flag = no_status;
				nb.net_flag = no_status;
				net_acc_status_led = 0;
		    tls_flag=0;
		    uint32_t r_sleep=*(__IO uint32_t *)(EEPROM_USER_SLEEP_FLAG);	
		    //printf("r_sleep:%x",r_sleep);
		      if(r_sleep==0xA8)
				{
				 at_sleep_flag=1;
				}	
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