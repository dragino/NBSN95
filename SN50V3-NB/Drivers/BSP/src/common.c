#include "common.h"
#include "nbInit.h"

static uint8_t sys_pwd[10]={0};
static char sensor_data[1200]={0};
uint8_t detect_flags=0;
uint8_t mode2_flag=0;
extern uint8_t rxbuf_u1;
extern uint8_t mqtt_qos;
SYSTEM sys    = {.pwd=sys_pwd};
SENSOR sensor ={.data=sensor_data};
USER user={0};
uint16_t fire_version=0;
uint16_t fire_pro=0;
uint16_t tem_store,hum_store=0;
extern float ds1820_value;
extern float ds1820_value2;
extern float ds1820_value3;
extern int32_t Weight_Shiwu;
extern uint8_t record_log[512];
uint16_t 	adc0_datalog,adc1_datalog,adc4_datalog;
uint16_t distance_datalog;
static uint8_t mod5_init_flag = 0;
uint8_t debugss=0;
extern uint8_t at_downlink_flag;
extern __IO bool ble_sleep_flags;
static char at_downlink_data[220]={0};
void product_information_print(void)
{
#ifdef NB_1D	
	#if defined NB_NS
	printf("\r\nDRAGINO SN50V3-NS-1D NB-IoT Sensor Node\r\n"
	#else	
	printf("\r\nDRAGINO SN50V3-NB-1D NB-IoT Sensor Node\r\n"
	#endif
#else	
	#if defined NB_NS
	printf("\r\nDRAGINO SN50V3-NS-GE NB-IoT Sensor Node\r\n"
	#else	
	printf("\r\nDRAGINO SN50V3-NB-GE NB-IoT Sensor Node\r\n"
	#endif
	#endif
										"Image Version: "AT_VERSION_STRING "\r\n"
										"NB-IoT Stack : "stack	 "\r\n"
	                  "Protocol in Used: ");

	if(sys.protocol == UDP_PRO)
		printf("UDP\r\n");
	else if(sys.protocol == MQTT_PRO)
		printf("MQTT\r\n");
	else if(sys.protocol == TCP_PRO)
		printf("TCP\r\n");
if(debugss ==1)
	{
	  printf("dragino_6601_ota\r\n");
	}	
}

void reboot_information_print(void)
{
	user_main_debug("reboot flag :0x%04X",RCC->CSR);					//Print register
//__HAL_RCC_GET_FLAG(RCC->CSR);
	
	/*Determine and print the reason for restart*/
	if(RCC->CSR & ((uint32_t)1<<31))
		user_main_printf("reboot error:Low-power!");						//Low power management
	else if(RCC->CSR & ((uint32_t)1<<30))
		user_main_printf("reboot error:Window watchdog!");			//Window watchdog
	else if(RCC->CSR & ((uint32_t)1<<29))
		user_main_printf("reboot error:Independent watchdog!");	//Independent watchdog
	else if(RCC->CSR & ((uint32_t)1<<28))
		user_main_printf("reboot error:Software!");							//Software reset
	else if(RCC->CSR & ((uint32_t)1<<27))
		user_main_printf("reboot error:POR/PDR!");							//por/pdr reset
	else if(RCC->CSR & ((uint32_t)1<<26))
		user_main_printf("reboot error:NRST!");									//NRST pin reset
	else if(RCC->CSR & ((uint32_t)1<<25))
		user_main_printf("reboot error:BOR!");									//Software writes to clear the RMVF bit
	
	__HAL_RCC_CLEAR_RESET_FLAGS(); 														//Clear flag
}

void EX_GPIO_Init(uint8_t state)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	if(state == 0)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		GPIO_InitStruct.Pin = GPIO_PIN_15;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = GPIO_PIN_4;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
	else if(state == 1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		GPIO_InitStruct.Pin = GPIO_PIN_15;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = GPIO_PIN_4;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);		
	}
	else if(state == 2)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		GPIO_InitStruct.Pin = GPIO_PIN_15;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = GPIO_PIN_4;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);		
	}
	else if(state == 3)
  {
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		GPIO_InitStruct.Pin = GPIO_PIN_15;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = GPIO_PIN_4;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
		
	}
	
	if(state!=0)
	{
		HAL_NVIC_SetPriority(EXTI4_15_IRQn, 2, 0);
		HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
	}
}

void led_on(uint16_t time)
{
	HAL_GPIO_WritePin(LED_RGB_PORT,LED_BLUE_PIN,GPIO_PIN_SET);
	HAL_Delay(time);

	HAL_GPIO_WritePin(LED_RGB_PORT,LED_BLUE_PIN,GPIO_PIN_RESET);
	HAL_Delay(100);
}

