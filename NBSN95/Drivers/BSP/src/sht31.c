#include "sht31.h"

void sht31Init(void)
{
	uint8_t SHT3X_Modecommand_Buffer[2]={0x20,0x32};
	MX_I2C1_Init();
	HAL_I2C_Master_Transmit(&hi2c1,0x88,SHT3X_Modecommand_Buffer,strlen((char*)SHT3X_Modecommand_Buffer),1000);
}

void sht31Data(void)
{
	float hum=0.0;
	float tem=0.0;
	uint8_t rxdata[6]={0};
	uint8_t errorHum = 0,errorTem = 0;
	uint8_t SHT3X_Start_Buffer[2]={0xE0,0x00};
	uint16_t AD_code = 0,sum1 = 0,sum2=0;
	sht31Init();
	
	while(HAL_I2C_Master_Transmit(&hi2c1,0x88,SHT3X_Start_Buffer,strlen((char*)SHT3X_Start_Buffer),1000) != HAL_OK)
	{
		sum1++;
		if(sum1>=500)
		{
			break;
		}
		if(HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
		{}
	}
	
	while(HAL_I2C_Master_Receive(&hi2c1,0x89,rxdata,6,1000) != HAL_OK)
	{
		sum2++;
		if(sum2>=3000)       // The minimum time required for temperature conversion, the timeout jumps out of the loop
		{
			break;
		}
		if(HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
		{}
	}
		
	AD_code=(rxdata[3]<<8)+rxdata[4];
	AD_code &=~0x0003;   //14bit
	hum=AD_code*100.0/(65536-1);
	if(hum>100.0)
	{
		hum=100.0;
	}
  if(hum<0.0)
	{
		errorHum++;
		if(errorHum==2)
		{
			sht31Init();
		}
		if(errorHum==3)
		{
			NVIC_SystemReset();
		}
	}

	AD_code=(rxdata[0]<<8)+rxdata[1];
	AD_code &=~0x0003;   //14bit
	tem=AD_code*175.0/(65536-1)-45.0;
	if((tem<-40.0)||(tem>125.0))
	{
		errorTem++;
		if(errorTem==2)
		{
			sht31Init();
		}
		if(errorTem==3)
		{
			NVIC_SystemReset();
		}
	}

  user_main_printf("Humidity =%.2f %%rh",hum);
	user_main_printf("tem =%.2f C",tem);	
	
	sensor.temSHT = (int)(tem*10);
	sensor.humSHT = (int)(hum*10);
}
