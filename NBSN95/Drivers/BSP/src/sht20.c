#include "sht20.h"

I2C_HandleTypeDef hi2c1;

void sht20Init(void)
{
	MX_I2C1_Init();
}

void sht20Data(void)
{
	float hum=0.0;
	float tem=0.0;
	uint8_t rxdata[6]={0};
	uint8_t errorHum = 0,errorTem = 0;
	uint8_t SHT2X_Hum_Buffer[1]={0xf5};
	uint8_t SHT2X_Tem_Buffer[1]={0xf3};
	uint16_t AD_code = 0,sum1 = 0,sum2=0;
	sht20Init();
	
	while(HAL_I2C_Master_Transmit(&hi2c1,0x80,SHT2X_Hum_Buffer,strlen((char*)SHT2X_Hum_Buffer),1000) != HAL_OK)
	{
		sum1++;
		if(sum1>=500)
		{
			break;
		}
		if(HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
		{}
	}
	
	while(HAL_I2C_Master_Receive(&hi2c1,0x81,rxdata,2,1000) != HAL_OK)
	{
		sum2++;
		if(sum2>=3000)       // The minimum time required for temperature conversion, the timeout jumps out of the loop
		{
			break;
		}
		if(HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
		{}
	}
		
	AD_code=(rxdata[0]<<8)+rxdata[1];
	AD_code &=~0x0003;   //14bit
	hum=AD_code*125.0/65536-6;
	if(hum>100.0)
	{
		hum=100.0;
	}
  if(hum<0.0)
	{
		errorHum++;
		if(errorHum==2)
		{
			sht20Init();
		}
		if(errorHum==3)
		{
			NVIC_SystemReset();
		}
	}
	
	memset(rxdata,0,sizeof(rxdata));
	while(HAL_I2C_Master_Transmit(&hi2c1,0x80,SHT2X_Tem_Buffer,strlen((char*)SHT2X_Tem_Buffer),1000) != HAL_OK)
	{
		sum1++;
		if(sum1>=500)
		{
			break;
		}
		if(HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
		{}
	}
	
	while(HAL_I2C_Master_Receive(&hi2c1,0x81,rxdata,2,1000) != HAL_OK)
	{
		sum2++;
		if(sum2>=3000)       // The minimum time required for temperature conversion, the timeout jumps out of the loop
		{
			break;
		}
		if(HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
		{}
	}
	
	AD_code=(rxdata[0]<<8)+rxdata[1];
	AD_code &=~0x0003;   //14bit
	tem=AD_code*175.72/65536-46.85;
	if((tem<-40.0)||(tem>125.0))
	{
		errorTem++;
		if(errorTem==2)
		{
			sht20Init();
		}
		if(errorTem==3)
		{
			NVIC_SystemReset();
		}
	}

  user_main_printf("Humidity =%.2f %%rh",hum);
	user_main_printf("tem =%.2f C",tem);	
	
	sensor.temSHT = tem*10;
	sensor.humSHT = hum*10;
}

void MX_I2C1_Init(void)
{
	hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00707CBB;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter 
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter 
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration    
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();
  
    /**I2C1 GPIO Configuration    
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6|GPIO_PIN_7);

  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
}