void BSP_sensor_Init( void  )
{		
	HAL_GPIO_WritePin(Power_5v_GPIO_Port, Power_5v_Pin, GPIO_PIN_RESET);
	HAL_Delay(1000);
	if((sys.mod==model1)||(sys.mod==model3))
  {
		MX_I2C1_Init();
		if(sht2x_Detect() == 1)
	 {
		detect_flags=1;
		printf("Use Sensor is SHT2x\r\n");
		HAL_Delay(20);
	 }
    if(sht3x_Detect() == 1)
	 {
		detect_flags=2;
		printf("Use Sensor is SHT3x\r\n");
		HAL_Delay(20);
	 }
    if(detect_flags==0)
	 {
			 printf("No I2C device detected\r\n");
			 HAL_Delay(20);
	 }
	 HAL_I2C_MspDeInit(&hi2c1);	
	}
	else if(sys.mod==model2)
	{
	  GPIO_ULT_INPUT_Init();
		GPIO_ULT_OUTPUT_Init();	
		HAL_Delay(100);
	  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)==0)
	  {
			mode2_flag=2;	
			printf("Use Sensor is ultrasonic distance measurement\r\n");
	    HAL_Delay(20);
			GPIO_ULT_INPUT_DeInit();
			GPIO_ULT_OUTPUT_DeInit();	
		}
		else
		{	
			MX_I2C1_Init();
			if(waitbusy(2)<999)
			{
			 mode2_flag=1;			
			 printf("Use Sensor is LIDAR_Lite_v3HP\r\n");
			 HAL_Delay(20);
			}	
			if(mode2_flag==0)
			{	 
			  if(ULT_Connection_Test()==1)
				{
					mode2_flag=3;
					printf("Use Sensor is ult sensor\r\n");	
					HAL_Delay(20);
				}	
				else
				{	
					printf("No distance measurement device detected\r\n");	
          HAL_Delay(20);					
				}
			}
		 }			
			HAL_I2C_MspDeInit(&hi2c1);	
		}
	 else if(sys.mod==model5)
	{
		    WEIGHT_SCK_Init();
	      WEIGHT_DOUT_Init();
	      Get_Maopi();
	      HAL_Delay(500);
        Get_Maopi();
      	WEIGHT_SCK_DeInit();
	      WEIGHT_DOUT_DeInit();	
		    printf("Use Sensor is HX711\r\n");	
        HAL_Delay(20);	
	}
 
	HAL_GPIO_WritePin(Power_5v_GPIO_Port, Power_5v_Pin, GPIO_PIN_SET);	
}

