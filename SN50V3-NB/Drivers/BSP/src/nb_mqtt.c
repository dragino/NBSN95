#include "nb_mqtt.h"
#include "time.h"
#include <time.h>
#include "nb_payload.h"
char buff[2000]={0};
char downlink_data[1000]={0};
uint8_t at_downlink_flag=0;

extern void pro_data(void);
extern void pro_data_thingspeak(void);
extern uint8_t mqtt_qos;
extern bool DNS_RE_FLAG;
extern char 	*ATSendStr;
extern int 	len_string;
extern uint8_t  try_num;
extern uint8_t  ota_continued;
extern NB_TaskStatus  nb_cmd_status;
extern uint8_t downlink_check_event;
extern uint8_t received_dwonlink_flags;
extern void downilnk_check_data(void);
extern void downilnk_ack_data(void);
extern uint8_t connect_status;
extern uint16_t chunk_index;
extern uint8_t rece_firmware_end;
extern volatile uint8_t get_sharekeys;
extern volatile uint32_t server_fw_size,record_server_fw_size;
extern volatile uint8_t check_firmware_exists;
extern uint8_t  ota_fail_times;
extern uint8_t  ota_upgrade_error;
extern uint8_t  Datalog_uplink;
extern uint8_t read_flag;
char server_fw_version[15]="\0";
char server_fw_title[20]="\0";
extern uint32_t zb25vq32_firmware_store_address;
/**
	* @brief  Configure CA certificate.
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_QSSLCACERT_run(const char* param)
{	
	NBTask[_AT_QSSLCACERT].set(param);
	
	if(nb_at_send(&NBTask[_AT_QSSLCACERT]) == NB_CMD_SUCC)
		nb_cmd_status = NB_CMD_SUCC;
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;
}

NB_TaskStatus nb_QSSLCACERT_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QSSLCFG "=0,0,\"cacert\"\r\n");	
	
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	
	user_main_debug("openPayload:%s",ATSendStr);
	return nb_cmd_status;	
}
/**
	* @brief  Configure client certificate.
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_QSSLCCERT_run(const char* param)
{	
	NBTask[_AT_QSSLCCERT].set(param);
	
	if(nb_at_send(&NBTask[_AT_QSSLCCERT]) == NB_CMD_SUCC)
		nb_cmd_status = NB_CMD_SUCC;
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;
}

NB_TaskStatus nb_QSSLCCERT_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QSSLCFG "=0,0,\"clientcert\"\r\n");	
	
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	
	user_main_debug("openPayload:%s",ATSendStr);
	return nb_cmd_status;	
}
/**
	* @brief  Configure client private key.
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_QSSLKEY_run(const char* param)
{	
	NBTask[_AT_QSSLKEY].set(param);
	
	if(nb_at_send(&NBTask[_AT_QSSLKEY]) == NB_CMD_SUCC)
		nb_cmd_status = NB_CMD_SUCC;
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;
}

NB_TaskStatus nb_QSSLKEY_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QSSLCFG "=0,0,\"clientkey\"\r\n");	
	
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	
	user_main_debug("openPayload:%s",ATSendStr);
	return nb_cmd_status;	
}

/**
	* @brief  Set MQTT configuration parameters
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_config_set(const char* param)
{
	ATSendStr  = NULL;
	ATSendStr = AT QMTCFG "=\"version\",0,1" NEWLINE;
	len_string = sizeof(AT QMTCFG "=\"version\",0,1" NEWLINE) - 1;
	
	try_num = 4;
	while(try_num--)
	{
		if(nb_at_send(&NBTask[_AT_MQTT_Config])== NB_CMD_SUCC)
		{
			break;
		}
		else
			HAL_Delay(100);
	}
	
	return nb_cmd_status;
}

/**
	* @brief  Manage server and client authentication.
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_QSSLCFG_run(const char* param)
{	
	try_num = 4;
	NBTask[_AT_QSSLCFG].set(param);
	
	if(nb_at_send(&NBTask[_AT_QSSLCFG]) == NB_CMD_SUCC)
		nb_cmd_status = NB_CMD_SUCC;
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;
}

NB_TaskStatus nb_QSSLCFG_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QSSLCFG "=0,0,\"seclevel\",");	
	sprintf(buff+strlen(buff), "%d\r\n", sys.cert);

	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	
	user_main_debug("openPayload:%s",ATSendStr);
	return nb_cmd_status;	
}

/**
	* @brief  Manage server and client authentication.
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_QMTCFGSSL_run(const char* param)
{	
	try_num = 4;
	NBTask[_AT_QMTCFG_SSL].set(param);
	
	if(nb_at_send(&NBTask[_AT_QMTCFG_SSL]) == NB_CMD_SUCC)
		nb_cmd_status = NB_CMD_SUCC;
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;
}

NB_TaskStatus nb_QMTCFGSSL_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	
	strcat(buff,AT QMTCFG "=\"ssl\",0,1,0,0\r\n");	
	
	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	
	user_main_debug("openPayload:%s",ATSendStr);
	return nb_cmd_status;	
}
/**
	* @brief  Open MQTT port
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_open_run(const char* param)
{	
	try_num = 4;
	NBTask[_AT_MQTT_OPEN].set(param);
	
	if(nb_at_send(&NBTask[_AT_MQTT_OPEN]) == NB_CMD_SUCC)
		nb_cmd_status = NB_CMD_SUCC;
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;
}

NB_TaskStatus nb_MQTT_open_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	if(DNS_RE_FLAG==1)
	{
	char* pos = strchr((char*)user.add_ip,',');
	
	strcat(buff,AT QMTOPEN "=0,\"");
	memcpy(buff+strlen(buff),user.add_ip,(pos-(char*)user.add_ip));
	strcat(buff,"\"");
	strcat(buff,(char*)&user.add_ip[(pos-(char*)user.add_ip)]);
	}
	else
  {
	char* pos = strchr((char*)user.add,',');
	
	strcat(buff,AT QMTOPEN "=0,\"");
	memcpy(buff+strlen(buff),user.add,(pos-(char*)user.add));
	strcat(buff,"\"");
	strcat(buff,(char*)&user.add[(pos-(char*)user.add)]);	
	}
	strcat(buff,"\r\n");

	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	
	user_main_debug("openPayload:%s",ATSendStr);
	return nb_cmd_status;	
}

/**
	* @brief  connect to the server
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_conn_run(const char* param)
{
	try_num = 4;
	NBTask[_AT_MQTT_CONN].set(param);
		
	if(nb_at_send(&NBTask[_AT_MQTT_CONN]) == NB_CMD_SUCC)
		nb_cmd_status = NB_CMD_SUCC;
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;
}

NB_TaskStatus nb_MQTT_conn_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QMTCONN"=0,\""); 
	strcat(buff,(char*)user.client);
	if(strstr((char*)user.uname,"NULL") ==NULL)
	{
		strcat(buff,"\",\"");
		strcat(buff,(char*)user.uname);
		if(strstr((char*)user.pwd,"NULL") ==NULL)
		{
			strcat(buff,"\",\"");
			strcat(buff,(char*)user.pwd);
		}
	}
	strcat(buff,"\"\r\n");

	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("connPayload:%s",ATSendStr);
	return nb_cmd_status;
}

/**
	* @brief  Subscribe to MQTT topics
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_sub_run(const char* param)
{
	try_num = 4;
	NBTask[_AT_MQTT_SUB].set(NULL);

	if(nb_at_send(&NBTask[_AT_MQTT_SUB]) == NB_CMD_SUCC)
		nb_cmd_status = NB_CMD_SUCC;
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;
}

NB_TaskStatus nb_MQTT_sub_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QMTSUB"=0,1,\"");

	if(sys.platform==1){
	strcat(buff,"channels/");
	strcat(buff,(char*)user.subtopic);	
	strcat(buff,"/subscribe");
	}
	else if(sys.platform==3)
	{
	strcat(buff,"tb/mqtt-integration-tutorial/sensors/");
	strcat(buff,(char*)user.subtopic);	
	strcat(buff,"/rx");	
	}
	else
	strcat(buff,(char*)user.subtopic);
	if(mqtt_qos==2)
	strcat(buff,(char*)"\",2\r\n");	
	else if(mqtt_qos==0)
	strcat(buff,(char*)"\",0\r\n");	
	else if(mqtt_qos==1)
	strcat(buff,(char*)"\",1\r\n");	
	
	ATSendStr  = NULL;
	ATSendStr  = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_MQTT_SUB].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}

/**
	* @brief  Send MQTT publish topic data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_pub_run(const char* param)
{
	try_num = 4;
	NBTask[_AT_MQTT_PUB].set(NULL);

	if(nb_at_send(&NBTask[_AT_MQTT_PUB]) == NB_CMD_SUCC)
	{
		nb_cmd_status = NB_CMD_SUCC;
	}
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;
}

NB_TaskStatus nb_MQTT_pub_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	if(mqtt_qos==2)
	strcat(buff,AT QMTPUB"=0,1,2,0,\"");
	else if(mqtt_qos==0)	
	strcat(buff,AT QMTPUB"=0,0,0,0,\"");
	else if(mqtt_qos==1)	
	strcat(buff,AT QMTPUB"=0,1,1,0,\"");
	
	strcat(buff,(char*)user.pubtopic);
	strcat(buff,(char*)"\",");
	sprintf(buff+strlen(buff), "%d\r\n", sensor.data_len);
	
	ATSendStr  = NULL;
	ATSendStr  = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_MQTT_PUB].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}

/**
	* @brief  Send MQTT publish topic1 data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_pub1_run(const char* param)
{
	try_num = 4;
	NBTask[_AT_MQTT_PUB1].set(NULL);

	if(nb_at_send(&NBTask[_AT_MQTT_PUB1]) == NB_CMD_SUCC)
	{
		nb_cmd_status = NB_CMD_SUCC;
	}
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;
}

NB_TaskStatus nb_MQTT_pub1_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	if(mqtt_qos==2)
	strcat(buff,AT QMTPUB"=0,1,2,0,\"channels/");
	else if(mqtt_qos==0)	
	strcat(buff,AT QMTPUB"=0,0,0,0,\"channels/");
	else if(mqtt_qos==1)	
	strcat(buff,AT QMTPUB"=0,1,1,0,\"channels/");
	
	strcat(buff,(char*)user.pubtopic);
	strcat(buff,(char*)"/publish");
	strcat(buff,(char*)"\",");
  pro_data_thingspeak();
	
	ATSendStr  = NULL;
	ATSendStr  = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_MQTT_PUB].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}


/**
	* @brief  Send MQTT publish topic2 data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_pub2_run(const char* param)
{
	try_num = 4;
	NBTask[_AT_MQTT_PUB2].set(NULL);

	if(nb_at_send(&NBTask[_AT_MQTT_PUB2]) == NB_CMD_SUCC)
	{
		nb_cmd_status = NB_CMD_SUCC;
	}
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;
}

NB_TaskStatus nb_MQTT_pub2_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	if(mqtt_qos==2)
	strcat(buff,AT QMTPUB"=0,1,2,0,\"");
	else if(mqtt_qos==0)	
	strcat(buff,AT QMTPUB"=0,0,0,0,\"");
	else if(mqtt_qos==1)	
	strcat(buff,AT QMTPUB"=0,1,1,0,\"");

	strcat(buff,(char*)user.pubtopic);
	strcat(buff,(char*)"\",");
	pro_data();	
	strcat(buff,"\r\n");	
	ATSendStr  = NULL;	
	ATSendStr  = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_MQTT_PUB].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}


/**
	* @brief  Send MQTT publish topic3 data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_pub3_run(const char* param)
{
	try_num = 4;
	NBTask[_AT_MQTT_PUB3].set(NULL);

	if(nb_at_send(&NBTask[_AT_MQTT_PUB3]) == NB_CMD_SUCC)
	{
		nb_cmd_status = NB_CMD_SUCC;
	}
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;
}

NB_TaskStatus nb_MQTT_pub3_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	if(mqtt_qos==2)
	strcat(buff,AT QMTPUB"=0,1,2,0,\"");
	else if(mqtt_qos==0)	
	strcat(buff,AT QMTPUB"=0,0,0,0,\"");
	else if(mqtt_qos==1)	
	strcat(buff,AT QMTPUB"=0,1,1,0,\"");	
	
	strcat(buff,(char*)user.pubtopic);
	strcat(buff,(char*)"\",");
	pro_data();
	strcat(buff,"\r\n");
	
	ATSendStr  = NULL;	
	ATSendStr  = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_MQTT_PUB].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}
/**
	* @brief  Send MQTT publish topic5 data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_pub5_run(const char* param)
{
	try_num = 4;
	NBTask[_AT_MQTT_PUB5].set(NULL);

	if(nb_at_send(&NBTask[_AT_MQTT_PUB5]) == NB_CMD_SUCC)
	{
		nb_cmd_status = NB_CMD_SUCC;
	}
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;
}

NB_TaskStatus nb_MQTT_pub5_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	if(mqtt_qos==2)
	strcat(buff,AT QMTPUB"=0,1,2,0,\"");
	else if(mqtt_qos==0)		
	strcat(buff,AT QMTPUB"=0,0,0,0,\"");
	else if(mqtt_qos==1)	
	strcat(buff,AT QMTPUB"=0,1,1,0,\"");
	strcat(buff,(char*)user.pubtopic);
	strcat(buff,(char*)"\",");
	if(downlink_check_event==1)
		downilnk_check_data();
	else if(received_dwonlink_flags==1)
		downilnk_ack_data();	
	else if(Datalog_uplink==1&&read_flag==1)
		Uplink_serial_log();	
	else if(ota_upgrade_error==1)
		Thingseye_mqtt_send_message_error_1();		
	else if(ota_upgrade_error==2)
		Thingseye_mqtt_send_message_error_2();	
	else if(ota_upgrade_error==3)
		Thingseye_mqtt_send_message_error_3();	
	else if(ota_upgrade_error==4)
		Thingseye_mqtt_send_message_error_4();	
	else if(ota_upgrade_error==5)
		Thingseye_mqtt_send_message_error_5();	
	else if(ota_upgrade_error==6)
		Thingseye_mqtt_send_message_error_6();	
	else if(ota_upgrade_error==7)
		Thingseye_mqtt_send_message_error_7();		
	else if(ota_upgrade_error==100)
		Thingseye_mqtt_send_message_update();		
	else	
    pro_data();
	strcat(buff,"\r\n");			
	ATSendStr  = NULL;
	ATSendStr  = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_MQTT_PUB].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}
/**
	* @brief  Send data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_send_run(const char* param)
{
	try_num = 4;
	NBTask[_AT_MQTT_SEND].set(NULL);

	if(nb_at_send(&NBTask[_AT_MQTT_SEND]) == NB_CMD_SUCC)
	{
		nb_cmd_status = NB_CMD_SUCC;
	}
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;
}

NB_TaskStatus nb_MQTT_send_set(const char* param)
{	
	ATSendStr  = NULL;
	ATSendStr  = sensor.data;
	len_string = sensor.data_len;
	user_main_debug("NBTask[_AT_MQTT_SEND].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}

/**
	* @brief  Read MQTT sublish topic data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_data_read_run(const char* param)
{
	return nb_cmd_status;
}

NB_TaskStatus nb_MQTT_data_read_set(const char* param)
{
	uint8_t downlink_code=0;
	memset(downlink_data,0,1000);		
	char* pos_start  = strstr((char*)nb.usart.data,"\",\"");	user_main_debug("pos_start:%p",&pos_start);
	if(sys.downlink_debug==1)
	{
	 user_main_printf("Debug downlink data:%s",nb.usart.data);
	}
	if(strstr((char*)pos_start,"{\"Config\":\"[") != NULL)
	{
	 downlink_code=1;
	}
	if(strstr((char*)nb.usart.data,"Event:Status") != NULL)
	{
	  downlink_check_event=1;
	}	
	if(downlink_code==1)
	{
	  char* pos_end    = strchr(pos_start,']');			user_main_debug("pos_end:%p",&pos_end);		
	  memcpy(downlink_data,&nb.usart.data[pos_start-((char*)nb.usart.data)+14],(pos_end-pos_start-13));	
	  at_downlink_flag=1;	
	}
	else
	{
		char* pos_end    = strchr(pos_start,'\n');			user_main_debug("pos_end:%p",&pos_end);		
	  memcpy(downlink_data,&nb.usart.data[pos_start-((char*)nb.usart.data)+3],(pos_end-pos_start-5));		
	}
	user_main_printf("Received downlink data:%s",downlink_data);
	if(downlink_check_event==0)
	 rxPayLoadDeal(downlink_data);
	
	return nb_cmd_status;
}

/**
	* @brief  Close MQTT port
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_close_run(const char* param)
{
	ATSendStr  = NULL;
	ATSendStr = AT QMTDISC"=0" NEWLINE;
	len_string = sizeof(AT QMTDISC"=0" NEWLINE) - 1;
	
	try_num = 3;
	NBTask[_AT_MQTT_CLOSE].set(NULL);

		while(try_num--)
	{	
	if(nb_at_send(&NBTask[_AT_MQTT_CLOSE]) == NB_CMD_SUCC)
	{
		nb_cmd_status = NB_CMD_SUCC;
		break;		
	}
	else
		nb_cmd_status = NB_CMD_FAIL;
  }	
	return nb_cmd_status;
}

NB_TaskStatus nb_MQTT_close_set(const char* param)
{
	user_main_debug("NBTask[_AT_MQTT_CLOSE].ATSendStr:%s",ATSendStr);

	return nb_cmd_status;
}



NB_TaskStatus ota_MQTT_open_run(const char* param)
{	
	NBTask[_AT_OTAOPEN].set(param);
	
	if(nb_at_send(&NBTask[_AT_OTAOPEN]) == NB_CMD_SUCC)
		nb_cmd_status = NB_CMD_SUCC;
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;	
}

NB_TaskStatus ota_MQTT_open_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	char* pos = strchr((char*)user.otaadd,',');
	
	strcat(buff,AT QMTOPEN "=0,\"");
	memcpy(buff+strlen(buff),user.otaadd,(pos-(char*)user.otaadd));
	strcat(buff,"\"");
	strcat(buff,(char*)&user.otaadd[(pos-(char*)user.otaadd)]);

	strcat(buff,"\r\n");

	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	
	user_main_debug("openPayload:%s",ATSendStr);
	return nb_cmd_status;	
}

NB_TaskStatus ota_MQTT_conn_run(const char* param)
{
	NBTask[_AT_OTACONN].set(param);
	
	if(nb_at_send(&NBTask[_AT_OTACONN]) == NB_CMD_SUCC)
		nb_cmd_status = NB_CMD_SUCC;
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;		
}

NB_TaskStatus ota_MQTT_conn_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QMTCONN"=0,\""); 
	strcat(buff,(char*)user.otaclient);
	if(strstr((char*)user.otauname,"NULL") ==NULL)
	{
		strcat(buff,"\",\"");
		strcat(buff,(char*)user.otauname);
		if(strstr((char*)user.otapwd,"NULL") ==NULL)
		{
			strcat(buff,"\",\"");
			strcat(buff,(char*)user.otapwd);
		}
	}
	strcat(buff,"\"\r\n");

	ATSendStr  = NULL;
	ATSendStr = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("connPayload:%s",ATSendStr);
	return nb_cmd_status;
}

NB_TaskStatus ota_MQTT_pub_run(const char* param)
{
	NBTask[_AT_OTAPUB].set(param);
	
	if(nb_at_send(&NBTask[_AT_OTAPUB]) == NB_CMD_SUCC)
		nb_cmd_status = NB_CMD_SUCC;
	else
		nb_cmd_status = NB_CMD_FAIL;
	
	return nb_cmd_status;		
}

NB_TaskStatus ota_MQTT_pub_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QMTPUB"=0,0,0,0,\"");
	if(connect_status==0)
	  strcat(buff,"v1/devices/me/telemetry");
	else if(connect_status==1)
	  strcat(buff,"v1/devices/me/attributes/request/1");	
	else if(connect_status==2)
	  strcat(buff,"v1/devices/me/telemetry");	
	else if(connect_status==3)
	  sprintf(buff+strlen(buff), "v2/fw/request/1/chunk/%d",chunk_index);		
	else if(connect_status==4)
	  strcat(buff,"v1/devices/me/telemetry");	
	
	strcat(buff,(char*)"\",");
	if(connect_status==0)	
	  Thingseye_mqtt_send_current_fw_info();
	else if(connect_status==1)
		Thingseye_mqtt_request_fw_info();
	else if(connect_status==2)
		Thingseye_mqtt_send_fw_downloading();
	else if(connect_status==3)
		sprintf(buff+strlen(buff), "4,\"1028\"");		
	else if(connect_status==4)
		Thingseye_mqtt_send_updated();	
	
	strcat(buff,"\r\n");
	
	ATSendStr  = NULL;
	ATSendStr  = buff;
	len_string = strlen(ATSendStr);
	user_main_debug("NBTask[_AT_MQTT_PUB].ATSendStr:%s",ATSendStr);
	return nb_cmd_status;
}

/**
	* @brief  MQTT URI:Scheduling tasks via URI 
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_uri_run(const char* param)
{
	user_main_debug("uri:%s",nb.usart.data);
	if(strstr((char*)nb.usart.data,QMTOPEN": 0,0") != NULL)
	{
		nb_cmd_status = NB_OPEN_SUCC;	
	}
	else if(strstr((char*)nb.usart.data,QMTCONN": 0,0,0") != NULL)
	{
		nb_cmd_status = NB_CONN_SUCC;	
	}
	else if(strstr((char*)nb.usart.data,QMTSUB": 0,1,0,2") != NULL)
	{
		nb_cmd_status = NB_SUB_SUCC;	
	}
	else if(strstr((char*)nb.usart.data,QMTSUB": 0,1,0,1") != NULL)
	{
		nb_cmd_status = NB_SUB_SUCC;	
	}
	else if(strstr((char*)nb.usart.data,QMTSUB": 0,1,0,0") != NULL)
	{
		nb_cmd_status = NB_SUB_SUCC;	
	}	
	else if(strstr((char*)nb.usart.data,QMTPUB": 0,1,0") != NULL)
	{
		nb_cmd_status = NB_PUB_SUCC;
	}	
	else if(strstr((char*)nb.usart.data,QMTPUB": 0,0,0") != NULL)
	{
		nb_cmd_status = NB_PUB_SUCC;
	}
	else if(strstr((char*)nb.usart.data,QMTDISC": 0,0") != NULL)
	{
		nb_cmd_status = NB_CLOSE_SUCC;
	}
	else
		nb_cmd_status = NB_OTHER;
	
//Judgment issued and received 
	if(strstr((char*)nb.usart.data,QMTRECV) != NULL)
	{
		nb_MQTT_data_read_set(NULL);
	}
//Ask if the process has failed 
	if(strstr((char*)nb.usart.data,QMTSTAT) != NULL)
	{
		nb_cmd_status = NB_ERROR;
	}
	return nb_cmd_status;
}

NB_TaskStatus ota_uri_run(const char* param)
{
	user_main_debug("uri:%s",nb.usart.data);
	if(strstr((char*)nb.usart.data,QMTOPEN": 0,0") != NULL)
	{
		nb_cmd_status = NB_OPEN_SUCC;	
	}
	else if(strstr((char*)nb.usart.data,QMTCONN": 0,0,0") != NULL)
	{
		nb_cmd_status = NB_CONN_SUCC;	
	}
	else if(strstr((char*)nb.usart.data,QMTSUB": 0,1,0,2") != NULL)
	{
		nb_cmd_status = NB_SUB_SUCC;	
	}
	else if(strstr((char*)nb.usart.data,QMTSUB": 0,1,0,1") != NULL)
	{
		nb_cmd_status = NB_SUB_SUCC;	
	}
	else if(strstr((char*)nb.usart.data,QMTSUB": 0,1,0,0") != NULL)
	{
		nb_cmd_status = NB_SUB_SUCC;	
	}	
	else if(strstr((char*)nb.usart.data,QMTPUB": 0,1,0") != NULL)
	{
		nb_cmd_status = NB_PUB_SUCC;
	}	
	else if(strstr((char*)nb.usart.data,QMTPUB": 0,0,0") != NULL)
	{
		nb_cmd_status = NB_PUB_SUCC;
	}
	else if(strstr((char*)nb.usart.data,QMTDISC": 0,0") != NULL)
	{
		nb_cmd_status = NB_CLOSE_SUCC;
	}	
//Judgment issued and received 
	if(strstr((char*)nb.usart.data,QMTRECV) != NULL)
	{
		if(connect_status==1)
		{
	     if(get_sharekeys==1)
	    {	
		     char *token = strtok((char *)nb.usart.data,"{},\":");
		
		     while(token != NULL)
		    {		
			    if(strcmp(token,"fw_size")== 0)
			   {
				   token = strtok(NULL,"{},\":");
				   server_fw_size = atoi(token);
				   get_sharekeys =0;
			   }	
			    else if(strcmp(token,"fw_title")== 0)
			   {
				   token = strtok(NULL,"{},\":");
				   strcpy(server_fw_title,token);				
			   }		
			    else if(strcmp(token,"fw_version")== 0)
			   {
				   token = strtok(NULL,"{},\":");
				   strcpy(server_fw_version,token);				
			   }		

			   token = strtok(NULL,"{},\":");
		    }
		
		    check_firmware_exists=0;
		    if(server_fw_size>0&&strncmp(server_fw_title, user.otatitle, strlen(user.otatitle)) == 0)
		   {
			   if(strncmp(server_fw_version, user.otaver, strlen(user.otaver)) != 0)
			  {
				  zb25vq32_firmware_store_address = 0;
			    check_firmware_exists=1;		
			  }
			   else
			  {
				  ota_fail_times=3;
				  ota_upgrade_error=6;					
				  check_firmware_exists=0;
			  }
		   }
		    else
		   {
			   printf("Inconsistent firmware title\r\n");
				 ota_fail_times=3;
				 ota_upgrade_error=1;
		   }
	   }
     if(check_firmware_exists == 1)		
		 {
				printf("Inconsistent firmware version\r\n");HAL_Delay(300);
				printf("server_fw_title %s\r\n",server_fw_title);HAL_Delay(300);
				printf("server_fw_version %s\r\n",server_fw_version);HAL_Delay(300);
				printf("server_fw_size %d\r\n",(unsigned int)server_fw_size);HAL_Delay(300);		 
				record_server_fw_size = server_fw_size;
				ZB25VQ32_ReleaseDeepPowerDown();
				//ERASE 128KB
				ZB25VQ32_WriteEnable();	
				ZB25VQ32_BlockErase(0, ZB25VQ32_ERASE_64K);
				ZB25VQ32_WriteDisable();
				HAL_Delay(500);
						
				ZB25VQ32_WriteEnable();
				ZB25VQ32_BlockErase(65536, ZB25VQ32_ERASE_64K);
				ZB25VQ32_WriteDisable();
				HAL_Delay(500);		

        nb_cmd_status = NB_CMD_ON;				 
		 }
     else
     {
        printf("Consistent firmware\r\n");
        HAL_Delay(100);						
				nb_cmd_status = NB_CMD_OFF;	 
		 }			 
		}
		
		if(connect_status==3)
		{
			if(rece_firmware_end==1)
			{
			  nb_cmd_status = NB_CMD_ON;
        rece_firmware_end=0;				
			}				
		}				
	}
	
	if(strstr((char*)nb.usart.data,QMTSTAT": 0,1") != NULL)
	{
		nb_cmd_status = NB_CMD_ON;
		ota_continued=1;
	}		
	return nb_cmd_status;
}