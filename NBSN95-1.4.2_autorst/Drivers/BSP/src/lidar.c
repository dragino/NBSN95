#include "lidar.h"

uint16_t LidarLite(void)
{
	  MX_I2C1_Init();
	  uint8_t dataByte[1]={0x04};
		uint8_t rxdata1[1]={0};
		uint8_t rxdata2[1]={0};
		uint16_t distance;
    waitbusy(); 		
    HAL_I2C_Mem_Write(&hi2c1,0xc4,0x00,1,dataByte,1,1000);	
		if(waitbusy()<9999)
		{		
			HAL_I2C_Mem_Read(&hi2c1,0xc5,0x0f,1,rxdata1,1,1000);
			HAL_I2C_Mem_Read(&hi2c1,0xc5,0x10,1,rxdata2,1,1000);
			distance=(rxdata1[0]<<8)+rxdata2[0];
			if(distance>4000)
			{
				user_main_printf("Distance is out of range");
				distance=65535;			
				return distance;			
			}
			else
			{
				user_main_printf("Distance =%dcm",distance);
				return distance;	
			}			
		}
		else
		{
	    user_main_printf("lidar_lite is not connect");
	    distance=4095;
	    return distance;			
		}
}

uint16_t waitbusy(void)
{
  uint16_t busyCounter = 0;
	uint8_t busy[1]={0x01};
  while (busy[0])      
  {
   if (busyCounter > 9999)
   {
		return busyCounter;			 
   }
		HAL_I2C_Mem_Read(&hi2c1,0xc5,0x01,1,busy,1,1000);
		busy[0] &=0x01;
		busyCounter++;
 }
	return busyCounter;
}
