/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "iwdg.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "at.h"
#include "lowpower.h"
#include "nbInit.h"
#include "hw_rtc.h"
//#include "low_power_manager.h"
//#include "vcom.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* debug macro definition ----------------------------------------------------*/
//#define ST_DEBUG	

/* NBIOT task macro definition, the task in NBTASK will not be executed after commenting out -*/
#define NBIOT

#ifndef ST_DEBUG
	#define lowpower_enter
#endif
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#define ID1                 0x1FF80050
#define ID2                 0x1FF80054
#define ID3                 0x1FF80064

uint8_t  rxbuf = 0;				
static uint16_t rxlen = 0;
static uint8_t  rxDATA[300]={0};
static uint8_t  uart2_recieve_flag = 0;

static uint8_t rxbuf_lp[5] = {0};
static uint8_t lpuart_recieve_flag = 0;
uint8_t rxbuf_u1 = 0; 	//usart1
uint8_t rxDATA_u1[100]={0};
uint8_t rxlen_u1 = 0;
extern uint8_t getsensorvalue_flag;
extern uint8_t detect_flags;
extern int32_t Weight_Shiwu;
extern uint16_t 	adc0_datalog,adc1_datalog,adc4_datalog;
extern uint16_t distance_datalog;
extern uint8_t mode2_flag;
static uint8_t pwd_time_count = 0;			//Password time count times

static uint16_t dns_time_count = 0;			//DNS time count times

static uint8_t task_num = _AT_IDLE;			//NB task directory

uint8_t error_num = 0;				    //Error count
uint8_t press_button_times=0;//Press the button times in a row fast
uint8_t is_time_to_send=0;
static uint8_t dns_reset_num;
static uint8_t uplink_time_num = 0;
uint8_t nbmodel_int = 0;	
bool at_sleep_flag=0;
uint8_t OnPressButtonTimeout_status=0;
uint8_t join_network_num = 0;	
uint8_t PressButtonTimeoutTime = 0;	
uint8_t user_key_duration=0;
uint8_t join_network_flag = 0;	
uint8_t join_network_time = 0;
uint8_t join_network_timer = 0;
uint8_t nds_network_time = 0;
uint8_t nds_timer_flag = 0;
uint8_t nds_timer_flag2= 0;
uint8_t user_key_exti_flag=0;
uint8_t MCU_ID[8];
char MCU_pwd[20];
char MCU_pwd_tem[20];
__IO bool ble_sleep_flags=0;
__IO bool ble_sleep_command=0;
bool sleep_status=0;//AT+SLEEP
bool tdc_clock_log_flag=0;
/* USER CODE END PV */
TimerEvent_t TxTimer;
TimerEvent_t CheckBLETimesTimer;
TimerEvent_t PressButtonTimesLedTimer;
TimerEvent_t PressButtonTimeoutTimer;
TimerEvent_t nb_intTimeoutTimer;
TimerEvent_t timesampleTimer;
void LoraStartTx(void);
void OnTxTimerEvent( void );
void OnCheckBLETimesEvent(void);
void GPIO_BLE_STATUS_Ioinit(void);
void nb_intTimeoutEvent(void);
void OnPressButtonTimesLedEvent(void);
void rename_ble(void);
void LoraStartCheckBLE(void);
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void user_key_event(void);
void OntimesampleEvent(void);
void compare_time(uint16_t time);
/* USER CODE BEGIN PFP */
static void USERTASK(void);
void HW_GetUniqueId( uint8_t *id );
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_LPUART1_UART_Init();
#ifndef ST_DEBUG
  MX_IWDG_Init();
