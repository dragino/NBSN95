/**
  ******************************************************************************
  * @file    ZB25VQ32.c
  * @Author  MCD Application Team
  * @brief   This file provides the ZB25VQ32 drivers.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "zb25vq32.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @defgroup ZB25VQ32 ZB25VQ32
  * @{
  */
#define ZB25VQ32_SPI_TIMEOUT 1000U /* 1000ms */

#define ZB25VQ32_CS_PORT	     GPIOH
#define ZB25VQ32_CS_PIN		     GPIO_PIN_0
#define ZB25VQ32_SCLK_PORT	   GPIOB
#define ZB25VQ32_SCLK_PIN		   GPIO_PIN_12
#define ZB25VQ32_SDI_PORT	     GPIOB
#define ZB25VQ32_SDI_PIN		   GPIO_PIN_13
#define ZB25VQ32_SDO_PORT	     GPIOB
#define ZB25VQ32_SDO_PIN		   GPIO_PIN_14

#define ZB25VQ32_CS_H        HAL_GPIO_WritePin(ZB25VQ32_CS_PORT, ZB25VQ32_CS_PIN, GPIO_PIN_SET)		
#define ZB25VQ32_CS_L        HAL_GPIO_WritePin(ZB25VQ32_CS_PORT, ZB25VQ32_CS_PIN, GPIO_PIN_RESET)	

#define ZB25VQ32_SCLK_H      HAL_GPIO_WritePin(ZB25VQ32_SCLK_PORT, ZB25VQ32_SCLK_PIN, GPIO_PIN_SET)		
#define ZB25VQ32_SCLK_L      HAL_GPIO_WritePin(ZB25VQ32_SCLK_PORT, ZB25VQ32_SCLK_PIN, GPIO_PIN_RESET)	

#define ZB25VQ32_SDI_H       HAL_GPIO_WritePin(ZB25VQ32_SDI_PORT, ZB25VQ32_SDI_PIN, GPIO_PIN_SET)		
#define ZB25VQ32_SDI_L       HAL_GPIO_WritePin(ZB25VQ32_SDI_PORT, ZB25VQ32_SDI_PIN, GPIO_PIN_RESET)
	
#define ZB25VQ32_SDO_READ()  HAL_GPIO_ReadPin(ZB25VQ32_SDO_PORT, ZB25VQ32_SDO_PIN)	
#define ZB25VQ32_SDO_L       HAL_GPIO_WritePin(ZB25VQ32_SDO_PORT, ZB25VQ32_SDO_PIN, GPIO_PIN_RESET)

static void delay_us(uint16_t num);
static uint8_t ZB25VQ32_ReadByte(void);
static uint8_t ZB25VQ32_Read_Len(uint8_t *buf, uint16_t len);
static void ZB25VQ32_WriteByte(uint8_t wdata);
static void ZB25VQ32_Write_Len(uint8_t *wdata, uint16_t len);

/** @defgroup ZB25VQ32_Exported_Functions ZB25VQ32 Exported Functions
  * @{
  */



/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
void ZB25VQ32_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  GPIO_InitStruct.Pin = ZB25VQ32_CS_PIN;
  HAL_GPIO_Init(ZB25VQ32_CS_PORT, &GPIO_InitStruct);
  HAL_GPIO_WritePin(ZB25VQ32_CS_PORT, ZB25VQ32_CS_PIN, GPIO_PIN_SET);

  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
  GPIO_InitStruct.Pin = ZB25VQ32_SCLK_PIN;
  HAL_GPIO_Init(ZB25VQ32_SCLK_PORT, &GPIO_InitStruct);
  HAL_GPIO_WritePin(ZB25VQ32_SCLK_PORT, ZB25VQ32_SCLK_PIN, GPIO_PIN_RESET);
	
  GPIO_InitStruct.Pin = ZB25VQ32_SDI_PIN;
  HAL_GPIO_Init(ZB25VQ32_SDI_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(ZB25VQ32_SDI_PORT, ZB25VQ32_SDI_PIN, GPIO_PIN_RESET);
	
	GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Pin = ZB25VQ32_SDO_PIN;
  HAL_GPIO_Init(ZB25VQ32_SDO_PORT, &GPIO_InitStruct);
}

