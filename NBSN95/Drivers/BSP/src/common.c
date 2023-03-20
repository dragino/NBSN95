#include "common.h"
#include "nbInit.h"

static uint8_t sys_pwd[10]={0};
static char sensor_raw_data[60]={0};
static char sensor_data[500]={0};

extern uint8_t rxbuf_u1;
SYSTEM sys    = {.pwd=sys_pwd};
SENSOR sensor ={.data=sensor_data};
USER user={0};

static uint8_t mod5_init_flag = 0;

static uint8_t 		link_num = 0;
static LinkedList link_first_address;

static char cacheData1[40]={0};
static char cacheData2[40]={0};
static char cacheData3[40]={0};
static char cacheData4[40]={0};
static char cacheData5[40]={0};
static char cacheData6[40]={0};
static char cacheData7[40]={0};
static char cacheData8[40]={0};
static char cacheData9[40]={0};
static char cacheData10[40]={0};
static char* cacheData0[10]={(char*)cacheData1,(char*)cacheData2,(char*)cacheData3,(char*)cacheData4,(char*)cacheData5,
												(char*)cacheData6,(char*)cacheData7,(char*)cacheData8,(char*)cacheData9,(char*)cacheData10};
static uint32_t cacheData_parameters[32]={0};
extern uint8_t 	bc35tobc95_flags;
void product_information_print(void)
{
	printf("\r\nDRAGINO NBSN95 NB-IoT Sensor Node\r\n"
										"Image Version: "AT_VERSION_STRING "\r\n"
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
	
	if(state == 0)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		GPIO_InitStruct.Pin = GPIO_PIN_14;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = GPIO_PIN_12;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
	else if(state == 1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		GPIO_InitStruct.Pin = GPIO_PIN_14;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = GPIO_PIN_12;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);		
	}
	else if(state == 2)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		GPIO_InitStruct.Pin = GPIO_PIN_14;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = GPIO_PIN_12;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);		
	}
	else if(state == 3)
  {
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		GPIO_InitStruct.Pin = GPIO_PIN_14;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = GPIO_PIN_12;
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
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
	HAL_Delay(time);

	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
	HAL_Delay(100);
}

uint8_t i2c_device_detection(void)
{	 
	if(sht2x_Detect() == 1)
	{
		user_main_printf("Use Sensor is SHT2x");
		return 1;
	}
	
	if(sht3x_Detect() == 1)
	{		 
		user_main_printf("Use Sensor is SHT3x");
		return 2;
	}
	sensor.humSHT = 0;
	sensor.temSHT = 0;
	user_main_printf("No I2C device detected");
	return 0;
}