#endif
	HW_GetUniqueId(MCU_ID);
  HW_RTC_Init( );
	HW_RTC_SetTimerContext();
  MX_ADC_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	new_firmware_update();
	config_Get();
	GPIO_BLE_STATUS_Ioinit();
	rename_ble();
  LoraStartCheckBLE();	
	shtDataINIT();
	reboot_information_print();
	BSP_sensor_Init();
	product_information_print();
	led_on(1000);
	HAL_Delay(3000);
	HAL_UART_Receive_IT(&huart2,(uint8_t*)&rxbuf,RXSIZE);
	HAL_UART_Receive_IT(&hlpuart1,rxbuf_lp,RXSIZE);
	My_UARTEx_StopModeWakeUp(&huart2);				//Enable serial port wake up
	My_UARTEx_StopModeWakeUp(&hlpuart1);			//Enable serial port wake up
  /* USER CODE END 2 */
	
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
#ifdef NBIOT
	if((*(__IO uint8_t *)EEPROM_USER_START_FDR_FLAG) == 0x01)
	{
		task_num = _AT_FLAG_INIT;
		HAL_FLASHEx_DATAEEPROM_Unlock();
		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,EEPROM_USER_START_FDR_FLAG,0x00);
		HAL_FLASHEx_DATAEEPROM_Lock();
	}
	else
		task_num = _AT_FLAG_INIT;
#else
	task_num = _AT_IDLE;
#endif
	while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
#ifdef NBIOT		
		
		if(task_num != _AT_IDLE)
		{
			HAL_Delay(1000);
			
			if(NBTASK(&task_num) == _AT_ERROR)
			{
				error_num++;
			}
	  }
		if((error_num > 6 && nb.uplink_flag == no_status)||(error_num > 6))
		{
			user_main_printf("Restart the module...");
			task_num = _AT_QRST;
			error_num = 0;
		}
		if(dns_reset_num > 2 )
		{
		NVIC_SystemReset();	
		dns_reset_num=0;
		}
		if(/*nb.recieve_flag == NB_RECIEVE &&*/ nb.uplink_flag == send && task_num == _AT_IDLE && sleep_status==0)
		{
			task_num = _AT_URI;
			nb.recieve_flag = NB_IDIE;
		}
		if(getsensorvalue_flag == 1&& nb.uplink_flag == no_status && sleep_status==0)
		{
			task_num = _AT_QSCLKOFF;
			nb.uplink_flag = send;
			NBTASK(&task_num);
			getsensorvalue_flag = 0;
		}

	
			if(is_time_to_send==1 && nb.uplink_flag == no_status && sleep_status==0)
	{		
       if(nb.net_flag == no_status)
	   {
		   task_num = _AT_FLAG_INIT;
		   is_time_to_send=0;
	   }
	   else if(nb.net_flag == success )
	   {
		   task_num = _AT_QSCLKOFF;
		   nb.uplink_flag = send;
		   is_time_to_send=0;
	   }		
		
			LPM_DisableStopMode();	
	}
#endif
		
		USERTASK();
	
		 if(ble_sleep_command==1)
	 {			
			HAL_Delay(50);					
			printf("AT+PWRM2\r\n");
			HAL_Delay(100);										
			ble_sleep_command=0;
			ble_sleep_flags=1;
	}	
	 
		user_key_event();
	
     if(at_sleep_flag==1 && nb.uplink_flag == no_status)
		 {
			  sys.inmod= '0';
			 	EX_GPIO_Init(0);
			 	at_sleep_flag=0;
		   	sleep_status=1;
			  TimerStop(&CheckBLETimesTimer);
				TimerStop(&TxTimer);
			  TimerStop(&timesampleTimer);
			  task_num = _AT_CFUNOFF;	
			  NBTASK(&task_num);	
				
				HAL_Delay(500);
				TimerInit( &PressButtonTimesLedTimer, OnPressButtonTimesLedEvent );
				TimerSetValue( &PressButtonTimesLedTimer, 5000);
				HAL_GPIO_WritePin(LED_RGB_PORT,LED_RED_PIN,GPIO_PIN_SET); 
				TimerStart( &PressButtonTimesLedTimer );
				
				ble_sleep_command=0;
				ble_sleep_flags=1;	
        HAL_Delay(50);	 			
			  printf("AT+PWRM2\r\n");
				HAL_Delay(100);	 
				printf("SLEEP\r\n");
		 }	
	
