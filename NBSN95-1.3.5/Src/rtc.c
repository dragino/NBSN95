/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
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
/* Includes ------------------------------------------------------------------*/
#include "rtc.h"
#include "common.h"

#define RTC_H12_AMorPM RTC_HOURFORMAT12_AM
#define ASYHCHPREDIV 0X7F
#define SYHCHPREDIV 0XFF
#define HOURS 1 // 0~23
#define MINUTES 1 // 0~59
#define SECONDS 1 // 0~59
#define WEEKDAY 1 // 1~7
#define DATE 1 // 1~31
#define MONTH 1 // 1~12
#define YEAR 1 // 0~99
#define RTC_Format_BINorBCD RTC_FORMAT_BIN
#define RTC_OUTPUT       RTC_OUTPUT_DISABLE
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;


/* RTC init function */

void RTC_CLK_Config(void) 
{
RTC_DateTypeDef RTC_DateStructure;
RTC_TimeTypeDef RTC_TimeStructure;
RTC_AlarmTypeDef sAlarm = {0};	
RCC_OscInitTypeDef RCC_OscInitStruct;
RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

hrtc.Instance = RTC;

__HAL_RCC_PWR_CLK_ENABLE();

HAL_PWR_EnableBkUpAccess();

RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
RCC_OscInitStruct.LSEState = RCC_LSE_ON;
RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
HAL_RCC_OscConfig(&RCC_OscInitStruct);;	
PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);	
__HAL_RCC_RTC_ENABLE();
HAL_RTC_WaitForSynchro(&hrtc);	

hrtc.Init.AsynchPrediv = ASYHCHPREDIV;
hrtc.Init.SynchPrediv = SYHCHPREDIV;
hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
hrtc.Init.OutPut = RTC_OUTPUT;
hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
if (HAL_RTC_Init(&hrtc) != HAL_OK) {
}
if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0)==0)
{
RTC_TimeStructure.TimeFormat = RTC_H12_AMorPM;
RTC_TimeStructure.Hours = 0x0;
RTC_TimeStructure.Minutes = 0x0;
RTC_TimeStructure.Seconds = 0x0;
RTC_TimeStructure.SubSeconds = 0;
RTC_TimeStructure.StoreOperation = RTC_DAYLIGHTSAVING_NONE;
RTC_TimeStructure.DayLightSaving = RTC_STOREOPERATION_RESET;
HAL_RTC_SetTime(&hrtc,&RTC_TimeStructure, RTC_FORMAT_BIN);

RTC_DateStructure.WeekDay = RTC_WEEKDAY_MONDAY;
RTC_DateStructure.Date = 1;
RTC_DateStructure.Month =  RTC_MONTH_JANUARY;
RTC_DateStructure.Year =  0;
HAL_RTC_SetDate(&hrtc,&RTC_DateStructure, RTC_FORMAT_BIN);

HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 1611878400);//20210129 00:00:00
HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0);
}

  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 0x0;
  sAlarm.AlarmTime.Minutes = 0x0;
  sAlarm.AlarmTime.Seconds = 0x3;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable the Alarm B
  */
  sAlarm.AlarmTime.Seconds = 0x5;
  sAlarm.Alarm = RTC_ALARM_B;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */
  My_AlarmInit(10,1);
}

void RTC_TimeAndDate_Show(void) 
{
uint8_t Rtctmp=0;
RTC_TimeTypeDef RTC_TimeStructure;
RTC_DateTypeDef RTC_DateStructure;
while (1) {
HAL_RTC_GetTime(&hrtc, &RTC_TimeStructure, RTC_FORMAT_BIN);
HAL_RTC_GetDate(&hrtc, &RTC_DateStructure, RTC_FORMAT_BIN);
if (Rtctmp != RTC_TimeStructure.Seconds) {
	
user_main_printf("The Date : Y:20%0.2d - M:%0.2d - D:%0.2d - W:%0.2d\r\n",RTC_DateStructure.Year,RTC_DateStructure.Month,RTC_DateStructure.Date,RTC_DateStructure.WeekDay);
	
user_main_printf("The Time : %0.2d:%0.2d:%0.2d \r\n\r\n",RTC_TimeStructure.Hours,RTC_TimeStructure.Minutes,RTC_TimeStructure.Seconds);

(void)RTC->DR;
	HAL_IWDG_Refresh(&hiwdg);	
}
Rtctmp = RTC_TimeStructure.Seconds;
} 
}

void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
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
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 0x0;
  sAlarm.AlarmTime.Minutes = 0x0;
  sAlarm.AlarmTime.Seconds = 0x3;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable the Alarm B
  */
  sAlarm.AlarmTime.Seconds = 0x5;
  sAlarm.Alarm = RTC_ALARM_B;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();

    /* RTC interrupt Init */
    HAL_NVIC_SetPriority(RTC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(RTC_IRQn);
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();

    /* RTC interrupt Deinit */
    HAL_NVIC_DisableIRQ(RTC_IRQn);
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