void txPayLoadDeal(SENSOR* Sensor)
{	
	if(ble_sleep_flags==1)
	{
		if((HAL_GPIO_ReadPin(DX_BT24_STATUS_PORT,DX_BT24_LINK_PIN)==1)||(HAL_GPIO_ReadPin(DX_BT24_STATUS_PORT,DX_BT24_WORK_PIN)==1))
		{
			HAL_Delay(50);					
			printf("AT+PWRM2\r\n");			
		}
	}	
	memset(Sensor->data,0,sizeof((char*)Sensor->data));
	sensor.humSHT = 0;
	sensor.temSHT = 0;
	Sensor->data_len = 0;
		
	Sensor->singal = nb.singal;
	
	HAL_GPIO_WritePin(Power_5v_GPIO_Port, Power_5v_Pin, GPIO_PIN_RESET);
	HAL_Delay(500+sys.power_time);
	Sensor->batteryLevel_mV = getVoltage();
	user_main_printf("remaining battery =%d mv",Sensor->batteryLevel_mV);
	sprintf(Sensor->data+strlen(Sensor->data), "%c", 'f');
	for(int i=0;i<strlen((char*)user.deui);i++)
		sprintf(Sensor->data+strlen(Sensor->data), "%c",  user.deui[i]);

		sprintf(Sensor->data+strlen(Sensor->data), "%.2x",  0x04);
		sprintf(Sensor->data+strlen(Sensor->data), "%.2x",  string_touint());
	
	sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->batteryLevel_mV);
	sprintf(Sensor->data+strlen(Sensor->data), "%.2x", Sensor->singal);
	sprintf(Sensor->data+strlen(Sensor->data), "%.2x", sys.mod-0x30);

	if(sys.mod == model1)
	{
		Sensor->temDs18b20_1 = (int)(DS18B20_GetTemp_SkipRom(1)*10);
		Sensor->adc1 = ADCModel(ADC_CHANNEL_4);
		
		MX_I2C1_Init();
		if(detect_flags == 1)
			sht20Data();
		else if(detect_flags == 2)
			sht31Data();
    HAL_I2C_MspDeInit(&hi2c1);	
		HAL_Delay(20);
		
		sprintf(Sensor->data+strlen(Sensor->data), "%c", (Sensor->temDs18b20_1>=0)?'0':'F');
		sprintf(Sensor->data+strlen(Sensor->data), "%.3x",(Sensor->temDs18b20_1>=0)?Sensor->temDs18b20_1:Sensor->temDs18b20_1*(-1));
		sprintf(Sensor->data+strlen(Sensor->data), "%.2x", HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));
		sprintf(Sensor->data+strlen(Sensor->data), "%.2x", Sensor->exit_state);
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->adc1);
		sprintf(Sensor->data+strlen(Sensor->data), "%c", (Sensor->temSHT>=0)?'0':'F');
		sprintf(Sensor->data+strlen(Sensor->data), "%.3x", (Sensor->temSHT>=0)?Sensor->temSHT:Sensor->temSHT*(-1));
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->humSHT);
	}
	else if(sys.mod == model2)
	{
		Sensor->temDs18b20_1 = DS18B20_GetTemp_SkipRom(1)*10;
		Sensor->adc1 = ADCModel(ADC_CHANNEL_4);
		
		 if(mode2_flag==1)
		{
			Sensor->distance = LidarLite();
			HAL_I2C_MspDeInit(&hi2c1);	
		}
		else if(mode2_flag==2)
		{
     sensor.distance = ULT_distance();
		 GPIO_ULT_INPUT_DeInit();
		 GPIO_ULT_OUTPUT_DeInit();
		}
    else if(mode2_flag==3)
		{	
		 ULT_Rest();
     MX_USART1_UART_Init	();		
     uart1_Init();
		 HAL_UART_Receive_IT(&huart1,(uint8_t*)&rxbuf_u1,RXSIZE);
	   HAL_Delay(100);
		 ULT_getData();
		 uart1_IoDeInit();
		 Sensor->distance = ULT_Data_processing();	
		}
		else
		{
			Sensor->distance = 0;			
		}
		
		sprintf(Sensor->data+strlen(Sensor->data), "%c", (Sensor->temDs18b20_1>=0)?'0':'F');
		sprintf(Sensor->data+strlen(Sensor->data), "%.3x",(Sensor->temDs18b20_1>=0)?Sensor->temDs18b20_1:Sensor->temDs18b20_1*(-1));		
		sprintf(Sensor->data+strlen(Sensor->data), "%.2x", HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));
		sprintf(Sensor->data+strlen(Sensor->data), "%.2x", Sensor->exit_state);
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->adc1);		
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->distance);
	}
	else if(sys.mod == model3)
	{
		Sensor->adc1 = ADCModel(ADC_CHANNEL_4);
		Sensor->adc2 = ADCModel(ADC_CHANNEL_1);
		Sensor->adc3 = ADCModel(ADC_CHANNEL_0);
	
		MX_I2C1_Init();
		if(detect_flags == 1)
			sht20Data();
		else if(detect_flags == 2)
			sht31Data();
    HAL_I2C_MspDeInit(&hi2c1);	
		HAL_Delay(20);
		
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->adc1);
		sprintf(Sensor->data+strlen(Sensor->data), "%.2x", HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));
		sprintf(Sensor->data+strlen(Sensor->data), "%.2x", Sensor->exit_state);
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->adc2);
		sprintf(Sensor->data+strlen(Sensor->data), "%c", (Sensor->temSHT>=0)?'0':'F');
		sprintf(Sensor->data+strlen(Sensor->data), "%.3x", (Sensor->temSHT>=0)?Sensor->temSHT:Sensor->temSHT*(-1));
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->humSHT);
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->adc3);			
	}
	else if(sys.mod == model4)
	{
		Sensor->adc1 = ADCModel(ADC_CHANNEL_4);
		Sensor->temDs18b20_1 = DS18B20_GetTemp_SkipRom(1)*10;DS18B20_IoDeInit(1);
		Sensor->temDs18b20_2 = DS18B20_GetTemp_SkipRom(2)*10;DS18B20_IoDeInit(2);
		Sensor->temDs18b20_3 = DS18B20_GetTemp_SkipRom(3)*10;DS18B20_IoDeInit(3);
				
		sprintf(Sensor->data+strlen(Sensor->data), "%c", (Sensor->temDs18b20_1>=0)?'0':'F');
		sprintf(Sensor->data+strlen(Sensor->data), "%.3x",(Sensor->temDs18b20_1>=0)?Sensor->temDs18b20_1:Sensor->temDs18b20_1*(-1));		
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->adc1);
		sprintf(Sensor->data+strlen(Sensor->data), "%.2x", HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));
		sprintf(Sensor->data+strlen(Sensor->data), "%.2x", Sensor->exit_state);
		sprintf(Sensor->data+strlen(Sensor->data), "%c", (Sensor->temDs18b20_2>=0)?'0':'F');
		sprintf(Sensor->data+strlen(Sensor->data), "%.3x",(Sensor->temDs18b20_2>=0)?Sensor->temDs18b20_2:Sensor->temDs18b20_2*(-1));		
		sprintf(Sensor->data+strlen(Sensor->data), "%c", (Sensor->temDs18b20_3>=0)?'0':'F');
		sprintf(Sensor->data+strlen(Sensor->data), "%.3x",(Sensor->temDs18b20_3>=0)?Sensor->temDs18b20_3:Sensor->temDs18b20_3*(-1));		
	}
	else if(sys.mod == model5)
	{
		if(mod5_init_flag == 0)
		{
				WEIGHT_SCK_Init();
	      WEIGHT_DOUT_Init();
	      Get_Maopi();
	      HAL_Delay(500);
        Get_Maopi();
      	WEIGHT_SCK_DeInit();
	      WEIGHT_DOUT_DeInit();		
			  mod5_init_flag =1;
		}
		
		Sensor->temDs18b20_1 = DS18B20_GetTemp_SkipRom(1)*10;
		Sensor->adc1 = ADCModel(ADC_CHANNEL_4);
	  WEIGHT_SCK_Init();
	  WEIGHT_DOUT_Init();
		int32_t Weight = Get_Weight();	
	  WEIGHT_SCK_DeInit();
	  WEIGHT_DOUT_DeInit();		
		user_main_printf("Weight is %d g",Weight);
		
		sprintf(Sensor->data+strlen(Sensor->data), "%c", (Sensor->temDs18b20_1>=0)?'0':'F');
		sprintf(Sensor->data+strlen(Sensor->data), "%.3x",(Sensor->temDs18b20_1>=0)?Sensor->temDs18b20_1:Sensor->temDs18b20_1*(-1));		
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->adc1);
		sprintf(Sensor->data+strlen(Sensor->data), "%.2x", HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));
		sprintf(Sensor->data+strlen(Sensor->data), "%.2x", Sensor->exit_state);
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Weight);
	}
	else if(sys.mod == model6)
	{
		sprintf(Sensor->data+strlen(Sensor->data), "%.8x", sensor.exit_count);
	}
	sprintf(Sensor->data+strlen(Sensor->data), "%.8x", sensor.time_stamp);
		
		int num = sys.sht_seq;
		for(uint8_t i=0;i<sys.sht_noud;i++)
		{
			num--;
			if(num<0)
				num=32+num;
			uint32_t r_time=*(__IO uint32_t *)(EEPROM_TIME_START_ADD+num*0x04);			
			
			if((sys.mod!=model6))
     {
			 uint32_t r_ad0_data=*(__IO uint32_t *)(EEPROM_D1_AD0_START_ADD+num*0x04);
			 sprintf(Sensor->data+strlen(Sensor->data), "%.4x", (r_ad0_data>>16)&0xFFFF);
			  if((sys.mod!=model3))
		   {
				 sprintf(Sensor->data+strlen(Sensor->data), "%.4x", r_ad0_data&0xFFFF);
			 }
		 }
			if((sys.mod==model1)||(sys.mod==model3))
     {
			uint32_t r_sht_data=*(__IO uint32_t *)(EEPROM_SHT_START_ADD+num*0x04);
			sprintf(Sensor->data+strlen(Sensor->data), "%.4x", (r_sht_data>>16)&0xFFFF);
			sprintf(Sensor->data+strlen(Sensor->data), "%.4x", r_sht_data&0xFFFF);
		 }
			if(sys.mod==model2)
    {	
			uint32_t r_distance_data=*(__IO uint32_t *)(EEPROM_DISTANCE_START_ADD+num*0x04);
			sprintf(Sensor->data+strlen(Sensor->data), "%.4x", r_distance_data&0xFFFF);
		}
			if(sys.mod==model3)
    {	
			uint32_t r_ad1_ad4_data=*(__IO uint32_t *)(EEPROM_AD1_AD4_START_ADD+num*0x04);
			sprintf(Sensor->data+strlen(Sensor->data), "%.4x", (r_ad1_ad4_data>>16)&0xFFFF);
			sprintf(Sensor->data+strlen(Sensor->data), "%.4x", r_ad1_ad4_data&0xFFFF);
		}
			if(sys.mod==model4)
    {	
			uint32_t r_d2_d3_data=*(__IO uint32_t *)(EEPROM_D2D3_START_ADD+num*0x04);
			sprintf(Sensor->data+strlen(Sensor->data), "%.4x", (r_d2_d3_data>>16)&0xFFFF);
			sprintf(Sensor->data+strlen(Sensor->data), "%.4x", r_d2_d3_data&0xFFFF);
		}
			if(sys.mod==model5)
    {	
			uint32_t r_weight_data=*(__IO uint32_t *)(EEPROM_WEIGHT_START_ADD+num*0x04);
			sprintf(Sensor->data+strlen(Sensor->data), "%.8x", r_weight_data);
		}
			if(sys.mod==model6)
    {	
			uint32_t r_count_data=*(__IO uint32_t *)(EEPROM_COUNT_START_ADD+num*0x04);
			sprintf(Sensor->data+strlen(Sensor->data), "%.8x", r_count_data);
		}		
			sprintf(Sensor->data+strlen(Sensor->data), "%.8x", r_time);			
		}				
	
		Sensor->data_len = strlen(Sensor->data);
	if(sys.protocol == UDP_PRO || sys.protocol == TCP_PRO)
	{
	 Sensor->data_len = strlen(Sensor->data)/2;
	}
	
	user_main_debug("Sensor->data:%s",Sensor->data);
	user_main_debug("Sensor->data_len:%d",Sensor->data_len);
	Sensor->exit_state = 0;
	sys.exit_flag = 0;
	HAL_GPIO_WritePin(Power_5v_GPIO_Port, Power_5v_Pin, GPIO_PIN_SET);
	HAL_IWDG_Refresh(&hiwdg);
}
	
