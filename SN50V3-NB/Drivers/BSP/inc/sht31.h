#ifndef __SHT31_H
#define __SHT31_H

#include "common.h"

void sht31Init(void);
void sht31Data(void);
uint8_t sht3x_Detect(void);
void sht31LowPower(void);

#endif
