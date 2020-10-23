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

/****************
Get time from network and update to clock
*****************/
void MyRtcInit(void)
{
	user_main_printf("Wait for calibration time......");
	uint8_t errorNum = 0;
//	nb_send(nb.recieve_data,AT CCLK"?\n");
//	
//	char* colon = strchr(nb.recieve_data,':');
//	char* comma = strchr(nb.recieve_data,',');
//	char* carriage = strchr(nb.recieve_data,'O');
	char* colon;
	char* comma;
	char* carriage;
	
	do 
	{
		HAL_Delay(1000);
		nb_send(nb.recieve_data,AT CCLK"?\n");
		colon = strchr(nb.recieve_data,':');
		comma = strchr(nb.recieve_data,',');
		carriage = strchr(nb.recieve_data,'O');
		if(colon == NULL || comma == NULL ||carriage == NULL)
			errorNum++;
		else
			errorNum = 10;
	}while(errorNum < 5);
	
	if(errorNum != 10)
	{
		user_main_printf("Failed to get time from network");
		return;
	}
	
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
