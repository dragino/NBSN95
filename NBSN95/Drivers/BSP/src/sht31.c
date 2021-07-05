#include "sht31.h"

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

void sht31Data(void)
{
	float hum=0.0;
	float tem=0.0;
	uint8_t rxdata[6]={0};
	uint8_t SHT3X_Start_Buffer[2]={0xE0,0x00};
	uint16_t AD_code = 0;
	sht31Init();
	HAL_Delay(100);
	HAL_I2C_Master_Transmit(&hi2c1,0x88,SHT3X_Start_Buffer,2,1000);
	HAL_I2C_Master_Receive(&hi2c1,0x89,rxdata,6,1000);
		
	AD_code=(rxdata[3]<<8)+rxdata[4];
	AD_code &=~0x0003;   //14bit
	hum=AD_code*100.0/(65536-1);
	if(hum>100.0 || hum<0.0)
	{
		hum=0.0;
	}

	AD_code=(rxdata[0]<<8)+rxdata[1];
	AD_code &=~0x0003;   //14bit
	tem=AD_code*175.0/(65536-1)-45.0;
	if((tem<-40.0)||(tem>125.0))
	{
		tem = 130.0;
	}

  user_main_printf("Humidity =%.2f %%rh",hum);
	user_main_printf("tem =%.2f C",tem);	
	
	sensor.temSHT = (int)(tem*10);
	sensor.humSHT = (int)(hum*10);
	
	sht31LowPower();
}
