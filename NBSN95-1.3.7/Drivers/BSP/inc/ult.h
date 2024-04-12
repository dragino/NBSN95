#ifndef __ULT_H
#define __ULT_H

#include "common.h"

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
 * @brief  
 *
 * @note
 * @retval None
 */

void ULT_Rest(void);
int ULT_Data_processing(void);
void ULT_getData();
void ULT_Data_Read(void);
void ULT_Init(void (*uart_init)(void),void (*uart_485_enable)(void));
void ULT_DeInit(void(*uart_disable)(void),void(*uart_485_disable)(void));
void ULT_Check_Sum(void);
uint8_t ULT_Connection_Test(UART_HandleTypeDef *UartHandle,void (*GPIO_OUT_Set)(uint8_t state));

#ifdef __cplusplus
}
#endif

#endif

