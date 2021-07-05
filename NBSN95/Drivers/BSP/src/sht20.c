#include "sht20.h"

uint8_t sht2x_Detect(void)
{	
	uint8_t sht2x_state_send[1]={0xE7};
	uint8_t sht2x_state_recieve[2]={0};
	HAL_I2C_Master_Transmit(&hi2c1,0x80,sht2x_state_send,1,1000);
	HAL_I2C_Master_Receive(&hi2c1,0x81,sht2x_state_recieve,2,1000);
//	printf("%.2x %.2x",sht2x_state_recieve[0],sht2x_state_recieve[1]);
	if(sht2x_state_recieve[0]!=0 || sht2x_state_recieve[1]!=0)
		return 1;
	return 0;
}

void sht20Data(void)
{
	float hum=0.0;
	float tem=0.0;
	uint8_t rxdata[6]={0};
	uint8_t SHT2X_Hum_Buffer[1]={0xf5};
	uint8_t SHT2X_Tem_Buffer[1]={0xf3};
	uint16_t AD_code = 0;

	HAL_Delay(100);
	HAL_I2C_Master_Transmit(&hi2c1,0x80,SHT2X_Hum_Buffer,strlen((char*)SHT2X_Hum_Buffer),1000);
	HAL_Delay(100);
	HAL_I2C_Master_Receive(&hi2c1,0x81,rxdata,2,1000);
		
	AD_code=(rxdata[0]<<8)+rxdata[1];
	AD_code &=~0x0003;   //14bit
	hum=AD_code*125.0/65536-6;
	if(hum>100.0 || hum<0.0)
	{
		hum=101.0;
	}
	memset(rxdata,0,sizeof(rxdata));
	HAL_Delay(100);
	HAL_I2C_Master_Transmit(&hi2c1,0x80,SHT2X_Tem_Buffer,strlen((char*)SHT2X_Tem_Buffer),1000);
	HAL_Delay(100);
	HAL_I2C_Master_Receive(&hi2c1,0x81,rxdata,2,1000);
	
	AD_code=(rxdata[0]<<8)+rxdata[1];
	AD_code &=~0x0003;   //14bit
	tem=AD_code*175.72/65536-46.85;
	if((tem<-40.0)||(tem>125.0))
	{
		tem = 126.0;
	}

  user_main_printf("Humidity =%.2f %%rh",hum);
	user_main_printf("tem =%.2f C",tem);	
	
	sensor.temSHT = (int)(tem*10);
	sensor.humSHT = (int)(hum*10);
}
