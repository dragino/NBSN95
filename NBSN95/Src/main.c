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
#include "rtc.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "at.h"
#include "lowpower.h"
#include "nbInit.h"
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
static uint8_t  rxbuf = 0;			
static uint16_t rxlen = 0;
static uint8_t  rxDATA[300]={0};
static uint8_t  uart2_recieve_flag = 0;

static uint8_t rxbuf_lp;
static uint8_t lpuart_recieve_flag = 0;

uint8_t rxbuf_u1 = 0; 	//usart1
uint8_t rxDATA_u1[100]={0};
uint8_t rxlen_u1 = 0;
extern uint8_t getsensorvalue_flag;
static uint8_t pwd_time_count = 0;			//Password time count times
extern uint8_t controlMCU_flag;
static uint16_t dns_time_count = 0;			//DNS time count times

static uint8_t task_num = _AT_IDLE;			//NB task directory

static uint8_t error_num = 0;				    //Error count

static uint8_t uplink_time_num = 0;
extern uint8_t dns_id_flags;
extern uint8_t 	bc35tobc95_flags;   //Switch module flag
static uint8_t dns_reset_num;
uint8_t join_network_num = 0;				  
uint8_t join_network_flag = 0;	
uint8_t join_network_time = 0;
uint8_t join_network_timer = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void USERTASK(void);
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
  MX_RTC_Init();
  MX_ADC_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	new_firmware_update();
	config_Get();
	reboot_information_print();
	product_information_print();
	List_Init(sys.list);
	led_on(1000);
	HAL_Delay(3000);
	HAL_UART_Receive_IT(&huart2,(uint8_t*)&rxbuf,RXSIZE);
	HAL_UART_Receive_IT(&hlpuart1,(uint8_t*)&rxbuf_lp,RXSIZE);
	My_UARTEx_StopModeWakeUp(&huart2);				//Enable serial port wake up
	My_UARTEx_StopModeWakeUp(&hlpuart1);			//Enable serial port wake up
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
#ifdef NBIOT
	if((*(__IO uint8_t *)EEPROM_USER_START_FDR_FLAG) == 0x01)
	{
		if(controlMCU_flag==0)	
{	
		task_num = _AT_FLAG_INIT;
}
else if(controlMCU_flag ==1)
{		
	task_num = _AT_IDLE;
}
		HAL_FLASHEx_DATAEEPROM_Unlock();
		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,EEPROM_USER_START_FDR_FLAG,0x00);
		HAL_FLASHEx_DATAEEPROM_Lock();
	}
	else
	{
		if(controlMCU_flag==0)	
{	
		task_num = _AT_FLAG_INIT;
}
else if(controlMCU_flag==1)
{		
	task_num = _AT_IDLE;
}
}
#else
	task_num = _AT_IDLE;
#endif
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
#ifdef NBIOT		
if(controlMCU_flag==0)	
{	
		if(task_num != _AT_IDLE)
		{
			HAL_Delay(1000);
			
			if(NBTASK(&task_num) == _AT_ERROR)
			{
				error_num++;
			}
	  }
		if((error_num > 3 && nb.uplink_flag == no_status)||(error_num > 6))
		{
			user_main_printf("Restart the module...");
			task_num = _AT_NRB;
			error_num = 0;
		}
		if(dns_reset_num > 2 )
		{
		NVIC_SystemReset();	
		dns_reset_num=0;
		}
		if(nb.recieve_flag == NB_RECIEVE && nb.uplink_flag == send)
		{
			task_num = _AT_URI;
			nb.recieve_flag = NB_IDIE;
		}
		if(getsensorvalue_flag == 1 && nb.uplink_flag == no_status)
		{
			task_num = _AT_CSQ;
			nb.uplink_flag = send;
			NBTASK(&task_num);
			getsensorvalue_flag = 0;
		}
}
#endif
		
		USERTASK();

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
			rxDATA[strlen((char*)rxDATA)] = '\n';
			HAL_UART_Transmit_DMA(&hlpuart1,(uint8_t*)rxDATA,strlen((char*)rxDATA));		
			HAL_Delay(700);					//Waiting to Send
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
	
	
	if(nb.recieve_flag == NB_RECIEVE && nb.dns_flag == running)
	{
		int dns_num=3;
		nb.recieve_flag = NB_IDIE;
		nb.dns_flag = no_status;
		while(dns_num--)
		{
		HAL_Delay(1500);
		if(NBTask[_AT_QDNS].get(NULL) == NB_CMD_SUCC)
		{
			task_num = _AT_UPLOAD_START;
			dns_reset_num=0;
			break;
		}
		else
		{
			task_num = _AT_NRB;
			if(dns_num==0)
		  dns_reset_num++;
		}
	  }
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &hlpuart1)
	{
		nb.usart.data[nb.usart.len++] = rxbuf_lp;	
		if(task_num == _AT_IDLE)
		{
			if(rxbuf_lp == '\r' || rxbuf_lp == '\n')
			{
				lpuart_recieve_flag = 1;
			}
		}
		HAL_UART_Receive_IT(&hlpuart1,(uint8_t*)&rxbuf_lp,RXSIZE);		
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
			if(controlMCU_flag==0)	
{	
	if(nb.net_flag == no_status)
		task_num = _AT_FLAG_INIT;
	else if(nb.net_flag == success && nb.uplink_flag == no_status)
	{
		task_num = _AT_CSQ;
		nb.uplink_flag = send;
	}
}	
	LPM_DisableStopMode();
}

