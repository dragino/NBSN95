#include "sht31.h"
float hum_value;
float tem_value;
extern uint16_t tem_store,hum_store;
float gxht30_temp_record=10,gxht30_hum_record=40;
extern bool tdc_clock_log_flag;
void sht31Init(void)
{
	uint8_t SHT3X_Modecommand_Buffer[2]={0x20,0x32};
	HAL_I2C_Master_Transmit(&hi2c1,0x88,SHT3X_Modecommand_Buffer,strlen((char*)SHT3X_Modecommand_Buffer),1000);
}

uint8_t sht3x_Detect(void)
{
	uint8_t sht3x_state_send[2]={0xF3,0x2D};
	uint8_t sht3x_state_recieve[2]={0};
	HAL_I2C_Master_Transmit(&hi2c1,0x88,sht3x_state_send,2,1000);
	HAL_I2C_Master_Receive(&hi2c1,0x89,sht3x_state_recieve,2,1000);
//	printf("%.2x %.2x",sht3x_state_recieve[0],sht3x_state_recieve[1]);
	if(sht3x_state_recieve[0]!=0 || sht3x_state_recieve[1]!=0)
		return 1;
	return 0;
}

void sht31LowPower(void)
{
	uint8_t SHT3X_Modecommand_Buffer[2]={0x24,0x0B};
	HAL_I2C_Master_Transmit(&hi2c1,0x88,SHT3X_Modecommand_Buffer,strlen((char*)SHT3X_Modecommand_Buffer),1000);
}

float temp_Dxpd(float adc_nums[])
{
    uint8_t i, j, isSorted;
    float temp=0.0;
    
    for(i=0; i<4-1; i++)
    {
        isSorted = 1;  
        for(j=0; j<4-1-i; j++)
        {
            if(adc_nums[j] > adc_nums[j+1])
            {
                temp = adc_nums[j];
                adc_nums[j] = adc_nums[j+1];
                adc_nums[j+1] = temp;
                isSorted = 0; 
            }
        }
        if(isSorted) break;
    }
  return adc_nums[1];
}

void sht31Data(void)
{
  HAL_StatusTypeDef   error;
  uint32_t currentTime=0;	
  uint8_t times = 0;	
  float temp1_value[4];
  float temp2_value[4];	
	float hum=0.0;
	float tem=0.0;
	uint8_t rxdata[6]={0};
	uint8_t SHT3X_Start_Buffer[2]={0xE0,0x00};
	uint16_t AD_code = 0;
  do
  {
     times++;
	  sht31Init();
	  HAL_Delay(100);		
		currentTime = TimerGetCurrentTime();
    while(HAL_I2C_Master_Transmit(&hi2c1,0x88,SHT3X_Start_Buffer,2,1000) != HAL_OK)
		{
			if(TimerGetElapsedTime(currentTime) >= 1000)
			{
				HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0);
				break;
			}
		}			
		error = HAL_I2C_Master_Receive(&hi2c1,0x89,rxdata,6,1000);
  }while(times < 4 && error != HAL_OK);	
	AD_code=(rxdata[3]<<8)+rxdata[4];
	AD_code &=~0x0003;   //14bit
	hum=AD_code*100.0/(65536-1);
	AD_code=(rxdata[0]<<8)+rxdata[1];
//	AD_code &=~0x0003;   //14bit
	tem=AD_code*175.0/(65536-1)-45.0;


   if(error != HAL_OK ||tem-gxht30_temp_record>30 || tem-gxht30_temp_record<-30
            || hum<15 || hum>=100 || hum-gxht30_hum_record>10 || hum-gxht30_hum_record<-10)
   {
     times = 0;    
     for(uint8_t kkk=0;kkk<4;kkk++)
     {		 
       do
       {
          times++;
				 	sht31Init();
	        HAL_Delay(100);
		      currentTime = TimerGetCurrentTime();
          while(HAL_I2C_Master_Transmit(&hi2c1,0x88,SHT3X_Start_Buffer,2,1000) != HAL_OK)
		    {
			      if(TimerGetElapsedTime(currentTime) >= 1000)
			     {
				     HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0);
				     break;
			     }
		    }	
		      error = HAL_I2C_Master_Receive(&hi2c1,0x89,rxdata,6,1000);
       }while(times < 4 && error != HAL_OK);	 
	    AD_code=(rxdata[3]<<8)+rxdata[4];
	    AD_code &=~0x0003;   //14bit
	    hum=AD_code*100.0/(65536-1);
	    AD_code=(rxdata[0]<<8)+rxdata[1];
   //	AD_code &=~0x0003;   //14bit
	    tem=AD_code*175.0/(65536-1)-45.0;
		  HAL_Delay(15);	 
      temp1_value[kkk]=tem;
      temp2_value[kkk]=hum;		
		 }	
       temp_Dxpd(temp1_value);
       temp_Dxpd(temp2_value);
       tem=(temp1_value[1]+temp1_value[2])/2.0;
       hum=(temp2_value[1]+temp2_value[2])/2.0;		 
   }
	
	  if(error == HAL_OK)
    {
        if(tem>125)
        {
            tem=125;
            tem=gxht30_temp_record;
        }
        else if(tem<-40)
        {
            tem=-40;
            tem=gxht30_temp_record;
        }
        
        if(hum>100)
        {
            hum=100;
            hum=gxht30_hum_record;
        }
        else if(hum<0)
        {
            hum=0;
            hum=gxht30_hum_record;
        }
        else if(hum==100)
        {
            hum=gxht30_hum_record;
        }
        
        gxht30_temp_record=tem;
        gxht30_hum_record=hum;
    }
    else
    {
        tem=130.0;
        hum=0.0;
    }
	
 		if(tdc_clock_log_flag==0)
	{
  user_main_printf("Humidity =%.2f %%rh",hum);
	user_main_printf("tem =%.2f C",tem);	
	}
  hum_value=hum;
  tem_value=tem;
	sensor.temSHT = (int)(tem*10);
	sensor.humSHT = (int)(hum*10);
 		if(tdc_clock_log_flag==1)
	{
	hum_store=(int)(hum*10);
	tem_store=(int)(tem*10);
	}
	sht31LowPower();
}
