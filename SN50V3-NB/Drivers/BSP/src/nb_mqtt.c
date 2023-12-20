#include "nb_mqtt.h"
#include "time.h"
#include <time.h>

char buff[2000]={0};
static char downlink_data[1000]={0};
uint8_t at_downlink_flag=0;
extern float hum_value;
extern float tem_value;
extern float ds1820_value;
extern float ds1820_value2;
extern float ds1820_value3;
void pro_data2(void);
extern uint8_t mqtt_qos;
extern bool DNS_RE_FLAG;
/**
	* @brief  Set MQTT configuration parameters
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_config_set(const char* param)
{
	NBTask[_AT_MQTT_Config].try_num = 4;
	while(NBTask[_AT_MQTT_Config].try_num--)
	{
		if(nb_at_send(&NBTask[_AT_MQTT_Config])== NB_CMD_SUCC)
		{
			break;
		}
		else
			HAL_Delay(100);
	}
	
	return NBTask[_AT_MQTT_Config].nb_cmd_status;
}

/**
	* @brief  Manage server and client authentication.
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_QSSLCFG_run(const char* param)
{	
	NBTask[_AT_QSSLCFG].try_num = 4;
	NBTask[_AT_QSSLCFG].set(param);
	
	if(nb_at_send(&NBTask[_AT_QSSLCFG]) == NB_CMD_SUCC)
		NBTask[_AT_QSSLCFG].nb_cmd_status = NB_CMD_SUCC;
	else
		NBTask[_AT_QSSLCFG].nb_cmd_status = NB_CMD_FAIL;
	
	return NBTask[_AT_QSSLCFG].nb_cmd_status;
}

NB_TaskStatus nb_QSSLCFG_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	strcat(buff,AT QSSLCFG "=0,0,\"seclevel\",");	
	sprintf(buff+strlen(buff), "%d\r\n", sys.cert);
	
	NBTask[_AT_QSSLCFG].ATSendStr = buff;
	NBTask[_AT_QSSLCFG].len_string = strlen(NBTask[_AT_QSSLCFG].ATSendStr);
	
	user_main_debug("openPayload:%s",NBTask[_AT_QSSLCFG].ATSendStr);
	return NBTask[_AT_QSSLCFG].nb_cmd_status;	
}

/**
	* @brief  Manage server and client authentication.
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_QMTCFGSSL_run(const char* param)
{	
	NBTask[_AT_QMTCFG_SSL].try_num = 4;
	NBTask[_AT_QMTCFG_SSL].set(param);
	
	if(nb_at_send(&NBTask[_AT_QMTCFG_SSL]) == NB_CMD_SUCC)
		NBTask[_AT_QMTCFG_SSL].nb_cmd_status = NB_CMD_SUCC;
	else
		NBTask[_AT_QMTCFG_SSL].nb_cmd_status = NB_CMD_FAIL;
	
	return NBTask[_AT_QMTCFG_SSL].nb_cmd_status;
}

NB_TaskStatus nb_QMTCFGSSL_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	
	strcat(buff,AT QMTCFG "=\"ssl\",0,1,0,0\r\n");	
	
	NBTask[_AT_QMTCFG_SSL].ATSendStr = buff;
	NBTask[_AT_QMTCFG_SSL].len_string = strlen(NBTask[_AT_QMTCFG_SSL].ATSendStr);
	
	user_main_debug("openPayload:%s",NBTask[_AT_QMTCFG_SSL].ATSendStr);
	return NBTask[_AT_QMTCFG_SSL].nb_cmd_status;	
}
/**
	* @brief  Open MQTT port
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_open_run(const char* param)
{	
	NBTask[_AT_MQTT_OPEN].try_num = 4;
	NBTask[_AT_MQTT_OPEN].set(param);
	
	if(nb_at_send(&NBTask[_AT_MQTT_OPEN]) == NB_CMD_SUCC)
		NBTask[_AT_MQTT_OPEN].nb_cmd_status = NB_CMD_SUCC;
	else
		NBTask[_AT_MQTT_OPEN].nb_cmd_status = NB_CMD_FAIL;
	
	return NBTask[_AT_MQTT_OPEN].nb_cmd_status;
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
	
	NBTask[_AT_MQTT_OPEN].ATSendStr = buff;
	NBTask[_AT_MQTT_OPEN].len_string = strlen(NBTask[_AT_MQTT_OPEN].ATSendStr);
	
	user_main_debug("openPayload:%s",NBTask[_AT_MQTT_OPEN].ATSendStr);
	return NBTask[_AT_MQTT_OPEN].nb_cmd_status;	
}

/**
	* @brief  connect to the server
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_conn_run(const char* param)
{
	NBTask[_AT_MQTT_CONN].try_num = 4;
	NBTask[_AT_MQTT_CONN].set(param);
		
	if(nb_at_send(&NBTask[_AT_MQTT_CONN]) == NB_CMD_SUCC)
		NBTask[_AT_MQTT_CONN].nb_cmd_status = NB_CMD_SUCC;
	else
		NBTask[_AT_MQTT_CONN].nb_cmd_status = NB_CMD_FAIL;
	
	return NBTask[_AT_MQTT_CONN].nb_cmd_status;
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
	
	NBTask[_AT_MQTT_CONN].ATSendStr = buff;
	NBTask[_AT_MQTT_CONN].len_string = strlen(NBTask[_AT_MQTT_CONN].ATSendStr);
	user_main_debug("connPayload:%s",NBTask[_AT_MQTT_CONN].ATSendStr);
	return NBTask[_AT_MQTT_CONN].nb_cmd_status;
}

/**
	* @brief  Subscribe to MQTT topics
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_sub_run(const char* param)
{
	NBTask[_AT_MQTT_SUB].try_num = 4;
	NBTask[_AT_MQTT_SUB].set(NULL);

	if(nb_at_send(&NBTask[_AT_MQTT_SUB]) == NB_CMD_SUCC)
		NBTask[_AT_MQTT_SUB].nb_cmd_status = NB_CMD_SUCC;
	else
		NBTask[_AT_MQTT_SUB].nb_cmd_status = NB_CMD_FAIL;
	
	return NBTask[_AT_MQTT_SUB].nb_cmd_status;
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
	
	NBTask[_AT_MQTT_SUB].ATSendStr  = buff;
	NBTask[_AT_MQTT_SUB].len_string = strlen(NBTask[_AT_MQTT_SUB].ATSendStr);
	user_main_debug("NBTask[_AT_MQTT_SUB].ATSendStr:%s",NBTask[_AT_MQTT_SUB].ATSendStr);
	return NBTask[_AT_MQTT_SUB].nb_cmd_status;
}

/**
	* @brief  Send MQTT publish topic data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_pub_run(const char* param)
{
	NBTask[_AT_MQTT_PUB].try_num = 4;
	NBTask[_AT_MQTT_PUB].set(NULL);

	if(nb_at_send(&NBTask[_AT_MQTT_PUB]) == NB_CMD_SUCC)
	{
		NBTask[_AT_MQTT_PUB].nb_cmd_status = NB_CMD_SUCC;
	}
	else
		NBTask[_AT_MQTT_PUB].nb_cmd_status = NB_CMD_FAIL;
	
	return NBTask[_AT_MQTT_PUB].nb_cmd_status;
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
	
	NBTask[_AT_MQTT_PUB].ATSendStr  = buff;
	NBTask[_AT_MQTT_PUB].len_string = strlen(NBTask[_AT_MQTT_PUB].ATSendStr);
	user_main_debug("NBTask[_AT_MQTT_PUB].ATSendStr:%s",NBTask[_AT_MQTT_PUB].ATSendStr);
	return NBTask[_AT_MQTT_PUB].nb_cmd_status;
}

/**
	* @brief  Send MQTT publish topic1 data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_pub1_run(const char* param)
{
	NBTask[_AT_MQTT_PUB1].try_num = 4;
	NBTask[_AT_MQTT_PUB1].set(NULL);

	if(nb_at_send(&NBTask[_AT_MQTT_PUB1]) == NB_CMD_SUCC)
	{
		NBTask[_AT_MQTT_PUB1].nb_cmd_status = NB_CMD_SUCC;
	}
	else
		NBTask[_AT_MQTT_PUB1].nb_cmd_status = NB_CMD_FAIL;
	
	return NBTask[_AT_MQTT_PUB1].nb_cmd_status;
}

NB_TaskStatus nb_MQTT_pub1_set(const char* param)
{
	uint16_t batteryLevel_mV=getVoltage();
  char buff1[500]={0};
	memset(buff1,0,sizeof(buff1));
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
	sprintf(buff1, "field1=%d&field2=%.2f&field3=%d&",sys.mod-0x30,batteryLevel_mV/1000.0,nb.singal);	
	if(sys.mod == model1)
	{
  sprintf(buff1+strlen(buff1), "field4=%.1f&",ds1820_value);	
			if(sys.inmod =='0')
        sprintf(buff1+strlen(buff1), "field5=%d&",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
			else
	      sprintf(buff1+strlen(buff1), "field5=%d&",sensor.exit_state);		
	sprintf(buff1+strlen(buff1), "field6=%d&",sensor.adc1);	
	sprintf(buff1+strlen(buff1), "field7=%.1f&field8=%.1f",tem_value,hum_value);	
	}
	else if(sys.mod == model2)
	{
  sprintf(buff1+strlen(buff1), "field4=%.1f&",ds1820_value);	
			if(sys.inmod =='0')
        sprintf(buff1+strlen(buff1), "field5=%d&",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
			else
	      sprintf(buff1+strlen(buff1), "field5=%d&",sensor.exit_state);		
	sprintf(buff1+strlen(buff1), "field6=%d&",sensor.adc1);
	sprintf(buff1+strlen(buff1), "field7=%d",sensor.distance);			
	}
	else if(sys.mod == model3)
	{
	 sprintf(buff1+strlen(buff1), "field4=%d&",sensor.adc1);
				if(sys.inmod =='0')
        sprintf(buff1+strlen(buff1), "field5=%d&",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
			else
	      sprintf(buff1+strlen(buff1), "field5=%d&",sensor.exit_state);
	 sprintf(buff1+strlen(buff1), "field6=%d&",sensor.adc2);	
	sprintf(buff1+strlen(buff1), "field7=%.1f,field8=%.1f&",tem_value,hum_value);				
	 sprintf(buff1+strlen(buff1), "field9=%d",sensor.adc3);					
	}		
	else if(sys.mod == model4)
	{
   sprintf(buff1+strlen(buff1), "field4=%.1f&",ds1820_value);	
	 sprintf(buff1+strlen(buff1), "field5=%d&",sensor.adc1);
			if(sys.inmod =='0')
        sprintf(buff1+strlen(buff1), "field6=%d&",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
			else
	      sprintf(buff1+strlen(buff1), "field6=%d&",sensor.exit_state);
   sprintf(buff1+strlen(buff1), "field7=%.1f&",ds1820_value2);	
   sprintf(buff1+strlen(buff1), "field8=%.1f",ds1820_value3);				
	}	
	else if(sys.mod == model5)
	{
   sprintf(buff1+strlen(buff1), "field4=%.1f&",ds1820_value);	
	 sprintf(buff1+strlen(buff1), "field5=%d&",sensor.adc1);
			if(sys.inmod =='0')
        sprintf(buff1+strlen(buff1), "field6=%d&",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
			else
	      sprintf(buff1+strlen(buff1), "field6=%d&",sensor.exit_state);
	  WEIGHT_SCK_Init();
	  WEIGHT_DOUT_Init();
		int32_t Weight = Get_Weight();	
	  WEIGHT_SCK_DeInit();
	  WEIGHT_DOUT_DeInit();					
	 sprintf(buff1+strlen(buff1), "field7=%d",Weight);	
	}		
	else if(sys.mod == model6)
	{
	sprintf(buff1+strlen(buff1), "field4=%d",sensor.exit_count);		
	}
	
	sprintf(buff+strlen(buff), "%d,%s\r\n",strlen(buff1),buff1);	
	
	NBTask[_AT_MQTT_PUB1].ATSendStr  = buff;
	NBTask[_AT_MQTT_PUB1].len_string = strlen(NBTask[_AT_MQTT_PUB1].ATSendStr);
	user_main_debug("NBTask[_AT_MQTT_PUB].ATSendStr:%s",NBTask[_AT_MQTT_PUB1].ATSendStr);
	return NBTask[_AT_MQTT_PUB1].nb_cmd_status;
}


/**
	* @brief  Send MQTT publish topic2 data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_pub2_run(const char* param)
{
	NBTask[_AT_MQTT_PUB2].try_num = 4;
	NBTask[_AT_MQTT_PUB2].set(NULL);

	if(nb_at_send(&NBTask[_AT_MQTT_PUB2]) == NB_CMD_SUCC)
	{
		NBTask[_AT_MQTT_PUB2].nb_cmd_status = NB_CMD_SUCC;
	}
	else
		NBTask[_AT_MQTT_PUB2].nb_cmd_status = NB_CMD_FAIL;
	
	return NBTask[_AT_MQTT_PUB2].nb_cmd_status;
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
	pro_data2();	
	strcat(buff,"\r\n");	
	NBTask[_AT_MQTT_PUB2].ATSendStr  = buff;
	NBTask[_AT_MQTT_PUB2].len_string = strlen(NBTask[_AT_MQTT_PUB2].ATSendStr);
	user_main_debug("NBTask[_AT_MQTT_PUB].ATSendStr:%s",NBTask[_AT_MQTT_PUB2].ATSendStr);
	return NBTask[_AT_MQTT_PUB2].nb_cmd_status;
}


/**
	* @brief  Send MQTT publish topic3 data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_pub3_run(const char* param)
{
	NBTask[_AT_MQTT_PUB3].try_num = 4;
	NBTask[_AT_MQTT_PUB3].set(NULL);

	if(nb_at_send(&NBTask[_AT_MQTT_PUB3]) == NB_CMD_SUCC)
	{
		NBTask[_AT_MQTT_PUB3].nb_cmd_status = NB_CMD_SUCC;
	}
	else
		NBTask[_AT_MQTT_PUB3].nb_cmd_status = NB_CMD_FAIL;
	
	return NBTask[_AT_MQTT_PUB3].nb_cmd_status;
}

NB_TaskStatus nb_MQTT_pub3_set(const char* param)
{
	uint16_t batteryLevel_mV=getVoltage();
  char buff1[500]={0};
	memset(buff1,0,sizeof(buff1));
	memset(buff,0,sizeof(buff));
	if(mqtt_qos==2)
	strcat(buff,AT QMTPUB"=0,1,2,0,\"");
	else if(mqtt_qos==0)	
	strcat(buff,AT QMTPUB"=0,0,0,0,\"");
	else if(mqtt_qos==1)	
	strcat(buff,AT QMTPUB"=0,1,1,0,\"");	
	
	strcat(buff,(char*)user.pubtopic);
	strcat(buff,(char*)"\",");
	sprintf(buff1, "{\"IMEI\":\"%s\",\"Model\":\"SN50V3-NB\",\"mod\":%d,\"battery\":%.2f,\"signal\":%d,",user.deui,sys.mod-0x30,batteryLevel_mV/1000.0,nb.singal);	
	if(sys.mod == model1)
	{
  sprintf(buff1+strlen(buff1), "\"DS18B20_Temp\":%.1f,",ds1820_value);	
			if(sys.inmod =='0')
        sprintf(buff1+strlen(buff1), "\"digital_in\":%d,",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
			else
	      sprintf(buff1+strlen(buff1), "\"interrupt\":%d,",sensor.exit_state);		
	sprintf(buff1+strlen(buff1), "\"adc1\":%d,",sensor.adc1);	
	sprintf(buff1+strlen(buff1), "\"temperature\":%.1f,\"humidity\":%.1f}",tem_value,hum_value);	
	}
	else if(sys.mod == model2)
	{
  sprintf(buff1+strlen(buff1), "\"DS18B20_Temp\":%.1f,",ds1820_value);	
			if(sys.inmod =='0')
        sprintf(buff1+strlen(buff1), "\"digital_in\":%d,",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
			else
	      sprintf(buff1+strlen(buff1), "\"interrupt\":%d,",sensor.exit_state);		
	sprintf(buff1+strlen(buff1), "\"adc1\":%d,",sensor.adc1);
	sprintf(buff1+strlen(buff1), "\"distance\":%d}",sensor.distance);			
	}
	else if(sys.mod == model3)
	{
	 sprintf(buff1+strlen(buff1), "\"adc1\":%d,",sensor.adc1);
				if(sys.inmod =='0')
        sprintf(buff1+strlen(buff1), "\"digital_in\":%d,",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
			else
	      sprintf(buff1+strlen(buff1), "\"interrupt\":%d,",sensor.exit_state);
	 sprintf(buff1+strlen(buff1), "\"adc2\":%d,",sensor.adc2);	
	sprintf(buff1+strlen(buff1), "\"temperature\":%.1f,\"humidity\":%.1f,",tem_value,hum_value);				
	 sprintf(buff1+strlen(buff1), "\"adc3\":%d}",sensor.adc3);					
	}		
	else if(sys.mod == model4)
	{
   sprintf(buff1+strlen(buff1), "\"DS18B20_Temp\":%.1f,",ds1820_value);	
	 sprintf(buff1+strlen(buff1), "\"adc1\":%d,",sensor.adc1);
			if(sys.inmod =='0')
        sprintf(buff1+strlen(buff1), "\"digital_in\":%d,",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
			else
	      sprintf(buff1+strlen(buff1), "\"interrupt\":%d,",sensor.exit_state);
   sprintf(buff1+strlen(buff1), "\"DS18B20_Temp2\":%.1f,",ds1820_value2);	
   sprintf(buff1+strlen(buff1), "\"DS18B20_Temp3\":%.1f}",ds1820_value3);				
	}	
	else if(sys.mod == model5)
	{
   sprintf(buff1+strlen(buff1), "\"DS18B20_Temp\":%.1f,",ds1820_value);	
	 sprintf(buff1+strlen(buff1), "\"adc1\":%d,",sensor.adc1);
			if(sys.inmod =='0')
        sprintf(buff1+strlen(buff1), "\"digital_in\":%d,",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
			else
	      sprintf(buff1+strlen(buff1), "\"interrupt\":%d,",sensor.exit_state);
	  WEIGHT_SCK_Init();
	  WEIGHT_DOUT_Init();
		int32_t Weight = Get_Weight();	
	  WEIGHT_SCK_DeInit();
	  WEIGHT_DOUT_DeInit();					
	 sprintf(buff1+strlen(buff1), "\"weight\":%d}",Weight);	
	}		
	else if(sys.mod == model6)
	{
	sprintf(buff1+strlen(buff1), "\"count\":%d}",sensor.exit_count);		
	}
	
	sprintf(buff+strlen(buff), "%d,%s\r\n",strlen(buff1),buff1);	
	NBTask[_AT_MQTT_PUB3].ATSendStr  = buff;
	NBTask[_AT_MQTT_PUB3].len_string = strlen(NBTask[_AT_MQTT_PUB3].ATSendStr);
	user_main_debug("NBTask[_AT_MQTT_PUB].ATSendStr:%s",NBTask[_AT_MQTT_PUB3].ATSendStr);
	return NBTask[_AT_MQTT_PUB3].nb_cmd_status;
}
/**
	* @brief  Send MQTT publish topic5 data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_pub5_run(const char* param)
{
	NBTask[_AT_MQTT_PUB5].try_num = 4;
	NBTask[_AT_MQTT_PUB5].set(NULL);

	if(nb_at_send(&NBTask[_AT_MQTT_PUB5]) == NB_CMD_SUCC)
	{
		NBTask[_AT_MQTT_PUB5].nb_cmd_status = NB_CMD_SUCC;
	}
	else
		NBTask[_AT_MQTT_PUB5].nb_cmd_status = NB_CMD_FAIL;
	
	return NBTask[_AT_MQTT_PUB5].nb_cmd_status;
}

NB_TaskStatus nb_MQTT_pub5_set(const char* param)
{
	memset(buff,0,sizeof(buff));
	if(mqtt_qos==2)
	strcat(buff,AT QMTPUB"=0,1,2,0,\"");
	else if(mqtt_qos==0)		
	strcat(buff,AT QMTPUB"=0,0,0,0,\"");
	else if(mqtt_qos==1)	
	strcat(buff,AT QMTPUB"=0,0,1,0,\"");
	strcat(buff,(char*)user.pubtopic);
	strcat(buff,(char*)"\",");
	pro_data2();
	strcat(buff,"\r\n");			
	NBTask[_AT_MQTT_PUB5].ATSendStr  = buff;
	NBTask[_AT_MQTT_PUB5].len_string = strlen(NBTask[_AT_MQTT_PUB5].ATSendStr);
	user_main_debug("NBTask[_AT_MQTT_PUB].ATSendStr:%s",NBTask[_AT_MQTT_PUB5].ATSendStr);
	return NBTask[_AT_MQTT_PUB5].nb_cmd_status;
}
/**
	* @brief  Send data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_send_run(const char* param)
{
	NBTask[_AT_MQTT_SEND].try_num = 4;
	NBTask[_AT_MQTT_SEND].set(NULL);

	if(nb_at_send(&NBTask[_AT_MQTT_SEND]) == NB_CMD_SUCC)
	{
		NBTask[_AT_MQTT_SEND].nb_cmd_status = NB_CMD_SUCC;
	}
	else
		NBTask[_AT_MQTT_SEND].nb_cmd_status = NB_CMD_FAIL;
	
	return NBTask[_AT_MQTT_SEND].nb_cmd_status;
}

NB_TaskStatus nb_MQTT_send_set(const char* param)
{	
	NBTask[_AT_MQTT_SEND].ATSendStr  = sensor.data;
	NBTask[_AT_MQTT_SEND].len_string = sensor.data_len;
	user_main_debug("NBTask[_AT_MQTT_SEND].ATSendStr:%s",NBTask[_AT_MQTT_SEND].ATSendStr);
	return NBTask[_AT_MQTT_SEND].nb_cmd_status;
}

/**
	* @brief  Read MQTT sublish topic data
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_data_read_run(const char* param)
{
	return NBTask[_AT_MQTT_READ].nb_cmd_status;
}

NB_TaskStatus nb_MQTT_data_read_set(const char* param)
{
	memset(downlink_data,0,1000);		
	char* pos_start  = strstr((char*)nb.usart.data,"\",\"");	user_main_debug("pos_start:%p",&pos_start);
	if(pos_start[3]=='{')
	{
	  char* pos_end    = strchr(pos_start,'}');			user_main_debug("pos_end:%p",&pos_end);		
	  memcpy(downlink_data,&nb.usart.data[pos_start-((char*)nb.usart.data)+3],(pos_end-pos_start-2));	
	  at_downlink_flag=1;	
	}
	else
	{
		char* pos_end    = strchr(pos_start,'\n');			user_main_debug("pos_end:%p",&pos_end);		
	  memcpy(downlink_data,&nb.usart.data[pos_start-((char*)nb.usart.data)+3],(pos_end-pos_start-5));		
	}
	user_main_printf("Received downlink data:%s",downlink_data);
	rxPayLoadDeal(downlink_data);
	
	return NBTask[_AT_MQTT_READ].nb_cmd_status;
}

/**
	* @brief  Close MQTT port
  * @param  Instruction parameter
  * @retval None
  */
