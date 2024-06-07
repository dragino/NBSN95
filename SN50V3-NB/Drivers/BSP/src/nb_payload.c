#include "nb_payload.h"
#include "time.h"
#include <time.h>
extern char buff[2000];
extern float hum_value;
extern float tem_value;
extern float ds1820_value;
extern float ds1820_value2;
extern float ds1820_value3;

void pro_data_thingspeak(void)
{
	uint16_t batteryLevel_mV=getVoltage();
  char buff1[500]={0};
	memset(buff1,0,sizeof(buff1));
	sprintf(buff1, "field1=%d&field2=%.2f&field3=%d&",sys.mod-0x30,batteryLevel_mV/1000.0,nb.singal);	
	if(sys.mod == model1)
	{
  sprintf(buff1+strlen(buff1), "field4=%.1f&",ds1820_value);	
  sprintf(buff1+strlen(buff1), "field5=%d&",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
	sprintf(buff1+strlen(buff1), "field6=%d&",sensor.exit_state);		
	sprintf(buff1+strlen(buff1), "field7=%d&",sensor.adc1);	
	sprintf(buff1+strlen(buff1), "field8=%.1f&field9=%.1f",tem_value,hum_value);	
	}
	else if(sys.mod == model2)
	{
  sprintf(buff1+strlen(buff1), "field4=%.1f&",ds1820_value);	
  sprintf(buff1+strlen(buff1), "field5=%d&",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
	sprintf(buff1+strlen(buff1), "field6=%d&",sensor.exit_state);		
	sprintf(buff1+strlen(buff1), "field7=%d&",sensor.adc1);
	sprintf(buff1+strlen(buff1), "field8=%d",sensor.distance);			
	}
	else if(sys.mod == model3)
	{
	 sprintf(buff1+strlen(buff1), "field4=%d&",sensor.adc1);
   sprintf(buff1+strlen(buff1), "field5=%d&",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
	 sprintf(buff1+strlen(buff1), "field6=%d&",sensor.exit_state);
	 sprintf(buff1+strlen(buff1), "field7=%d&",sensor.adc2);	
	sprintf(buff1+strlen(buff1), "field8=%.1f,field9=%.1f&",tem_value,hum_value);				
	 sprintf(buff1+strlen(buff1), "field10=%d",sensor.adc3);					
	}		
	else if(sys.mod == model4)
	{
   sprintf(buff1+strlen(buff1), "field4=%.1f&",ds1820_value);	
	 sprintf(buff1+strlen(buff1), "field5=%d&",sensor.adc1);
   sprintf(buff1+strlen(buff1), "field6=%d&",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
	 sprintf(buff1+strlen(buff1), "field7=%d&",sensor.exit_state);
   sprintf(buff1+strlen(buff1), "field8=%.1f&",ds1820_value2);	
   sprintf(buff1+strlen(buff1), "field9=%.1f",ds1820_value3);				
	}	
	else if(sys.mod == model5)
	{
   sprintf(buff1+strlen(buff1), "field4=%.1f&",ds1820_value);	
	 sprintf(buff1+strlen(buff1), "field5=%d&",sensor.adc1);
   sprintf(buff1+strlen(buff1), "field6=%d&",HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));	
	  sprintf(buff1+strlen(buff1), "field7=%d&",sensor.exit_state);
	  WEIGHT_SCK_Init();
	  WEIGHT_DOUT_Init();
		int32_t Weight = Get_Weight();	
	  WEIGHT_SCK_DeInit();
	  WEIGHT_DOUT_DeInit();					
	 sprintf(buff1+strlen(buff1), "field8=%d",Weight);	
	}		
	else if(sys.mod == model6)
	{
	sprintf(buff1+strlen(buff1), "field4=%d",sensor.exit_count);		
	}
	
	sprintf(buff+strlen(buff), "%d,%s\r\n",strlen(buff1),buff1);	
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
			 	if(sys.protocol == UDP_PRO || sys.protocol == TCP_PRO)
		      strcat(buff,(char*)"\"");
		 }
		 str_beg=strlen(buff);
	sprintf(buff+strlen(buff),  "{\"IMEI\":\"%s\",\"Model\":\"SN50V3-NB\",\"mod\":%d,\"battery\":%.2f,\"signal\":%d,",user.deui,sys.mod-0x30,batteryLevel_mV/1000.0,nb.singal);	
   mode_data(buff);
		int num = sys.sht_seq;
		int16_t tem,hum,d1,d2,d3;
		uint16_t ad0,ad1,ad4,distance;
		uint32_t count;
	  int32_t weight;
		time_t curtime;
	  struct tm *info;	
			int num2 = sys.sht_noud;
		if(sys.protocol == MQTT_PRO)	
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
		}	
			strcat(buff,(char*)"}");	
      str_end=strlen(buff);
		if(sys.protocol == COAP_PRO)
			break;
	}
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
}