static void delay_us(uint16_t num)
{
	__NOP();
}

static uint8_t ZB25VQ32_ReadByte(void)
{
	uint8_t read = 0;
  for (uint8_t i = 0; i < 8; i++)
  {
    ZB25VQ32_SCLK_L;
    read = read<<1;
    delay_us(1);
    if(ZB25VQ32_SDO_READ())
    {
      read++;
    }
    ZB25VQ32_SCLK_H;
    delay_us(1);
  }

  return read;	
}

static uint8_t ZB25VQ32_Read_Len(uint8_t *buf, uint16_t len)
{
  uint8_t read = 0;
  
  delay_us(1);
	while(len)
	{
		*buf=ZB25VQ32_ReadByte();		
		len--;
		buf++;  
	}

  return read;
}

static void ZB25VQ32_WriteByte(uint8_t wdata)
{ 
  for(uint8_t i = 0; i < 8; i++)
  {
    ZB25VQ32_SCLK_L;
    if (wdata & 0x80){ZB25VQ32_SDI_H;}
    else {ZB25VQ32_SDI_L;}
    delay_us(1);
    ZB25VQ32_SCLK_H;
    wdata <<= 1;
    delay_us(1);
  }
}

static void ZB25VQ32_Write_Len(uint8_t *wdata, uint16_t len)
{
	for(uint16_t i=0;i<len;i++)
	{
		ZB25VQ32_WriteByte(wdata[i]); 
	}
}

uint8_t ZB25VQ32_ReadAddressData(uint32_t ReadAddr)
{
	uint8_t data;
  uint8_t cmd[4U];

  /* Send the command */
  cmd[0U] = ZB25VQ32_READ_CMD;
  cmd[1U] = (uint8_t)((ReadAddr & 0x00FF0000U) >> 16);
  cmd[2U] = (uint8_t)((ReadAddr & 0x0000FF00U) >> 8);
  cmd[3U] = (uint8_t)(ReadAddr & 0x000000FFU);

	ZB25VQ32_CS_L;

  ZB25VQ32_Write_Len(cmd, 4);
	data = ZB25VQ32_ReadByte();

	ZB25VQ32_CS_H;

  return data;	
}

/**
  * @brief  Get Flash information
  * @param  pInfo pointer to Device Info structure
  * @retval Status
  *    - ZB25VQ32_OK
  */
int32_t ZB25VQ32_GetFlashInfo(ZB25VQ32_Info_t *pInfo)
{
  /* Configure the structure with the memory configuration */
  pInfo->FlashSize              = ZB25VQ32_FLASH_SIZE;
  pInfo->EraseSectorSize        = ZB25VQ32_BLOCK_64K;
  pInfo->EraseSectorsNumber     = (ZB25VQ32_FLASH_SIZE / ZB25VQ32_BLOCK_64K);
  pInfo->EraseSubSectorSize     = ZB25VQ32_SECTOR_4K;
  pInfo->EraseSubSectorNumber   = (ZB25VQ32_FLASH_SIZE / ZB25VQ32_SECTOR_4K);
  pInfo->EraseSubSector1Size    = ZB25VQ32_SECTOR_4K;
  pInfo->EraseSubSector1Number  = (ZB25VQ32_FLASH_SIZE / ZB25VQ32_SECTOR_4K);
  pInfo->ProgPageSize           = ZB25VQ32_PAGE_SIZE;
  pInfo->ProgPagesNumber        = (ZB25VQ32_FLASH_SIZE / ZB25VQ32_PAGE_SIZE);

  return ZB25VQ32_OK;
}

