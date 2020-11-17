#include "common.h"

SYSTEM sys = {0};
SENSOR sensor={0};
USER user={0};

static uint8_t mod5_init_flag = 0;
static uint8_t i2cDevice = 0;

uint8_t uplink(void)
{
	if(strlen((char*)user.add) == 0 || strlen((char*)user.uri) == 0)
	{
		user_main_printf("Parameter configuration error\r\n");
		return fail;
	}
	
	uint8_t state = success;
	char payload[400]={0};
	user_main_printf("*****Upload start:%d*****\r\n",sys.uplink_count++);
	
	payLoadDeal(sys.mod,sensor.data);

	if(sockCreat() == fail)
		state = fail;
	else
	{
		if(sys.protocol == COAP_PRO)
			dataSendPut(payload);
		else if(sys.protocol == UDP_PRO)
			dataSendUDP(payload);
	}
	
	HAL_Delay(1000);
	sockClosd();
	user_main_printf("Send complete");
	user_main_printf("*****End of upload*****\r\n");
	return state;
}

void product_information_print(void)
{
	user_main_printf("\r\nDRAGINO NBSN50-95 NB-IoT Sensor Node\r\n"
										"Image Version: "version "\r\n"
										"NB-IoT Stack : "stack	 "\r\n");
}

void EX_GPIO_Init(uint8_t state)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOB_CLK_ENABLE();
	/*Configure GPIO pin : PB14 */
  GPIO_InitStruct.Pin = GPIO_PIN_14;
	if(state == 0)
	{
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
	}
	else if(state == 1)
	{
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
	}
	else if(state == 2)
	{
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
	}
	else if(state == 3)
  {
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	}
	
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	if(state != 0)
	{
		/* EXTI interrupt init*/
		HAL_NVIC_SetPriority(EXTI4_15_IRQn, 2, 0);
		HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
	}
	else
	{
		HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
	}
}

void led_on(uint16_t time)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
	HAL_Delay(time);

	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
	HAL_Delay(10);
}

void i2cDetection(void)
{
	uint8_t sum=0;
	uint8_t txdata1[1]={0xE7},txdata2[2]={0xF3,0x2D};
	i2cDevice = 0;
	sht20Init();
	while(HAL_I2C_Master_Transmit(&hi2c1,0x80,txdata1,1,1000) != HAL_OK)
	{
		 sum++;
		 if(sum>20)
		 {
			 i2cDevice=0;
			 break;
		 }
	 }
	 if(HAL_I2C_Master_Transmit(&hi2c1,0x80,txdata1,1,1000) == HAL_OK)
	 {
		 i2cDevice=1;
	   user_main_printf("Use Sensor is SHT20");
	 }
	 
	 if(i2cDevice==0)
	 {	 
		sum=0;		 			 
		sht31Init();	 
		while(HAL_I2C_Master_Transmit(&hi2c1,0x88,txdata2,2,1000) != HAL_OK) 
		{
		 sum++;
		 if(sum>20)
		 {
			 i2cDevice=0;
			 break;
		 }
		}
	  if(HAL_I2C_Master_Transmit(&hi2c1,0x88,txdata2,2,1000) == HAL_OK)
	  {
			 i2cDevice=2;
			 user_main_printf("Use Sensor is SHT31");
	  }
   }
	 if(i2cDevice==0)
	 {
//		 HAL_I2C_MspDeInit(&hi2c1);	 
		 user_main_printf("No I2C device detected");
	 }	
}

