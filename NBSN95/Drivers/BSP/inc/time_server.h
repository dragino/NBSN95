/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIME_SERVER_H__
#define __TIME_SERVER_H__

#include "common.h"
#include "rtc.h"

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

typedef struct TimerEvent_s
{
    uint32_t Timestamp;         //! Expiring timer value in ticks from TimerContext
    uint32_t ReloadValue;       //! Reload Value when Timer is restarted
    uint8_t IsRunning;          //! Is the timer currently running
    void ( *Callback )( void ); //! Timer IRQ callback function
    struct TimerEvent_s *Next;  //! Pointer to the next Timer object.
} TimerEvent_t;

void My_AlarmInit(uint16_t timer,uint8_t alarmX);
void MyRtcInit(void);
long GetTick(char *str_time);
#endif 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