#ifdef lowpower_enter
		if(task_num == _AT_IDLE && uart2_recieve_flag==0)
		{
			LPM_EnterStopMode(SystemClock_Config);
		}
#endif
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI
                              |RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_LPUART1
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_HSI;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_HSI;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_HSI;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
static void USERTASK(void)
{
	if(sys.pwd_flag==1 && sys.pwd_flag == 0 && pwd_time_count == 0)
	{
		HAL_UART_Transmit(&huart2,(uint8_t*)"Password timeout\r\n",20,20);
	}
	
	if(sys.pwd_flag == 0 && uart2_recieve_flag)
	{
		rtrim((char*)rxDATA);
		if(strcmp((char*)rxDATA,(char*)sys.pwd) == 0 && strlen((char*)rxDATA) < 9)
		{
			user_main_printf("Password Correct");
			sys.pwd_flag = 1;
		}
		else
		{
			user_main_printf("Password Incorrect");
		}
	}
	else if(sys.pwd_flag != 0 && uart2_recieve_flag)
	{
		ATEerror_t AT_State = ATInsPro((char*)rxDATA);
		if(AT_State == AT_OK)
			printf("%s\r\n",ATError_description[AT_OK]);
		else if(AT_State == AT_PARAM_ERROR)
			printf("%s",ATError_description[AT_PARAM_ERROR]);
		else if(AT_State == AT_BUSY_ERROR)
			printf("%s",ATError_description[AT_BUSY_ERROR]);
		else if(AT_State == AT_TEST_PARAM_OVERFLOW)
			printf("%s",ATError_description[AT_TEST_PARAM_OVERFLOW]);
		else if(AT_State == AT_RX_ERROR)
			printf("%s",ATError_description[AT_RX_ERROR]);
		else if(AT_State == AT_ERROR)
		{
			nb.usart.len = 0;
			memset(nb.usart.data,0,NB_RX_SIZE);
			rxDATA[strlen((char*)rxDATA)] = '\r';
			rxDATA[strlen((char*)rxDATA)] = '\n';
			HAL_UART_Transmit_DMA(&hlpuart1,(uint8_t*)rxDATA,strlen((char*)rxDATA));		
			HAL_Delay(1500);					//Waiting to Send
		}
	}
		
	if(uart2_recieve_flag == 1)
	{
		rxlen = 0;
		uart2_recieve_flag = 0;
		memset(rxDATA,0,sizeof(rxDATA));
	}
	if(lpuart_recieve_flag == 1)
	{
		printf("%s",nb.usart.data);
		nb.usart.len = 0;
		lpuart_recieve_flag = 0;
		memset(nb.usart.data,0,NB_RX_SIZE);
	}

	if(tdc_clock_log_flag==1 && sys.clock_switch==1 && nb.uplink_flag !=send && sleep_status==0)
	{
    get_sensorvalue();
		nb_cclk_run(NULL);
		memset((char*)nb.usart.data,0,sizeof(nb.usart.data));	
		shtDataWrite();
		tdc_clock_log_flag=0;
	}
	if(/*nb.recieve_flag == NB_RECIEVE &&*/ nb.dns_flag == running )
	{
		int dns_num=7;
		nb.recieve_flag = NB_IDIE;
		nb.dns_flag = no_status;
		while(dns_num--)
		{
	  HAL_IWDG_Refresh(&hiwdg);				
		HAL_Delay(2000);		
		if(NBTask[_AT_QDNS].get(NULL) == NB_CMD_SUCC)
		{
			if( nds_timer_flag2==0)
			{
			task_num = _AT_UPLOAD_START;
			}
			dns_reset_num=0;
			break;
		}
		else
		{
			if( nds_timer_flag2==0)
			{
			task_num = _AT_QRST;

			if(dns_num==0)
		  dns_reset_num++;
		  }
		}
	  }
		nds_timer_flag2=0;
	}	
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &hlpuart1)
	{
		nb.usart.data[nb.usart.len++] = rxbuf_lp[0];	
		if(task_num == _AT_IDLE)
		{
			if(rxbuf_lp[1] == '\r' && rxbuf_lp[0] == '\n')
			{
				lpuart_recieve_flag = 1;
			}
		}
		rxbuf_lp[1] = rxbuf_lp[0];
		HAL_UART_Receive_IT(&hlpuart1,rxbuf_lp,RXSIZE);		
	}
	else if(huart == &huart2)
	{
		rxDATA[rxlen++] = rxbuf;
		if(rxbuf == '\r' || rxbuf == '\n')
		{
			uart2_recieve_flag = 1;		
		}
		HAL_UART_Receive_IT(&huart2,(uint8_t*)&rxbuf,RXSIZE);
	}
	else if(huart == &huart1)
	{
		if(rxlen_u1 < 40)
			rxDATA_u1[rxlen_u1++] = rxbuf_u1;
		HAL_UART_Receive_IT(&huart1,(uint8_t*)&rxbuf_u1,RXSIZE);
	}
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	 TimerIrqHandler( );
}