/**
  * @brief  Wait until Write In Progress (WIP) bit is equal to 0
  * @param  Ctx Component object pointer
  * @retval Status
  *    - ZB25VQ32_ERROR_AUTOPOLLING
  *    - ZB25VQ32_ERROR_COMMAND
  *    - ZB25VQ32_OK
  */
int32_t ZB25VQ32_AutoPollingMemReady(void)
{
  int32_t ret = ZB25VQ32_OK;
  uint8_t statusRegister;
  uint8_t cmd = ZB25VQ32_READ_STATUS_REG_CMD;

	ZB25VQ32_CS_L;

  ZB25VQ32_WriteByte(cmd);
	statusRegister = ZB25VQ32_ReadByte();

	ZB25VQ32_CS_H;
	
  return ret;
}

/**
  * @brief  Wait until Write Enable Latch (WEL) bit is set to 1
  * @param  Component object pointer
  * @retval Status
  *    - ZB25VQ32_ERROR_RECEIVE
  *    - ZB25VQ32_ERROR_COMMAND
  *    - ZB25VQ32_OK
  */
int32_t ZB25VQ32_AutoPollingMemReadyToWrite(void)
{
  int32_t ret = ZB25VQ32_OK;
  uint8_t statusRegister;
  uint8_t cmd = ZB25VQ32_READ_STATUS_REG_CMD;

	ZB25VQ32_CS_L;

  ZB25VQ32_WriteByte(cmd);
	statusRegister = ZB25VQ32_ReadByte();

	ZB25VQ32_CS_H;
	
  return ret;
}

/* Read/Write Array Commands (3 Byte Address Command Set) *********************/
/**
  * @brief  Reads an amount of data from the memory.
  * @param  Ctx Component object pointer
  * @param  pData Pointer to data to be read
  * @param  ReadAddr Read start address
  * @param  Size Size of data to read in Byte
  * @retval Status
  *    - ZB25VQ32_ERROR_RECEIVE
  *    - ZB25VQ32_ERROR_COMMAND
  *    - ZB25VQ32_OK
  */
int32_t ZB25VQ32_Read(uint8_t *pData, uint32_t ReadAddr, uint16_t Size)
{
  int32_t ret = ZB25VQ32_OK;
  uint8_t cmd[4U];

  /* Send the command */
  cmd[0U] = ZB25VQ32_READ_CMD;
  cmd[1U] = (uint8_t)((ReadAddr & 0x00FF0000U) >> 16);
  cmd[2U] = (uint8_t)((ReadAddr & 0x0000FF00U) >> 8);
  cmd[3U] = (uint8_t)(ReadAddr & 0x000000FFU);

	ZB25VQ32_CS_L;

  ZB25VQ32_Write_Len(cmd, 4);
	ZB25VQ32_Read_Len(pData, Size);

	ZB25VQ32_CS_H;
	
  return ret;
}

/**
  * @brief  Reads an amount of data from the memory.
  * @param  Ctx Component object pointer
  * @param  pData Pointer to data to be read
  * @param  ReadAddr Read start address
  * @param  Size Size of data to read in Byte
  * @retval Status
  *    - ZB25VQ32_ERROR_RECEIVE
  *    - ZB25VQ32_ERROR_COMMAND
  *    - ZB25VQ32_OK
  */
int32_t ZB25VQ32_FastRead(uint8_t *pData, uint32_t ReadAddr, uint16_t Size)
{
  int32_t ret = ZB25VQ32_OK;
  uint8_t cmd[5U];

  /* Send the command */
  cmd[0U] = ZB25VQ32_FAST_READ_CMD;
  cmd[1U] = (uint8_t)((ReadAddr & 0x00FF0000U) >> 16);
  cmd[2U] = (uint8_t)((ReadAddr & 0x0000FF00U) >> 8);
  cmd[3U] = (uint8_t)(ReadAddr & 0x000000FFU);
  cmd[4U] = 0xAAU; /* Dummy */

	ZB25VQ32_CS_L;

  ZB25VQ32_Write_Len(cmd, 5);
	ZB25VQ32_Read_Len(pData, Size);

	ZB25VQ32_CS_H;
	
  return ret;
}

