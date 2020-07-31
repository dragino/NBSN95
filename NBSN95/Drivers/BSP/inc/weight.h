#ifndef __WEIGHT_H
#define __WEIGHT_H

#include "common.h"

/* ---------------------------  WEIGHT HW definition -------------------------------*/
#define WEIGHT_SCK_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()
#define WEIGHT_SCK_PORT       		 GPIOA	 
#define WEIGHT_SCK_PIN       			 GPIO_PIN_11 
#define WEIGHT_DOUT_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()
#define WEIGHT_DOUT_PORT      		 GPIOB
#define WEIGHT_DOUT_PIN     			 GPIO_PIN_12  
#define HX711_SCK_0          			 HAL_GPIO_WritePin(WEIGHT_SCK_PORT,WEIGHT_SCK_PIN ,GPIO_PIN_RESET)
#define HX711_SCK_1          			 HAL_GPIO_WritePin(WEIGHT_SCK_PORT,WEIGHT_SCK_PIN ,GPIO_PIN_SET)
#define HX711_DOUT_0          		 HAL_GPIO_WritePin(WEIGHT_DOUT_PORT,WEIGHT_DOUT_PIN ,GPIO_PIN_RESET)
#define HX711_DOUT_1          		 HAL_GPIO_WritePin(WEIGHT_DOUT_PORT,WEIGHT_DOUT_PIN ,GPIO_PIN_SET)
/* ---------------------------  +5v PWR OUT definition -------------------------------*/
#define PWR_OUT_PORT              GPIOB
#define PWR_OUT_PIN               GPIO_PIN_5
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

#define RCC_GPIO_CLK_ENABLE( __GPIO_PORT__ )              \
do {                                                    \
    switch( __GPIO_PORT__)                                \
    {                                                     \
      case GPIOA_BASE: __HAL_RCC_GPIOA_CLK_ENABLE(); break;    \
      case GPIOB_BASE: __HAL_RCC_GPIOB_CLK_ENABLE(); break;    \
      case GPIOC_BASE: __HAL_RCC_GPIOC_CLK_ENABLE(); break;    \
      case GPIOD_BASE: __HAL_RCC_GPIOD_CLK_ENABLE(); break;    \
      case GPIOH_BASE: default:  __HAL_RCC_GPIOH_CLK_ENABLE(); \
    }                                                    \
  } while(0)  

void WEIGHT_SCK_Init(void);
void WEIGHT_DOUT_Init(void);
uint32_t HX711_Read(void);
void Get_Maopi(void);
int32_t Get_Weight(void);
void HW_GPIO_Init( GPIO_TypeDef* port, uint16_t GPIO_Pin, GPIO_InitTypeDef* initStruct);
#ifdef __cplusplus
}
#endif

#endif