void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc)
{
	HAL_IWDG_Refresh(&hiwdg);	
#ifdef NBIOT	
	if(sys.pwd_flag==1)
	{
		pwd_time_count++;
		if(pwd_time_count == 30)
		{
			sys.pwd_flag = 0;
			pwd_time_count = 0;
		}
	}	
#endif	

	if(sys.dns_timer==1&& sleep_status==0)
	{
		nds_network_time++;
		if(nds_network_time==(sys.dns_time*360))
		{
		 nds_network_time=0;
     nds_timer_flag=1;	
     nds_timer_flag2=1;			
		 task_num = _AT_QDNS;
		}
	}	
	
	if(nb.net_flag == fail && join_network_flag==1 && sleep_status==0)
	{
		task_num = _AT_CSQ;
    join_network_timer=1;
    join_network_flag=0;
	}

	if(nb.net_flag == success && nb.uplink_flag == send && sleep_status==0)
	{
		uplink_time_num++;
		if(uplink_time_num>=12)
		{
			uplink_time_num = 0;
			error_num++;
			if(sys.protocol == UDP_PRO)	{task_num	=	_AT_UDP_CLOSE;}
			else if(sys.protocol == MQTT_PRO)	{task_num	=	_AT_MQTT_CLOSE;}
			else if(sys.protocol == TCP_PRO)	{task_num	=	_AT_TCP_CLOSE;}
		}
	}
	else 
		uplink_time_num = 0;

	if(join_network_timer==1 && sleep_status==0)
	{
		join_network_time++;
		if(join_network_time==(sys.csq_time*6))
		{
		 join_network_time=0;
		join_network_timer=0;
		nb.net_flag = no_status;
    task_num	=	_AT_CFUNOFF;
		}
	}	
	My_AlarmInit(10,1);
}

void LoraStartCheckBLE(void)
{
  TimerInit( &CheckBLETimesTimer, OnCheckBLETimesEvent );
  TimerSetValue( &CheckBLETimesTimer,  60000); 
  TimerStart( &CheckBLETimesTimer);	
}

void OnCheckBLETimesEvent(void)
{
	if(HAL_GPIO_ReadPin(DX_BT24_STATUS_PORT,DX_BT24_LINK_PIN)==1)
	{
		TimerSetValue( &CheckBLETimesTimer,  60000); 
		TimerStart( &CheckBLETimesTimer);	
	}
	else
	{
		TimerStop( &CheckBLETimesTimer);	
		ble_sleep_command=1;
	}		
}

void OnPressButtonTimesLedEvent(void)
{
	TimerStop(&PressButtonTimesLedTimer);
	HAL_GPIO_WritePin(LED_RGB_PORT,LED_RED_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_RGB_PORT,LED_GREEN_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_RGB_PORT,LED_BLUE_PIN,GPIO_PIN_RESET);
}

void OnPressButtonTimeoutEvent(void)
{
	TimerStop(&PressButtonTimeoutTimer);
	OnPressButtonTimeout_status=0;
	press_button_times=0;
}

