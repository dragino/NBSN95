#include "nb_payload.h"
#include "time.h"
#include <time.h>
extern char buff[2000];
extern float hum_value;
extern float tem_value;
extern float ds1820_value;
extern float ds1820_value2;
extern float ds1820_value3;
extern uint16_t pro_data_num;
extern int32_t Weight_Shiwu;
void pro_data_thingspeak(void)
{
	memset(buff,0,sizeof(buff));	
	uint16_t batteryLevel_mV=getVoltage();
	
	sprintf(buff, "field1=%d&field2=%.2f&field3=%d&field4=%f&field5=%f",sys.mod,batteryLevel_mV/1000.0,nb.singal,sensor.latitude,sensor.longitude);	
	if(sys.mod == model1)
	{
  sprintf(buff+strlen(buff), "field6=%.1f&",ds1820_value);	
  sprintf(buff+strlen(buff), "field7=%d&",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
	sprintf(buff+strlen(buff), "field8=%d&",sensor.exit_state);		
	sprintf(buff+strlen(buff), "field9=%d&",sensor.adc1);	
	sprintf(buff+strlen(buff), "field10=%.1f&field11=%.1f",tem_value,hum_value);	
	}
	else if(sys.mod == model2)
	{
  sprintf(buff+strlen(buff), "field6=%.1f&",ds1820_value);	
  sprintf(buff+strlen(buff), "field7=%d&",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
	sprintf(buff+strlen(buff), "field8=%d&",sensor.exit_state);		
	sprintf(buff+strlen(buff), "field9=%d&",sensor.adc1);
	sprintf(buff+strlen(buff), "field10=%d",sensor.distance);			
	}
	else if(sys.mod == model3)
	{
	 sprintf(buff+strlen(buff), "field6=%d&",sensor.adc1);
   sprintf(buff+strlen(buff), "field7=%d&",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
	 sprintf(buff+strlen(buff), "field8=%d&",sensor.exit_state);
	 sprintf(buff+strlen(buff), "field9=%d&",sensor.adc2);	
	sprintf(buff+strlen(buff), "field10=%.1f,field11=%.1f&",tem_value,hum_value);				
	 sprintf(buff+strlen(buff), "field12=%d",sensor.adc3);					
	}		
	else if(sys.mod == model4)
	{
   sprintf(buff+strlen(buff), "field6=%.1f&",ds1820_value);	
	 sprintf(buff+strlen(buff), "field7=%d&",sensor.adc1);
   sprintf(buff+strlen(buff), "field8=%d&",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
	 sprintf(buff+strlen(buff), "field9=%d&",sensor.exit_state);
   sprintf(buff+strlen(buff), "field10=%.1f&",ds1820_value2);	
   sprintf(buff+strlen(buff), "field11=%.1f",ds1820_value3);				
	}	
	else if(sys.mod == model5)
	{
   sprintf(buff+strlen(buff), "field6=%.1f&",ds1820_value);	
	 sprintf(buff+strlen(buff), "field7=%d&",sensor.adc1);
   sprintf(buff+strlen(buff), "field8=%d&",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
	  sprintf(buff+strlen(buff), "field9=%d&",sensor.exit_state);				
	 sprintf(buff+strlen(buff), "field10=%d",Weight_Shiwu);	
	}		
	else if(sys.mod == model6)
	{
	sprintf(buff+strlen(buff), "field6=%d",sensor.exit_count);		
	}
	else if(sys.mod == model7)
	{
	 sprintf(buff+strlen(buff), "field6=%d&",sensor.adc1);
	 sprintf(buff+strlen(buff), "field7=%d&",sensor.adc2);
	 sprintf(buff+strlen(buff), "field8=%d&",sensor.adc3);	
   sprintf(buff+strlen(buff), "field9=%.1f&",ds1820_value);	
   sprintf(buff+strlen(buff), "field10=%.1f&",ds1820_value2);	
   sprintf(buff+strlen(buff), "field11=%.1f",ds1820_value3);			
	}	
	else if(sys.mod == model8)
	{
		if(sensor.count_mode==1)
		{		
	   sprintf(buff+strlen(buff), "field6=%d&",sensor.exit_count);		
	   sprintf(buff+strlen(buff), "field7=%d&",sensor.exit_count_pa4);	
	   sprintf(buff+strlen(buff), "field8=%d",sensor.exit_count_pa0);	
		}else{
			 sprintf(buff+strlen(buff), "field6=%d&",sensor.adc2);
			 sprintf(buff+strlen(buff), "field7=%.1f",ds1820_value);	
		}
	}
	else if(sys.mod == model9)
	{
   sprintf(buff+strlen(buff), "field6=%d&",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));		
	 sprintf(buff+strlen(buff), "field7=%d&",sensor.exit_count_pa0);		
	sprintf(buff+strlen(buff), "field8=%.1f&field9=%.1f",tem_value,hum_value);			
	}	
	else if(sys.mod == model10)
	{	
			 sprintf(buff+strlen(buff), "field6=%.1f&",ds1820_value);	
			sprintf(buff+strlen(buff), "field7=%d&",sensor.adc1);	
	 sprintf(buff+strlen(buff), "field8=%.2f",tem_value);			
	}
      pro_data_num=strlen(buff);
	
}

void pro_data(void)
{
	memset(buff,0,sizeof(buff));
		uint16_t batteryLevel_mV=getVoltage();
		time_t curtime;
	  struct tm *info;			
	curtime = sensor.gps_timestamp;	
  info = localtime( &curtime );	
  char gpstime[80];
	memset(gpstime,0,sizeof(gpstime));
	strftime(gpstime, 80, "%Y-%m-%dT%H:%M:%SZ", info);
		curtime = sensor.time_stamp;	
	  info = localtime( &curtime );
    char nowtime[80];	
	  memset(nowtime,0,sizeof(nowtime));
		strftime(nowtime, 80, "%Y-%m-%dT%H:%M:%SZ", info);
	sprintf(buff+strlen(buff),  "{\"IMEI\":\"%s\",\"IMSI\":\"%s\",\"Model\":\"SN50V3-CB\",\"mod\":%d,\"battery\":%.2f,\"signal\":%d,\"time\":\"%s\",\"latitude\":%f,\"longitude\":%f,\"gps_time\":\"%s\",",user.deui,user.ccid,sys.mod,batteryLevel_mV/1000.0,nb.singal,nowtime,sensor.latitude,sensor.longitude,gpstime);	
   mode_data(buff);
		int num = sys.sht_seq;
		int16_t tem,hum,d1,d2,d3;
		uint16_t ad0,ad1,ad4,distance;
		uint32_t count,count2,count3;
	  int32_t weight;

			int num2 = sys.sht_noud;
		if(sys.protocol == UDP_PRO ||sys.protocol == TCP_PRO)	
		{
	   if(num2>=24)
			num2=24;
	  }
		if(sys.protocol == COAP_PRO)	
		{		 
	  if(num2>=15)
			num2=15;
		}
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
			strftime(mini, 80, "%Y-%m-%dT%H:%M:%SZ", info);	
			
			if((sys.mod!=model6))
     {
			 uint32_t r_d1_ad0_data=*(__IO uint32_t *)(EEPROM_D1_AD0_START_ADD+num*0x04);
			 ad0 = ((r_d1_ad0_data>>16)&0xFFFF);
			  if((sys.mod!=model3))
		   {
				 d1 = (r_d1_ad0_data&0xFFFF);
			 }
		 }
			if((sys.mod==model1)||(sys.mod==model3)||(sys.mod==model9))
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
			else if(sys.mod==model7)
    {	
			uint32_t r_d2_d3_data=*(__IO uint32_t *)(EEPROM_D2D3_START_ADD+num*0x04);
			uint32_t r_ad1_ad4_data=*(__IO uint32_t *)(EEPROM_AD1_AD4_START_ADD+num*0x04);
			ad1 = ((r_ad1_ad4_data>>16)&0xFFFF);
			ad4 = (r_ad1_ad4_data&0xFFFF);
			d2 = ((r_d2_d3_data>>16)&0xFFFF);
			d3 = (r_d2_d3_data&0xFFFF);
		}
			else if(sys.mod==model8)
    {
				if(sensor.count_mode==1)
		  {			
			 uint32_t r_count_data=*(__IO uint32_t *)(EEPROM_COUNT_START_ADD+num*0x04);
			 uint32_t r_count2_data=*(__IO uint32_t *)(EEPROM_COUNT2_START_ADD+num*0x04);
			 uint32_t r_count3_data=*(__IO uint32_t *)(EEPROM_COUNT3_START_ADD+num*0x04);				
			 count = r_count_data;
			 count2 = r_count2_data;
			 count3 = r_count3_data;
			}else{
			    uint32_t r_ad1_ad4_data=*(__IO uint32_t *)(EEPROM_AD1_AD4_START_ADD+num*0x04);	
          ad1 = ((r_ad1_ad4_data>>16)&0xFFFF);						
			}
		}		
			 else if(sys.mod ==model9)
			{
			uint32_t r_count3_data=*(__IO uint32_t *)(EEPROM_COUNT3_START_ADD+num*0x04);
			count3 = r_count3_data;	
      }
			 else if(sys.mod ==model10)
			{			
			uint32_t r_sht_data=*(__IO uint32_t *)(EEPROM_SHT_START_ADD+num*0x04);
			tem = ((r_sht_data>>16)&0xFFFF);
      }			
			sprintf(buff+strlen(buff),",\"%d\":",i+1);	
					if(sys.mod==model1)
       {
			  sprintf(buff+strlen(buff),"[%.1f,%.1f,%d,%.1f,\"%s\"]",(float)tem/10.0,(float)hum/10.0,ad0,(float)d1/10.0,mini);	
		   }
			 	else if(sys.mod==model2)
       {
			  sprintf(buff+strlen(buff),"[%d,%d,%.1f,\"%s\"]",distance,ad0,(float)d1/10.0,mini);	
		   }
			 	else if(sys.mod==model3)
       {
			  sprintf(buff+strlen(buff),"[%.1f,%.1f,%d,%d,%d,\"%s\"]",(float)tem/10.0,(float)hum/10.0,ad0,ad1,ad4,mini);	
		   }
			 	else if(sys.mod==model4)
       {
			  sprintf(buff+strlen(buff),"[%d,%.1f,%.1f,%.1f,\"%s\"]",ad0,(float)d1/10.0,(float)d2/10.0,(float)d3/10.0,mini);	
		   }
			 	else if(sys.mod==model5)
       {
			  sprintf(buff+strlen(buff),"[%d,%.1f,%d,\"%s\"]",ad0,(float)d1/10.0,weight,mini);	
		   }
			 	else if(sys.mod==model6)
       {
			  sprintf(buff+strlen(buff),"[%d,\"%s\"]",count,mini);	
		   }
			 	else if(sys.mod==model7)
       {
			  sprintf(buff+strlen(buff),"[%d,%d,%d,%.1f,%.1f,%.1f,\"%s\"]",ad0,ad1,ad4,(float)d1/10.0,(float)d2/10.0,(float)d3/10.0,mini);	
		   }
			 	else if(sys.mod==model8)
       {
				 if(sensor.count_mode==1)
		    {
			    sprintf(buff+strlen(buff),"[%d,%d,%d,\"%s\"]",count,count2,count3,mini);	
				}else{
				 sprintf(buff+strlen(buff),"[%d,%.1f,\"%s\"]",ad1,(float)d1/10.0,mini);	
				}
		   }
			 	else if(sys.mod==model9)
       {
			  sprintf(buff+strlen(buff),"[%.1f,%.1f,%d,\"%s\"]",(float)tem/10.0,(float)hum/10.0,count3,mini);	
		   }	
			 	else if(sys.mod==model10)
       {
			  sprintf(buff+strlen(buff),"[%.1f,%d,%.1f,\"%s\"]",(float)d1/10.0,ad0,(float)tem/100.0,mini);	
		   }			 
		}	
			strcat(buff,(char*)"}");	
      pro_data_num=strlen(buff);
}

void mode_data(char *buff)
{
	if(sys.mod == model1)
	{
  sprintf(buff+strlen(buff), "\"DS18B20_Temp\":%.1f,",ds1820_value);	
        sprintf(buff+strlen(buff), "\"digital_in\":%d,",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
	      sprintf(buff+strlen(buff), "\"interrupt\":%d,",sensor.exit_state);		
		    sprintf(buff+strlen(buff), "\"interrupt_level\":%d,",sensor.exit_level);	
	sprintf(buff+strlen(buff), "\"adc1\":%d,",sensor.adc1);	
	sprintf(buff+strlen(buff), "\"temperature\":%.1f,\"humidity\":%.1f",tem_value,hum_value);	
	}
	else if(sys.mod == model2)
	{
  sprintf(buff+strlen(buff), "\"DS18B20_Temp\":%.1f,",ds1820_value);	
        sprintf(buff+strlen(buff), "\"digital_in\":%d,",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
	      sprintf(buff+strlen(buff), "\"interrupt\":%d,",sensor.exit_state);	
sprintf(buff+strlen(buff), "\"interrupt_level\":%d,",sensor.exit_level);			
	sprintf(buff+strlen(buff), "\"adc1\":%d,",sensor.adc1);
	sprintf(buff+strlen(buff), "\"distance\":%d",sensor.distance);			
	}
	else if(sys.mod == model3)
	{
	 sprintf(buff+strlen(buff), "\"adc1\":%d,",sensor.adc1);
        sprintf(buff+strlen(buff), "\"digital_in\":%d,",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
	      sprintf(buff+strlen(buff), "\"interrupt\":%d,",sensor.exit_state);
		sprintf(buff+strlen(buff), "\"interrupt_level\":%d,",sensor.exit_level);	
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
		sprintf(buff+strlen(buff), "\"interrupt_level\":%d,",sensor.exit_level);	
   sprintf(buff+strlen(buff), "\"DS18B20_Temp2\":%.1f,",ds1820_value2);	
   sprintf(buff+strlen(buff), "\"DS18B20_Temp3\":%.1f",ds1820_value3);				
	}	
	else if(sys.mod == model5)
	{
   sprintf(buff+strlen(buff), "\"DS18B20_Temp\":%.1f,",ds1820_value);	
	 sprintf(buff+strlen(buff), "\"adc1\":%d,",sensor.adc1);
        sprintf(buff+strlen(buff), "\"digital_in\":%d,",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
	      sprintf(buff+strlen(buff), "\"interrupt\":%d,",sensor.exit_state);
		sprintf(buff+strlen(buff), "\"interrupt_level\":%d,",sensor.exit_level);						
	 sprintf(buff+strlen(buff), "\"weight\":%d",Weight_Shiwu);	
	}		
	else if(sys.mod == model6)
	{
	sprintf(buff+strlen(buff), "\"count\":%d",sensor.exit_count);		
	}	
	else if(sys.mod == model7)
	{
    sprintf(buff+strlen(buff), "\"digital_in\":%d,",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
	  sprintf(buff+strlen(buff), "\"interrupt\":%d,",sensor.exit_state);
		sprintf(buff+strlen(buff), "\"interrupt_level\":%d,",sensor.exit_level);		
	 sprintf(buff+strlen(buff), "\"adc1\":%d,",sensor.adc1);
	 sprintf(buff+strlen(buff), "\"adc2\":%d,",sensor.adc2);					
	 sprintf(buff+strlen(buff), "\"adc3\":%d,",sensor.adc3);		
   sprintf(buff+strlen(buff), "\"DS18B20_Temp\":%.1f,",ds1820_value);			
   sprintf(buff+strlen(buff), "\"DS18B20_Temp2\":%.1f,",ds1820_value2);	
   sprintf(buff+strlen(buff), "\"DS18B20_Temp3\":%.1f",ds1820_value3);	
	}
	else if(sys.mod == model8)
	{
				if(sensor.count_mode==1)
		{
	    sprintf(buff+strlen(buff), "\"count1\":%d,",sensor.exit_count);	
	    sprintf(buff+strlen(buff), "\"count2\":%d,",sensor.exit_count_pa4);	
	    sprintf(buff+strlen(buff), "\"count3\":%d",sensor.exit_count_pa0);				
		}else{
	    sprintf(buff+strlen(buff), "\"adc2\":%d,",sensor.adc2);	
      sprintf(buff+strlen(buff), "\"DS18B20_Temp\":%.1f,",ds1820_value);
	    sprintf(buff+strlen(buff), "\"interrupt\":%d,",sensor.exit_state);
		  sprintf(buff+strlen(buff), "\"interrupt_level\":%d,",sensor.exit_level);
	    sprintf(buff+strlen(buff), "\"interrupt_pa4\":%d,",sensor.exit_state_pa4);
		  sprintf(buff+strlen(buff), "\"interrupt_level_pa4\":%d,",sensor.exit_level_pa4);
	    sprintf(buff+strlen(buff), "\"interrupt_pa8\":%d,",sensor.exit_state_pa0);
		  sprintf(buff+strlen(buff), "\"interrupt_level_pa8\":%d",sensor.exit_level_pa0);			
		}
	}
	else if(sys.mod == model9)
	{
   sprintf(buff+strlen(buff), "\"digital_in\":%d,",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));			
	 sprintf(buff+strlen(buff), "\"count\":%d,",sensor.exit_count_pa0);
	 sprintf(buff+strlen(buff), "\"temperature\":%.1f,\"humidity\":%.1f",tem_value,hum_value);			
	}	
	else if(sys.mod == model10)
	{
   sprintf(buff+strlen(buff), "\"DS18B20_Temp\":%.1f,",ds1820_value);		
	sprintf(buff+strlen(buff), "\"adc1\":%d,",sensor.adc1);			
	  sprintf(buff+strlen(buff), "\"interrupt\":%d,",sensor.exit_state);
		sprintf(buff+strlen(buff), "\"interrupt_level\":%d,",sensor.exit_level);	
	 sprintf(buff+strlen(buff), "\"temperature\":%.2f",tem_value);			
	}		
}