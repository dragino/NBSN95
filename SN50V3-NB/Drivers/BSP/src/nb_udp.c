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
	  memcpy(buff+strlen(buff),user.add_ip+1,(pos-(char*)user.add_ip)-1);	
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
	  memcpy(buff+strlen(buff),user.add_ip+1,(pos-(char*)user.add_ip)-1);
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
		end++;

		memcpy(downlink_data,nb.usart.data +(start - (char*)nb.usart.data),end-start);
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

void pro_data(void)
{
		uint16_t str_end,str_beg=0;
		uint16_t batteryLevel_mV=getVoltage();
		for(uint8_t j=0;j<2;j++)
		{
     if(j==1)
		 {
       sprintf(buff+str_beg,"%d,",str_end-str_beg);
		   strcat(buff,(char*)"\"");
		 }
		 str_beg=strlen(buff);
	sprintf(buff+strlen(buff),  "{\"IMEI\":\"%s\",\"Model\":\"SN50V3-NB\",\"mod\":%d,\"battery\":%.2f,\"signal\":%d,",user.deui,sys.mod-0x30,batteryLevel_mV/1000.0,nb.singal);	
	if(sys.mod == model1)
	{
  sprintf(buff+strlen(buff), "\"DS18B20_Temp\":%.1f,",ds1820_value);	
        sprintf(buff+strlen(buff), "\"digital_in\":%d,",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
	      sprintf(buff+strlen(buff), "\"interrupt\":%d,",sensor.exit_state);		
	sprintf(buff+strlen(buff), "\"adc1\":%d,",sensor.adc1);	
	sprintf(buff+strlen(buff), "\"temperature\":%.1f,\"humidity\":%.1f",tem_value,hum_value);	
	}
	else if(sys.mod == model2)
	{
  sprintf(buff+strlen(buff), "\"DS18B20_Temp\":%.1f,",ds1820_value);	
        sprintf(buff+strlen(buff), "\"digital_in\":%d,",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
	      sprintf(buff+strlen(buff), "\"interrupt\":%d,",sensor.exit_state);		
	sprintf(buff+strlen(buff), "\"adc1\":%d,",sensor.adc1);
	sprintf(buff+strlen(buff), "\"distance\":%d",sensor.distance);			
	}
	else if(sys.mod == model3)
	{
	 sprintf(buff+strlen(buff), "\"adc1\":%d,",sensor.adc1);
        sprintf(buff+strlen(buff), "\"digital_in\":%d,",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
	      sprintf(buff+strlen(buff), "\"interrupt\":%d,",sensor.exit_state);
	 sprintf(buff+strlen(buff), "\"adc2\":%d,",sensor.adc2);	
	sprintf(buff+strlen(buff), "\"temperature\":%.1f,\"humidity\":%.1f,",tem_value,hum_value);				
	 sprintf(buff+strlen(buff), "\"adc3\":%d",sensor.adc3);					
	}		
	else if(sys.mod == model4)
	{
   sprintf(buff+strlen(buff), "\"DS18B20_Temp\":%.1f,",ds1820_value);	
	 sprintf(buff+strlen(buff), "\"adc1\":%d,",sensor.adc1);
        sprintf(buff+strlen(buff), "\"digital_in\":%d,",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
	      sprintf(buff+strlen(buff), "\"interrupt\":%d,",sensor.exit_state);
   sprintf(buff+strlen(buff), "\"DS18B20_Temp2\":%.1f,",ds1820_value2);	
   sprintf(buff+strlen(buff), "\"DS18B20_Temp3\":%.1f",ds1820_value3);				
	}	
	else if(sys.mod == model5)
	{
   sprintf(buff+strlen(buff), "\"DS18B20_Temp\":%.1f,",ds1820_value);	
	 sprintf(buff+strlen(buff), "\"adc1\":%d,",sensor.adc1);
        sprintf(buff+strlen(buff), "\"digital_in\":%d,",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
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
		for(uint8_t i=0;i<sys.sht_noud;i++)
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
		 else if(sys.mod==model3)
    {	
			uint32_t r_ad1_ad4_data=*(__IO uint32_t *)(EEPROM_AD1_AD4_START_ADD+num*0x04);
			ad1 = ((r_ad1_ad4_data>>16)&0xFFFF);
			ad4 = (r_ad1_ad4_data&0xFFFF);
		}
			else if(sys.mod==model4)
    {	
			uint32_t r_d2_d3_data=*(__IO uint32_t *)(EEPROM_D2D3_START_ADD+num*0x04);
			d2 = ((r_d2_d3_data>>16)&0xFFFF);
			d3 = (r_d2_d3_data&0xFFFF);
		}
			else if(sys.mod==model5)
    {	
			uint32_t r_weight_data=*(__IO uint32_t *)(EEPROM_WEIGHT_START_ADD+num*0x04);
			weight = r_weight_data;
		}
			else if(sys.mod==model6)
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