void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc)
{
	HAL_IWDG_Refresh(&hiwdg);
			if(controlMCU_flag==0)	
{		
#ifdef NBIOT	
	if(sys.pwd_flag==1)
	{
		pwd_time_count++;
		if(pwd_time_count == 17)
		{
			sys.pwd_flag = 0;
			pwd_time_count = 0;
		}
	}
#endif	

	if(nb.net_flag == fail && join_network_flag==1)
	{
		task_num = _AT_CSQ;
    join_network_timer=1;
    join_network_flag=0;
	}
	if(nb.net_flag == success && nb.uplink_flag == send)
	{
		uplink_time_num++;
		if(uplink_time_num>=7)
		{
			uplink_time_num = 0;
			error_num++;
			if(sys.protocol == COAP_PRO)			
       {
				 if(bc35tobc95_flags==0)
					{
				  	task_num=_AT_COAP_CLOSE;
						}else{
						task_num=_AT_COAP_CLOSE_BC95GV;
						}
       }
			else if(sys.protocol == UDP_PRO)	{task_num	=	_AT_UDP_CLOSE;}
			else if(sys.protocol == MQTT_PRO)	{task_num	=	_AT_MQTT_CLOSE;}
			else if(sys.protocol == TCP_PRO)	{task_num	=	_AT_TCP_CLOSE;}
		}
	}
	else 
		uplink_time_num = 0;
	
	if(dns_id_flags==1)
	{
		dns_time_count++;
		if(dns_time_count == 2400)
		{
      if(sys.protocol == COAP_PRO)			{task_num = _AT_QDNS;}
			else if(sys.protocol == UDP_PRO)	{task_num	=	_AT_QDNS;}
			dns_time_count=0;
		}
	}
	
	if(join_network_timer==1)
	{
		join_network_time++;
		if(join_network_time==(sys.csq_time*3))
		{
		 join_network_time=0;
		join_network_timer=0;
		nb.net_flag = no_status;
    task_num	=	_AT_CFUNOFF;
		}
	}	
}	
	My_AlarmInit(18,1);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{	
	if(GPIO_Pin == GPIO_PIN_14)
	{		
		if(sys.mod == model6)
			sensor.exit_count++;
		else if(nb.net_flag == success && sys.mod != model6 && (task_num < _AT_COAP_CONFIG || task_num > _AT_TCP_CLOSE))
		{
			if(nb.uplink_flag == no_status)
			{
			task_num = _AT_CSQ;
		  nb.uplink_flag = send;
			sys.exit_flag = 1;
			//sensor.exit_state = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)==1?1:2;
			sensor.exit_state = 1;
			LPM_DisableStopMode();	
			}
		}
	}
	else if(GPIO_Pin == GPIO_PIN_13)
	{		
		nb.recieve_flag = NB_RECIEVE;
	}
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