char* payLoadDeal(uint8_t model,char* payload)
{	
	memset(payload,0,sizeof((char*)payload));
	HAL_GPIO_WritePin(Power_5v_GPIO_Port, Power_5v_Pin, GPIO_PIN_RESET);
	HAL_Delay(500+sys.power_time);
	
	if(model != model6)
	{
		if(sensor.exit_count>255)
			sensor.exit_count = 255;
	}
	
	if(model == model1)
	{
		sensor.batteryLevel_mV = getVoltage();
		sensor.temDs18b20_1 = DS18B20_GetTemp_SkipRom(1)*10;
		sensor.adc0 = ADCModel(ADC_CHANNEL_0);
		
		i2cDetection();
		if(i2cDevice == 1)
			sht20Data();
		else if(i2cDevice == 2)
			sht31Data();
		
		sprintf(payload+strlen(payload), "%.4x", sensor.batteryLevel_mV);
		sprintf(payload+strlen(payload), "%.2x", 0xFF);
		sprintf(payload+strlen(payload), "%.2x", sys.mod - 0x30);
		sprintf(payload+strlen(payload), "%.4x", sensor.temDs18b20_1);		
		sprintf(payload+strlen(payload), "%.2x", sensor.exit_flag);
		sprintf(payload+strlen(payload), "%.4x", sensor.adc0);
		sprintf(payload+strlen(payload), "%.4x", sensor.temSHT);
		sprintf(payload+strlen(payload), "%.4x", sensor.humSHT);
		sprintf(payload+strlen(payload), "%.8x", sensor.exit_count);
	}
	else if(model == model2)
	{
		sensor.batteryLevel_mV = getVoltage();
		sensor.temDs18b20_1 = DS18B20_GetTemp_SkipRom(1)*10;
		sensor.adc0 = ADCModel(ADC_CHANNEL_0);
		sensor.distance = LidarLite();
		if(sensor.distance == 4095)
		{
			sensor.distance = 0;
			sensor.distance = ULT_distance();
			
			GPIO_ULT_INPUT_DeInit();
			GPIO_ULT_OUTPUT_DeInit();
		}
		
		sprintf(payload+strlen(payload), "%.4x", sensor.batteryLevel_mV);
		sprintf(payload+strlen(payload), "%.2x", 0xFF);
		sprintf(payload+strlen(payload), "%.2x", sys.mod - 0x30);		
		sprintf(payload+strlen(payload), "%.4x", sensor.temDs18b20_1);
		sprintf(payload+strlen(payload), "%.2x", sensor.exit_flag);
		sprintf(payload+strlen(payload), "%.4x", sensor.adc0);		
		sprintf(payload+strlen(payload), "%.4x", sensor.distance);
	}
	else if(model == model3)
	{
		sensor.batteryLevel_mV = getVoltage();
		sensor.adc0 = ADCModel(ADC_CHANNEL_0);
		sensor.adc1 = ADCModel(ADC_CHANNEL_1);
		sensor.adc4 = ADCModel(ADC_CHANNEL_4);
	
		i2cDetection();
		if(i2cDevice == 1)
			sht20Data();
		else if(i2cDevice == 2)
			sht31Data();
		
		sprintf(payload+strlen(payload), "%.4x", sensor.batteryLevel_mV);
		sprintf(payload+strlen(payload), "%.2x", 0xFF);
		sprintf(payload+strlen(payload), "%.2x", sys.mod - 0x30);		
		
		sprintf(payload+strlen(payload), "%.4x", sensor.adc0);
		sprintf(payload+strlen(payload), "%.2x", sensor.exit_flag);
		sprintf(payload+strlen(payload), "%.4x", sensor.adc1);
		sprintf(payload+strlen(payload), "%.4x", sensor.temSHT);
		sprintf(payload+strlen(payload), "%.4x", sensor.humSHT);
		sprintf(payload+strlen(payload), "%.4x", sensor.adc4);			
	}
	else if(model == model4)
	{
		sensor.batteryLevel_mV = getVoltage();
		sensor.adc0 = ADCModel(ADC_CHANNEL_0);
		sensor.temDs18b20_1 = DS18B20_GetTemp_SkipRom(1)*10;DS18B20_IoDeInit(1);
		sensor.temDs18b20_2 = DS18B20_GetTemp_SkipRom(2)*10;DS18B20_IoDeInit(2);
		sensor.temDs18b20_3 = DS18B20_GetTemp_SkipRom(3)*10;DS18B20_IoDeInit(3);
		
		sprintf(payload+strlen(payload), "%.4x", sensor.batteryLevel_mV);
		sprintf(payload+strlen(payload), "%.2x", 0xFF);
		sprintf(payload+strlen(payload), "%.2x", sys.mod - 0x30);
				
		sprintf(payload+strlen(payload), "%.4x", sensor.temDs18b20_1);
		sprintf(payload+strlen(payload), "%.4x", sensor.adc0);
		sprintf(payload+strlen(payload), "%.2x", sensor.exit_flag);
		sprintf(payload+strlen(payload), "%.4x", sensor.temDs18b20_2);
		sprintf(payload+strlen(payload), "%.4x", sensor.temDs18b20_3);
	}
	else if(model == model5)
	{
		if(mod5_init_flag == 0)
		{
			at_weight_reset(NULL);
			mod5_init_flag =1;
		}
		
		sensor.batteryLevel_mV = getVoltage();
		sensor.temDs18b20_1 = DS18B20_GetTemp_SkipRom(1)*10;
		sensor.adc0 = ADCModel(ADC_CHANNEL_0);
		int32_t Weight = Get_Weight();		
		user_main_printf("Weight is %d g",Weight);
		
		sprintf(payload+strlen(payload), "%.4x", sensor.batteryLevel_mV);
		sprintf(payload+strlen(payload), "%.2x", 0xFF);
		sprintf(payload+strlen(payload), "%.2x", sys.mod - 0x30);
		sprintf(payload+strlen(payload), "%.4x", sensor.temDs18b20_1);
		sprintf(payload+strlen(payload), "%.4x", sensor.adc0);
		sprintf(payload+strlen(payload), "%.2x", sensor.exit_flag);
		sprintf(payload+strlen(payload), "%.4x", Weight);
	}
	else if(model == model6)
	{
		sensor.batteryLevel_mV = getVoltage();
		
		sprintf(payload+strlen(payload), "%.4x", sensor.batteryLevel_mV);
		sprintf(payload+strlen(payload), "%.2x", 0xFF);
		sprintf(payload+strlen(payload), "%.2x", sys.mod - 0x30);
		sprintf(payload+strlen(payload), "%.2x", (int)(sensor.factor*pow(10,sensor.factor_number)));
		sprintf(payload+strlen(payload), "%.2x", sensor.factor_number);
		sprintf(payload+strlen(payload), "%.8x", sensor.exit_count);
	}
	
	sensor.exit_flag = 0;
	user_main_debug("payload:%s\r\n",(payload));

	HAL_GPIO_WritePin(Power_5v_GPIO_Port, Power_5v_Pin, GPIO_PIN_SET);
	
	return payload;
}