/**
  * @brief  Writes an amount of data to the SPI memory.
  *    For 256 bytes page program, the 8 least significant address bits byte
  *    should be set to 0 this function otherwise returns ZB25VQ32_ERROR_ADDRESS
  * @param  Ctx Component object pointer
  * @param  pData Pointer to data to be written
  * @param  WriteAddr Write start address
  * @param  Size Size of data to write. Range 1 ~ 256
  * @retval Status
  *    - ZB25VQ32_ERROR_ADDRESS
  *    - ZB25VQ32_ERROR_COMMAND
  *    - ZB25VQ32_ERROR_TRANSMIT
  *    - ZB25VQ32_OK
  */
int32_t ZB25VQ32_PageProgram(uint8_t *pData, uint32_t WriteAddr, uint16_t Size)
{
  int32_t ret = ZB25VQ32_OK;
  uint8_t cmd[4U];

	ZB25VQ32_CS_L;
  if ((Size >= ZB25VQ32_PAGE_SIZE) && ((WriteAddr & 0x000000FFU) != 0U))
  {
    ret = ZB25VQ32_ERROR_ADDRESS;
  }
  else
  {
    /* 1- Send Page Program (PP) command */
    cmd[0U] = ZB25VQ32_PAGE_PROG_CMD;
    cmd[1U] = (uint8_t)((WriteAddr & 0x00FF0000U) >> 16);
    cmd[2U] = (uint8_t)((WriteAddr & 0x0000FF00U) >> 8);
    cmd[3U] = (uint8_t)(WriteAddr & 0x000000FFU);
    ZB25VQ32_Write_Len(cmd, 4);
		ZB25VQ32_Write_Len(pData, Size);
  }	
	ZB25VQ32_CS_H;
	
  return ret;
}

/**
  * @brief  Erases the specified sector of the SPI memory
  *         ZB25VQ32 support 4K size block erase command.
  * @param  Ctx Component object pointer
  * @param  SectorAddress Sector address to erase
  * @retval Status
  *    - ZB25VQ32_ERROR_COMMAND
  *    - ZB25VQ32_OK
  */
int32_t ZB25VQ32_SectorErase(uint32_t SectorAddress)
{
  int32_t ret = ZB25VQ32_OK;
  uint8_t cmd[4U];

  /* 1- Send Sector Erase (SE) for erasing the data of chosen block */
  cmd[0U] = ZB25VQ32_SECTOR_ERASE_4K_CMD;
  cmd[1U] = (uint8_t)((SectorAddress & 0x00FF0000U) >> 16);
  cmd[2U] = (uint8_t)((SectorAddress & 0x0000FF00U) >> 8);
  cmd[3U] = (uint8_t)(SectorAddress & 0x000000FFU);

	ZB25VQ32_CS_L;
	ZB25VQ32_Write_Len(cmd, 4);
	ZB25VQ32_CS_H;
	
  return ret;
}

/**
  * @brief  Erases the specified block of the SPI memory
  *         ZB25VQ32 64K size block erase command.
  * @param  Ctx Component object pointer
  * @param  BlockAddress Block address to erase
  * @param  BlockSize Block size to erase
  * @retval Status
  *    - ZB25VQ32_ERROR_COMMAND
  *    - ZB25VQ32_OK
  */