void txPayLoadDeal(SENSOR* Sensor,LinkedList L)
{	
	memset(Sensor->data,0,sizeof((char*)Sensor->data));
	memset(sensor_raw_data,0,sizeof(sensor_raw_data));
	Sensor->data_len = 0;
	Sensor->singal = nb.singal;
	
	HAL_GPIO_WritePin(Power_5v_GPIO_Port, Power_5v_Pin, GPIO_PIN_RESET);
	HAL_Delay(500+sys.power_time);
	
	Sensor->batteryLevel_mV = getVoltage();
	sprintf(Sensor->data+strlen(Sensor->data), "%c", 'f');
	for(int i=0;i<strlen((char*)user.deui);i++)
		sprintf(Sensor->data+strlen(Sensor->data), "%c",  user.deui[i]);
	if(sys.mod == model3)
		sprintf(Sensor->data+strlen(Sensor->data), "%.2x",  string_touint());
	else
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x",  string_touint());
	
	sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->batteryLevel_mV);
	sprintf(Sensor->data+strlen(Sensor->data), "%.2x", Sensor->singal);
	sprintf(Sensor->data+strlen(Sensor->data), "%.2x", sys.mod-0x30);
	
	if(sys.mod == model1)
	{
		Sensor->temDs18b20_1 = (int)(DS18B20_GetTemp_SkipRom(1)*10);
		Sensor->adc0 = ADCModel(ADC_CHANNEL_0);
		
		uint8_t i2c_device_flag = i2c_device_detection();
		if(i2c_device_flag == 1)
			sht20Data();
		else if(i2c_device_flag == 2)
			sht31Data();

		sprintf(Sensor->data+strlen(Sensor->data), "%c", (Sensor->temDs18b20_1>=0)?'0':'F');
		sprintf(Sensor->data+strlen(Sensor->data), "%.3x",(Sensor->temDs18b20_1>=0)?Sensor->temDs18b20_1:Sensor->temDs18b20_1*(-1));		
		if(sys.inmod =='0')
			sprintf(Sensor->data+strlen(Sensor->data), "%.2x", HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12));
		else
			sprintf(Sensor->data+strlen(Sensor->data), "%.2x", Sensor->exit_state);
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
		if(sensor.distance == 4095)
		{
		sensor.distance = 0;	
		ULT_Rest();
    MX_USART1_UART_Init	();		
    uart1_Init();
		HAL_UART_Receive_IT(&huart1,(uint8_t*)&rxbuf_u1,RXSIZE);
	  HAL_Delay(100);
		ULT_getData();
		uart1_IoDeInit();
		Sensor->distance = ULT_Data_processing();	
			if(sensor.distance == 0)
			{
				 MAXSONAR_Rest();
			   MX_USART1_UART_Init	();	
         uart1_Init();	
         MAXSONAR_getData();
		     uart1_IoDeInit();		
		     Sensor->distance = MAXSONAR_Data_processing();					
			}
		}
		}
		
		sprintf(Sensor->data+strlen(Sensor->data), "%c", (Sensor->temDs18b20_1>=0)?'0':'F');
		sprintf(Sensor->data+strlen(Sensor->data), "%.3x",(Sensor->temDs18b20_1>=0)?Sensor->temDs18b20_1:Sensor->temDs18b20_1*(-1));		
		if(sys.inmod =='0')
			sprintf(Sensor->data+strlen(Sensor->data), "%.2x", HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12));
		else
			sprintf(Sensor->data+strlen(Sensor->data), "%.2x", Sensor->exit_state);
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->adc0);		
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->distance);
	}
	else if(sys.mod == model3)
	{
		Sensor->adc0 = ADCModel(ADC_CHANNEL_0);
		Sensor->adc1 = ADCModel(ADC_CHANNEL_1);
		Sensor->adc4 = ADCModel(ADC_CHANNEL_4);
	
		uint8_t i2c_state = i2c_device_detection();
		if(i2c_state == 1)
			sht20Data();
		else if(i2c_state == 2)
			sht31Data();	
		
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->adc0);
		if(sys.inmod =='0')
			sprintf(Sensor->data+strlen(Sensor->data), "%.2x", HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12));
		else
			sprintf(Sensor->data+strlen(Sensor->data), "%.2x", Sensor->exit_state);
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
		if(sys.inmod =='0')
			sprintf(Sensor->data+strlen(Sensor->data), "%.2x", HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12));
		else
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
		Sensor->adc0 = ADCModel(ADC_CHANNEL_0);
	  WEIGHT_SCK_Init();
	  WEIGHT_DOUT_Init();
		int32_t Weight = Get_Weight();	
	  WEIGHT_SCK_DeInit();
	  WEIGHT_DOUT_DeInit();		
		user_main_printf("Weight is %d g",Weight);
		
		sprintf(Sensor->data+strlen(Sensor->data), "%c", (Sensor->temDs18b20_1>=0)?'0':'F');
		sprintf(Sensor->data+strlen(Sensor->data), "%.3x",(Sensor->temDs18b20_1>=0)?Sensor->temDs18b20_1:Sensor->temDs18b20_1*(-1));		
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Sensor->adc0);
		if(sys.inmod =='0')
			sprintf(Sensor->data+strlen(Sensor->data), "%.2x", HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12));
		else
			sprintf(Sensor->data+strlen(Sensor->data), "%.2x", Sensor->exit_state);
		sprintf(Sensor->data+strlen(Sensor->data), "%.4x", Weight);
	}
	else if(sys.mod == model6)
	{
		sprintf(Sensor->data+strlen(Sensor->data), "%.8x", sensor.exit_count);
	}
	
	sprintf(Sensor->data+strlen(Sensor->data), "%.8x", sensor.time_stamp);