void txPayLoadDeal2(SENSOR* Sensor)
{	
	user_main_printf("remaining battery =%d mv",Sensor->batteryLevel_mV);	
	Sensor->batteryLevel_mV = getVoltage();
  get_sensorvalue();
	if(sys.mod == model6)
	{
	user_main_printf("count is %d ",sensor.exit_count);
	}
}



		
/**
	* @brief  Processing received data 
  * @param  Instruction parameter
  * @retval None
  */
void rxPayLoadDeal(char* payload)
{
	if(at_downlink_flag==1)
	{
		at_downlink_flag=0;
		char* pos_start;
		char* pos_end;
	
		memset(at_downlink_data,0,220);		
	  pos_start  = strstr((char*)payload,"AT+SERVADDR\":\"");
	  pos_end    = strchr(pos_start,'\n');	
	  memcpy(at_downlink_data,&nb.usart.data[pos_start-((char*)nb.usart.data)+14],(pos_end-pos_start-16));		
		memset(user.add,0,sizeof(user.add));
	  memcpy(user.add,at_downlink_data,strlen(at_downlink_data));	

		memset(at_downlink_data,0,220);		
	  pos_start  = strstr((char*)payload,"AT+CLIENT\":\"");
	  pos_end    = strchr(pos_start,'\n');	
	  memcpy(at_downlink_data,&nb.usart.data[pos_start-((char*)nb.usart.data)+12],(pos_end-pos_start-14));		
		memset(user.client,0,sizeof(user.client));
	  memcpy(user.client,at_downlink_data,strlen(at_downlink_data));	

		memset(at_downlink_data,0,220);		
	  pos_start  = strstr((char*)payload,"AT+UNAME\":\"");
	  pos_end    = strchr(pos_start,'\n');	
	  memcpy(at_downlink_data,&nb.usart.data[pos_start-((char*)nb.usart.data)+11],(pos_end-pos_start-13));		
		memset(user.uname,0,sizeof(user.uname));
	  memcpy(user.uname,at_downlink_data,strlen(at_downlink_data));	

		memset(at_downlink_data,0,220);		
	  pos_start  = strstr((char*)payload,"AT+PWD\":\"");
	  pos_end    = strchr(pos_start,'\n');	
	  memcpy(at_downlink_data,&nb.usart.data[pos_start-((char*)nb.usart.data)+9],(pos_end-pos_start-11));		
		memset(user.pwd,0,sizeof(user.pwd));
	  memcpy(user.pwd,at_downlink_data,strlen(at_downlink_data));	

		memset(at_downlink_data,0,220);		
	  pos_start  = strstr((char*)payload,"AT+PUBTOPIC\":\"");
	  pos_end    = strchr(pos_start,'\n');	
	  memcpy(at_downlink_data,&nb.usart.data[pos_start-((char*)nb.usart.data)+14],(pos_end-pos_start-16));		
		memset(user.pubtopic,0,sizeof(user.pubtopic));
	  memcpy(user.pubtopic,at_downlink_data,strlen(at_downlink_data));	

	  memset(at_downlink_data,0,220);		
	  pos_start  = strstr((char*)payload,"AT+SUBTOPIC\":\"");
	  pos_end    = strchr(pos_start,'\n');	
	  memcpy(at_downlink_data,&nb.usart.data[pos_start-((char*)nb.usart.data)+14],(pos_end-pos_start-16));		
		memset(user.subtopic,0,sizeof(user.subtopic));
	  memcpy(user.subtopic,at_downlink_data,strlen(at_downlink_data));	

		memset(at_downlink_data,0,220);		
	  pos_start  = strstr((char*)payload,"AT+TDC\":\"");
	  pos_end    = strchr(pos_start,'\n');	
	  memcpy(at_downlink_data,&nb.usart.data[pos_start-((char*)nb.usart.data)+9],(pos_end-pos_start-11));	
	  uint32_t tdc = atoi(at_downlink_data);		
	  sys.tdc = tdc;

		memset(at_downlink_data,0,220);		
	  pos_start  = strstr((char*)payload,"AT+APN\":\"");
	  pos_end    = strchr(pos_start,'\n');	
	  memcpy(at_downlink_data,&nb.usart.data[pos_start-((char*)nb.usart.data)+9],(pos_end-pos_start-11));		
		memset(user.apn,0,sizeof(user.apn));
	  memcpy(user.apn,at_downlink_data,strlen(at_downlink_data));	

		memset(at_downlink_data,0,220);		
	  pos_start  = strstr((char*)payload,"AT+PRO\":\"");
	  pos_end    = strchr(pos_start,'\n');	
	  memcpy(at_downlink_data,&nb.usart.data[pos_start-((char*)nb.usart.data)+9],(pos_end-pos_start-10));		
		uint8_t protocol = at_downlink_data[0]-0x30;
	  sys.protocol = protocol;
		sys.platform =at_downlink_data[2]-0x30;
		
	  config_Set();	
	  if(strstr((char*)at_downlink_data,"ATZ") != NULL)
		{
		 user_main_printf("Reset the device after receiving the downlink...");		
	   NVIC_SystemReset();
		}
	}
	else
  {
	uint8_t dataCom[10]={0};
	rtrim((char*)payload);
	StrToHex((char*)dataCom,payload,strlen(payload));
	uint8_t dataCom_len = strlen(payload)/2;
	user_main_debug("dataCom_len:%d",dataCom_len);
	
	switch(dataCom[0])
	{
		case 0x01:
			if(dataCom_len == 4)
			{
				int tdc=( dataCom[1]<<16 | dataCom[2]<<8 | dataCom[3] );
				if(tdc >= 60 && tdc <= 0xFFFFFF)
				{
					sys.tdc = tdc;
					config_Set();
				}
			}
			break;
		case 0x04:
			if(dataCom_len == 2 && dataCom[1] == 0xFF)
				NVIC_SystemReset();
			break;
		case 0x06:
			if(dataCom_len == 4 && (dataCom[3]<=3))
			{
				sys.inmod = dataCom[3]+0x30;
				EX_GPIO_Init(sys.inmod-0x30);
				config_Set();
			}			
			break;
		default:
			printf("Downstream parameter error\n");
			break;
	}
  }	
}

