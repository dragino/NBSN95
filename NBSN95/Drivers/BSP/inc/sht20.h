#ifndef __SHT20_H
#define __SHT20_H

#include "common.h"

extern I2C_HandleTypeDef hi2c1;
void sht20Init(void);
void sht20Data(void);

void MX_I2C1_Init(void);
void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle);
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle);

#endif
