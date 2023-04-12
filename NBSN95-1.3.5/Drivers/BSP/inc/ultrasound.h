#ifndef _Ultrasound_h_
#define _Ultrasound_h_

#include "common.h"

void GPIO_ULT_INPUT_Init(void);
void GPIO_ULT_OUTPUT_Init(void);
void GPIO_ULT_INPUT_DeInit(void);
void GPIO_ULT_OUTPUT_DeInit(void);
uint16_t ULT_distance(void);

#endif