void nb_intTimeoutEvent(void)
{
	TimerStop(&nb_intTimeoutTimer);
	if(sleep_status==0)
	  task_num = _AT;
}

void OntimesampleEvent(void)
{
	TimerSetValue( &timesampleTimer, sys.tr_time * 60000); 
	TimerStart( &timesampleTimer);	
  tdc_clock_log_flag=1;
}

void GPIO_BLE_STATUS_Ioinit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	DX_BT24_STATUS_ENABLE();
  GPIO_InitStruct.Pin = DX_BT24_LINK_PIN|DX_BT24_WORK_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DX_BT24_STATUS_PORT, &GPIO_InitStruct);	
}

void rename_ble(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	DX_BT24_CLK_ENABLE();
  GPIO_InitStruct.Pin = DX_BT24_RST_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DX_BT24_PORT, &GPIO_InitStruct);	
  HAL_GPIO_WritePin(DX_BT24_PORT,DX_BT24_RST_PIN,GPIO_PIN_RESET);	
  HAL_Delay(300);	 
  HAL_GPIO_WritePin(DX_BT24_PORT,DX_BT24_RST_PIN,GPIO_PIN_SET);	
		 
  HAL_Delay(200);	
	
	printf("AT+NAME%s\r\n",user.deui);

	 HAL_Delay(50);
		
	 HAL_GPIO_WritePin(DX_BT24_PORT,DX_BT24_RST_PIN,GPIO_PIN_RESET);	
	 HAL_Delay(200);	 
	 HAL_GPIO_WritePin(DX_BT24_PORT,DX_BT24_RST_PIN,GPIO_PIN_SET);	
}

void OnTxTimerEvent( void )
{
	is_time_to_send=1;
}

void LoraStartTx(void)
{
    TimerInit( &TxTimer, OnTxTimerEvent );
    TimerSetValue( &TxTimer,  sys.tdc*1000); 
    OnTxTimerEvent();
}

void HW_GetUniqueId( uint8_t *id )
{
    id[7] = ( (*(uint32_t* )ID1 ) + (*(uint32_t* )ID3 )) >> 24;
    id[6] = ( (*(uint32_t* )ID1 ) + (*(uint32_t* )ID3 )) >> 16;
    id[5] = ( (*(uint32_t* )ID1 ) + (*(uint32_t* )ID3 )) >> 8;
    id[4] = ( (*(uint32_t* )ID1 ) + (*(uint32_t* )ID3 ));
    id[3] = ( (*(uint32_t* )ID2 )) >> 24;
    id[2] = ( (*(uint32_t* )ID2 )) >> 16;
    id[1] = ( (*(uint32_t* )ID2 )) >> 8;
    id[0] = ( (*(uint32_t* )ID2 ));
	
	for(int i=5;i<8;i++)
	{
	  sprintf(MCU_pwd_tem,"%02x",id[i]);
		strcat(MCU_pwd,MCU_pwd_tem);
	}
//	user_main_printf("%s",MCU_pwd);	
	
}