/**
	* @brief  Processing version number 
  * @param  NULL
  * @retval None
  */
uint16_t string_touint(void)
{
	char *p;	
	uint8_t chanum=0;	
	uint16_t versi;
	char version[8]="";
	p=AT_VERSION_STRING;
	
	while(*p++!='\0')
	{
		if(*p>='0'&&*p<='9')
		{
			version[chanum++]=*p;
		}		 
	}
	versi=atoi(version);
	return versi;
}

void new_firmware_update(void)
{
	uint32_t update_flags,r_config;
	uint16_t config_ver,config_pro;
	r_config=*(__IO uint32_t *)EEPROM_USER_START_ADD;
	config_ver=r_config>>16;
	config_pro=r_config&0xFFFF;
	fire_version=string_touint();
	fire_pro=product_id;
  if((config_ver!=fire_version)||(config_pro!=fire_pro))
	{
		update_flags = (fire_version<<16)| fire_pro;
		HAL_FLASHEx_DATAEEPROM_Unlock();
		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,EEPROM_USER_START_ADD,update_flags);//store hardversion
		HAL_FLASHEx_DATAEEPROM_Lock();
		at_fdr1_run(NULL);
	}
}
void StrToHex(char *pbDest, char *pszSrc, int nLen)
{
	char h1, h2;
	char s1, s2;
	for (int i = 0; i < nLen; i++)
	{
		h1 = pszSrc[2 * i];
		h2 = pszSrc[2 * i + 1];
 
		s1 = toupper(h1) - 0x30;
		if (s1 > 9)
			s1 -= 7;
 
		s2 = toupper(h2) - 0x30;
		if (s2 > 9)
			s2 -= 7;
 
		pbDest[i] = s1 * 16 + s2;
	}
}

int hexToint(char *str)
{  
	int i;  
	int n = 0;  
	if (str[0] == '0' && (str[1]=='x' || str[1]=='X'))   
		i = 2;   
	else  
		i = 0;  
	for (; (str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'z') || (str[i] >='A' && str[i] <= 'Z');++i)  
	{  
		if (tolower(str[i]) > '9')
			n = 16 * n + (10 + tolower(str[i]) - 'a');  
		else  
			n = 16 * n + (tolower(str[i]) - '0');  
	}
	return n;  
}