int32_t ZB25VQ32_BlockErase(uint32_t BlockAddress, ZB25VQ32_Erase_t BlockSize)
{
  int32_t ret = ZB25VQ32_OK;
  uint8_t cmd[4U];

  /* Setup erase command */
  switch (BlockSize)
  {
    default :
    case ZB25VQ32_ERASE_4K :
      cmd[0U] = ZB25VQ32_SECTOR_ERASE_4K_CMD;
      break;

    case ZB25VQ32_ERASE_64K :
      cmd[0U] = ZB25VQ32_BLOCK_ERASE_64K_CMD;
      break;

    case ZB25VQ32_ERASE_CHIP :
      return ZB25VQ32_ChipErase();
      break;
  }

	ZB25VQ32_CS_L;
  cmd[1U] = (uint8_t)((BlockAddress & 0x00FF0000U) >> 16);
  cmd[2U] = (uint8_t)((BlockAddress & 0x0000FF00U) >> 8);
  cmd[3U] = (uint8_t)(BlockAddress & 0x000000FFU);	
	ZB25VQ32_Write_Len(cmd, 4);
	ZB25VQ32_CS_H;
	
  return ret;
}

/**
  * @brief  Whole chip erase of the SPI memory
  * @param  Ctx Component object pointer
  * @retval Status
  *    - ZB25VQ32_ERROR_COMMAND
  *    - ZB25VQ32_OK
  */
int32_t ZB25VQ32_ChipErase(void)
{
  int32_t ret = ZB25VQ32_OK;
  uint8_t cmd = ZB25VQ32_CHIP_ERASE_CMD;

	ZB25VQ32_CS_L;
	ZB25VQ32_WriteByte(cmd);
	ZB25VQ32_CS_H;
	
  return ret;
}

/* Register/Setting Commands **************************************************/
/**
  * @brief  This function sets the (WEL) Write Enable Latch bit
  * @param  Ctx Component object pointer
  * @retval Status
  *    - ZB25VQ32_ERROR_COMMAND
  *    - ZB25VQ32_OK
  */
int32_t ZB25VQ32_WriteEnable(void)
{
  int32_t ret = ZB25VQ32_OK;
  uint8_t cmd = ZB25VQ32_WRITE_ENABLE_CMD;

	ZB25VQ32_CS_L;
	ZB25VQ32_WriteByte(cmd);
	ZB25VQ32_CS_H;
	
  return ret;
}

/**
  * @brief  This function resets the (WEL) Write Enable Latch bit
  * @param  Ctx Component object pointer
  * @retval Status
  *    - ZB25VQ32_ERROR_COMMAND
  *    - ZB25VQ32_OK
  */
int32_t ZB25VQ32_WriteDisable(void)
{
  int32_t ret = ZB25VQ32_OK;
  uint8_t cmd = ZB25VQ32_WRITE_DISABLE_CMD;

	ZB25VQ32_CS_L;
	ZB25VQ32_WriteByte(cmd);
	ZB25VQ32_CS_H;
	
  return ret;
}

/**
  * @brief  Read Flash Status register
  * @param  Ctx Component object pointer
  * @param  Value pointer to status register value
  * @retval Status
  *    - ZB25VQ32_ERROR_COMMAND
  *    - ZB25VQ32_ERROR_RECEIVE
  *    - ZB25VQ32_OK
  */
int32_t ZB25VQ32_ReadStatusRegister(uint8_t *Value)
{
  int32_t ret = ZB25VQ32_OK;
  uint8_t cmd = ZB25VQ32_READ_STATUS_REG_CMD;

	ZB25VQ32_CS_L;
	ZB25VQ32_WriteByte(cmd);
	*Value = ZB25VQ32_ReadByte();
	ZB25VQ32_CS_H;
	
  return ret;
}

/**
  * @brief  Write Flash Status register value
  * @param  Ctx Component object pointer
  * @param  Value Status register value
  * @retval Status
  *    - ZB25VQ32_ERROR_COMMAND
  *    - ZB25VQ32_OK
  */
int32_t ZB25VQ32_WriteStatusRegister(uint8_t Value)
{
  int32_t ret = ZB25VQ32_OK;
  uint8_t cmd[2U];

	ZB25VQ32_CS_L;
  cmd[0U] = ZB25VQ32_WRITE_STATUS_REG_CMD;
  cmd[1U] = Value;

  ZB25VQ32_Write_Len(cmd, 2);
	ZB25VQ32_CS_H;
	
  return ret;
}

