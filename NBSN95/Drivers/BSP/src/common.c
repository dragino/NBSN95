#include "common.h"

SYSTEM sys = {0};
SENSOR sensor={0};
USER user={0};

static uint8_t mod5_init_flag = 0;
static uint8_t i2c_device_flag=0;

void product_information_print(void)
{
	printf("\r\nDRAGINO NBSN95 NB-IoT Sensor Node\r\n"
										"Image Version: "version "\r\n"
										"NB-IoT Stack : "stack	 "\r\n"
	                  "Protocol in Used: ");
	if(sys.protocol == COAP_PRO)
		printf("COAP\r\n");
	else if(sys.protocol == UDP_PRO)
		printf("UDP\r\n");
	else if(sys.protocol == MQTT_PRO)
		printf("MQTT\r\n");
	else if(sys.protocol == TCP_PRO)
		printf("TCP\r\n");
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
	HAL_Delay(100);
}

void i2c_device_detection(void)
{
	uint8_t sum=0;
	uint8_t txdata1[1]={0xE7},txdata2[2]={0xF3,0x2D};
	i2c_device_flag = 0;
	sht20Init();
	while(HAL_I2C_Master_Transmit(&hi2c1,0x80,txdata1,1,1000) != HAL_OK)
	{
		 sum++;
		 if(sum>20)
		 {
			 i2c_device_flag=0;
			 break;
		 }
	 }
	 if(HAL_I2C_Master_Transmit(&hi2c1,0x80,txdata1,1,1000) == HAL_OK)
	 {
		 i2c_device_flag=1;
	   user_main_printf("Use Sensor is SHT20");
	 }
	 
	 if(i2c_device_flag==0)
	 {	 
		sum=0;		 			 
		sht31Init();	 
		while(HAL_I2C_Master_Transmit(&hi2c1,0x88,txdata2,2,1000) != HAL_OK) 
		{
		 sum++;
		 if(sum>20)
		 {
			 i2c_device_flag=0;
			 break;
		 }
		}
	  if(HAL_I2C_Master_Transmit(&hi2c1,0x88,txdata2,2,1000) == HAL_OK)
	  {
			 i2c_device_flag=2;
			 user_main_printf("Use Sensor is SHT31");
	  }
   }
	 if(i2c_device_flag==0)
	 {
//		 HAL_I2C_MspDeInit(&hi2c1);	 
		 user_main_printf("No I2C device detected");
	 }	
}

