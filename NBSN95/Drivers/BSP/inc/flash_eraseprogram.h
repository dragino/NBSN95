/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_ERASEPROGRAM_H__
#define __FLASH_ERASEPROGRAM_H__

#include "stm32l0xx_hal.h"
#include "usart.h"

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
#define FLASH_USER_START_ADDR_CONFIG   (FLASH_BASE + FLASH_PAGE_SIZE * 800)        /* Start @ of user Flash area store config */
#define FLASH_USER_END_ADDR            (FLASH_USER_START_ADDR_CONFIG + FLASH_PAGE_SIZE)   /* End @ of user Flash area store key*/

#define FLASH_USER_START_USER_KEY      (FLASH_USER_START_ADDR_CONFIG + 0x10)


void  FLASH_erase(uint32_t page_address,uint8_t page);
void  FLASH_program(uint32_t add, uint32_t *data, uint8_t count);
void  FLASH_program_on_addr(uint32_t addr,uint32_t data);
uint32_t FLASH_read(uint32_t Address);
	 
#ifdef __cplusplus
}
#endif

#endif /* __FLASH_ERASEPROGRAM_H__ */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