//	sprintf(Sensor->data+strlen(Sensor->data), "%.4x", sys.uplink_count);   //count packet
	memcpy(sensor_raw_data,&Sensor->data[16],strlen(Sensor->data)-16);

	if(sys.list_flag ==1 && sys.cum_flag == '1')	
	{
		LinkedList L0 = L;
		for(int i=0;i<link_num;i++)
		{
			if(L0->Timestamp != 0 && L0->data_valid_flag != 0)
			{
				strcat(Sensor->data,L0->data);
				L0->data_valid_flag = invalid;
			}
			L0 = L0->next;
		}
	}

	Sensor->data_len = strlen(Sensor->data)/2;
		if(sys.protocol == COAP_PRO&&bc35tobc95_flags==0)
		{
		Sensor->data[strlen(Sensor->data)]=0x1A;
		Sensor->data_len = Sensor->data_len*2+1;
		}
		
		if(sys.protocol == MQTT_PRO)
		{
		Sensor->data[strlen(Sensor->data)]=0x1A;
		Sensor->data_len = Sensor->data_len*2+1;
		}
	
	user_main_debug("Sensor->data:%s",Sensor->data);
	user_main_debug("Sensor->data_len:%d",Sensor->data_len);
	Sensor->exit_state = 0;
	sys.exit_flag = 0;
	HAL_GPIO_WritePin(Power_5v_GPIO_Port, Power_5v_Pin, GPIO_PIN_SET);
}

void txPayLoadDeal2(SENSOR* Sensor)
{	
	memset(Sensor->data,0,sizeof((char*)Sensor->data));
	memset(sensor_raw_data,0,sizeof(sensor_raw_data));

	HAL_GPIO_WritePin(Power_5v_GPIO_Port, Power_5v_Pin, GPIO_PIN_RESET);
	HAL_Delay(500+sys.power_time);
	
	Sensor->batteryLevel_mV = getVoltage();

	if(sys.mod == model1)
	{
		Sensor->temDs18b20_1 = (int)(DS18B20_GetTemp_SkipRom(1)*10);
		Sensor->adc0 = ADCModel(ADC_CHANNEL_0);
		
		uint8_t i2c_device_flag = i2c_device_detection();
		if(i2c_device_flag == 1)
			sht20Data();
		else if(i2c_device_flag == 2)
			sht31Data();
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
		if(sensor.distance == 4095)
		{
		sensor.distance = 0;			
    ULT_Rest();
    MX_USART1_UART_Init	();		
    uart1_Init();
		HAL_UART_Receive_IT(&huart1,(uint8_t*)&rxbuf_u1,RXSIZE);
	  HAL_Delay(100);
		ULT_getData();
		uart1_IoDeInit();
    ULT_Data_processing();
		}
		}
	}
	else if(sys.mod == model3)
	{
		Sensor->adc0 = ADCModel(ADC_CHANNEL_0);
		Sensor->adc1 = ADCModel(ADC_CHANNEL_1);
		Sensor->adc4 = ADCModel(ADC_CHANNEL_4);
	
		uint8_t i2c_state = i2c_device_detection();
		if(i2c_state == 1)
			sht20Data();
		else if(i2c_state == 2)
			sht31Data();	
	}
	else if(sys.mod == model4)
	{
		Sensor->adc0 = ADCModel(ADC_CHANNEL_0);
		Sensor->temDs18b20_1 = DS18B20_GetTemp_SkipRom(1)*10;DS18B20_IoDeInit(1);
		Sensor->temDs18b20_2 = DS18B20_GetTemp_SkipRom(2)*10;DS18B20_IoDeInit(2);
		Sensor->temDs18b20_3 = DS18B20_GetTemp_SkipRom(3)*10;DS18B20_IoDeInit(3);	
	}
	else if(sys.mod == model5)
	{
		Sensor->temDs18b20_1 = DS18B20_GetTemp_SkipRom(1)*10;
		Sensor->adc0 = ADCModel(ADC_CHANNEL_0);
	  WEIGHT_SCK_Init();
	  WEIGHT_DOUT_Init();
		int32_t Weight = Get_Weight();	
	  WEIGHT_SCK_DeInit();
	  WEIGHT_DOUT_DeInit();			
		user_main_printf("Weight is %d g",Weight);
	}
	else if(sys.mod == model6)
	{
	user_main_printf("count is %d ",sensor.exit_count);
	}
	HAL_GPIO_WritePin(Power_5v_GPIO_Port, Power_5v_Pin, GPIO_PIN_SET);
}