int countchar(char *str,char a)
{
    int n=0;
    int i = 0;
    while(*(str+i)!='\0')
    {
        if(*(str+i) == a)
            n++;
        i++;
    }
    return n;
}

uint8_t is_ipv4_addr(char *ip)
{
	if (ip == NULL || ip[0] == '\0' || strstr(ip,"NULL")!=NULL) 
		return 2;//Parameter error 
	
	char *p=strrchr(ip,',');	
	if(p==NULL)
		return 2;//Parameter error 
	
	char add_buf[50]={0};
	memcpy(add_buf,ip,p-(char*)user.add);
	
	for (uint8_t i = 0, count = 0; i < strlen(add_buf); i++) 
	{
		if ((add_buf[i] != '.') && (add_buf[i] < '0' || add_buf[i] > '9')) 
		{
			return 0;
		}
		if (add_buf[i] == '.')
		{
			count++;
			if (count > 3) 
			{
				return 0;
			}
		}
	}
	
	return 1;
}

uint8_t is_ipv6_addr(char *ip)
{
	if (ip == NULL || ip[0] == '\0' || strstr(ip,"NULL")!=NULL) 
		return 2;//Parameter error 
	
	char *p=strrchr(ip,',');	
	if(p==NULL)
		return 2;//Parameter error 
	
	char add_buf[60]={0};
	memcpy(add_buf,ip,p-(char*)user.add);	
	
	int len=strlen(add_buf);
	int count=0;
	for(int i=0;i<len;i++)
	{
	  if(add_buf[i]==':')
		{
		   count++;
			continue;
		}
		if((add_buf[i] >='0' && add_buf[i] <='9') || (add_buf[i] >='a' && add_buf[i] <='f') || (add_buf[i] >='A' && add_buf[i] <='F'))
			continue;
		else
			return 0;
	}
 if(count==7)
	return 1;
}

char* Int2String(int num,char *str)
{
    int i = 0;
    if(num<0)
    {
        num = -num;
        str[i++] = '-';
    } 
    do
    {
        str[i++] = num%10+48;
        num /= 10;  
    }while(num);
    
    str[i] = '\0';
    
    int j = 0;
    if(str[0]=='-')
    {
        j = 1;
        ++i;
    }
    for(;j<i/2;j++)
    {
        str[j] = str[j] + str[i-1-j];
        str[i-1-j] = str[j] - str[i-1-j];
        str[j] = str[j] - str[i-1-j];
    } 
    return str; 
}
void shtDataINIT(void)
{
	sys.sht_seq = 0;
	for(uint8_t i=0;i<32;i++)
	{
		uint32_t r_time=*(__IO uint32_t *)(EEPROM_TIME_START_ADD+i*0x04);
		if(r_time==0)
		{
			sys.sht_seq = i;
			return;
		}
	}
	
	for(uint8_t i=0;i<32;i++)
	{
		uint32_t r_time_1=*(__IO uint32_t *)(EEPROM_TIME_START_ADD+i*0x04);
		uint32_t r_time_2=*(__IO uint32_t *)(EEPROM_TIME_START_ADD+(i+1)*0x04);
		if(r_time_1 >= r_time_2)
		{
			sys.sht_seq = i+1;
			return;
		}
	}
}

void shtDataWrite(void)
{	
	if(sys.sht_seq>=32)
		sys.sht_seq = 0;
  if(sys.mod == model1 ||sys.mod == model3)	
	{
	uint32_t w_sht_data=tem_store<<16 | hum_store;	
	user_main_debug("w_sht_data:%x",w_sht_data);
	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,EEPROM_SHT_START_ADD +sys.sht_seq * 0x04,w_sht_data);
	HAL_FLASHEx_DATAEEPROM_Lock();
	}

  if(sys.mod !=model6)	
	{
	uint32_t w_ad0_d1_data=adc0_datalog<<16 | ((int16_t)(ds1820_value*10)&0xFFFF);	
	user_main_debug("w_sht_data:%x",w_ad0_d1_data);
	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,EEPROM_D1_AD0_START_ADD +sys.sht_seq * 0x04,w_ad0_d1_data);
	HAL_FLASHEx_DATAEEPROM_Lock();		
	}		
  if(sys.mod ==model2)	
	{
	uint32_t w_distance_data=distance_datalog;	
	user_main_debug("w_sht_data:%x",w_distance_data);
	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,EEPROM_DISTANCE_START_ADD +sys.sht_seq * 0x04,w_distance_data);
	HAL_FLASHEx_DATAEEPROM_Lock();			
	}
  	else if(sys.mod ==model3)	
	{
	uint32_t w_ad14_data=adc1_datalog<<16 | adc4_datalog;	
	user_main_debug("w_sht_data:%x",w_ad14_data);
	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,EEPROM_AD1_AD4_START_ADD +sys.sht_seq * 0x04,w_ad14_data);
	HAL_FLASHEx_DATAEEPROM_Lock();			
	}
  	else if(sys.mod ==model4)	
	{
	uint32_t w_d2d3_data=(int16_t)(ds1820_value2*10)<<16 | ((int16_t)(ds1820_value3*10)&0xFFFF);	
	user_main_debug("w_sht_data:%x",w_d2d3_data);
	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,EEPROM_D2D3_START_ADD +sys.sht_seq * 0x04,w_d2d3_data);
	HAL_FLASHEx_DATAEEPROM_Lock();			
	}
  	else if(sys.mod ==model5)	
	{
	uint32_t w_weight_data=Weight_Shiwu;	
	user_main_debug("w_sht_data:%x",w_weight_data);
	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,EEPROM_WEIGHT_START_ADD +sys.sht_seq * 0x04,w_weight_data);
	HAL_FLASHEx_DATAEEPROM_Lock();			
	}	
  	else if(sys.mod ==model6)	
	{
	uint32_t w_count_data=sensor.exit_count;	
	user_main_debug("w_sht_data:%x",w_count_data);
	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,EEPROM_COUNT_START_ADD +sys.sht_seq * 0x04,w_count_data);
	HAL_FLASHEx_DATAEEPROM_Lock();			
	}		

	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,EEPROM_TIME_START_ADD+sys.sht_seq * 0x04,sensor.time_stamp);
	HAL_FLASHEx_DATAEEPROM_Lock();	
	sys.sht_seq++;	
}

