/* Includes ------------------------------------------------------------------*/
#include "flash_eraseprogram.h"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t PAGEError = 0;
__IO uint32_t data32 = 0;

/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;	

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Erase the user Flash area
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/
void  FLASH_erase(uint32_t page_address,uint8_t page)
{
	uint8_t err_num = 0;
	HAL_FLASH_Unlock();
  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.PageAddress = page_address;
  EraseInitStruct.NbPages     = page;
	while(err_num!=3)
	{
		if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
		{
			/*
				Error occurred while page erase.
				User can add here some code to deal with this error.
				PAGEError will contain the faulty page and then to know the code error on this page,
				user can call function 'HAL_FLASH_GetError()'
			*/   
				/* indicate error in Erase operation */		
			user_main_error("erase_error_flag:%x",HAL_FLASH_GetError());
			err_num++;
			if(err_num == 3)
				printf("error in Erase operation\n\r");
		}
		else
			break;
	}
/* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();
}
void  FLASH_program(uint32_t add, uint32_t *data, uint8_t count)
{
	uint32_t Address=0;
	uint16_t i=0;
	HAL_FLASH_Unlock();
  Address = add;

  while (i<count)
  {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, data[i]) == HAL_OK)
    {
      Address = Address + 4;
			i++;
		}
    else
    {
      /* Error occurred while writing data in Flash memory.*/
      
			printf("error in Write operation\n\r");
			printf("write_error_flag:%x",HAL_FLASH_GetError());
			HAL_FLASH_Lock();
			return;
    }
  }

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();
}

uint32_t FLASH_read(uint32_t Address)
{
	data32 = *(__IO uint32_t *)Address;
	return data32;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