NB_TaskStatus nb_MQTT_close_run(const char* param)
{
	NBTask[_AT_MQTT_CLOSE].try_num = 3;
	NBTask[_AT_MQTT_CLOSE].set(NULL);

		while(NBTask[_AT_MQTT_CLOSE].try_num--)
	{	
	if(nb_at_send(&NBTask[_AT_MQTT_CLOSE]) == NB_CMD_SUCC)
	{
		NBTask[_AT_MQTT_CLOSE].nb_cmd_status = NB_CMD_SUCC;
		break;		
	}
	else
		NBTask[_AT_MQTT_CLOSE].nb_cmd_status = NB_CMD_FAIL;
  }	
	return NBTask[_AT_MQTT_CLOSE].nb_cmd_status;
}

NB_TaskStatus nb_MQTT_close_set(const char* param)
{
	user_main_debug("NBTask[_AT_MQTT_CLOSE].ATSendStr:%s",NBTask[_AT_MQTT_CLOSE].ATSendStr);

	return NBTask[_AT_MQTT_CLOSE].nb_cmd_status;
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
		NBTask[_AT_MQTT_URI].nb_cmd_status = NB_OPEN_SUCC;	
	}
	else if(strstr((char*)nb.usart.data,QMTCONN": 0,0,0") != NULL)
	{
		NBTask[_AT_MQTT_URI].nb_cmd_status = NB_CONN_SUCC;	
	}
	else if(strstr((char*)nb.usart.data,QMTSUB": 0,1,0,2") != NULL)
	{
		NBTask[_AT_MQTT_URI].nb_cmd_status = NB_SUB_SUCC;	
	}
	else if(strstr((char*)nb.usart.data,QMTSUB": 0,1,0,1") != NULL)
	{
		NBTask[_AT_MQTT_URI].nb_cmd_status = NB_SUB_SUCC;	
	}
	else if(strstr((char*)nb.usart.data,QMTSUB": 0,1,0,0") != NULL)
	{
		NBTask[_AT_MQTT_URI].nb_cmd_status = NB_SUB_SUCC;	
	}	
	else if(strstr((char*)nb.usart.data,QMTPUB": 0,1,0") != NULL)
	{
		NBTask[_AT_MQTT_URI].nb_cmd_status = NB_PUB_SUCC;
	}
	else if(strstr((char*)nb.usart.data,QMTPUB": 0,0,0") != NULL)
	{
		NBTask[_AT_MQTT_URI].nb_cmd_status = NB_PUB_SUCC;
	}
	else if(strstr((char*)nb.usart.data,QMTDISC": 0,0") != NULL)
	{
		NBTask[_AT_MQTT_URI].nb_cmd_status = NB_CLOSE_SUCC;
	}
	else
		NBTask[_AT_MQTT_URI].nb_cmd_status = NB_OTHER;
	