/**
	* @brief  Processing received data 
  * @param  Instruction parameter
  * @retval None
  */
void rxPayLoadDeal(char* payload)
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
	uint16_t update_flags,r_config;
	r_config=*(__IO uint16_t *)EEPROM_USER_START_ADD;	
  if(r_config!=string_touint())
	{
		update_flags = string_touint();
		HAL_FLASHEx_DATAEEPROM_Unlock();
		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_HALFWORD,EEPROM_USER_START_ADD,update_flags);//store hardversion
		HAL_FLASHEx_DATAEEPROM_Lock();
		at_fdr_run(NULL);
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
//Create data storage linked list 
LinkedList List_Init(LinkedList L)
{
	link_first_address= createList();
	sys.list = link_first_address;
	upLink_fail_read(sys.list);
	sys.list = sortList(sys.list);
	user_main_debug("sys.list_num:%d",sys.list->num);
	user_main_debug("Number of linked lists:%d",link_num);
	return L;
}
//Create data storage linked list 
LinkedList createList(void)
{
	LinkedList L0,L1;
	L0 = (Node *)malloc(sizeof(Node));
	if(L0 != NULL)
		link_num++;
	else
		return L0;
	
	L0->Timestamp = 0;
	L0->num=0;
	L0->data_valid_flag = invalid;
	L0->data = cacheData0[0];

	L1 = (Node *)malloc(sizeof(Node));
	if(L1 != NULL)
		link_num++;
	else
		return L0;
	
	L1->Timestamp = 0;
	L1->num=1;
	L1->data_valid_flag = invalid;
	L1->data = cacheData0[1];

	L0->next = L1;

	for (int i = 0; i<8; i++)
	{
		LinkedList L2;
		L2 = (Node *)malloc(sizeof(Node));
		if(L2 != NULL)
			link_num++;
		else
		{
			L1->next = L0;
			return L0;
		}
		
		L2->Timestamp = 0;
		L2->data = cacheData0[i+2];
		L2->num=2+i;
		L2->data_valid_flag = invalid;
		L2->next = L0;
		
		L1->next = L2;
		L1 = L2;
	}
	L1->next = L0;
	return L0;
}
//Print linked list
LinkedList printList(LinkedList L)
{
	LinkedList L0 = L;
	uint8_t n=0;
	for(uint8_t i=0;i<link_num;i++)
	{
		if(L0->Timestamp != 0)
			printf("%d:%s\r\n", n++,L0->data);
		L0 = L0->next;
	}
	return L;
}
//List sort
LinkedList sortList(LinkedList L)
{
	LinkedList L0 = L;
	for(uint8_t i=0;i<link_num;i++)
	{
		if(L0->Timestamp == 0)
			return L0;
		else
			L0 = L0->next;
	}

	L0 = L;
	uint32_t m = L0->Timestamp;
	for(uint8_t i=0;i<link_num;i++)
	{
		if(m > L0->Timestamp)
		{
			m = L0->Timestamp;
			L = L0;
		}
		L0 = L0->next;
	}
	
	return L;
}
//Clear cached data 
LinkedList upLink_flash_clear(LinkedList L)
{
	for(int i=0;i<link_num;i++)
	{
		L->Timestamp = 0;
		L->data_valid_flag = invalid;
		memset(L->data , 0,strlen(L->data));
		L=L->next;
	}
	FLASH_erase(FLASH_USER_START_CACHE_Data1,4);
	sys.list = link_first_address;
	return L;
}
//Process the buffered data to be sent 
LinkedList txCachePayLoadDeal(SENSOR* Sensor,LinkedList L)
{
	memset(Sensor->data,0,sizeof((char*)Sensor->data));
	Sensor->data_len = 0;
	for(int i=0;i<strlen((char*)user.deui);i++)
		sprintf(Sensor->data+strlen(Sensor->data), "%c",  user.deui[i]);
	strcat(Sensor->data,L->data);
	Sensor->data_len = strlen(Sensor->data)/2;
	if(sys.protocol == MQTT_PRO || sys.protocol == COAP_PRO)
	{
		Sensor->data[strlen(Sensor->data)]=0x1A;
		Sensor->data_len = Sensor->data_len*2+1;
	}
	user_main_debug("Sensor->data:%s",Sensor->data);
	user_main_debug("Sensor->data_len:%d",Sensor->data_len);
	
	L = L->next;
	return L;
}
//Write cache data to flash 
LinkedList upLink_fail_write(LinkedList L)
{	
	memset(L->data,0,sizeof((char*)L->data));
	L->Timestamp = sensor.time_stamp;
	L->data_valid_flag = effective ;
	memcpy(L->data,sensor_raw_data,strlen(sensor_raw_data));
	user_main_debug("L->Timestamp:%d",L->Timestamp);
	user_main_debug("L->data:%s",L->data);
	if(L->num/3==0)
	{
		FLASH_erase(FLASH_USER_START_CACHE_Data1,1);
		memset(cacheData_parameters,0,sizeof(cacheData_parameters));
		for(uint8_t i=0,j=0;i<strlen(cacheData1);i=i+4,j++)
				cacheData_parameters[j]= cacheData1[i+0]<<24 | cacheData1[i+1]<<16 | cacheData1[i+2]<<8 | cacheData1[i+3];
		
		for(uint8_t i=0,j=0;i<strlen(cacheData2);i=i+4,j++)
				cacheData_parameters[j+11]= cacheData2[i+0]<<24 | cacheData2[i+1]<<16 | cacheData2[i+2]<<8 | cacheData2[i+3];

		for(uint8_t i=0,j=0;i<strlen(cacheData3);i=i+4,j++)
				cacheData_parameters[j+22]= cacheData3[i+0]<<24 | cacheData3[i+1]<<16 | cacheData3[i+2]<<8 | cacheData3[i+3];
		FLASH_program(FLASH_USER_START_CACHE_Data1,cacheData_parameters, 32);
	}
	else if(L->num/3==1)
	{
		FLASH_erase(FLASH_USER_START_CACHE_Data4,1);
		memset(cacheData_parameters,0,sizeof(cacheData_parameters));
		for(uint8_t i=0,j=0;i<strlen(cacheData4);i=i+4,j++)
				cacheData_parameters[j]= cacheData4[i+0]<<24 | cacheData4[i+1]<<16 | cacheData4[i+2]<<8 | cacheData4[i+3];

		for(uint8_t i=0,j=0;i<strlen(cacheData5);i=i+4,j++)
				cacheData_parameters[j+11]= cacheData5[i+0]<<24 | cacheData5[i+1]<<16 | cacheData5[i+2]<<8 | cacheData5[i+3];

		for(uint8_t i=0,j=0;i<strlen(cacheData6);i=i+4,j++)
				cacheData_parameters[j+22]= cacheData6[i+0]<<24 | cacheData6[i+1]<<16 | cacheData6[i+2]<<8 | cacheData6[i+3];
		FLASH_program(FLASH_USER_START_CACHE_Data4,cacheData_parameters, 32);
	}
	else if(L->num/3==2)
	{
		FLASH_erase(FLASH_USER_START_CACHE_Data7,1);
		memset(cacheData_parameters,0,sizeof(cacheData_parameters));
		for(uint8_t i=0,j=0;i<strlen(cacheData7);i=i+4,j++)
				cacheData_parameters[j]= cacheData7[i+0]<<24 | cacheData7[i+1]<<16 | cacheData7[i+2]<<8 | cacheData7[i+3];

		for(uint8_t i=0,j=0;i<strlen(cacheData8);i=i+4,j++)
				cacheData_parameters[j+11]= cacheData8[i+0]<<24 | cacheData8[i+1]<<16 | cacheData8[i+2]<<8 | cacheData8[i+3];

		for(uint8_t i=0,j=0;i<strlen(cacheData9);i=i+4,j++)
				cacheData_parameters[j+22]= cacheData9[i+0]<<24 | cacheData9[i+1]<<16 | cacheData9[i+2]<<8 | cacheData9[i+3];
		FLASH_program(FLASH_USER_START_CACHE_Data7,cacheData_parameters, 32);
	}
	else if(L->num/3==3)
	{
		FLASH_erase(FLASH_USER_START_CACHE_Data10,1);
		memset(cacheData_parameters,0,sizeof(cacheData_parameters));
		for(uint8_t i=0,j=0;i<strlen(cacheData10);i=i+4,j++)
				cacheData_parameters[j]= cacheData10[i+0]<<24 | cacheData10[i+1]<<16 | cacheData10[i+2]<<8 | cacheData10[i+3];
		FLASH_program(FLASH_USER_START_CACHE_Data10,cacheData_parameters, 10);
	}
	
	L = L->next;
	return L;
}
//Read cache data from flash 
LinkedList upLink_fail_read(LinkedList L)
{
	LinkedList L0 = L;
	char buff[10]={0};
	uint32_t add = 0;
	for(uint8_t n=0;n<link_num;n++)
	{
		if(n==0) 			add=FLASH_USER_START_CACHE_Data1;
		else if(n==1) add=FLASH_USER_START_CACHE_Data2;
		else if(n==2) add=FLASH_USER_START_CACHE_Data3;
		else if(n==3) add=FLASH_USER_START_CACHE_Data4;
		else if(n==4) add=FLASH_USER_START_CACHE_Data5;
		else if(n==5) add=FLASH_USER_START_CACHE_Data6;
		else if(n==6) add=FLASH_USER_START_CACHE_Data7;
		else if(n==7) add=FLASH_USER_START_CACHE_Data8;
		else if(n==8) add=FLASH_USER_START_CACHE_Data9;
		else if(n==9) add=FLASH_USER_START_CACHE_Data10;
		
		for(uint8_t i=0,j=0;i<10;i++,j=j+4)
		{
			uint32_t temp		= FLASH_read(add+i*4);		
			L0->data[j] 		= (temp>>24) & 0x000000FF;
			L0->data[j+1] 	= (temp>>16) & 0x000000FF;
			L0->data[j+2] 	= (temp>>8)  & 0x000000FF;
			L0->data[j+3] 	= (temp)     & 0x000000FF;
		}
		memset(buff,0,10);
		memcpy(buff,&L0->data[strlen(L0->data)-8],8);
		if(strlen(L0->data)!=0)
		{
			L0->Timestamp = hexToint(buff);
			L0->data_valid_flag = effective ;
			sys.list_flag = 1;
		}
		L0 = L0->next;
	}
	return L;
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