void txPayLoadDeal(SENSOR* Sensor)
{	
	memset(Sensor->data,0,sizeof(Sensor->data));
	Sensor->data_len = 0;
	Sensor->singal = nb.singal;
	
	HAL_GPIO_WritePin(Power_5v_GPIO_Port, Power_5v_Pin, GPIO_PIN_RESET);
	HAL_Delay(500+sys.power_time);
	
	Sensor->batteryLevel_mV = getVoltage();

	for(int i=0;i<strlen((char*)user.deui);i++)
		sprintf(Sensor->data+strlen(Sensor->data), "%c",  user.deui[i]);
	
	sprintf(Sensor->data+strlen(Sensor->data), "%.4x", atoi(version_s));
	sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->batteryLevel_mV);
	sprintf(Sensor->data+strlen(Sensor->data), "%.2x", Sensor->singal);
	sprintf(Sensor->data+strlen(Sensor->data), "%.2x", sys.mod-0x30);
	sprintf(Sensor->data+strlen(Sensor->data), "%.4x", sys.uplink_count);
	
	if(sys.mod == model1)
	{
		Sensor->temDs18b20_1 = (int)(DS18B20_GetTemp_SkipRom(1)*10);
		Sensor->adc0 = ADCModel(ADC_CHANNEL_0);
		
		i2c_device_detection();
		if(i2c_device_flag == 1)
			sht20Data();
		else if(i2c_device_flag == 2)
			sht31Data();
	
		sprintf(Sensor->data+strlen(Sensor->data), "%c", (Sensor->temDs18b20_1>=0)?'0':'F');
		sprintf(Sensor->data+strlen(Sensor->data), "%.3x",(Sensor->temDs18b20_1>=0)?Sensor->temDs18b20_1:Sensor->temDs18b20_1*(-1));		
		sprintf(Sensor->data+strlen(Sensor->data), "%.2x", Sensor->exit_flag);
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->adc0);
		sprintf(Sensor->data+strlen(Sensor->data), "%c", (Sensor->temSHT>=0)?'0':'F');
		sprintf(Sensor->data+strlen(Sensor->data), "%.3x", (Sensor->temSHT>=0)?Sensor->temSHT:Sensor->temSHT*(-1));
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->humSHT);
	}
	else if(sys.mod == model2)
	{
		Sensor->temDs18b20_1 = DS18B20_GetTemp_SkipRom(1)*10;
		Sensor->adc0 = ADCModel(ADC_CHANNEL_0);
		Sensor->distance = LidarLite();
		if(sensor.distance == 4095)
		{
			sensor.distance = 0;
			sensor.distance = ULT_distance();
			
			GPIO_ULT_INPUT_DeInit();
			GPIO_ULT_OUTPUT_DeInit();
		}
		
		sprintf(Sensor->data+strlen(Sensor->data), "%c", (Sensor->temDs18b20_1>=0)?'0':'F');
		sprintf(Sensor->data+strlen(Sensor->data), "%.3x",(Sensor->temDs18b20_1>=0)?Sensor->temDs18b20_1:Sensor->temDs18b20_1*(-1));		
		sprintf(Sensor->data+strlen(Sensor->data), "%.2x", Sensor->exit_flag);
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->adc0);		
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->distance);
	}
	else if(sys.mod == model3)
	{
		Sensor->adc0 = ADCModel(ADC_CHANNEL_0);
		Sensor->adc1 = ADCModel(ADC_CHANNEL_1);
		Sensor->adc4 = ADCModel(ADC_CHANNEL_4);
	
		i2c_device_detection();
		if(i2c_device_flag == 1)
			sht20Data();
		else if(i2c_device_flag == 2)
			sht31Data();	
		
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->adc0);
		sprintf(Sensor->data+strlen(Sensor->data), "%.2x", Sensor->exit_flag);
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->adc1);
		sprintf(Sensor->data+strlen(Sensor->data), "%c", (Sensor->temSHT>=0)?'0':'F');
		sprintf(Sensor->data+strlen(Sensor->data), "%.3x", (Sensor->temSHT>=0)?Sensor->temSHT:Sensor->temSHT*(-1));
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->humSHT);
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->adc4);			
	}
	else if(sys.mod == model4)
	{
		Sensor->adc0 = ADCModel(ADC_CHANNEL_0);
		Sensor->temDs18b20_1 = DS18B20_GetTemp_SkipRom(1)*10;DS18B20_IoDeInit(1);
		Sensor->temDs18b20_2 = DS18B20_GetTemp_SkipRom(2)*10;DS18B20_IoDeInit(2);
		Sensor->temDs18b20_3 = DS18B20_GetTemp_SkipRom(3)*10;DS18B20_IoDeInit(3);
				
		sprintf(Sensor->data+strlen(Sensor->data), "%c", (Sensor->temDs18b20_1>=0)?'0':'F');
		sprintf(Sensor->data+strlen(Sensor->data), "%.3x",(Sensor->temDs18b20_1>=0)?Sensor->temDs18b20_1:Sensor->temDs18b20_1*(-1));		
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->adc0);
		sprintf(Sensor->data+strlen(Sensor->data), "%.2x", Sensor->exit_flag);
		sprintf(Sensor->data+strlen(Sensor->data), "%c", (Sensor->temDs18b20_2>=0)?'0':'F');
		sprintf(Sensor->data+strlen(Sensor->data), "%.3x",(Sensor->temDs18b20_2>=0)?Sensor->temDs18b20_2:Sensor->temDs18b20_2*(-1));		
		sprintf(Sensor->data+strlen(Sensor->data), "%c", (Sensor->temDs18b20_3>=0)?'0':'F');
		sprintf(Sensor->data+strlen(Sensor->data), "%.3x",(Sensor->temDs18b20_3>=0)?Sensor->temDs18b20_3:Sensor->temDs18b20_3*(-1));		
	}
	else if(sys.mod == model5)
	{
		if(mod5_init_flag == 0)
		{
			at_weight_reset(NULL);
			mod5_init_flag =1;
		}
		
		Sensor->temDs18b20_1 = DS18B20_GetTemp_SkipRom(1)*10;
		Sensor->adc0 = ADCModel(ADC_CHANNEL_0);
		int32_t Weight = Get_Weight();		
		user_main_printf("Weight is %d g",Weight);
		
		sprintf(Sensor->data+strlen(Sensor->data), "%c", (Sensor->temDs18b20_1>=0)?'0':'F');
		sprintf(Sensor->data+strlen(Sensor->data), "%.3x",(Sensor->temDs18b20_1>=0)?Sensor->temDs18b20_1:Sensor->temDs18b20_1*(-1));		
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->adc0);
		sprintf(Sensor->data+strlen(Sensor->data), "%.2x", Sensor->exit_flag);
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Weight);
	}
	else if(sys.mod == model6)
	{
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", (int)(sensor.factor*100));
		sprintf(Sensor->data+strlen(Sensor->data), "%.8x", sensor.exit_count);
	}
	
	Sensor->data_len = strlen(Sensor->data)/2;	
	if(sys.protocol == MQTT_PRO || sys.protocol == COAP_PRO)
	{
		Sensor->data[strlen(Sensor->data)]=0x1A;
		Sensor->data_len = Sensor->data_len*2+1;
	}
	
	user_main_debug("Sensor->data:%s",Sensor->data);
	user_main_debug("Sensor->data_len:%d",Sensor->data_len);
	Sensor->exit_flag = 0;	
	HAL_GPIO_WritePin(Power_5v_GPIO_Port, Power_5v_Pin, GPIO_PIN_SET);
}

void rxPayLoadDeal(char* payload)
{
	char dataCom[10]={0};
	if(payload[0]=='0' && payload[1]=='1' &&	strlen(payload) == 8)
	{
		memcpy(dataCom,(payload)+2,6);
		int tdc = hexToint(dataCom);
		if(tdc <= 0xFFFFFF || tdc >= 30)
		{
			sys.tdc = tdc;
			sys.tdc = hexToint(dataCom);
			config_Set();
		}
	}
	else if(payload[0]=='0' && payload[1]=='4' &&	strlen(payload) == 4)
	{
		if(payload[2]=='F' && payload[3]=='F')
			NVIC_SystemReset();
	}
	else if(payload[0]=='0' && payload[1]=='6' &&	strlen(payload) == 8)
	{
		memcpy(dataCom,(payload)+2,6);
		uint8_t inmod = hexToint(dataCom);
		if(inmod >= '0' && inmod <= '3')
		{
			sys.inmod = inmod;
			EX_GPIO_Init(sys.inmod - 0x30);
			config_Set();
		}
	}
	else if(payload[0]=='0' && payload[1]=='7' &&	strlen(payload) == 8)
	{
		memcpy(dataCom,(payload)+2,6);
		sensor.exit_count = hexToint(dataCom);
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