void shtDataClear(void)
{	
	sys.sht_seq=0;
	HAL_FLASHEx_DATAEEPROM_Unlock();
	for(uint8_t i=0;i<32;i++)
	{
		HAL_FLASHEx_DATAEEPROM_Erase(EEPROM_SHT_START_ADD + i * 0x04);
		HAL_FLASHEx_DATAEEPROM_Erase(EEPROM_TIME_START_ADD+ i * 0x04);		
		HAL_FLASHEx_DATAEEPROM_Erase(EEPROM_D1_AD0_START_ADD+ i * 0x04);	
		HAL_FLASHEx_DATAEEPROM_Erase(EEPROM_D2D3_START_ADD+ i * 0x04);	
		HAL_FLASHEx_DATAEEPROM_Erase(EEPROM_DISTANCE_START_ADD+ i * 0x04);	
		HAL_FLASHEx_DATAEEPROM_Erase(EEPROM_AD1_AD4_START_ADD+ i * 0x04);	
		HAL_FLASHEx_DATAEEPROM_Erase(EEPROM_COUNT_START_ADD+ i * 0x04);	
		HAL_FLASHEx_DATAEEPROM_Erase(EEPROM_WEIGHT_START_ADD+ i * 0x04);	
	}
	HAL_FLASHEx_DATAEEPROM_Lock();
}

void shtDataPrint(void)
{	
	uint8_t num = sys.sht_seq;
	for(uint8_t i=0;i<32;i++)
	{
		if(num>=32)
			num = 0;
		uint32_t r_time=*(__IO uint32_t *)(EEPROM_TIME_START_ADD+num*0x04);
		
		if(r_time!=0)
		{		
			int16_t tem,hum,d1,d2,d3;
			uint16_t ad0,ad1,ad4,distance;
			int32_t weight;
			uint32_t count;
			if(sys.mod ==model1)
			{
			uint32_t r_sht_data=*(__IO uint32_t *)(EEPROM_SHT_START_ADD+num*0x04);
			uint32_t r_d1_ad0_data=*(__IO uint32_t *)(EEPROM_D1_AD0_START_ADD+num*0x04);
			user_main_debug("r_sht_data:%x",r_sht_data);
			user_main_debug("r_d1_ad0_data:%x",r_d1_ad0_data);
			tem = ((r_sht_data>>16)&0xFFFF);
			hum = (r_sht_data&0xFFFF);
			ad0 = ((r_d1_ad0_data>>16)&0xFFFF);
			d1 = (r_d1_ad0_data&0xFFFF);	
				printf("mod1: tem:%.2f hum:%.2f ad1:%d ds18b20_1:%.1f",(float)tem/10.0,(float)hum/10.0,ad0,(float)d1/10.0);
			GetTime(r_time);	
      }
				else if(sys.mod ==model2)
			{
			uint32_t r_dis_data=*(__IO uint32_t *)(EEPROM_DISTANCE_START_ADD+num*0x04);
			uint32_t r_d1_ad0_data=*(__IO uint32_t *)(EEPROM_D1_AD0_START_ADD+num*0x04);
			user_main_debug("r_dis_data:%x",r_dis_data);
			user_main_debug("r_d1_ad0_data:%x",r_d1_ad0_data);
			distance = (r_dis_data&0xFFFF);
			ad0 = ((r_d1_ad0_data>>16)&0xFFFF);
			d1 = (r_d1_ad0_data&0xFFFF);	
				printf("mod2: distance:%d ad1:%d ds18b20_1:%.1f",distance,ad0,(float)d1/10.0);
			GetTime(r_time);	
      }
				else if(sys.mod ==model3)
			{
			uint32_t r_sht_data=*(__IO uint32_t *)(EEPROM_SHT_START_ADD+num*0x04);
			uint32_t r_d1_ad0_data=*(__IO uint32_t *)(EEPROM_D1_AD0_START_ADD+num*0x04);
			uint32_t r_ad1_ad4_data=*(__IO uint32_t *)(EEPROM_AD1_AD4_START_ADD+num*0x04);
			user_main_debug("r_sht_data:%x",r_sht_data);
			user_main_debug("r_d1_ad0_data:%x",r_d1_ad0_data);
			user_main_debug("r_ad1_ad4_data:%x",r_ad1_ad4_data);
			tem = ((r_sht_data>>16)&0xFFFF);
			hum = (r_sht_data&0xFFFF);
			ad0 = ((r_d1_ad0_data>>16)&0xFFFF);
      ad1 = ((r_ad1_ad4_data>>16)&0xFFFF);
			ad4 = (r_ad1_ad4_data&0xFFFF);
			printf("mod3: tem:%.2f hum:%.2f ad1:%d ad2:%d ad3:%d",(float)tem/10.0,(float)hum/10.0,ad0,ad1,ad4);
			GetTime(r_time);	
      }
				else if(sys.mod ==model4)
			{
			uint32_t r_d1_ad0_data=*(__IO uint32_t *)(EEPROM_D1_AD0_START_ADD+num*0x04);
			uint32_t r_d2d3_data=*(__IO uint32_t *)(EEPROM_D2D3_START_ADD+num*0x04);
			user_main_debug("r_d1_ad0_data:%x",r_d1_ad0_data);
			user_main_debug("r_d2d3_data:%x",r_d2d3_data);
			ad0 = ((r_d1_ad0_data>>16)&0xFFFF);
			d1 = (r_d1_ad0_data&0xFFFF);	
      d2 = ((r_d2d3_data>>16)&0xFFFF);
			d3 = (r_d2d3_data&0xFFFF);
			printf("mod4:ad1:%d ds18b20_1:%.1f ds18b20_2:%.1f ds18b20_3:%.1f",ad0,(float)d1/10.0,(float)d2/10.0,(float)d3/10.0);
			GetTime(r_time);	
      }
				else if(sys.mod ==model5)
			{
			uint32_t r_d1_ad0_data=*(__IO uint32_t *)(EEPROM_D1_AD0_START_ADD+num*0x04);
			uint32_t r_weight_data=*(__IO uint32_t *)(EEPROM_WEIGHT_START_ADD+num*0x04);
			user_main_debug("r_d1_ad0_data:%x",r_d1_ad0_data);
			user_main_debug("r_weight_data:%x",r_weight_data);
			ad0 = ((r_d1_ad0_data>>16)&0xFFFF);
			d1 = (r_d1_ad0_data&0xFFFF);	
			weight = r_weight_data;
			printf("mod5:ad1:%d ds18b20_1:%.1f weight:%d",ad0,(float)d1/10.0,weight);
			GetTime(r_time);	
      }	
				else if(sys.mod ==model6)
			{
			uint32_t r_count_data=*(__IO uint32_t *)(EEPROM_COUNT_START_ADD+num*0x04);
			user_main_debug("r_count_data:%x",r_count_data);
			count = r_count_data;
			printf("mod6:count:%d",count);
			GetTime(r_time);	
      }			
		}
		num++;
	}
}
void DatalogPrint(void)
{
	uint32_t add;
for(uint8_t num=0;num<20;num++)
{
	add= FLASH_USER_START_DATALOG+num*FLASH_PAGE_SIZE*4;
	memset(record_log,0,sizeof(record_log));
	for(uint16_t i=0,j=0;i<128;i++,j=j+4)
	{
		uint32_t temp  = FLASH_read(add+i*4);
		record_log[j] 	 = (temp>>24) & 0x000000FF;
		record_log[j+1] 	 = (temp>>16) & 0x000000FF;
		record_log[j+2] 	 = (temp>>8)  & 0x000000FF;
		record_log[j+3] 		 = (temp)     & 0x000000FF;
	}
	printf("%s",record_log);	
}
}

