/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIME_SERVER_H__
#define __TIME_SERVER_H__

#include "common.h"
#include "time.h"

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
/**
 * @brief  initialises the 
 *
 * @note
 * @retval None
 */
#include "utilities.h"

typedef struct TimerEvent_s
{
    uint32_t Timestamp;         //! Expiring timer value in ticks from TimerContext
    uint32_t ReloadValue;       //! Reload Value when Timer is restarted
    bool IsRunning;          //! Is the timer currently running
    void ( *Callback )( void ); //! Timer IRQ callback function
    struct TimerEvent_s *Next;  //! Pointer to the next Timer object.
} TimerEvent_t;

typedef struct SysTime_s
{
    uint32_t Seconds;
    int16_t  SubSeconds;
}SysTime_t;

SysTime_t SysTimeGet( void );

void My_AlarmInit(uint32_t timer,uint8_t alarmX);
void MyRtcInit(void);
long GetTick(char *str_time);
void GetTime(uint32_t Time);

void TimerInit( TimerEvent_t *obj, void ( *callback )( void ) );

void TimerIrqHandler( void );

void TimerStart( TimerEvent_t *obj );

void TimerStop( TimerEvent_t *obj );

void TimerReset( TimerEvent_t *obj );

void TimerSetValue( TimerEvent_t *obj, uint32_t value );

TimerTime_t TimerGetCurrentTime( void );

TimerTime_t TimerGetElapsedTime( TimerTime_t savedTime );

void SysTimeLocalTime( const uint32_t timestamp, struct tm *localtime );
#endif 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
