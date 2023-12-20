#include "time_server.h"
#include "time.h"
#include <time.h>
#include "hw.h"
void get_time(void)
{
	RTC_DateTypeDef sdatestructureget;			
	RTC_TimeTypeDef stimestructureget;
	HAL_RTC_GetTime(&RtcHandle, &stimestructureget, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, RTC_FORMAT_BIN);
	printf("[%.2d-%.2d-%.2d ", sdatestructureget.Year,sdatestructureget.Month, sdatestructureget.Date);
	printf(" %.2d:%.2d:%.2d]", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);			
}

void My_AlarmInit(uint32_t timer,uint8_t alarmX)
{
	if(alarmX == 1)
		HAL_RTC_DeactivateAlarm(&RtcHandle,RTC_ALARM_B);
	else if(alarmX == 0)
		HAL_RTC_DeactivateAlarm(&RtcHandle,RTC_ALARM_A);
	
  RTC_AlarmTypeDef sAlarm = {0};
	uint8_t _hour = 0,_min = 0,_sec = 0;
	uint8_t hour = 0,min = 0,sec = 0;
	uint8_t minFlag=0,secFlag=0;
	
	RTC_TimeTypeDef stimestructureget;
	RTC_DateTypeDef sdatestructureget;
	HAL_RTC_GetTime(&RtcHandle, &stimestructureget, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, RTC_FORMAT_BIN);
	
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
  if (HAL_RTC_SetAlarm_IT(&RtcHandle, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
}

long GetTick(char *str_time)  
{  
	struct tm stm;  
	int iY, iM, iD, iH, iMin, iS;  

	memset(&stm,0,sizeof(stm));  

	iY = atoi(str_time);  
	iM = atoi(str_time+5);  
	iD = atoi(str_time+8);  
	iH = atoi(str_time+11);  
	iMin = atoi(str_time+14);  
	iS = atoi(str_time+17);  

	stm.tm_year=iY-1900;  
	stm.tm_mon=iM-1;  
	stm.tm_mday=iD;  
	stm.tm_hour=iH;  
	stm.tm_min=iMin;  
	stm.tm_sec=iS;  

	/*printf("%d-%0d-%0d %0d:%0d:%0d\n", iY, iM, iD, iH, iMin, iS);*/  

	return mktime(&stm);  
} 

void GetTime(uint32_t Time)
{	
	time_t curtime;
	curtime = Time;
	printf("%s", ctime(&curtime));
}


#define exec_cb( _callback_ )     \
  do {                          \
      if( _callback_ == NULL )    \
      {                           \
        while(1);                 \
      }                           \
      else                        \
      {                           \
        _callback_( );               \
      }                           \
  } while(0);                   



static TimerEvent_t *TimerListHead = NULL;

/*!
 * \brief Adds or replace the head timer of the list.
 *
 * \remark The list is automatically sorted. The list head always contains the
 *     next timer to expire.
 *
 * \param [IN]  obj Timer object to be become the new head
 * \param [IN]  remainingTime Remaining time of the previous head to be replaced
 */
static void TimerInsertNewHeadTimer( TimerEvent_t *obj );

/*!
 * \brief Adds a timer to the list.
 *
 * \remark The list is automatically sorted. The list head always contains the
 *     next timer to expire.
 *
 * \param [IN]  obj Timer object to be added to the list
 * \param [IN]  remainingTime Remaining time of the running head after which the object may be added
 */
static void TimerInsertTimer( TimerEvent_t *obj );

/*!
 * \brief Sets a timeout with the duration "timestamp"
 * 
 * \param [IN] timestamp Delay duration
 */
static void TimerSetTimeout( TimerEvent_t *obj );

/*!
 * \brief Check if the Object to be added is not already in the list
 * 
 * \param [IN] timestamp Delay duration
 * \retval true (the object is already in the list) or false  
 */
static bool TimerExists( TimerEvent_t *obj );



void TimerInit( TimerEvent_t *obj, void ( *callback )( void ) )
{
  obj->Timestamp = 0;
  obj->ReloadValue = 0;
  obj->IsRunning = false;
  obj->Callback = callback;
  obj->Next = NULL;
}

void TimerStart( TimerEvent_t *obj )
{
  uint32_t elapsedTime = 0;
  
  BACKUP_PRIMASK();
  
  DISABLE_IRQ( );
  

  if( ( obj == NULL ) || ( TimerExists( obj ) == true ) )
  {
    RESTORE_PRIMASK( );
    return;
  }
  obj->Timestamp = obj->ReloadValue;
  obj->IsRunning = false;

  if( TimerListHead == NULL )
  {
    HW_RTC_SetTimerContext( );
    TimerInsertNewHeadTimer( obj ); // insert a timeout at now+obj->Timestamp
  }
  else 
  {
    elapsedTime = HW_RTC_GetTimerElapsedTime( );
    obj->Timestamp += elapsedTime;
  
    if( obj->Timestamp < TimerListHead->Timestamp )
    {
      TimerInsertNewHeadTimer( obj);
    }
    else
    {
      TimerInsertTimer( obj);
    }
  }
  RESTORE_PRIMASK( );
}

static void TimerInsertTimer( TimerEvent_t *obj)
{
  TimerEvent_t* cur = TimerListHead;
  TimerEvent_t* next = TimerListHead->Next;

  while (cur->Next != NULL )
  {  
    if( obj->Timestamp  > next->Timestamp )
    {
        cur = next;
        next = next->Next;
    }
    else
    {
        cur->Next = obj;
        obj->Next = next;
        return;

    }
  }
  cur->Next = obj;
  obj->Next = NULL;
}

static void TimerInsertNewHeadTimer( TimerEvent_t *obj )
{
  TimerEvent_t* cur = TimerListHead;

  if( cur != NULL )
  {
    cur->IsRunning = false;
  }

  obj->Next = cur;
  TimerListHead = obj;
  TimerSetTimeout( TimerListHead );
}

void TimerIrqHandler( void )
{
  TimerEvent_t* cur;
  TimerEvent_t* next;
  

  
  uint32_t old =  HW_RTC_GetTimerContext( );
  uint32_t now =  HW_RTC_SetTimerContext( );
  uint32_t DeltaContext = now - old; //intentionnal wrap around
  
  /* update timeStamp based upon new Time Reference*/
  /* beacuse delta context should never exceed 2^32*/
  if ( TimerListHead != NULL )
  {
    for (cur=TimerListHead; cur->Next != NULL; cur= cur->Next)
    {
      next =cur->Next;
      if (next->Timestamp > DeltaContext)
      {
        next->Timestamp -= DeltaContext;
      }
      else
      {
        next->Timestamp = 0 ;
      }
    }
  }
  
  /* execute imediately the alarm callback */
  if ( TimerListHead != NULL )
  {
    cur = TimerListHead;
    TimerListHead = TimerListHead->Next;
    exec_cb( cur->Callback );
  }


  // remove all the expired object from the list
  while( ( TimerListHead != NULL ) && ( TimerListHead->Timestamp < HW_RTC_GetTimerElapsedTime(  )  ))
  {
   cur = TimerListHead;
   TimerListHead = TimerListHead->Next;
   exec_cb( cur->Callback );
  }

  /* start the next TimerListHead if it exists AND NOT running */
  if(( TimerListHead != NULL ) && (TimerListHead->IsRunning == false))
  {
    TimerSetTimeout( TimerListHead );
  }
}

void TimerStop( TimerEvent_t *obj ) 
{
  BACKUP_PRIMASK();
  
  DISABLE_IRQ( );
  
  TimerEvent_t* prev = TimerListHead;
  TimerEvent_t* cur = TimerListHead;

  // List is empty or the Obj to stop does not exist 
  if( ( TimerListHead == NULL ) || ( obj == NULL ) )
  {
    RESTORE_PRIMASK( );
    return;
  }

  if( TimerListHead == obj ) // Stop the Head                  
  {
    if( TimerListHead->IsRunning == true ) // The head is already running 
    {    
      if( TimerListHead->Next != NULL )
      {
        TimerListHead->IsRunning = false;
        TimerListHead = TimerListHead->Next;
        TimerSetTimeout( TimerListHead );
      }
      else
      {
        HW_RTC_StopAlarm( );
        TimerListHead = NULL;
      }
    }
    else // Stop the head before it is started
    {   
      if( TimerListHead->Next != NULL )   
      {
        TimerListHead = TimerListHead->Next;
      }
      else
      {
        TimerListHead = NULL;
      }
    }
  }
  else // Stop an object within the list
  {      
    while( cur != NULL )
    {
      if( cur == obj )
      {
        if( cur->Next != NULL )
        {
          cur = cur->Next;
          prev->Next = cur;
        }
        else
        {
          cur = NULL;
          prev->Next = cur;
        }
        break;
      }
      else
      {
        prev = cur;
        cur = cur->Next;
      }
    }   
  }
  
  RESTORE_PRIMASK( );
}  
  
static bool TimerExists( TimerEvent_t *obj )
{
  TimerEvent_t* cur = TimerListHead;

  while( cur != NULL )
  {
    if( cur == obj )
    {
      return true;
    }
    cur = cur->Next;
  }
  return false;
}

void TimerReset( TimerEvent_t *obj )
{
  TimerStop( obj );
  TimerStart( obj );
}

void TimerSetValue( TimerEvent_t *obj, uint32_t value )
{
  uint32_t minValue = 0;
  uint32_t ticks = HW_RTC_ms2Tick( value );

  TimerStop( obj );

  minValue = HW_RTC_GetMinimumTimeout( );
  
  if( ticks < minValue )
  {
    ticks = minValue;
  }

  obj->Timestamp = ticks;
  obj->ReloadValue = ticks;
}

TimerTime_t TimerGetCurrentTime( void )
{
  uint32_t now = HW_RTC_GetTimerValue( );
  return  HW_RTC_Tick2ms(now);
}

TimerTime_t TimerGetElapsedTime( TimerTime_t past )
{
  uint32_t nowInTicks = HW_RTC_GetTimerValue( );
  uint32_t pastInTicks = HW_RTC_ms2Tick( past );
  /* intentional wrap around. Works Ok if tick duation below 1ms */
  return HW_RTC_Tick2ms( nowInTicks- pastInTicks );
}

static void TimerSetTimeout( TimerEvent_t *obj )
{
  int32_t minTicks= HW_RTC_GetMinimumTimeout( );
  obj->IsRunning = true; 

  //in case deadline too soon
  if(obj->Timestamp  < (HW_RTC_GetTimerElapsedTime(  ) + minTicks) )
  {
    obj->Timestamp = HW_RTC_GetTimerElapsedTime(  ) + minTicks;
  }
  HW_RTC_SetAlarm( obj->Timestamp );
}