//Judgment issued and received 
	if(strstr((char*)nb.usart.data,QMTRECV) != NULL)
	{
		nb_MQTT_data_read_set(NULL);
	}
//Ask if the process has failed 
	if(strstr((char*)nb.usart.data,QMTSTAT) != NULL)
	{
		NBTask[_AT_MQTT_URI].nb_cmd_status = NB_ERROR;
	}
	return NBTask[_AT_MQTT_URI].nb_cmd_status;
}

void pro_data2(void)
{
		uint16_t str_end,str_beg=0;
		uint16_t batteryLevel_mV=getVoltage();
		for(uint8_t j=0;j<2;j++)
		{
     if(j==1)
		 {
       sprintf(buff+str_beg,"%d,",str_end-str_beg);
		 }
		 str_beg=strlen(buff);		
	sprintf(buff+strlen(buff),"{\"IMEI\":\"%s\",\"Model\":\"SN50V3-NB\",\"mod\":%d,\"battery\":%.2f,\"signal\":%d,",user.deui,sys.mod-0x30,batteryLevel_mV/1000.0,nb.singal);	
	if(sys.mod == model1)
	{
  sprintf(buff+strlen(buff), "\"DS18B20_Temp\":%.1f,",ds1820_value);	
			if(sys.inmod =='0')
        sprintf(buff+strlen(buff), "\"digital_in\":%d,",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
			else
	      sprintf(buff+strlen(buff), "\"interrupt\":%d,",sensor.exit_state);		
	sprintf(buff+strlen(buff), "\"adc1\":%d,",sensor.adc1);	
	sprintf(buff+strlen(buff), "\"temperature\":%.1f,\"humidity\":%.1f",tem_value,hum_value);	
	}
	else if(sys.mod == model2)
	{
  sprintf(buff+strlen(buff), "\"DS18B20_Temp\":%.1f,",ds1820_value);	
			if(sys.inmod =='0')
        sprintf(buff+strlen(buff), "\"digital_in\":%d,",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
			else
	      sprintf(buff+strlen(buff), "\"interrupt\":%d,",sensor.exit_state);		
	sprintf(buff+strlen(buff), "\"adc1\":%d,",sensor.adc1);
	sprintf(buff+strlen(buff), "\"distance\":%d",sensor.distance);			
	}
	else if(sys.mod == model3)
	{
	 sprintf(buff+strlen(buff), "\"adc1\":%d,",sensor.adc1);
				if(sys.inmod =='0')
        sprintf(buff+strlen(buff), "\"digital_in\":%d,",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
			else
	      sprintf(buff+strlen(buff), "\"interrupt\":%d,",sensor.exit_state);
	 sprintf(buff+strlen(buff), "\"adc2\":%d,",sensor.adc2);	
	sprintf(buff+strlen(buff), "\"temperature\":%.1f,\"humidity\":%.1f,",tem_value,hum_value);				
	 sprintf(buff+strlen(buff), "\"adc3\":%d",sensor.adc3);					
	}		
	else if(sys.mod == model4)
	{
   sprintf(buff+strlen(buff), "\"DS18B20_Temp\":%.1f,",ds1820_value);	
	 sprintf(buff+strlen(buff), "\"adc1\":%d,",sensor.adc1);
			if(sys.inmod =='0')
        sprintf(buff+strlen(buff), "\"digital_in\":%d,",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
			else
	      sprintf(buff+strlen(buff), "\"interrupt\":%d,",sensor.exit_state);
   sprintf(buff+strlen(buff), "\"DS18B20_Temp2\":%.1f,",ds1820_value2);	
   sprintf(buff+strlen(buff), "\"DS18B20_Temp3\":%.1f",ds1820_value3);				
	}	
	else if(sys.mod == model5)
	{
   sprintf(buff+strlen(buff), "\"DS18B20_Temp\":%.1f,",ds1820_value);	
	 sprintf(buff+strlen(buff), "\"adc1\":%d,",sensor.adc1);
			if(sys.inmod =='0')
        sprintf(buff+strlen(buff), "\"digital_in\":%d,",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
			else
	      sprintf(buff+strlen(buff), "\"interrupt\":%d,",sensor.exit_state);
	  WEIGHT_SCK_Init();
	  WEIGHT_DOUT_Init();
		int32_t Weight = Get_Weight();	
	  WEIGHT_SCK_DeInit();
	  WEIGHT_DOUT_DeInit();					
	 sprintf(buff+strlen(buff), "\"weight\":%d",Weight);	
	}		
	else if(sys.mod == model6)
	{
	sprintf(buff+strlen(buff), "\"count\":%d",sensor.exit_count);		
	}
		int num = sys.sht_seq;
		int16_t tem,hum,d1,d2,d3;
		uint16_t ad0,ad1,ad4,distance;
		uint32_t count;
	  int32_t weight;
		time_t curtime;
	  struct tm *info;
	  int num2 = sys.sht_noud;
	  if(num2>=24)
			num2=24;
		
		for(uint8_t i=0;i<num2;i++)
		{	
			num--;
			if(num<0)
				num=32+num;
			uint32_t r_time=*(__IO uint32_t *)(EEPROM_TIME_START_ADD+num*0x04);		
	    curtime = r_time;	
      info = localtime( &curtime );			
      char mini[80];
			memset(mini,0,sizeof(mini));
			strftime(mini, 80, "%Y/%m/%d %H:%M:%S", info);	
			
			if((sys.mod!=model6))
     {
			 uint32_t r_d1_ad0_data=*(__IO uint32_t *)(EEPROM_D1_AD0_START_ADD+num*0x04);
			 ad0 = ((r_d1_ad0_data>>16)&0xFFFF);
			  if((sys.mod!=model3))
		   {
				 d1 = (r_d1_ad0_data&0xFFFF);
			 }
		 }
			if((sys.mod==model1)||(sys.mod==model3))
     {
			uint32_t r_sht_data=*(__IO uint32_t *)(EEPROM_SHT_START_ADD+num*0x04);
			tem = ((r_sht_data>>16)&0xFFFF);
			hum = (r_sht_data&0xFFFF);
		 }
			if(sys.mod==model2)
    {	
			uint32_t r_distance_data=*(__IO uint32_t *)(EEPROM_DISTANCE_START_ADD+num*0x04);
			distance = (r_distance_data&0xFFFF);
		}
			if(sys.mod==model3)
    {	
			uint32_t r_ad1_ad4_data=*(__IO uint32_t *)(EEPROM_AD1_AD4_START_ADD+num*0x04);
			ad1 = ((r_ad1_ad4_data>>16)&0xFFFF);
			ad4 = (r_ad1_ad4_data&0xFFFF);
		}
			if(sys.mod==model4)
    {	
			uint32_t r_d2_d3_data=*(__IO uint32_t *)(EEPROM_D2D3_START_ADD+num*0x04);
			d2 = ((r_d2_d3_data>>16)&0xFFFF);
			d3 = (r_d2_d3_data&0xFFFF);
		}
			if(sys.mod==model5)
    {	
			uint32_t r_weight_data=*(__IO uint32_t *)(EEPROM_WEIGHT_START_ADD+num*0x04);
			weight = r_weight_data;
		}
			if(sys.mod==model6)
    {	
			uint32_t r_count_data=*(__IO uint32_t *)(EEPROM_COUNT_START_ADD+num*0x04);
			count = r_count_data;
		}					
			sprintf(buff+strlen(buff),",\"%d\":",i+1);	
					if(sys.mod==model1)
       {
			  sprintf(buff+strlen(buff),"{%.1f,%.1f,%d,%.1f,%s}",(float)tem/10.0,(float)hum/10.0,ad0,(float)d1/10.0,mini);	
		   }
			 	else if(sys.mod==model2)
       {
			  sprintf(buff+strlen(buff),"{%d,%d,%.1f,%s}",distance,ad0,(float)d1/10.0,mini);	
		   }
			 	else if(sys.mod==model3)
       {
			  sprintf(buff+strlen(buff),"{%.1f,%.1f,%d,%d,%d,%s}",(float)tem/10.0,(float)hum/10.0,ad0,ad1,ad4,mini);	
		   }
			 	else if(sys.mod==model4)
       {
			  sprintf(buff+strlen(buff),"{%d,%.1f,%.1f,%.1f,%s}",ad0,(float)d1/10.0,(float)d2/10.0,(float)d3/10.0,mini);	
		   }
			 	else if(sys.mod==model5)
       {
			  sprintf(buff+strlen(buff),"{%d,%.1f,%d,%s}",ad0,(float)d1/10.0,weight,mini);	
		   }
			 	else if(sys.mod==model6)
       {
			  sprintf(buff+strlen(buff),"{%d,%s}",count,mini);	
		   }
		}	
			strcat(buff,(char*)"}");	
      str_end=strlen(buff);
	}
}