void DatalogClear(void)
{
	FLASH_erase(FLASH_USER_START_DATALOG,(FLASH_USER_END_DATALOG - FLASH_USER_START_DATALOG) / FLASH_PAGE_SIZE);	
}

void get_sensorvalue(void)
{
		HAL_GPIO_WritePin(Power_5v_GPIO_Port, Power_5v_Pin, GPIO_PIN_RESET);	
	  HAL_Delay(500+sys.power_time);
			if((sys.mod==model1)||(sys.mod==model3))
  {
		MX_I2C1_Init();
    if(detect_flags == 1)
			sht20Data();
		else if(detect_flags == 2)
			sht31Data();
    HAL_I2C_MspDeInit(&hi2c1);	
		HAL_Delay(20);
	}
			if((sys.mod!=model6))
  {
		adc0_datalog = ADCModel(ADC_CHANNEL_4);
		if((sys.mod!=model3))
		{
		 DS18B20_GetTemp_SkipRom(1);
		 DS18B20_IoDeInit(1);
		}
	}
			if(sys.mod==model2)
  {
		 if(mode2_flag==1)
		{
			distance_datalog = LidarLite();
			HAL_I2C_MspDeInit(&hi2c1);	
		}
		else if(mode2_flag==2)
		{
     distance_datalog = ULT_distance();
		 GPIO_ULT_INPUT_DeInit();
		 GPIO_ULT_OUTPUT_DeInit();
		}
    else if(mode2_flag==3)
		{	
		 ULT_Rest();
     MX_USART1_UART_Init	();		
     uart1_Init();
		 HAL_UART_Receive_IT(&huart1,(uint8_t*)&rxbuf_u1,RXSIZE);
	   HAL_Delay(100);
		 ULT_getData();
		 uart1_IoDeInit();
		 distance_datalog = ULT_Data_processing();	
		}
		else
		{
			distance_datalog = 0;			
		}
	}
			if(sys.mod==model3)
  {
	  adc1_datalog = ADCModel(ADC_CHANNEL_1);
		adc4_datalog = ADCModel(ADC_CHANNEL_0);
	}
			if(sys.mod==model4)
  {	
		DS18B20_GetTemp_SkipRom(2);
		DS18B20_IoDeInit(2);
		DS18B20_GetTemp_SkipRom(3);
		DS18B20_IoDeInit(3);
	}
	if(sys.mod == model5)
	{
			  WEIGHT_SCK_Init();
	  WEIGHT_DOUT_Init();
		int32_t Weight = Get_Weight();	
	  WEIGHT_SCK_DeInit();
	  WEIGHT_DOUT_DeInit();			
	}		
  HAL_GPIO_WritePin(Power_5v_GPIO_Port, Power_5v_Pin, GPIO_PIN_SET);
}
