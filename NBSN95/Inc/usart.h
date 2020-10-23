/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "stdio.h"
/* USER CODE END Includes */

extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart2;

/* USER CODE BEGIN Private defines */
#define RXSIZE 1
/* Serial 2 switch control */
#define  UART2_ENABLE_RE()       huart2.Instance->CR1|= (uint32_t)0x0004	            
#define  UART2_DISABLE_RE()      huart2.Instance->CR1&= (~(uint32_t)0x0004) 

void My_UARTEx_StopModeWakeUp(UART_HandleTypeDef* uartHandle);
/* USER CODE END Private defines */

void MX_LPUART1_UART_Init(void);
void MX_USART2_UART_Init(void);

/* USER CODE BEGIN Prototypes */

#define user_main_printf(format, ...) do{																								\
																				RTC_DateTypeDef sdatestructureget;			\
																				RTC_TimeTypeDef stimestructureget;			\
																				HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);	\
																				HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);	\
																				printf("[%.2d-%.2d-%.2d ", sdatestructureget.Year,sdatestructureget.Month, sdatestructureget.Date);	\
																				printf(" %.2d:%.2d:%.2d]", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);		\
																				printf( format "\r\n", ##__VA_ARGS__);				\
																			}while(0)

//	#define user_main_printf(format, ...) printf(format "\r\n", ##__VA_ARGS__)
			
//#define USER_MAIN_DEBUG			
																			
#ifdef USER_MAIN_DEBUG
	#define user_main_info(format, ...) 	printf("[main info]" format "\r\n", ##__VA_ARGS__)
	#define user_main_debug(format, ...)  printf("[main debug]" format "\r\n", ##__VA_ARGS__)
	#define user_main_error(format, ...)  printf("[main error]" format "\r\n",##__VA_ARGS__)
#else	
	#define user_main_info(format, ...)
	#define user_main_debug(format, ...)
	#define user_main_error(format, ...)
#endif

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
