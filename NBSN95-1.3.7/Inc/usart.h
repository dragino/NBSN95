/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "stdio.h"
//#include "stdarg.h"
/* USER CODE END Includes */

extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

/* USER CODE BEGIN Private defines */
#define RXSIZE 1
/* Serial 2 switch control */
#define  UART2_ENABLE_RE()       huart2.Instance->CR1|= (uint32_t)0x0004	            
#define  UART2_DISABLE_RE()      huart2.Instance->CR1&= (~(uint32_t)0x0004) 

void uart1_Init(void);
void uart1_IoDeInit(void);
void My_UARTEx_StopModeWakeUp(UART_HandleTypeDef* uartHandle);
/* USER CODE END Private defines */

void MX_LPUART1_UART_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);

/* USER CODE BEGIN Prototypes */

#define user_main_printf(format, ...) 	printf("[%d]" format "\r\n",HAL_GetTick(),##__VA_ARGS__)
			
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

#endif /* __USART_H__ */

