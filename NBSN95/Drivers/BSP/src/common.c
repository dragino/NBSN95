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
		dataSendPut(payload);
		sockClosd();
	}
	
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
		GPIO_InitStruct.Pull = GPIO_NOPULL;
	}
	else if(state == 3)
  {
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
	}
	
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	if(state != 0)
	{
		/* EXTI interrupt init*/
		HAL_NVIC_SetPriority(EXTI4_15_IRQn, 2, 0);
		HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
	}
}

void led_on(uint16_t time)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
	HAL_Delay(time);

	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
	HAL_Delay(10);
}

void MyRtcInit(void)
{
	user_main_printf("Wait for calibration time");
	
	nb_send(nb.recieve_data,AT CCLK"?\n");
	
	char* colon = strchr(nb.recieve_data,':');
	char* comma = strchr(nb.recieve_data,',');
	char* carriage = strchr(nb.recieve_data,'O');
	
	char datebuff[10] = {0};
	char timebuff[15] = {0};
	char snapbuff[3] = {0};
	
	int year = 0,mounth = 0,date = 0;
	int hour = 0,min = 0,		sec = 0 ;
	int jet_lag = 0;
	
	for(int i = 0;i<(comma - colon)-1;i++)
	{
		datebuff[i] = nb.recieve_data[(comma - colon) +i-1];
	}
		
	for(int i=0;i<3;i++)
	{
		memset(snapbuff,0,sizeof(snapbuff));
		if(i == 0)
		{
			snapbuff[0]=datebuff[0];
			snapbuff[1]=datebuff[1];
			year = atoi(snapbuff);
		}
		else if(i == 1)
		{
			snapbuff[0]=datebuff[3];
			snapbuff[1]=datebuff[4];
			mounth = atoi(snapbuff);
		}
		else if(i == 2)
		{
			snapbuff[0]=datebuff[6];
			snapbuff[1]=datebuff[7];
			date = atoi(snapbuff);
		}
	}

	for(int i = 0;i<(carriage - comma)-5;i++)
	{
		timebuff[i] = nb.recieve_data[(carriage - comma) +i+1];
	}
		
	for(int i=0;i<4;i++)
	{
		memset(snapbuff,0,sizeof(snapbuff));
		if(i == 0)
		{
			snapbuff[0]=timebuff[0];
			snapbuff[1]=timebuff[1];
			hour = atoi(snapbuff);
		}
		else if(i == 1)
		{
			snapbuff[0]=timebuff[3];
			snapbuff[1]=timebuff[4];
			min = atoi(snapbuff);
		}
		else if(i == 2)
		{
			snapbuff[0]=timebuff[6];
			snapbuff[1]=timebuff[7];
			sec = atoi(snapbuff);
		}
		else if(i == 3)
		{
			char* zheng = strchr(timebuff,'+');
			char* fu = strchr(timebuff,'-');
			if(zheng != NULL)
				sprintf(snapbuff+strlen(snapbuff), "%s", &timebuff[zheng-timebuff +1]);
			else if(fu != NULL)
				sprintf(snapbuff+strlen(snapbuff), "%s", &timebuff[fu-timebuff +1]);
			
			jet_lag = atoi(snapbuff);
			if(jet_lag > 0)
			{
				if(zheng != NULL)
				{
					hour = hour + jet_lag/4;
					if(hour >= 24)
						hour = hour - 24;
				}
				else if(fu != NULL)
				{
					hour = hour - jet_lag/4;
					if(hour < 0)
						hour =  24 + hour ;
				}
			}
		}		
	}
	
	RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /** Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
    
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date 
  */
  sTime.Hours = (hour )%10+(hour )/10*16;
  sTime.Minutes = (min )%10+(min )/10*16;
  sTime.Seconds = (sec )%10+(sec )/10*16;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = (mounth )%10+(mounth )/10*16;;
  sDate.Date = (date )%10+(date )/10*16;;
  sDate.Year = (year )%10+(year )/10*16;;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
	user_main_printf("Calibration time completed");
	HAL_IWDG_Refresh(&hiwdg);
	My_AlarmInit(18,1);
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
		sprintf(payload+strlen(payload), "%.2x", sensor.exit_count);
		sprintf(payload+strlen(payload), "%.4x", sensor.adc0);
		sprintf(payload+strlen(payload), "%.4x", sensor.temSHT);
		sprintf(payload+strlen(payload), "%.4x", sensor.humSHT);
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
		sprintf(payload+strlen(payload), "%.2x", sensor.exit_count);
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
		sprintf(payload+strlen(payload), "%.2x", sensor.exit_count);
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
		sprintf(payload+strlen(payload), "%.2x", sensor.exit_count);
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
		sprintf(payload+strlen(payload), "%.2x", sensor.exit_count);
		sprintf(payload+strlen(payload), "%.4x", Weight);
	}
	
	user_main_debug("payload:%s\r\n",(payload));

	HAL_GPIO_WritePin(Power_5v_GPIO_Port, Power_5v_Pin, GPIO_PIN_SET);
	
	return payload;
}
