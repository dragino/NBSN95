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
#define FLASH_USER_END_ADDR            (FLASH_USER_START_ADDR_CONFIG + FLASH_PAGE_SIZE*9)   /* End @ of user Flash area store key*/


#define FLASH_USER_START_COAP      		 (FLASH_USER_START_ADDR_CONFIG + FLASH_PAGE_SIZE*1)
#define FLASH_USER_START_MQTT_CLIENT   (FLASH_USER_START_ADDR_CONFIG + FLASH_PAGE_SIZE*2)
#define FLASH_USER_START_MQTT_PUBTOPIC    (FLASH_USER_START_ADDR_CONFIG + FLASH_PAGE_SIZE*3)
#define FLASH_USER_START_MQTT_SUBTOPIC    (FLASH_USER_START_ADDR_CONFIG + FLASH_PAGE_SIZE*4)
#define FLASH_USER_START_MQTT_UNAME    (FLASH_USER_START_ADDR_CONFIG + FLASH_PAGE_SIZE*5)
#define FLASH_USER_START_MQTT_PWD    (FLASH_USER_START_ADDR_CONFIG + FLASH_PAGE_SIZE*6)
#define FLASH_USER_START_SERVADDR_ADD  (FLASH_USER_START_ADDR_CONFIG + FLASH_PAGE_SIZE*8)

#define EEPROM_USER_START_ADD						(DATA_EEPROM_BASE)
#define EEPROM_USER_START_VER						(EEPROM_USER_START_ADD)
#define EEPROM_USER_START_FDR_FLAG			(EEPROM_USER_START_VER + 0x04)
#define EEPROM_SHT_START_ADD						(DATA_EEPROM_BANK2_BASE)
#define EEPROM_TIME_START_ADD						(EEPROM_SHT_START_ADD + 0x04*50)
#define EEPROM_D1_AD0_START_ADD						(EEPROM_TIME_START_ADD + 0x04*50)
#define EEPROM_D2D3_START_ADD						(EEPROM_D1_AD0_START_ADD + 0x04*50)
#define EEPROM_DISTANCE_START_ADD						(EEPROM_D2D3_START_ADD + 0x04*50)
#define EEPROM_AD1_AD4_START_ADD						(EEPROM_DISTANCE_START_ADD + 0x04*50)
#define EEPROM_WEIGHT_START_ADD						(EEPROM_AD1_AD4_START_ADD + 0x04*50)
#define EEPROM_COUNT_START_ADD						(EEPROM_WEIGHT_START_ADD + 0x04*50)

void  FLASH_erase(uint32_t page_address,uint8_t page);
void  FLASH_program(uint32_t add, uint32_t *data, uint8_t count);
uint32_t FLASH_read(uint32_t Address);
	 
#ifdef __cplusplus
}
#endif

#endif /* __FLASH_ERASEPROGRAM_H__ */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
