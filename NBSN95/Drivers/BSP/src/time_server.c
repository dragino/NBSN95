#include "time_server.h"

void get_time()
{
	RTC_DateTypeDef sdatestructureget;			
	RTC_TimeTypeDef stimestructureget;
	HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
	printf("[%.2d-%.2d-%.2d ", sdatestructureget.Year,sdatestructureget.Month, sdatestructureget.Date);
	printf(" %.2d:%.2d:%.2d]", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);			
}

void My_AlarmInit(uint16_t timer,uint8_t alarmX)
{
	if(alarmX == 1)
		HAL_RTC_DeactivateAlarm(&hrtc,RTC_ALARM_B);
	else if(alarmX == 0)
		HAL_RTC_DeactivateAlarm(&hrtc,RTC_ALARM_A);
	
  RTC_AlarmTypeDef sAlarm = {0};
	uint8_t _hour = 0,_min = 0,_sec = 0;
	uint8_t hour = 0,min = 0,sec = 0;
	uint8_t minFlag=0,secFlag=0;
	
	RTC_TimeTypeDef stimestructureget;
	RTC_DateTypeDef sdatestructureget;
	HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
	
	uint16_t _median = 0;
	_hour 	= timer /3600;
	_median = timer %3600;
	_min  	= _median /60;
	_sec  	= _median %60;
	sec =	(stimestructureget.Seconds+_sec)%10+(stimestructureget.Seconds+_sec)/10*16;	
	if(sec > 0x59)
	{
		sAlarm.AlarmTime.Seconds = sec - 0x60;
		secFlag = 1;
	}
	else
		sAlarm.AlarmTime.Seconds = sec;
	
	if(secFlag)
	{
		min = (stimestructureget.Minutes+_min +1)%10+(stimestructureget.Minutes+_min +1)/10*16;
	}
	else
		min = (stimestructureget.Minutes+_min)%10+(stimestructureget.Minutes+_min)/10*16;
	if(min > 0x59)
	{
		sAlarm.AlarmTime.Minutes = min - 0x60;
		minFlag = 1;
	}
	else
		sAlarm.AlarmTime.Minutes = min;
	
	if(minFlag)
	{
		hour = (stimestructureget.Hours+_hour +1)%10+(stimestructureget.Hours+_hour +1)/10*16;
	}	
	else
		hour = (stimestructureget.Hours+_hour)%10+(stimestructureget.Hours+_hour)/10*16;
	if(hour > 0x23)
	{
		sAlarm.AlarmTime.Hours = hour - 0x24;
	}
	else
		sAlarm.AlarmTime.Hours = hour;

	sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;
	
	if(alarmX == 0)
		sAlarm.Alarm = RTC_ALARM_A;
	else 
		sAlarm.Alarm = RTC_ALARM_B;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
}
