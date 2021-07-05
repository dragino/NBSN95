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
#define FLASH_USER_START_ADDR_CONFIG   (FLASH_BASE + FLASH_PAGE_SIZE * 800)        					/* Start @ of user Flash area store config */
#define FLASH_USER_END_ADDR            (FLASH_USER_START_ADDR_CONFIG + FLASH_PAGE_SIZE*4)   /* End @ of user Flash area store key*/

#define FLASH_USER_START_SERVER_ADD    (FLASH_USER_START_ADDR_CONFIG + 0x04*25)
#define FLASH_USER_START_COAP      		 (FLASH_USER_START_ADDR_CONFIG + FLASH_PAGE_SIZE*1)
#define FLASH_USER_START_MQTT_BASIC    (FLASH_USER_START_ADDR_CONFIG + FLASH_PAGE_SIZE*2)
#define FLASH_USER_START_MQTT_TOPIC    (FLASH_USER_START_ADDR_CONFIG + FLASH_PAGE_SIZE*3)

#define FLASH_USER_START_CACHE_Data1   (FLASH_USER_START_ADDR_CONFIG + FLASH_PAGE_SIZE*10)	//0x08019500
#define FLASH_USER_START_CACHE_Data2   (FLASH_USER_START_CACHE_Data1 + 0x04*11)
#define FLASH_USER_START_CACHE_Data3   (FLASH_USER_START_CACHE_Data1 + 0x04*22)
#define FLASH_USER_START_CACHE_Data4   (FLASH_USER_START_ADDR_CONFIG + FLASH_PAGE_SIZE*11)
#define FLASH_USER_START_CACHE_Data5   (FLASH_USER_START_CACHE_Data4 + 0x04*11)
#define FLASH_USER_START_CACHE_Data6   (FLASH_USER_START_CACHE_Data4 + 0x04*22)
#define FLASH_USER_START_CACHE_Data7   (FLASH_USER_START_ADDR_CONFIG + FLASH_PAGE_SIZE*12)
#define FLASH_USER_START_CACHE_Data8   (FLASH_USER_START_CACHE_Data7 + 0x04*11)
#define FLASH_USER_START_CACHE_Data9   (FLASH_USER_START_CACHE_Data7 + 0x04*22)
#define FLASH_USER_START_CACHE_Data10  (FLASH_USER_START_ADDR_CONFIG + FLASH_PAGE_SIZE*13)

#define EEPROM_USER_START_ADD						(DATA_EEPROM_BASE)
#define EEPROM_USER_START_VER						(EEPROM_USER_START_ADD)
#define EEPROM_USER_START_FDR_FLAG			(EEPROM_USER_START_VER + 0x04)

void  FLASH_erase(uint32_t page_address,uint8_t page);
void  FLASH_program(uint32_t add, uint32_t *data, uint8_t count);
uint32_t FLASH_read(uint32_t Address);
	 
#ifdef __cplusplus
}
#endif

#endif /* __FLASH_ERASEPROGRAM_H__ */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