/**
  * @brief  Deep power down
  *         The device is not active and all Write/Program/Erase instruction are ignored.
  * @param  Ctx Component object pointer
  * @retval Status
  *    - ZB25VQ32_ERROR_COMMAND
  *    - ZB25VQ32_OK
  */
int32_t ZB25VQ32_EnterDeepPowerDown(void)
{
  int32_t ret = ZB25VQ32_OK;
  uint8_t cmd = ZB25VQ32_DEEP_POWER_DOWN_CMD;

	ZB25VQ32_CS_L;
	ZB25VQ32_WriteByte(cmd);
	ZB25VQ32_CS_H;
	
	ZB25VQ32_SCLK_L;
  ZB25VQ32_SDI_L;
	ZB25VQ32_SDO_L;
	
  return ret;
}

/**
  * @brief  Release Deep power down
  *         The device is now active and all Write/Program/Erase instruction are available.
  * @param  Ctx Component object pointer
  * @retval Status
  *    - ZB25VQ32_ERROR_COMMAND
  *    - ZB25VQ32_OK
  */
int32_t ZB25VQ32_ReleaseDeepPowerDown(void)
{
  int32_t ret = ZB25VQ32_OK;
  uint8_t cmd = ZB25VQ32_RELEASE_FROM_DEEP_POWER_DOWN_CMD;

	ZB25VQ32_CS_L;
	ZB25VQ32_WriteByte(cmd);
	ZB25VQ32_CS_H;
	
  return ret;
}

/* ID/Security Commands *******************************************************/
/**
  * @brief  Read Flash 3 Byte IDs 
  * 9Fh(SPI Mode) Manufacturer ID = 5E Memory Type =40h Capacity = 16h
  *         Manufacturer ID, Memory type, Memory density
  * @param  Ctx Component object pointer
  * @param  ID pointer to flash id value
  * @retval Status
  *    - ZB25VQ32_ERROR_COMMAND
  *    - ZB25VQ32_ERROR_RECEIVE
  *    - ZB25VQ32_OK
  */
int32_t ZB25VQ32_ReadID(uint8_t *ID)
{
  int32_t ret = ZB25VQ32_OK;
  uint8_t cmd = ZB25VQ32_READ_ID_CMD;

	ZB25VQ32_CS_L;
	ZB25VQ32_WriteByte(cmd);
	ZB25VQ32_Read_Len(ID, 3);
	ZB25VQ32_CS_H;
	
  return ret;
}

/**
  * @brief  Reads an amount of data from the memory
  * @param  Ctx Component object pointer
  * @param  pData Pointer to data to be read
  * @param  ReadAddr Read start address
  * @param  Size Size of data to read in Byte
  * @retval Status
  *    - ZB25VQ32_ERROR_COMMAND
  *    - ZB25VQ32_ERROR_RECEIVE
  *    - ZB25VQ32_OK
  */
int32_t ZB25VQ32_ReadSFDP(uint8_t *pData, uint32_t ReadAddr, uint16_t Size)
{
  int32_t ret = ZB25VQ32_OK;
  uint8_t cmd[5U];

  ZB25VQ32_CS_L;
  cmd[0U] = ZB25VQ32_READ_SERIAL_FLASH_DISCO_PARAM_CMD;
  cmd[1U] = (uint8_t)((ReadAddr & 0x00FF0000U) >> 16);
  cmd[2U] = (uint8_t)(ReadAddr & 0x0000FF00U) >> 8;
  cmd[3U] = (uint8_t)(ReadAddr & 0x000000FFU);
  cmd[4U] = 0xAAU; /* Dummy */

  ZB25VQ32_Write_Len(cmd, 5);
  ZB25VQ32_Read_Len(pData, Size);
	ZB25VQ32_CS_H;
	
  return ret;
}