void user_key_event(void)
{
		if(user_key_exti_flag==1)
	{		
		user_key_exti_flag=0;
		
		if(OnPressButtonTimeout_status==0)
		{
			OnPressButtonTimeout_status=1;
			TimerInit( &PressButtonTimeoutTimer, OnPressButtonTimeoutEvent );
			TimerSetValue( &PressButtonTimeoutTimer, 5000);
			TimerStart( &PressButtonTimeoutTimer );
		}
		
		press_button_times++;

		HAL_GPIO_WritePin(LED_RGB_PORT, LED_GREEN_PIN, GPIO_PIN_SET);

		uint32_t currentTime = TimerGetCurrentTime();		
		
		while(HAL_GPIO_ReadPin(GPIO_USERKEY_PORT,GPIO_USERKEY_PIN)==GPIO_PIN_RESET)
		{				
			if(TimerGetElapsedTime(currentTime) >= 1000 && TimerGetElapsedTime(currentTime) < 3000)//send
			{			
			  user_key_duration=1;
			}			
			else if(TimerGetElapsedTime(currentTime) >= 3000)//system reset,Activation Mode
			{ 
        press_button_times=0;					
				for(int i=0;i<10;i++)
				{
					HAL_GPIO_TogglePin(LED_RGB_PORT,LED_GREEN_PIN);
					HAL_Delay(100);
				}
				user_key_duration=3;
				break;
			}			
    }		
			
		HAL_GPIO_WritePin(LED_RGB_PORT,LED_GREEN_PIN,GPIO_PIN_RESET);

		if(press_button_times==5)
		{	
			press_button_times=0;
			user_key_duration=2;
		}

		switch(user_key_duration)
		{
			case 1:
			{
				user_key_duration=0;

				  GPIO_InitTypeDef GPIO_InitStruct = {0};
          __HAL_RCC_GPIOA_CLK_ENABLE();
				  GPIO_InitStruct.Pin = GPIO_PIN_12;
          GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
          GPIO_InitStruct.Pull = GPIO_NOPULL;
          HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	      	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);					
			    HAL_Delay(200);	 
	      	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);				
			    TimerSetValue( &CheckBLETimesTimer,  60000); 
				  TimerStart( &CheckBLETimesTimer);
				  ble_sleep_flags=0;
				
				if(sleep_status==0 && nb.uplink_flag == no_status)
				{
					TimerStop(&TxTimer);
					TimerSetValue( &TxTimer,  sys.tdc*1000);
					TimerStart(&TxTimer);
					is_time_to_send=1;
				}
				break;
			}
			
			case 2://sleep
			{
			  sys.inmod= '0';
			 	EX_GPIO_Init(0);
				sleep_status=1;
			  TimerStop(&CheckBLETimesTimer);
				TimerStop(&TxTimer);
				TimerStop(&timesampleTimer);
			  task_num = _AT_CFUNOFF;	
			  NBTASK(&task_num);	
				
				
				HAL_Delay(500);
				TimerInit( &PressButtonTimesLedTimer, OnPressButtonTimesLedEvent );
				TimerSetValue( &PressButtonTimesLedTimer, 5000);
				HAL_GPIO_WritePin(LED_RGB_PORT,LED_RED_PIN,GPIO_PIN_SET); 
				TimerStart( &PressButtonTimesLedTimer );
				
				ble_sleep_command=0;
				ble_sleep_flags=1;	
        HAL_Delay(50);	 			
			  printf("AT+PWRM2\r\n");
				HAL_Delay(100);	 
				printf("SLEEP\r\n");				
								
				user_key_duration=0;					
				break;
			}
			
			case 3://system reset,Activation Mode
			{
				user_key_duration=0;
	      NVIC_SystemReset();	
				break;
			}
			
			default:
				break;
		}		
	}
		
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{	
	if(GPIO_Pin == GPIO_PIN_15)
	{		
		if(sys.mod == model6)
			sensor.exit_count++;
		else if(nb.net_flag == success && sys.mod != model6 && (task_num < _AT_UDP_OPEN || task_num > _AT_TCP_CLOSE))
		{
			if(nb.uplink_flag == no_status)
			{
			task_num = _AT_QSCLKOFF;
		  nb.uplink_flag = send;
			sys.exit_flag = 1;
			//sensor.exit_state = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)==1?1:2;
			sensor.exit_state = 1;
			LPM_DisableStopMode();	
			}
		}
	}
	else if(GPIO_Pin == GPIO_PIN_8)
	{		
		nb.recieve_flag = NB_RECIEVE;
	}
	else if(GPIO_Pin == GPIO_PIN_7)
	{		
	     user_key_exti_flag = 1;
	}

}
void compare_time(uint16_t time)
{
	uint16_t time_temp;
	if(time<=sys.strat_time)
	{
		time_temp=sys.strat_time-time;
	}
	else
	{
		time_temp=sys.strat_time+3600-time;
	}
	
	if(sys.strat_time==65535)
	{
		TimerSetValue( &timesampleTimer,  sys.tr_time*60000); 
		TimerStart( &timesampleTimer);		
	}
	else
	{
		TimerSetValue( &timesampleTimer,  time_temp*1000); 
		TimerStart( &timesampleTimer);
	}		
}
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
//	if(huart == (&hlpuart1))
//	{
//		user_main_error("hlpuart1 ERROR");
//		uint32_t isrflags = READ_REG(huart->Instance->ISR);
//		switch(huart->ErrorCode)
//    {
//				case HAL_UART_ERROR_NONE:
//						user_main_error("HAL_UART_ERROR_NONE\r\n");
//						break;
//				case HAL_UART_ERROR_PE:
//						user_main_error("HAL_UART_ERROR_PE\r\n");
//						READ_REG(huart->Instance->RDR);//PE清标志，第二步读DR
//						READ_REG(huart->Instance->TDR);
//						__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_PE);//清标志
//						break;
//				case HAL_UART_ERROR_NE:
//						user_main_error("HAL_UART_ERROR_NE\r\n");
//						READ_REG(huart->Instance->RDR);//NE清标志，第二步读DR
//						READ_REG(huart->Instance->TDR);
//						__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_NE);//清标志
//						break;
//				case HAL_UART_ERROR_FE:
//						user_main_error("HAL_UART_ERROR_FE\r\n");
//						READ_REG(huart->Instance->RDR);//FE清标志，第二步读DR
//						READ_REG(huart->Instance->TDR);
//						__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_FE);//清标志
//						break;
//				case HAL_UART_ERROR_ORE:
//						user_main_error("HAL_UART_ERROR_ORE\r\n");
//						READ_REG(huart->Instance->RDR);//ORE清标志，第二步读DR
//						READ_REG(huart->Instance->TDR);
//						__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_ORE);//清标志
//						break;
//				case HAL_UART_ERROR_DMA:
//						user_main_error("HAL_UART_ERROR_DMA\r\n");
//						break;
//				default:
//						user_main_error("other\r\n");
//						break;
//    }
//		MX_LPUART1_UART_Init();
//		HAL_UART_Receive_IT(&hlpuart1,rxbuf_lp,RXSIZE);
//	}
//	else	if(huart == (&huart2))
//	{
//		user_main_error("huart2 ERROR");
//		uint32_t isrflags = READ_REG(huart->Instance->ISR);
//		switch(huart->ErrorCode)
//    {
//				case HAL_UART_ERROR_NONE:
//						user_main_error("HAL_UART_ERROR_NONE\r\n");
//						break;
//				case HAL_UART_ERROR_PE:
//						user_main_error("HAL_UART_ERROR_PE\r\n");
//						READ_REG(huart->Instance->RDR);//PE清标志，第二步读DR
//						READ_REG(huart->Instance->TDR);
//						__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_PE);//清标志
//						break;
//				case HAL_UART_ERROR_NE:
//						user_main_error("HAL_UART_ERROR_NE\r\n");
//						READ_REG(huart->Instance->RDR);//NE清标志，第二步读DR
//						READ_REG(huart->Instance->TDR);
//						__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_NE);//清标志
//						break;
//				case HAL_UART_ERROR_FE:
//						user_main_error("HAL_UART_ERROR_FE\r\n");
//						READ_REG(huart->Instance->RDR);//FE清标志，第二步读DR
//						READ_REG(huart->Instance->TDR);
//						__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_FE);//清标志
//						break;
//				case HAL_UART_ERROR_ORE:
//						user_main_error("HAL_UART_ERROR_ORE\r\n");
//						READ_REG(huart->Instance->RDR);//ORE清标志，第二步读DR
//						READ_REG(huart->Instance->TDR);
//						__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_ORE);//清标志
//						break;
//				case HAL_UART_ERROR_DMA:
//						user_main_error("HAL_UART_ERROR_DMA\r\n");
//						break;
//				default:
//						user_main_error("other\r\n");
//						break;
//    }
//		MX_USART2_UART_Init();
//		HAL_UART_Receive_IT(&huart2,(uint8_t*)&rxbuf,RXSIZE);
//	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
